unit D3dUtils;

interface
uses
  Direct3D9, D3DX9;

procedure Setup;
procedure Run;
procedure Cleanup;
procedure ResetDevice;

var
  D3D9: IDirect3D9;
  Device9: IDirect3DDevice9;
  d3dpp: TD3DPresentParameters;
  Wnd: THandle;
  bReseting: Boolean;
  bFullScreen: Boolean;


implementation

uses
  Windows, Messages, CnDebug, Classes, SysUtils, MMSystem;

type
  TUpdateFrame = procedure(dwTime: Cardinal);
procedure FullScreenSwitch; forward;

// 是否支持硬件顶点运算
function SupportHWVertexProcess: Boolean;
var
  Caps: TD3DCaps9;
begin
  if Succeeded(D3D9.GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Caps)) then
    Result := Caps.DevCaps and D3DDEVCAPS_HWTRANSFORMANDLIGHT <> 0
  else
    Result := False;
end;

//------------------------------------------------------------------------------
// 创建窗口
function CreateWnd(WndProc: Pointer; Width, Height: Integer): THandle;
var
  szClsName: array [0..64] of char;
  wc: TWndClassEx;
begin
  szClsName := 'D3D.Test.Application';
  Result := 0;

  wc.cbSize := SizeOf(wc);
  wc.style := CS_VREDRAW or CS_HREDRAW or CS_DBLCLKS;
  wc.lpfnWndProc := WndProc;
  wc.cbClsExtra := 0;
  wc.cbWndExtra := 0;
  wc.hInstance := HInstance;
  wc.hIcon := LoadIcon(0, IDI_APPLICATION);
  wc.hCursor := LoadCursor(0, IDC_ARROW);
  wc.hbrBackground := 0;
  wc.lpszMenuName := '';
  wc.lpszClassName := szClsName;
  wc.hIconSm := 0;

  if RegisterClassEx(wc) = 0 then
    Exit;

  Result := CreateWindow(
    szClsName,
    szClsName,
    WS_OVERLAPPED or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX,
    100, 100,
    Width, Height,
    0, 0,
    HInstance,
    nil);
end;

// 消息循环
procedure MessageLoop(UpdateFrame: TUpdateFrame; dwFrameTime: Cardinal);
var
  msg: TMsg;
  CurrTime: Cardinal;
  LastTime: Cardinal;
  Time: Cardinal;
begin
  LastTime := timeGetTime;
  while True do
  begin
    if PeekMessage(msg, 0, 0, 0, PM_REMOVE) then
    begin
      if msg.message = WM_QUIT then
        Break
      else begin
        TranslateMessage(msg);
        DispatchMessage(msg);
      end;
    end
    else begin
      CurrTime := timeGetTime;
      Time := CurrTime - LastTime;
      if Time > dwFrameTime then
      begin
        UpdateFrame(Time);
        LastTime := CurrTime;
      end;
      Sleep(10);
    end;
  end;
end;

// 窗口过程
function WndProc(hwnd: THandle; Msg: UINT; wParam: LongInt; lParam: LongInt): HRESULT; stdcall;
var
  ps: TPaintStruct;
begin
  case Msg of
    WM_PAINT:
    begin
      BeginPaint(hwnd, ps);
      EndPaint(hwnd, ps);
      Result := 0;
    end;
    WM_LBUTTONDOWN:
    begin
      Result := 0;
    end;
    WM_DESTROY:
    begin
      PostQuitMessage(0);
      Result := 0;
    end;
    WM_RBUTTONDOWN:
    begin
      // 全屏与窗口模式之间的切换
      FullScreenSwitch;
      Result := 0;
    end;    
    else
      Result := DefWindowProc(hwnd, Msg, wParam, lParam);
  end;
end;

//------------------------------------------------------------------------------
// 全屏切换
procedure FullScreenSwitch;
begin
  if bFullScreen then
  begin
    d3dpp.Windowed := True;
    if Succeeded(Device9.Reset(d3dpp)) then
      bFullScreen := False;
  end
  else begin
    d3dpp.Windowed := False;
    if Succeeded(Device9.Reset(d3dpp)) then
      bFullScreen := True;
  end;
end;

//------------------------------------------------------------------------------
// 初始化D3D
function InitD3D(Width, Height: Integer; bWindowed: Boolean; Format: TD3DFormat): Boolean;
var
  Flag: Cardinal;
begin
  Result := False;

  // Create window
  Wnd := CreateWnd(@WndProc, Width, Height);
  if Wnd = 0 then
    Exit;

  // Create IDirect3D9
  D3D9 := Direct3DCreate9(D3D_SDK_VERSION);
  if D3D9 = nil then
    Exit;

  // Show window
  ShowWindow(Wnd, SW_SHOWNORMAL);
  UpdateWindow(Wnd);

  bFullScreen := not bWindowed;

  // Fill D3DPresentParameters
  FillChar(d3dpp, SizeOf(d3dpp), 0);
  d3dpp.Windowed := bWindowed;
  d3dpp.hDeviceWindow := Wnd;
  d3dpp.BackBufferWidth := Width;
  d3dpp.BackBufferHeight := Height;
  d3dpp.BackBufferCount := 1;
  d3dpp.BackBufferFormat := Format;
  d3dpp.SwapEffect := D3DSWAPEFFECT_DISCARD;
  d3dpp.EnableAutoDepthStencil := True;
  d3dpp.AutoDepthStencilFormat := D3DFMT_D16;

  // Check Vertex processing
  if SupportHWVertexProcess then
    Flag := D3DCREATE_HARDWARE_VERTEXPROCESSING
  else
    Flag := D3DCREATE_SOFTWARE_VERTEXPROCESSING;

  // Create Device by default adapter.
  if Failed(D3D9.CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Wnd, Flag, @d3dpp, Device9)) then
    Exit;

  // 禁用背面消除
  Device9.SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  // 启用深度缓冲
  Device9.SetRenderState(D3DRS_ZENABLE, 1);

  Result := True;
end;

//------------------------------------------------------------------------------
// 粒子系统

// 浮点数随机值
function RandomFloat(LowValue: Single; HighValue: Single): Single;
begin
  if LowValue > HighValue then
    Result := 0
  else begin
    Result := Random(10000) * 0.0001;
    Result := Result * (HighValue - LowValue) + LowValue;
  end;
end;

// 随机向量
procedure RandomVector(out VecOut: TD3DXVector3; const VecMin, VecMax: TD3DXVector3);
begin
  VecOut.x := RandomFloat(VecMin.x, VecMax.x);
  VecOut.y := RandomFloat(VecMin.y, VecMax.y);
  VecOut.z := RandomFloat(VecMin.z, VecMax.z);
end;

// 点是否在箱子里
function PointInside(const P, PMin, PMax: TD3DXVector3): Boolean;
begin
  if (P.x >= PMin.x) and (P.y >= PMin.y) and (P.z >= PMin.z) and
    (P.x <= PMax.x) and (P.y <= PMax.y) and (P.z <= PMax.z) then
    Result := True
  else
    Result := False;
end;

type
  // 粒子顶点
  PParticleVertex = ^TParticleVertex;
  TParticleVertex = packed record
    Pos:    TD3DXVector3;      // 位置
    Color:  TD3DColor;         // 颜色
    Size:   Single;            // 尺寸
  end;
const
  PARTICLE_FVF = D3DFVF_XYZ or D3DFVF_DIFFUSE or D3DFVF_PSIZE;

type
  // 粒子属性
  PParticleProp = ^TParticleProp;
  TParticleProp = packed record
    Pos: TD3DXVector3;    // 粒子在世界坐标中的位置
    Vel: TD3DXVector3;    // 粒子速度
    Acc: TD3DXVector3;    // 粒子加速度
    LifeTime: Single;     // 粒子生命周期
    Age: Single;          // 粒子年龄
    Color: TD3DColor;     // 粒子颜色
    ColorFade: TD3DColor; // 粒子消失时的颜色，过滤
    Alive: Boolean;       // 粒子是否活着
  end;

type
  // 粒子系统基类
  TParticleSystem = class
  protected
    FDevice: IDirect3DDevice9;
    FOrgin: TD3DXVector3;        // 粒子的出发点
    FMaxNum: Cardinal;           // 最大粒子数
    FBoxMin: TD3DXVector3;       // 粒子所在范围
    FBoxMax: TD3DXVector3;       // 粒子所在范围
    FParticleList: TList;        // 粒子列表
    FSize: Cardinal;             // 系统中粒子的大小
    FVbSize: Cardinal;           // 顶点缓冲区能够容纳的粒子数
    FVbOffset: Cardinal;
    FVbBatchSize: Cardinal;
    FVBuffer: IDirect3DVertexBuffer9;   // 顶点缓冲
    FTex: IDirect3DTexture9;            // 纹理
  protected
    procedure RemoveDeadParticle; virtual;
  public
    constructor Create;
    destructor Destroy; override;
    procedure Init(Device9: IDirect3DDevice9; TexFile: string;
      const BoxMin, BoxMax: TD3DXVector3); virtual;
    procedure ResetAll; virtual;
    procedure ResetParticle(Prop: PParticleProp); virtual; abstract;
    procedure AddParticle; overload; 
    procedure AddParticle(nNum: Cardinal); overload;
    procedure Update(Delta: Single); virtual; abstract;
    procedure PreRender; virtual;
    procedure Render; virtual;
    procedure PostRender; virtual;
    function IsEmpty: Boolean;
    function IsDead: Boolean;
  end;

procedure TParticleSystem.Init(Device9: IDirect3DDevice9; TexFile: string;
  const BoxMin, BoxMax: TD3DXVector3);
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
end;

procedure TParticleSystem.AddParticle(nNum: Cardinal);
var
  i: Integer;
begin
  for i := 0 to nNum - 1 do
   AddParticle;
end;

procedure TParticleSystem.Render;
var
  pv: Pointer;
  vt: PParticleVertex;
  Flag: Cardinal;
  i: Integer;
  Prop: PParticleProp;
  nBatch: Cardinal;
begin
  if IsEmpty then
    Exit;

  PreRender;

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
  FVBuffer.Lock(FVbOffset * SizeOf(TParticleVertex), FVbBatchSize * Sizeof(TParticleVertex),
    pv, Flag);

  nBatch := 0;
  for i := 0 to FParticleList.Count - 1 do
  begin
    Prop := PParticleProp(FParticleList[i]);
    if not Prop.Alive then
      Continue;

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
      FVBuffer.Lock(FVbOffset * SizeOf(TParticleVertex), FVbBatchSize * Sizeof(TParticleVertex),
        pv, Flag);

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

procedure TParticleSystem.RemoveDeadParticle;
var
  i: Integer;
  Prop: PParticleProp;
begin
  for i := FParticleList.Count - 1 downto 0 do
  begin
    Prop := PParticleProp(FParticleList[i]);
    if not Prop.Alive then
    begin
      FParticleList.Delete(i);
      Dispose(Prop);
    end;
  end;
end;

function TParticleSystem.IsDead: Boolean;
var
  i: Integer;
  Prop: PParticleProp;
begin
  Result := True;
  for i := 0 to FParticleList.Count - 1 do
  begin
    Prop := PParticleProp(FParticleList[i]);
    if Prop.Alive then
    begin
      Result := False;
      Break;
    end;
  end;
end;

function TParticleSystem.IsEmpty: Boolean;
begin
  Result := FParticleList.Count = 0;
end;

procedure TParticleSystem.PostRender;
begin
  FDevice.SetRenderState(D3DRS_LIGHTING, 1);
  FDevice.SetRenderState(D3DRS_POINTSPRITEENABLE, 0);
  FDevice.SetRenderState(D3DRS_POINTSCALEENABLE, 0);
  FDevice.SetRenderState(D3DRS_ALPHATESTENABLE, 0);
end;

procedure TParticleSystem.PreRender;
begin
  FDevice.SetRenderState(D3DRS_LIGHTING, 0);
  FDevice.SetRenderState(D3DRS_POINTSPRITEENABLE, 1);
  FDevice.SetRenderState(D3DRS_POINTSCALEENABLE, 1);
  FDevice.SetRenderState(D3DRS_POINTSIZE, FSize);
  FDevice.SetRenderState(D3DRS_POINTSIZE_MIN, 0);

  // 粒子相对于位置的尺寸
  FDevice.SetRenderState(D3DRS_POINTSCALE_A, 0);
  FDevice.SetRenderState(D3DRS_POINTSCALE_B, 0);
  FDevice.SetRenderState(D3DRS_POINTSCALE_C, 1);

  // 使用纹理的Alpha值
  FDevice.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  FDevice.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

  // 半透明混合
  FDevice.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  FDevice.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  FDevice.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
end;

procedure TParticleSystem.AddParticle;
var
  Prop: PParticleProp;
begin
  New(Prop);
  ResetParticle(Prop);
  FParticleList.Add(Prop);
end;

procedure TParticleSystem.ResetAll;
var
  i: Integer;
begin
  for i := 0 to FParticleList.Count - 1 do
    ResetParticle(PParticleProp(FParticleList[i]));
end;

constructor TParticleSystem.Create;
begin
  FParticleList := TList.Create;
end;

destructor TParticleSystem.Destroy;
var
  i: Integer;
begin
  for i := 0 to FParticleList.Count - 1 do
    Dispose(PParticleProp(FParticleList.Items[i]));
  FParticleList.Free;
  inherited;
end;

type
  TSnowParticleSys = class(TParticleSystem)
  public
    constructor Create;
    procedure ResetParticle(Prop: PParticleProp); override;
    procedure Update(Delta: Single); override;
    procedure Init(Device9: IDirect3DDevice9; TexFile: String;
      const BoxMin, BoxMax: TD3DXVector3); override;
  end;

procedure TSnowParticleSys.Init(Device9: IDirect3DDevice9; TexFile: String;
  const BoxMin, BoxMax: TD3DXVector3);
begin
  inherited;
  AddParticle(2000);
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

    D3DXVec3Scale(Vec, Prop.Vel, Delta);
    D3DXVec3Add(Prop.Pos, Prop.Pos, Vec);

    // 边界判断
    if not PointInside(Prop.Pos, FBoxMin, FBoxMax) then
      ResetParticle(Prop);
  end;
end;

procedure TSnowParticleSys.ResetParticle(Prop: PParticleProp);
begin
  Prop.Alive := True;

  // 创建
  RandomVector(Prop.Pos, FBoxMin, FBoxMax);
  Prop.Pos.y := FBoxMax.y;

  // 速度
  Prop.Vel.x := RandomFloat(0, 1) * -3;
  Prop.Vel.y := RandomFloat(0, 1) * -10;
  Prop.Vel.z := 0;

  // 白色 
  Prop.Color := $FFFFFF;
end;

constructor TSnowParticleSys.Create;
begin
  FSize := 3;
  FVbSize := 2048;
  FVbBatchSize := 512;
  inherited;
end;

//------------------------------------------------------------------------------
// 渲染
var
Snow: TSnowParticleSys;

// 启动变换: 茶壶旋转
procedure SetupTransform;
var
  mat: TD3DMatrix;
  eye, LookAt, Up: TD3DVector;
begin
  // 设定物体坐标与世界坐标原点一样
  D3DXMatrixTranslation(Mat, 0, 0, 0);
  Device9.SetTransform(D3DTS_WORLD, Mat);
  
  // 相机转换
  eye := D3DXVector3(0, 2, -20);
  LookAt := D3DXVector3(0, 0, 0);
  Up := D3DXVector3(0, 1, 0);
  D3DXMatrixLookAtLH(mat, eye, LookAt, Up);
  Device9.SetTransform(D3DTS_VIEW, mat);

  // 投影转换
  D3DXMatrixPerspectiveFovLH(mat, D3DX_PI * 0.5, d3dpp.BackBufferWidth / d3dpp.BackBufferHeight, 1, 1000);
  Device9.SetTransform(D3DTS_PROJECTION, mat);
end;

// 更新帧
procedure UpdateFrame(dwTime: Cardinal);
begin
  if bReseting then
  begin
    ResetDevice;
    Exit;
  end;

  Snow.Update(dwTime * 0.001);

  // 清除后面缓冲， 深度缓冲
  Device9.Clear(0, nil, D3DCLEAR_TARGET or D3DCLEAR_ZBUFFER,
    D3DCOLOR_ARGB(255, 0, 0, 0), 1, 0);

  Device9.BeginScene;
  try
    // 坐标转换
    SetupTransform;

    Snow.Render;
  finally
    Device9.EndScene;
  end;

  // 当设备丢失时，Present会返回D3DERR_DEVICELOST，此时应该进行设备重设置处理
  // 什么时候设备会丢失，一个典型的例子就是全屏时，通过Alt+Tab切换窗口便会出现。
  if Device9.Present(nil, nil, 0, nil) = D3DERR_DEVICELOST then
    bReseting := True;
end;

//------------------------------------------------------------------------------
// 重置设备
procedure ResetDevice;
begin
  // 要等到返回D3DERR_DEVICENOTRESET，才可以试着Reset。
  if Device9.TestCooperativeLevel = D3DERR_DEVICENOTRESET then
  begin
    if Succeeded(Device9.Reset(d3dpp)) then
    begin
      bReseting := False;
    end;
  end;
end;

//------------------------------------------------------------------------------
// 初始化
procedure Setup;
begin
  Randomize;

  // 如果在全屏下传入D3DFMT_UNKNOWN，创建会失败，
  // 但在窗口模式下传入D3DFMT_UNKNOWN会成功，并且再切换全屏时也没有问题
  if not InitD3D(800, 600, True, D3DFMT_X8R8G8B8) then
  begin
    MessageBox(GetActiveWindow, 'D3D创建化失败！', '', MB_OK);
    Exit;
  end;

  Snow := TSnowParticleSys.Create;
  Snow.Init(Device9, 'snowball.bmp',
    D3DXVECTOR3(-10, -10, -10),
    D3DXVECTOR3(10, 10, 10));
end;

// 清除
procedure Cleanup;
begin
  Snow.Free;
end;

// 运行程序
procedure Run;
begin
  MessageLoop(UpdateFrame, 10);
end;
//------------------------------------------------------------------------------

end.
