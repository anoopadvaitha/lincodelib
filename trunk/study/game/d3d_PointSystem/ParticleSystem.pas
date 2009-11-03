// 粒子系统实现：模拟下雪
unit ParticleSystem;

interface
uses
  Direct3D9, D3DX9, classes, SysUtils, Windows;

type
  // 粒子顶点
  PParticleVertex = ^TParticleVertex;
  TParticleVertex = packed record
    Pos:    TD3DXVector3;      // 位置
    Color:  TD3DColor;         // 颜色
  end;
const
  PARTICLE_FVF = D3DFVF_XYZ or D3DFVF_DIFFUSE;

type
  // 粒子属性
  PParticleProp = ^TParticleProp;
  TParticleProp = packed record
    Pos: TD3DXVector3;    // 粒子位置
    Vel: TD3DXVector3;    // 粒子速度
    Color: TD3DColor;     // 粒子颜色
  end;

type
  // 模拟雪的粒子系统
  TSnowParticleSys = class
  protected
    FDevice: IDirect3DDevice9;          // D3D设备
    FBoxMin: TD3DXVector3;              // 粒子所在范围
    FBoxMax: TD3DXVector3;              // 粒子所在范围
    FParticleList: TList;               // 粒子列表
    FSize: Single;                      // 系统中粒子的大小
    FVbSize: Cardinal;                  // 顶点缓冲区能够容纳的粒子数
    FVbOffset: Cardinal;                // 批量绘粒子时用到
    FVbBatchSize: Cardinal;             // 批量绘粒子时用到
    FVBuffer: IDirect3DVertexBuffer9;   // 顶点缓冲
    FTex: IDirect3DTexture9;            // 纹理
  protected
    // 点是否在箱子里
    function PointInside(const P, PMin, PMax: TD3DXVector3): Boolean;
    // 绘制之前
    procedure PreRender;
    // 绘制之后
    procedure PostRender;
  public
    // size 为粒子的大小
    constructor Create(size: Single);
    destructor Destroy; override;
    // 初始化粒子, Num为粒子数
    procedure Init(Device9: IDirect3DDevice9; TexFile: string;
      const BoxMin, BoxMax: TD3DXVector3; Num: Cardinal);
    // 重设所有粒子
    procedure ResetAll;
    // 重设粒子
    procedure ResetParticle(Prop: PParticleProp);
    // 增加粒子
    procedure AddParticle;
    // 增加粒子
    procedure AddParticles(nNum: Cardinal);
    // 更新粒子
    procedure Update(Delta: Single);
    // 绘制
    procedure Render;
    // 是否为空
    function IsEmpty: Boolean;
  end;

implementation

uses
  D3dUtils;

// 点是否在箱子里
function TSnowParticleSys.PointInside(const P, PMin, PMax: TD3DXVector3): Boolean;
begin
  if (P.x >= PMin.x) and (P.y >= PMin.y) and (P.z >= PMin.z) and
    (P.x <= PMax.x) and (P.y <= PMax.y) and (P.z <= PMax.z) then
    Result := True
  else
    Result := False;
end;

procedure TSnowParticleSys.Init(Device9: IDirect3DDevice9; TexFile: string;
  const BoxMin, BoxMax: TD3DXVector3; Num: Cardinal);
begin
  FDevice := Device9;
  if not Assigned(FDevice) then
    Exit;

  if Failed(FDevice.CreateVertexBuffer(FVbSize * SizeOf(TParticleVertex),
    D3DUSAGE_DYNAMIC or D3DUSAGE_POINTS or D3DUSAGE_WRITEONLY,
    PARTICLE_FVF,
    D3DPOOL_DEFAULT,
    FVBuffer,
    nil)) then
    Exit;

  if FileExists(TexFile) then
    D3DXCreateTextureFromFile(FDevice, PChar(TexFile), FTex);

  FBoxMin := BoxMin;
  FBoxMax := BoxMax;
  AddParticles(Num);
end;

procedure TSnowParticleSys.AddParticles(nNum: Cardinal);
var
  i: Integer;
begin
  for i := 0 to nNum - 1 do
   AddParticle;
end;

procedure TSnowParticleSys.Render;
var
  pv: Pointer;
  vt: PParticleVertex;
  Flag: Cardinal;
  i: Integer;
  Prop: PParticleProp;
  nBatch: Cardinal;
begin
  if IsEmpty or not Assigned(FDevice) then
    Exit;

  PreRender;

  if Assigned(FTex) then
    FDevice.SetTexture(0, FTex);
  FDevice.SetFVF(PARTICLE_FVF);
  FDevice.SetStreamSource(0, FVBuffer, 0, SizeOf(TParticleVertex));

  // 批量绘制
  if FVbOffset > FVbSize then
    FVbOffset := 0;

  if FVbOffset > 0 then
    Flag := D3DLOCK_NOOVERWRITE
  else
    Flag := D3DLOCK_DISCARD;
  FVBuffer.Lock(FVbOffset * SizeOf(TParticleVertex),
    FVbBatchSize * Sizeof(TParticleVertex), pv, Flag);

  nBatch := 0;
  for i := 0 to FParticleList.Count - 1 do
  begin
    Prop := PParticleProp(FParticleList[i]);
    vt := PParticleVertex(pv);
    vt.Pos := Prop.Pos;
    vt.Color := Prop.Color;
    Inc(PParticleVertex(pv), 1);

    Inc(nBatch);
    if nBatch = FVbBatchSize then
    begin
      FVBuffer.Unlock;

      FDevice.DrawPrimitive(D3DPT_POINTLIST, FVbOffset, FVbBatchSize);

      Inc(FVbOffset, FVbBatchSize);
      if FVbOffset >= FVbSize then
        FVbOffset := 0;

      if FVbOffset > 0 then
        Flag := D3DLOCK_NOOVERWRITE
      else
        Flag := D3DLOCK_DISCARD;
      FVBuffer.Lock(FVbOffset * SizeOf(TParticleVertex),
        FVbBatchSize * Sizeof(TParticleVertex), pv, Flag);

      nBatch := 0;
    end;
  end;

  FVBuffer.Unlock;

  if nBatch > 0 then
  begin
    FDevice.DrawPrimitive(D3DPT_POINTLIST, FVbOffset, nBatch);
  end;

  PostRender;
end;

function TSnowParticleSys.IsEmpty: Boolean;
begin
  Result := FParticleList.Count = 0;
end;

procedure TSnowParticleSys.PostRender;
begin
  FDevice.SetRenderState(D3DRS_LIGHTING, 1);
  FDevice.SetRenderState(D3DRS_POINTSPRITEENABLE, 0);
  FDevice.SetRenderState(D3DRS_POINTSCALEENABLE, 0);
  FDevice.SetRenderState(D3DRS_ALPHATESTENABLE, 0);
end;

procedure TSnowParticleSys.PreRender;
begin
  FDevice.SetRenderState(D3DRS_LIGHTING, 0);
  FDevice.SetRenderState(D3DRS_POINTSPRITEENABLE, 1);
  FDevice.SetRenderState(D3DRS_POINTSCALEENABLE, 1);
  FDevice.SetRenderState(D3DRS_POINTSIZE, SingleToDWord(FSize));
  FDevice.SetRenderState(D3DRS_POINTSIZE_MIN, SingleToDWord(0));

  FDevice.SetRenderState(D3DRS_POINTSCALE_A, SingleToDWord(0));
  FDevice.SetRenderState(D3DRS_POINTSCALE_B, SingleToDWord(0));
  FDevice.SetRenderState(D3DRS_POINTSCALE_C, SingleToDWord(1.0));
end;

procedure TSnowParticleSys.AddParticle;
var
  Prop: PParticleProp;
begin
  New(Prop);
  ResetParticle(Prop);
  FParticleList.Add(Prop);
end;

procedure TSnowParticleSys.ResetAll;
var
  i: Integer;
begin
  for i := 0 to FParticleList.Count - 1 do
    ResetParticle(PParticleProp(FParticleList[i]));
end;

constructor TSnowParticleSys.Create(size: Single);
begin
  FParticleList := TList.Create;
  FSize := size;
  FVbSize := 2048;
  FVbBatchSize := 512;
end;

destructor TSnowParticleSys.Destroy;
var
  i: Integer;
begin
  for i := 0 to FParticleList.Count - 1 do
    Dispose(PParticleProp(FParticleList.Items[i]));
  FParticleList.Free;
  inherited;
end;

procedure TSnowParticleSys.Update(Delta: Single);
var
  i: Integer;
  Prop: PParticleProp;
  Vec: TD3DXVector3;
begin
  for i := 0 to FParticleList.Count - 1 do
  begin
    Prop := PParticleProp(FParticleList[i]);

    // 当前位置加上位移
    D3DXVec3Scale(Vec, Prop.Vel, Delta);
    D3DXVec3Add(Prop.Pos, Prop.Pos, Vec);

    // 边界判断
    if not PointInside(Prop.Pos, FBoxMin, FBoxMax) then
      ResetParticle(Prop);
  end;
end;

procedure TSnowParticleSys.ResetParticle(Prop: PParticleProp);
begin
  // 位置
  RandomVector(Prop.Pos, FBoxMin, FBoxMax);
  Prop.Pos.y := FBoxMax.y;

  // 速度
  Prop.Vel.x := RandomFloat(0.2, 1) * -2;
  Prop.Vel.y := RandomFloat(0.2, 1) * -6;
  Prop.Vel.z := 0;

  // 白色
  Prop.Color := $FFFFFFFF;
end;

end.
