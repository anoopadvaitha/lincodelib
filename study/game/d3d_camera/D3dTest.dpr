program D3dTest;

uses
  D3dUtils in 'D3dUtils.pas',
  Direct3D9,
  D3DX9,
  Windows,
  Camera in 'Camera.pas';

var
  gCamera: TCamera;
  gVB: IDirect3DVertexBuffer9;
  Mesh: ID3DXMesh;

type
  PD3DVertex = ^TD3DVertex;
  TD3DVertex = packed record
    x, y, z: Single;
    Color: DWORD;
  end;
const
  D3DFVF_CUSTOM = D3DFVF_XYZ or D3DFVF_DIFFUSE;

procedure InitMeth;
begin
  D3DXCreateTeapot(Device9, mesh, nil);
end;

procedure InitVertex;
var
  vts: PChar;
begin
  if Failed(Device9.CreateVertexBuffer(3 * SizeOf(TD3DVertex), 0, D3DFVF_CUSTOM,
    D3DPOOL_MANAGED, gVB, nil)) then
      Exit;

  if Succeeded(gVB.Lock(0, 3 * SizeOf(TD3DVertex), Pointer(vts), 0)) then
  try
    with PD3DVertex(vts)^ do
    begin
      x := 0;
      y := 5;
      z := -5;
      Color := $ff00ff00;
    end;

    Inc(vts, SizeOf(TD3DVertex));
    with PD3DVertex(vts)^ do
    begin
      x := 5;
      y := 0;
      z := -5;
      Color := $ff00ff00;
    end;

    Inc(vts, SizeOf(TD3DVertex));
    with PD3DVertex(vts)^ do
    begin
      x := -5;
      y := 0;
      z := -5;
      Color := $ff00ff00;
    end;
  finally
    gVB.Unlock;
  end;
end;

// 更新帧
procedure RenderFrame(dwTime: Cardinal);
var
  M: TD3DXMatrix;
begin
  if IsKeyDown(Ord('W')) then
    gCamera.ForBack(0.5)
  else if IsKeyDown(Ord('S')) then
    gCamera.ForBack(-0.5)
  else if IsKeyDown(Ord('A')) then
    gCamera.LeftRight(-0.5)
  else if IsKeyDown(Ord('D')) then
    gCamera.LeftRight(0.5)
  else if IsKeyDown(Ord('R')) then
    gCamera.UpDown(0.5)
  else if IsKeyDown(Ord('F')) then
    gCamera.UpDown(-0.5)
  else if IsKeyDown(VK_UP) then
    gCamera.RotateRight(-D3DX_PI * 0.03)
  else if IsKeyDown(VK_DOWN) then
    gCamera.RotateRight(D3DX_PI * 0.03)
  else if IsKeyDown(VK_LEFT) then
    gCamera.RotateUp(-D3DX_PI * 0.03)
  else if IsKeyDown(VK_RIGHT) then
    gCamera.RotateUp(D3DX_PI * 0.03)
  else if IsKeyDown(Ord('M')) then
    gCamera.RotateLook(-D3DX_PI * 0.03)
  else if IsKeyDown(Ord('N')) then
    gCamera.RotateLook(D3DX_PI * 0.03);

  // 相机坐标
  gCamera.GetViewMatrix(M);
  Device9.SetTransform(D3DTS_VIEW, M);

  // 清除后台缓冲， 深度缓冲
  Device9.Clear(0, nil, D3DCLEAR_TARGET or D3DCLEAR_ZBUFFER, 0, 1, 0);

  Device9.BeginScene;
  try
    mesh.DrawSubset(0);
  finally
    Device9.EndScene;
  end;

  Device9.Present(nil, nil, 0, nil);
end;

// 初始化
procedure Setup;
var
  mat: TD3DXMatrix;
begin
  if not InitD3D(800, 600, True, D3DFMT_X8R8G8B8) then
  begin
    MessageBox(GetActiveWindow, 'D3D创建化失败！', '', MB_OK);
    Exit;
  end;

  InitMeth;

  Device9.SetRenderState(D3DRS_LIGHTING, 0);
  Device9.SetRenderState(D3DRS_ZENABLE, 1);
  Device9.SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

  // 投影转换
  D3DXMatrixPerspectiveFovLH(mat, D3DX_PI * 0.5, d3dpp.BackBufferWidth / d3dpp.BackBufferHeight, 1, 100);
  Device9.SetTransform(D3DTS_PROJECTION, mat);
  
  gCamera := TCamera.Create(D3DXVector3(0, 0, -15));
end;

// 运行程序
procedure Run;
begin
  MessageLoop(RenderFrame, 10);
end;

// 清除
procedure Cleanup;
begin
  TermD3D;
  gCamera.Free;
end;

begin
  Setup;
  Run;
  Cleanup;
end.
