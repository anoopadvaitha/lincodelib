{*****************************************************************************
 * brief: Kama游戏开发库，测试消息循环
 *
 * autor: linzhenqun
 * date:  2009-7-6
 * email: linzhenqun@gmail.com
*****************************************************************************}
unit TestMessageLoop;

interface
uses
  SysUtils, Windows, Messages, KmWndClasses;

procedure DoTestMessageLoop;

implementation

type
  TWndProc = function (Window: HWND; Message, WParam: Longint; LParam: Longint): Longint; stdcall;

// 注册窗口类
function RegisteWndClass(szClsName: PChar; WndProc: TWndProc): Boolean;
var
  wc: TWndClass;
begin
  wc.style := CS_HREDRAW or CS_VREDRAW;
  wc.lpfnWndProc := @WndProc;
  wc.cbClsExtra := 0;
  wc.cbWndExtra := 0;
  wc.hInstance := HInstance;
  wc.hIcon := LoadIcon(0, IDI_APPLICATION);
  wc.hCursor := LoadCursor(0, IDC_ARROW);
  wc.hbrBackground := GetStockObject(WHITE_BRUSH);
  wc.lpszClassName := szClsName;

  Result := RegisterClass(wc) <> 0;
end;

// 创建窗口
function CreateWindowHandle(szClsName, szTitle: PChar; nWidth, nHeight: Integer): THandle;
begin
  Result := CreateWindow(szClsName, szTitle, WS_OVERLAPPEDWINDOW, 100, 100, nWidth, nHeight, 0,
    0, HInstance, nil);
end;

var
  gHand: THandle;
  gIdleCount: Cardinal;
  gMessage: Cardinal;

// 事件接收类
type
  TMsgEvent = class
  public
    procedure OnMessage(var Msg: TMsg; var Handled: Boolean);
    procedure OnIdle(Sender: TObject; var Done: Boolean);
  end;

{ TMsgEvent }

procedure TMsgEvent.OnIdle(Sender: TObject; var Done: Boolean);
begin
  Inc(gIdleCount);
end;

procedure TMsgEvent.OnMessage(var Msg: TMsg; var Handled: Boolean);
begin
  gMessage := Msg.message;
end;


// 窗口过程
function WindowProc(Window: HWND; Message, WParam: Longint; LParam: Longint): Longint; stdcall;
var
  ps: TPaintStruct;
  dc: HDC;
  str: string;
begin
  case Message of
    WM_CREATE:
    begin
      SetTimer(Window, 1, 100, nil);
      Exit(0);
    end;
    WM_DESTROY:
    begin
      KillTimer(gHand, 1);
      PostQuitMessage(0);
      Exit(0);
    end;
    WM_TIMER:
    begin
      InvalidateRect(Window, nil, False);
      Exit(0);
    end;
    WM_PAINT:
    begin
      dc := BeginPaint(Window, ps);
      str := Format('Idle count: %d', [gIdleCount]);
      TextOut(dc, 10, 10, PChar(str), Length(str));
      str := Format('OnMessage: %d', [gMessage]);
      TextOut(dc, 10, 30, PChar(str), Length(str));
      EndPaint(Window, ps);
      Exit(0);
    end;
    WM_LBUTTONDOWN:
    begin
      SetWindowLong(gHand, GWL_STYLE, GetWindowLong(gHand, GWL_STYLE) and not WS_SYSMENU);
      SetWindowPos(gHand, 0, 0, 0, 0, 0, SWP_FRAMECHANGED or SWP_NOSIZE or SWP_NOMOVE or SWP_NOZORDER);
    end;
    WM_KEYDOWN:
    begin
      SetWindowLong(gHand, GWL_STYLE, GetWindowLong(gHand, GWL_STYLE) and not WS_SYSMENU);
      SetWindowPos(gHand, 0, 0, 0, 0, 0, SWP_FRAMECHANGED or SWP_NOSIZE or SWP_NOMOVE or SWP_NOZORDER);
    end;
  end;
  Result := DefWindowProc(Window, Message, WParam, LParam);
end;

// 测试消息循环
procedure DoTestMessageLoop;
var
  szClsName: array [0..63] of Char;
  MsgLoop: TMessageLoop;
  Event: TMsgEvent;
begin
  szClsName := 'Kama Test MesageLoop';
  if not RegisteWndClass(szClsName, WindowProc) then
  begin
    MessageBox(0, 'RegisteClass failed', '', MB_OK);
    Exit;
  end;

  gHand := CreateWindowHandle(szClsName, szClsName, 600, 480);
  if gHand = 0 then
  begin
    MessageBox(0, 'CreateWindow failed', '', MB_OK);
    Exit;
  end;

  ShowWindow(gHand, SW_SHOWNORMAL);
  UpdateWindow(gHand);

  MsgLoop := TMessageLoop.Create;
  Event := TMsgEvent.Create;
  try
    MsgLoop.OnIdle := Event.OnIdle;
    MsgLoop.OnMessageFilter := Event.OnMessage;
    MsgLoop.Run;
  finally
    MsgLoop.Free;
    Event.Free;
  end;
end;

end.
