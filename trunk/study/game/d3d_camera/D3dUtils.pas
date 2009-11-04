unit D3dUtils;

interface
uses
  Direct3D9, D3DX9, Windows, Messages, Classes, SysUtils, MMSystem;

type
  // 渲染回调
  TRenderFrame = procedure(dwTime: Cardinal);

// 初始化D3D
function InitD3D(Width, Height: Integer; bWindowed: Boolean; Format: TD3DFormat): Boolean;

// 终止D3D
procedure TermD3D;

// 消息循环
procedure MessageLoop(RenderFrame: TRenderFrame; nFrameTime: Cardinal);

// 浮点数转为整数
function SingleToDWord(v: Single): DWORD;

// 浮点数随机值
function RandomFloat(LowValue: Single; HighValue: Single): Single;

// 随机向量
procedure RandomVector(out VecOut: TD3DXVector3; const VecMin, VecMax: TD3DXVector3);

// 判断某个键是否按下
function IsKeyDown(vKey: Word): Boolean;

var
  D3D9: IDirect3D9;                 // D3D接口
  Device9: IDirect3DDevice9;        // D3D设备接口
  d3dpp: TD3DPresentParameters;     // D3D参数
  Wnd: THandle;                     // 窗口句柄

implementation

function IsKeyDown(vKey: Word): Boolean;
begin
  Result := GetAsyncKeyState(vKey) < 0;
end;

function SingleToDWord(v: Single): DWORD;
begin
  Result := PDWORD(@v)^;
end;

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
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    Width,
    Height,
    0,
    0,
    HInstance,
    nil);
end;

procedure MessageLoop(RenderFrame: TRenderFrame; nFrameTime: Cardinal);
var
  msg: TMsg;
  nCurrTime: Cardinal;
  nLastTime: Cardinal;
  nTime: Cardinal;
begin
  nLastTime := timeGetTime;
  while True do
  begin
    if PeekMessage(msg, 0, 0, 0, PM_REMOVE) then
    begin
      if msg.message <> WM_QUIT then
      begin
        TranslateMessage(msg);
        DispatchMessage(msg);
      end
      else
        Break;
    end
    else begin
      nCurrTime := timeGetTime;
      nTime := nCurrTime - nLastTime;
      if nTime > nFrameTime then
      begin
        RenderFrame(nTime);
        nLastTime := nCurrTime;
      end;
      
    end;
  end;
end;

// 窗口过程
function WndProc(hwnd: THandle; Msg: UINT; wParam: LongInt; lParam: LongInt): HRESULT; stdcall;
var
  ps: TPaintStruct;
begin
  case Msg of
    WM_CREATE:
    begin
      SetTimer(hwnd, 1, 10, nil);
    end;
    WM_PAINT:
    begin
      BeginPaint(hwnd, ps);
      EndPaint(hwnd, ps);
      Result := 0;
    end;
    WM_DESTROY:
    begin
      KillTimer(hwnd, 1);
      PostQuitMessage(0);
      Result := 0;
    end;
    else
      Result := DefWindowProc(hwnd, Msg, wParam, lParam);
  end;
end;

procedure TermD3D;
begin
  Device9 := nil;
  D3D9 := nil;
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

  Result := True;
end;

end.
