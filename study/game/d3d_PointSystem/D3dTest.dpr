program D3dTest;

uses
  D3dUtils in 'D3dUtils.pas',
  ParticleSystem in 'ParticleSystem.pas',
  Direct3D9, D3DX9, Windows;

var
  Snow: TSnowParticleSys;

// 启动变换
procedure SetupTransform;
var
  mat: TD3DMatrix;
  eye, LookAt, Up: TD3DVector;
begin
  // 相机转换
  eye := D3DXVector3(0, 0, -13);
  LookAt := D3DXVector3(0, 0, 0);
  Up := D3DXVector3(0, 1, 0);
  D3DXMatrixLookAtLH(mat, eye, LookAt, Up);
  Device9.SetTransform(D3DTS_VIEW, mat);

  // 投影转换
  D3DXMatrixPerspectiveFovLH(mat, D3DX_PI * 0.5, d3dpp.BackBufferWidth / d3dpp.BackBufferHeight, 1, 100);
  Device9.SetTransform(D3DTS_PROJECTION, mat);
end;

// 更新帧
procedure RenderFrame(dwTime: Cardinal);
begin
  Snow.Update(dwTime * 0.001);

  // 清除后台缓冲， 深度缓冲
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

  Device9.Present(nil, nil, 0, nil);
end;


// 初始化
procedure Setup;
begin
  Randomize;

  if not InitD3D(800, 600, True, D3DFMT_X8R8G8B8) then
  begin
    MessageBox(GetActiveWindow, 'D3D创建化失败！', '', MB_OK);
    Exit;
  end;

  Snow := TSnowParticleSys.Create(0.1);
  Snow.Init(Device9, 'snowball.bmp',
    D3DXVECTOR3(-10, -10, -10),
    D3DXVECTOR3(10, 10, 10),
    1500);
end;

// 运行程序
procedure Run;
begin
  MessageLoop(RenderFrame, 10);
end;

// 清除
procedure Cleanup;
begin
  Snow.Free;
  TermD3D;
end;

begin
  Setup;
  Run;
  Cleanup;
end.
