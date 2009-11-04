unit D3dUtils;

{
  �õ�Ԫ��ɵĹ��ܣ�
  1���������ڣ�CreateWnd
  2����Ϣѭ����MessageLoop
  3����ʼ��D3D��InitD3D
  4��ȫ���봰��ģʽ�л��� FullScreenSwitch
  5��D3D��飺DoCheckAdpter
  6���豸��ʧʱ���Իָ�������ResetDevice
  7��ͼԪ����InitVertex, DrawPrimitive
  8�����ã�SetViewPort
}

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

  VertexBuf: IDirect3DVertexBuffer9;  // �����б�
  VertexBuf2: IDirect3DVertexBuffer9; // �����б�
  TeapotMesh: ID3DXMesh;              // ���
  Tex: IDirect3DTexture9;             // ����


implementation

uses
  Windows, Messages, CnDebug;

type
  TUpdateFrame = procedure;
procedure FullScreenSwitch; forward;

//------------------------------------------------------------------------------
// �豸���
procedure EnumAdpterModes(Adapter: UINT);
var
  nMode: Integer;

  procedure DoEnumAdpterModes(Adpater, nMode: UINT; Format: TD3DFormat);
  var
    i: Integer;
    pMode: TD3DDisplayMode;
  begin
    for i := 0 to nMode - 1 do
    begin
      D3D9.EnumAdapterModes(Adpater, Format, i, pMode);
      CnDebugger.LogFmt('    Mode: %d; Width: %d; Heihgt: %d; RefreshRate: %d',
        [i, pMode.Width, pMode.Height, pMode.RefreshRate]);
    end;
  end;
begin
  // D3DFMT_A1R5G5B5
  nMode := D3D9.GetAdapterModeCount(Adapter, D3DFMT_A1R5G5B5);
  CnDebugger.LogFmt('  D3DFMT_A1R5G5B5''s Adapter mode count: %d', [nMode]);
  DoEnumAdpterModes(Adapter, nMode, D3DFMT_A1R5G5B5);

  // D3DFMT_A2R10G10B10
  nMode := D3D9.GetAdapterModeCount(Adapter, D3DFMT_A2R10G10B10);
  CnDebugger.LogFmt('  D3DFMT_A2R10G10B10''s Adapter mode count: %d', [nMode]);
  DoEnumAdpterModes(Adapter, nMode, D3DFMT_A2R10G10B10);

  // D3DFMT_A8R8G8B8
  nMode := D3D9.GetAdapterModeCount(Adapter, D3DFMT_A8R8G8B8);
  CnDebugger.LogFmt('  D3DFMT_A8R8G8B8''s Adapter mode count: %d', [nMode]);
  DoEnumAdpterModes(Adapter, nMode, D3DFMT_A8R8G8B8);

  // D3DFMT_R5G6B5
  nMode := D3D9.GetAdapterModeCount(Adapter, D3DFMT_R5G6B5);
  CnDebugger.LogFmt('  D3DFMT_R5G6B5''s Adapter mode count: %d', [nMode]);
  DoEnumAdpterModes(Adapter, nMode, D3DFMT_R5G6B5);

  // D3DFMT_X1R5G5B5
  nMode := D3D9.GetAdapterModeCount(Adapter, D3DFMT_X1R5G5B5);
  CnDebugger.LogFmt('  D3DFMT_X1R5G5B5''s Adapter mode count: %d', [nMode]);
  DoEnumAdpterModes(Adapter, nMode, D3DFMT_X1R5G5B5);

  // D3DFMT_X8R8G8B8
  nMode := D3D9.GetAdapterModeCount(Adapter, D3DFMT_X8R8G8B8);
  CnDebugger.LogFmt('  D3DFMT_X8R8G8B8''s Adapter mode count: %d', [nMode]);
  DoEnumAdpterModes(Adapter, nMode, D3DFMT_X8R8G8B8);
end;

procedure CheckCurDisplayMode(Adapter: UINT);
var
  pMode: TD3DDisplayMode;
begin
  D3D9.GetAdapterDisplayMode(Adapter, pMode);
  CnDebugger.LogFmt('  Current Display mode: Width: %d; Heihgt: %d; RefreshRate: %d' +
    ' Format: %d',  [pMode.Width, pMode.Height, pMode.RefreshRate, Integer(pMode.Format)]);
end;

procedure CheckDeviceType(Adapter: UINT);
var
  bSucc: Boolean;
begin
  bSucc := D3D9.CheckDeviceType(Adapter, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, False) = S_OK;
  if bSucc then
    CnDebugger.LogMsg('  CheckDeviceType Support: D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8')
  else
    CnDebugger.LogMsg('  CheckDeviceType not Support: D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8');

  bSucc := D3D9.CheckDeviceType(Adapter, D3DDEVTYPE_HAL, D3DFMT_A1R5G5B5, D3DFMT_A1R5G5B5, False) = S_OK;
  if bSucc then
    CnDebugger.LogMsg('  CheckDeviceType Support: D3DDEVTYPE_HAL, D3DFMT_A1R5G5B5')
  else
    CnDebugger.LogMsg('  CheckDeviceType not Support: D3DDEVTYPE_HAL, D3DFMT_A1R5G5B5');

  bSucc := D3D9.CheckDeviceType(Adapter, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, D3DFMT_A8R8G8B8, False) = S_OK;
  if bSucc then
    CnDebugger.LogMsg('  CheckDeviceType Support: D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8')
  else
    CnDebugger.LogMsg('  CheckDeviceType not Support: D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8');

  bSucc := D3D9.CheckDeviceType(Adapter, D3DDEVTYPE_HAL, D3DFMT_R5G6B5, D3DFMT_R5G6B5, False) = S_OK;
  if bSucc then
    CnDebugger.LogMsg('  CheckDeviceType Support: D3DDEVTYPE_HAL, D3DFMT_R5G6B5')
  else
    CnDebugger.LogMsg('  CheckDeviceType not Support: D3DDEVTYPE_HAL, D3DFMT_R5G6B5');

end;

procedure CheckCaps(Adapter: UINT);
var
  Caps: TD3DCaps9;
begin
  D3D9.GetDeviceCaps(Adapter, D3DDEVTYPE_HAL, Caps);
  if Caps.DevCaps and D3DDEVCAPS_HWTRANSFORMANDLIGHT <> 0 then
    CnDebugger.LogMsg('  Support D3DDEVTYPE_HAL')
  else
    CnDebugger.LogMsg('  not Support D3DDEVTYPE_HAL');
end;  

// ����豸
procedure DoCheckAdpter;
var
  i, c: Integer;
begin
  c := D3D9.GetAdapterCount;
  CnDebugger.LogFmt('Adapter count: %d', [c]);
  for i := 0 to c - 1 do
  begin
    EnumAdpterModes(i);
    CheckCurDisplayMode(i);
    CheckDeviceType(i);
    // �Ƿ�֧�ֶ�������
    CheckCaps(i);
  end;
end;

// �Ƿ�֧��Ӳ����������
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
// ��������
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

// ��Ϣѭ��
procedure MessageLoop(UpdateFrame: TUpdateFrame; dwFrameTime: Cardinal);
var
  msg: TMsg;
  dwTick: Cardinal;
begin
  dwTick := 0;
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
      if GetTickCount - dwTick > dwFrameTime then
      begin
        UpdateFrame;
        dwTick := GetTickCount;
      end;
      Sleep(10);
    end;
  end;
end;

// ���ڹ���
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
      // ȫ���봰��ģʽ֮����л�
      FullScreenSwitch;
      Result := 0;
    end;    
    else
      Result := DefWindowProc(hwnd, Msg, wParam, lParam);
  end;
end;

//------------------------------------------------------------------------------
// ȫ���л�
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
// ��ʼ��D3D
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
  d3dpp.AutoDepthStencilFormat := D3DFMT_D24S8;

  // Check Vertex processing
  if SupportHWVertexProcess then
    Flag := D3DCREATE_HARDWARE_VERTEXPROCESSING
  else
    Flag := D3DCREATE_SOFTWARE_VERTEXPROCESSING;

  // Create Device by default adapter.
  if Failed(D3D9.CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Wnd, Flag, @d3dpp, Device9)) then
    Exit;

  // ���ñ�������
  Device9.SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  // ������Ȼ���
  Device9.SetRenderState(D3DRS_ZENABLE, 1);
  Device9.SetRenderState(D3DRS_LIGHTING, 0);

  Result := True;
end;

//------------------------------------------------------------------------------
// �����ӿ�
procedure SetViewPort;
var
  vp: D3DVIEWPORT9;
begin
  vp.X := 0;
  vp.Y := 0;
  vp.Width := d3dpp.BackBufferWidth;
  vp.Height := d3dpp.BackBufferHeight;
  vp.MinZ := 0;
  vp.MaxZ := 1;
  Device9.SetViewport(vp);
end;

//------------------------------------------------------------------------------
// ��Ⱦ
type
  PD3DVertex = ^TD3DVertex;
  TD3DVertex = packed record
    x, y, z: Single;
    Color: DWORD;
  end;
const
  D3DFVF_CUSTOM = D3DFVF_XYZ or D3DFVF_DIFFUSE;

type
  PD3DVertex2 = ^TD3DVertex2;
  TD3DVertex2 = packed record
    x, y, z: Single;
  end;

// �����任: �����ת
procedure SetupTransform;
var
  uTime: LongWord;
  fAngle: Single;
  mat: TD3DMatrix;
  eye, LookAt, Up: TD3DVector;
begin
//   ����ת��, �Զ���ת
//  uTime := GetTickCount mod 2000;
//  fAngle := uTime * 2 * D3DX_PI / 2000;
//  D3DXMatrixRotationY(mat, fAngle);
//  Device9.SetTransform(D3DTS_WORLD, mat);

  // ���ת��
  eye := D3DXVector3(0, 2, -20);
  LookAt := D3DXVector3(0, 0, 0);
  Up := D3DXVector3(0, 1, 0);
  D3DXMatrixLookAtLH(mat, eye, LookAt, Up);
  Device9.SetTransform(D3DTS_VIEW, mat);

  // ͶӰת��
  D3DXMatrixPerspectiveFovLH(mat, D3DX_PI * 0.5, d3dpp.BackBufferWidth / d3dpp.BackBufferHeight, 1, 100);
  Device9.SetTransform(D3DTS_PROJECTION, mat);
end;

// �����任: �����ת
procedure SetupTransform2;
var
  uTime: LongWord;
  fAngle, x, z: Single;
  mat: TD3DMatrix;
  eye, LookAt, Up: TD3DVector;
begin
  // ���
  uTime := GetTickCount mod 2000;
  fAngle := uTime * 2 * D3DX_PI / 2000;

  // �۲�����ת��
  x := 4 * Cos(fAngle);
  z := 4 * Sin(fAngle);
  eye := D3DXVector3(x, 2, z);
  LookAt := D3DXVector3(0, 0, 0);
  Up := D3DXVector3(0, 1, 0);
  D3DXMatrixLookAtLH(mat, eye, LookAt, Up);
  Device9.SetTransform(D3DTS_VIEW, mat);

  // ͶӰת��
  D3DXMatrixPerspectiveFovLH(mat, D3DX_PI * 0.5, d3dpp.BackBufferWidth / d3dpp.BackBufferHeight, 1, 100);
  Device9.SetTransform(D3DTS_PROJECTION, mat);
end;

// ����
procedure SetLight;
var
  mtr: TD3DMaterial9;
  light: TD3DLight9;
  vecDir: TD3DVector;
begin
  // ����
  FillChar(mtr, SizeOf(mtr), 0);
  // ������
  mtr.Ambient.r := 1;
  mtr.Ambient.g := 1;
  mtr.Ambient.b := 1;
  mtr.Ambient.a := 1;
  // �����
  mtr.Diffuse.r := 1;
  mtr.Diffuse.g := 1;
  mtr.Diffuse.b := 1;
  mtr.Diffuse.a := 1;
  Device9.SetMaterial(mtr);

  // ������
  Device9.SetRenderState(D3DRS_AMBIENT, $00405B43);

  // ����⣬ ��ɫ��
  FillChar(light, SizeOf(light), 0);
  light._Type := D3DLIGHT_DIRECTIONAL;
  light.Diffuse.r := 0;
  light.Diffuse.g := 1;
  light.Diffuse.b := 0;
  light.Diffuse.a := 1;
  vecDir := D3DXVector3(10, 0, 4);
  D3DXVec3Normalize(light.Direction, vecDir);
  Device9.SetLight(0, light);
  Device9.LightEnable(0, True);

  Device9.SetRenderState(D3DRS_LIGHTING, 1);
end;

// ��ͼԪ
procedure DrawObject;
begin
  if Succeeded(Device9.BeginScene) then
  try
    // ��ͼԪ�� û������ת��
    //Device9.SetTexture(0, Tex);
    if Failed(Device9.SetStreamSource(0, VertexBuf, 0, SizeOf(TD3DVertex))) then
      Exit;
    if Failed(Device9.SetFVF(D3DFVF_CUSTOM)) then
      Exit;
    Device9.DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    //Device9.SetTexture(0, nil);

    // ����ת��
    SetupTransform;

    // ����
    //SetLight;

    // �����
    //TeapotMesh.DrawSubset(0);
  finally
    Device9.EndScene;
  end;
end;

// ����֡
procedure UpdateFrame;
begin
  if bReseting then
  begin
    ResetDevice;
    Exit;
  end;

  // ������滺�壬 ��Ȼ��壬 ģ�建��
  Device9.Clear(0, nil, D3DCLEAR_TARGET or D3DCLEAR_ZBUFFER or D3DCLEAR_STENCIL,
    D3DCOLOR_ARGB(255, 255, 0, 0), 1, 0);

  // �����
  DrawObject;

  // ���豸��ʧʱ��Present�᷵��D3DERR_DEVICELOST����ʱӦ�ý����豸�����ô���
  // ʲôʱ���豸�ᶪʧ��һ�����͵����Ӿ���ȫ��ʱ��ͨ��Alt+Tab�л����ڱ����֡�
  if Device9.Present(nil, nil, 0, nil) = D3DERR_DEVICELOST then
    bReseting := True;
end;

// ��ʼ������
procedure InitLineVertex;
var
  vts: PChar;
begin
  if Failed(Device9.CreateVertexBuffer(4 * SizeOf(TD3DVertex2), 0, D3DFVF_XYZ,
    D3DPOOL_MANAGED, VertexBuf2, nil)) then
      Exit;

  if Succeeded(VertexBuf2.Lock(0, 4 * SizeOf(TD3DVertex2), Pointer(vts), 0)) then
  try
    with PD3DVertex2(vts)^ do
    begin
      x := -10;
      y := 0;
      z := 0;
    end;

    Inc(vts, SizeOf(TD3DVertex2));
    with PD3DVertex2(vts)^ do
    begin
      x := 10;
      y := 0;
      z := 0;
    end;

    Inc(vts, SizeOf(TD3DVertex2));
    with PD3DVertex2(vts)^ do
    begin
      x := 0;
      y := 0;
      z := -10;
    end;

    Inc(vts, SizeOf(TD3DVertex2));
    with PD3DVertex2(vts)^ do
    begin
      x := 0;
      y := 0;
      z := 10;
    end;
  finally
    VertexBuf2.Unlock;
  end;
end;

// ��ʼ������
procedure InitVertex;
var
  vts: PChar;
begin
  if Failed(Device9.CreateVertexBuffer(4 * SizeOf(TD3DVertex), 0, D3DFVF_CUSTOM,
    D3DPOOL_MANAGED, VertexBuf, nil)) then
      Exit;

  if Succeeded(VertexBuf.Lock(0, 3 * SizeOf(TD3DVertex), Pointer(vts), 0)) then
  try
    with PD3DVertex(vts)^ do
    begin
      x := -10;
      y := 0;
      z := 1;
      Color := $ff00ff00;
    end;

    Inc(vts, SizeOf(TD3DVertex));
    with PD3DVertex(vts)^ do
    begin
      x := 0;
      y := 10;
      z := 1;
      Color := $ff00ff00;
    end;

    Inc(vts, SizeOf(TD3DVertex));
    with PD3DVertex(vts)^ do
    begin
      x := 10;
      y := 0;
      z := 1;
      Color := $ff00ff00;
    end;

  finally
    VertexBuf.Unlock;
  end;
end;

// �������
procedure InitTeapot;
begin
  D3DXCreateTeapot(Device9, TeapotMesh, nil);
end;

// ��������
procedure InitTexture;
begin
  D3DXCreateTextureFromFile(Device9, 'banana.bmp', Tex);
end;

//------------------------------------------------------------------------------
// �����豸
procedure ResetDevice;
begin
  // Ҫ�ȵ�����D3DERR_DEVICENOTRESET���ſ�������Reset��
  if Device9.TestCooperativeLevel = D3DERR_DEVICENOTRESET then
  begin
    if Succeeded(Device9.Reset(d3dpp)) then
    begin
      bReseting := False;
    end;
  end;
end;

//------------------------------------------------------------------------------
// ��ʼ��
procedure Setup;
begin
  // �����ȫ���´���D3DFMT_UNKNOWN��������ʧ�ܣ�
  // ���ڴ���ģʽ�´���D3DFMT_UNKNOWN��ɹ����������л�ȫ��ʱҲû������
  if not InitD3D(800, 600, True, D3DFMT_X8R8G8B8) then
  begin
    MessageBox(GetActiveWindow, 'D3D������ʧ�ܣ�', '', MB_OK);
    Exit;
  end;

  // �����ӿ�
  //SetViewPort;

  // ��ʼ�������б�
  InitVertex;
  InitLineVertex;
  InitTexture;
  InitTeapot;
end;

// ���
procedure Cleanup;
begin
end;

// ���г���
procedure Run;
begin
  MessageLoop(UpdateFrame, 30);
end;
//------------------------------------------------------------------------------

end.
