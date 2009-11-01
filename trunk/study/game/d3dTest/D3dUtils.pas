unit D3dUtils;

{
  该单元完成的功能：
  1、创建窗口：CreateWnd
  2、消息循环：MessageLoop
  3、初始化D3D：InitD3D
  4、全屏与窗口模式切换： FullScreenSwitch
  5、D3D检查：DoCheckAdpter
  6、设备丢失时尝试恢复回来：ResetDevice
  7、图元绘制InitVertex, DrawPrimitive
  8、剪裁：SetViewPort
}

interface
uses
  Direct3D9;

procedure Setup;
procedure Run;
procedure Cleanup;

var
  D3D9: IDirect3D9;
  Device9: IDirect3DDevice9;
  d3dpp: TD3DPresentParameters;
  Wnd: THandle;
  bReseting: Boolean;
  bFullScreen: Boolean;
  VertexBuf: IDirect3DVertexBuffer9;

implementation

uses
  Windows, Messages, CnDebug;

type
  TUpdateFrame = procedure;
procedure FullScreenSwitch; forward;

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
  wc.hbrBackground := GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName := '';
  wc.lpszClassName := szClsName;
  wc.hIconSm := 0;

  if RegisterClassEx(wc) = 0 then
    Exit;

  Result := CreateWindow(szClsName, szClsName, WS_OVERLAPPED or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX, 100, 100,
    Width, Height, 0, 0, HInstance, nil);
end;

// 消息循环
procedure MessageLoop(UpdateFrame: TUpdateFrame);
var
  msg: TMsg;
begin
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
      UpdateFrame;
      WaitMessage;
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

// 检查设备
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
    // 是否支持顶点运算
    CheckCaps(i);
  end;
end;

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

function InitD3D(Width, Height: Integer; bWindowed: Boolean; Format: TD3DFormat): Boolean;
var
  Flag: Cardinal;
begin
  Result := False;

  // Create window
  Wnd := CreateWnd(@WndProc, Width, Height);
  if Wnd = 0 then
    Exit;

  // Show window
  ShowWindow(Wnd, SW_SHOWNORMAL);
  UpdateWindow(Wnd);


  // Create IDirect3D9
  D3D9 := Direct3DCreate9(D3D_SDK_VERSION);
  if D3D9 = nil then
    Exit;

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

  // Check Vertex processing
  if SupportHWVertexProcess then
    Flag := D3DCREATE_HARDWARE_VERTEXPROCESSING
  else
    Flag := D3DCREATE_SOFTWARE_VERTEXPROCESSING;

  // Create Device by default adapter.
  if Failed(D3D9.CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Wnd, Flag, @d3dpp, Device9)) then
    Exit;

  Result := True;
end;

procedure ResetDevice;
begin
  // 要等到返回D3DERR_DEVICENOTRESET，才可以试着Reset。
  if Device9.TestCooperativeLevel = D3DERR_DEVICENOTRESET then
  begin
    if Succeeded(Device9.Reset(d3dpp)) then
      bReseting := False;
  end;
end;

type
  PD3DVertex = ^TD3DVertex;
  TD3DVertex = packed record
    x, y, z, rhw: Single;
    color: DWORD;
  end;

procedure DrawPrimitive;
begin
  if Succeeded(Device9.BeginScene) then
  try
    if Failed(Device9.SetStreamSource(0, VertexBuf, 0, SizeOf(TD3DVertex))) then
      Exit;

    if Failed(Device9.SetFVF(D3DFVF_XYZRHW or D3DFVF_DIFFUSE)) then
      Exit;

    Device9.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
  finally
    Device9.EndScene;
  end;
end;  

// 更新
procedure UpdateFrame;
begin
  if bReseting then
  begin
    ResetDevice;
    Exit;
  end;

  // draw scene
  Device9.Clear(0, nil, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 0, 0), 1, 0);

  // draw Primitive
  DrawPrimitive;

  // 当设备丢失时，Present会返回D3DERR_DEVICELOST，此时应该进行设备重设置处理
  // 什么时候设备会丢失，一个典型的例子就是全屏时，通过Alt+Tab切换窗口便会出现。
  if Device9.Present(nil, nil, 0, nil) = D3DERR_DEVICELOST then
    bReseting := True;
end;

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

procedure InitVertex;
var
  vts: PChar;
begin
  if Failed(Device9.CreateVertexBuffer(4 * SizeOf(TD3DVertex), 0, D3DFVF_XYZRHW or D3DFVF_DIFFUSE,
    D3DPOOL_MANAGED, VertexBuf, nil)) then
      Exit;

  if Succeeded(VertexBuf.Lock(0, 4 * SizeOf(TD3DVertex), Pointer(vts), 0)) then
  try
    with PD3DVertex(vts)^ do
    begin
      x := 0-0.5;
      y := 0-0.5;
      z := 0;
      rhw := 1;
      color := $ff00ff00;
    end;

    Inc(vts, SizeOf(TD3DVertex));
    with PD3DVertex(vts)^ do
    begin
      x := 200-0.5;
      y := -0.5;
      z := 0;
      rhw := 1;
      color := $ff0000ff;
    end;

    Inc(vts, SizeOf(TD3DVertex));
    with PD3DVertex(vts)^ do
    begin
      x := -0.5;
      y := 200-0.5;
      z := 0.5;
      rhw := 1;
      color := $ff00ff00;
    end;

    Inc(vts, SizeOf(TD3DVertex));
    with PD3DVertex(vts)^ do
    begin
      x := 200-0.5;
      y := 200-0.5;
      z := 0.5;
      rhw := 1;
      color := $ff0000ff;
    end;

  finally
    VertexBuf.Unlock;
  end;
end;

//------------------------------------------------------------------------------
// 初始化
procedure Setup;
begin
end;

// 清除
procedure Cleanup;
begin
end;

// 运行程序
procedure Run;
begin
  if not InitD3D(800, 600, True, D3DFMT_X8R8G8B8) then
  begin
    MessageBox(GetActiveWindow, 'D3D创建化失败！', '', MB_OK);
    Exit;
  end;

  SetViewPort;

  InitVertex;
  
  MessageLoop(UpdateFrame);
end;
//------------------------------------------------------------------------------

end.
