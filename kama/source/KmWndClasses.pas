{*****************************************************************************
 * brief: Kama游戏开发库，窗口和消息的封装类
 *
 * autor: linzhenqun
 * date:  2009-7-5
 * email: linzhenqun@gmail.com
*****************************************************************************}

unit KmWndClasses;

interface
uses
  Windows, Messages, SysUtils, Classes;

const
  cWndClassName         = 'Kama.TWindow';     // 窗口类名

  KMWM_BASE             = $A000;              // 消息基值


type
  // 消息过滤事件，可以在这里阻断消息的派发
  TMessageFilterEvent = procedure (var Msg: TMsg; var Handled: Boolean) of object;
  // 空闲事件
  TIdleEvent = procedure (Sender: TObject; var Done: Boolean) of object;

  { 消息循环类 }
  TMessageLoop = class
  private
    FOnIdle: TIdleEvent;
    FOnMessageFilter: TMessageFilterEvent;
    FTerminated: Boolean;
  protected
    procedure DoMessageFilter(var Msg: TMsg; var Handled: Boolean); virtual;
    procedure DoIdle(Done: Boolean); virtual;
    procedure Idle; virtual;
  public
    // 开始执行消息环境，当Terminated为True时结束
    // 当Terminated在两种情况下会为True：
    // 1、收到WM_QUIT时。
    // 2、手动设为True。
    procedure Run; virtual;
    // 处理一次消息队列里的消息，当消息队列没有消息时，会触发一次Idle事件
    procedure HandleMessage;
    // 循环处理消息队列里的消息，直到队列没有消息为止，不会触发Idle事件
    procedure ProcessMessages;
    // 处理一次消息队列里的消息
    function ProcessMessage(var Msg: TMsg): Boolean;
    // 结束程序
    procedure Terminate;

    property OnIdle: TIdleEvent read FOnIdle write FOnIdle;
    property OnMessageFilter: TMessageFilterEvent read FOnMessageFilter write FOnMessageFilter;
    property Terminated: Boolean read FTerminated;
  end;

  // 标题栏按钮
  TBorderIcon = (
    biSystemMenu,   // 系统菜单
    biMinimize,     // 最小化按钮
    biMaximize      // 最大化按钮
  );

  // 标题栏按钮集合
  TBorderIcons = set of TBorderIcon;

  // 边框风格
  TBorderStyle = (
    bsNone,          // 无边框
    bsSingle,        // 细边框，不可拉动大小
    bsSizeable       // 厚边框，可以拉动大小
    );

  // 窗口状态
  TWindowState = (
    wsNormal,         // 正常
    wsMinimized,      // 最小化
    wsMaximized       // 最大化
  );

  // 关闭询问事件
  TCloseQueryEvent = procedure(Sender: TObject; var CanClose: Boolean) of object;

  { 窗口封装类:
    这里只根据游戏程序的需要，对顶层窗口作简单的封装
    该类与窗口句柄的生命周期一一对应，即：
      该类创建时，窗口句柄也创建
      该类消毁时，窗口句柄也消毁
      窗口句柄消毁时，该类也消毁}
  TWindow = class
  private
    FProcThunk: Pointer;
    FWindowProc: TWndMethod;
    FDestroying: Boolean;
    FHandle: THandle;
    FHeight: Integer;
    FLeft: Integer;
    FTop: Integer;
    FWidth: Integer;
    FOnCreate: TNotifyEvent;
    FOnDestroy: TNotifyEvent;
    FOnShow: TNotifyEvent;
    FOnHide: TNotifyEvent;
    FOnSizeChange: TNotifyEvent;
    FOnClose: TNotifyEvent;
    FOnQueryClose: TCloseQueryEvent;
    procedure SetBorderIcons(const Value: TBorderIcons);
    procedure SetBorderStyle(const Value: TBorderStyle);
    procedure SetCaption(const Value: string);
    procedure SetEnable(const Value: Boolean);
    procedure SetTopMost(const Value: Boolean);
    procedure SetVisible(const Value: Boolean);
    function GetEnable: Boolean;
    function GetTopMost: Boolean;
    function GetBorderIcons: TBorderIcons;
    function GetBorderStyle: TBorderStyle;
    function GetCaption: string;
    function GetVisible: Boolean;
    function GetWindowState: TWindowState;
    procedure SetHeight(const Value: Integer);
    procedure SetLeft(const Value: Integer);
    procedure SetTop(const Value: Integer);
    procedure SetWidth(const Value: Integer);
    function GetClientHeight: Integer;
    function GetClientSize: TPoint;
    procedure SetClientSize(Value: TPoint);
    function GetClientWidth: Integer;
    procedure SetClientHeight(const Value: Integer);
    procedure SetClientWidth(const Value: Integer);
    procedure SetWindowState(const Value: TWindowState);
  protected
    function StyleToBorderStyle(Style: LongWord): TBorderStyle;
    function StyleToBorderIcons(Style: LongWord): TBorderIcons;
    function BorderIconsToStyle(OldStyle: LongWord; BIS: TBorderIcons): LongWord;
    function BorderStyleToStyle(OldStyle: LongWord; BS: TBorderStyle): LongWord;
    function RegisterWndClass: Boolean;
    function CreateWndHandle(ALeft, ATop, AWidth, AHeight: Integer;
      BIS: TBorderIcons; BS: TBorderStyle; Caption: string): Boolean;
    procedure MainWndProc(var Message: TMessage);
    procedure WndProc(var Message: TMessage); virtual;
    procedure WMClose(var Message: TWMClose); message WM_CLOSE;
    procedure WMSize(var Message: TWMSize); message WM_SIZE;
    procedure WMMove(var Message: TWMMove); message WM_MOVE;
    procedure WMNCDestroy(var Message: TWMNCDestroy); message WM_NCDESTROY;
    procedure UpdateBounds;
    function CloseQuery: Boolean; dynamic;
    procedure DoShow; dynamic;
    procedure DoHide; dynamic;
    procedure DoClose; dynamic;
    procedure DoSizeChange; dynamic;
    procedure DoCreate; dynamic;
    procedure DoDestroy; dynamic;
  public
    // 构造函数，调用的时候窗口就创建了，参数很多，但都很容易理解
    constructor Create(
      ALeft: Integer = 0;
      ATop: Integer = 0;
      AWidth: Integer = 600;
      AHeight: Integer = 480;
      BIS: TBorderIcons = [biSystemMenu, biMinimize];
      BS: TBorderStyle = bsSingle;
      Caption: string = '';
      Icon: HICON = 0);
    destructor Destroy; override;
    procedure DefaultHandler(var Message); override;
    procedure Close;
    procedure Show(const WndState: TWindowState = wsNormal);
    procedure Hide;
    procedure BringToFront;
    procedure SendToBack;
    procedure SetBounds(ALeft, ATop, AWidth, AHeight: Integer); overload;
    procedure SetBounds(const Rect: TRect); overload;
    procedure AlignWindow(nPercentX: Integer = 50; nPercentY: Integer = 50);
    procedure AfterConstruction; override;
    procedure BeforeDestruction; override;

    property Enable: Boolean read GetEnable write SetEnable;
    property TopMost: Boolean read GetTopMost write SetTopMost;
    property Visible: Boolean read GetVisible write SetVisible;
    property BorderIcons: TBorderIcons read GetBorderIcons write SetBorderIcons;
    property BorderStyle: TBorderStyle read GetBorderStyle write SetBorderStyle;
    property WindowState: TWindowState read GetWindowState write SetWindowState;
    property Handle: THandle read FHandle;
    property Caption: string read GetCaption write SetCaption;
    property WindowProc: TWndMethod read FWindowProc write FWindowProc;
    property Left: Integer read FLeft write SetLeft;
    property Top: Integer read FTop write SetTop;
    property Width: Integer read FWidth write SetWidth;
    property Height: Integer read FHeight write SetHeight;
    property ClientSize: TPoint read GetClientSize write SetClientSize;
    property ClientHeight: Integer read GetClientHeight write SetClientHeight;
    property ClientWidth: Integer read GetClientWidth write SetClientWidth;
    property OnCreate: TNotifyEvent read FOnCreate write FOnCreate;
    property OnDestroy: TNotifyEvent read FOnDestroy write FOnDestroy;
    property OnClose: TNotifyEvent read FOnClose write FOnClose;
    property OnQueryClose: TCloseQueryEvent read FOnQueryClose write FOnQueryClose;
  end;

  { 主窗口 }
  TMainWindow = class(TWindow)
  protected
    procedure DoClose; override;
  end;

implementation

threadvar
  CreatingWnd: TWindow;     // 线程安全的考虑

function InitWndProc(hwnd: THandle; Message, WParam: Longint;
  LParam: Longint): Longint; stdcall;
var
  Wnd: TWindow;
begin
  //　Message是窗口的第一个消息
  Wnd := CreatingWnd;
  CreatingWnd := nil;
  Wnd.FHandle := hwnd;
  SetWindowLong(hwnd, GWL_WNDPROC, Longint(Wnd.FProcThunk));

  asm
    PUSH    LParam
    PUSH    WParam
    PUSH    Message
    PUSH    hwnd
    MOV     EAX, Wnd
    CALL    [EAX].TWindow.FProcThunk
    MOV     Result,EAX
  end;
end;

{ TMessageLoop }

procedure TMessageLoop.DoMessageFilter(var Msg: TMsg; var Handled: Boolean);
begin
  if Assigned(FOnMessageFilter) then
    FOnMessageFilter(Msg, Handled);
end;

procedure TMessageLoop.HandleMessage;
var
  Msg: TMsg;
begin
  if not ProcessMessage(Msg) then Idle;
end;

procedure TMessageLoop.Idle;
var
  Done: Boolean;
begin
  Done := True;
  DoIdle(Done);
  if Done then WaitMessage;
end;

procedure TMessageLoop.DoIdle(Done: Boolean);
begin
  if Assigned(FOnIdle) then
    FOnIdle(Self, Done);
end;

function TMessageLoop.ProcessMessage(var Msg: TMsg): Boolean;
var
  Handled: Boolean;
begin
  Result := False;
  if PeekMessage(Msg, 0, 0, 0, PM_REMOVE) then
  begin
    Result := True;
    if Msg.Message <> WM_QUIT then
    begin
      Handled := False;
      DoMessageFilter(Msg, Handled);
      if not Handled then
      begin
        TranslateMessage(Msg);
        DispatchMessage(Msg);
      end;
    end
    else
      FTerminated := True;
  end;
end;

procedure TMessageLoop.ProcessMessages;
var
  Msg: TMsg;
begin
  while ProcessMessage(Msg) do {loop};
end;

procedure TMessageLoop.Run;
begin
  repeat
    HandleMessage;
  until FTerminated;
end;

procedure TMessageLoop.Terminate;
begin
  PostQuitMessage(0);
end;

{ TWindow }

procedure TWindow.AfterConstruction;
begin
  DoCreate;
end;

procedure TWindow.AlignWindow(nPercentX, nPercentY: Integer);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if nPercentX < 0 then
    nPercentX := 0;
  if nPercentX > 100 then
    nPercentX := 100;
  if nPercentY < 0 then
    nPercentY := 0;
  if nPercentY > 100 then
    nPercentY := 100;

  SetBounds(
    (GetSystemMetrics(SM_CXSCREEN) - FWidth) * nPercentX div 100,
    (GetSystemMetrics(SM_CYSCREEN) - FHeight) * nPercentY div 100,
    FWidth,
    FHeight);
end;

procedure TWindow.BeforeDestruction;
begin
  DoDestroy;
end;

function TWindow.BorderIconsToStyle(OldStyle: LongWord;
  BIS: TBorderIcons): LongWord;
begin
  Result := OldStyle;
  Result := Result and not WS_SYSMENU;
  Result := Result and not WS_MINIMIZEBOX;
  Result := Result and not WS_MAXIMIZEBOX;
  if biSystemMenu in BIS then
  begin
    Result := Result or WS_SYSMENU;
    if biMinimize in BIS then Result := Result or WS_MINIMIZEBOX;
    if biMaximize in BIS then Result := Result or WS_MAXIMIZEBOX;
  end;
end;

function TWindow.BorderStyleToStyle(OldStyle: LongWord;
  BS: TBorderStyle): LongWord;
begin
  Result := OldStyle;
  Result := Result and not WS_THICKFRAME;
  Result := Result and not WS_BORDER;
  Result := Result and not WS_CAPTION;
  Result := Result and not WS_POPUP;
  if BS = bsNone then
    Result := Result or WS_POPUP
  else if BS = bsSingle then
    Result := Result or WS_CAPTION or WS_BORDER
  else if BS = bsSizeable then
    Result := Result or WS_CAPTION or WS_THICKFRAME;
end;

procedure TWindow.BringToFront;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  SetWindowPos(FHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE + SWP_NOSIZE);
end;

procedure TWindow.Close;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if CloseQuery then
  begin
    DoClose;
    Hide;
  end;
end;

function TWindow.CloseQuery: Boolean;
begin
  Result := True;
  if Assigned(FOnQueryClose) then
    FOnQueryClose(Self, Result);
end;

constructor TWindow.Create(ALeft, ATop, AWidth, AHeight: Integer;
  BIS: TBorderIcons; BS: TBorderStyle;
  Caption: string; Icon: HICON);
begin
  FLeft := ALeft;
  FTop := ATop;
  FWidth := AWidth;
  FHeight := AHeight;

  FProcThunk := Classes.MakeObjectInstance(MainWndProc);
  WindowProc := WndProc;
  if RegisterWndClass then
  begin
    if CreateWndHandle(ALeft, ATop, AWidth, AHeight, BIS, BS, Caption) then
      SendMessage(FHandle, WM_SETICON, ICON_BIG, Icon)
    else
      Assert(FHandle <> 0, 'Window Create failed');
  end;
end;

function TWindow.CreateWndHandle(ALeft, ATop, AWidth, AHeight: Integer;
  BIS: TBorderIcons; BS: TBorderStyle;
  Caption: string): Boolean;
begin
  CreatingWnd := Self;
  Result := CreateWindow(cWndClassName, PChar(Caption),
    BorderIconsToStyle(0, BIS) or BorderStyleToStyle(0, BS),
    ALeft, ATop, AWidth, AHeight, 0, 0, HInstance, nil) <> 0;
end;

procedure TWindow.DefaultHandler(var Message);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  with TMessage(Message) do
    Result := CallWindowProc(@DefWindowProc, FHandle, Msg, WParam, LParam);
end;

destructor TWindow.Destroy;
begin
  if not FDestroying then
  begin
    FDestroying := True;
    Assert(FHandle <> 0, 'FHandle has not created');
    DestroyWindow(FHandle);
  end;
  Classes.FreeObjectInstance(FProcThunk);
  inherited;
end;

procedure TWindow.DoClose;
begin
  if Assigned(FOnClose) then
    FOnClose(Self);
end;

procedure TWindow.DoCreate;
begin
  if Assigned(FOnCreate) then
    FOnCreate(Self);
end;

procedure TWindow.DoDestroy;
begin
  if Assigned(FOnDestroy) then
    FOnDestroy(Self);
end;

procedure TWindow.DoHide;
begin
  if Assigned(FOnHide) then
    FOnHide(Self);
end;

procedure TWindow.DoShow;
begin
  if Assigned(FOnShow) then
    FOnShow(Self);
end;

procedure TWindow.DoSizeChange;
begin
  if Assigned(FOnSizeChange) then
    FOnSizeChange(Self);
end;

function TWindow.GetBorderIcons: TBorderIcons;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  Result := StyleToBorderIcons(GetWindowLong(FHandle, GWL_STYLE));
end;

function TWindow.GetBorderStyle: TBorderStyle;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  Result := StyleToBorderStyle(GetWindowLong(FHandle, GWL_STYLE));
end;

function TWindow.GetCaption: string;
var
  Len: Integer;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  Len := GetWindowTextLength(FHandle);
  SetString(Result, PChar(nil), Len);
  if Len <> 0 then
    GetWindowText(FHandle, PChar(Result), Len + 1);
end;

function TWindow.GetClientHeight: Integer;
begin
  Result := ClientSize.Y;
end;

function TWindow.GetClientSize: TPoint;
var
  R: TRect;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if IsIconic(Handle) then
  begin
    SetRect(R, 0, 0, 0, 0);
    AdjustWindowRectEx(R, GetWindowLong(FHandle, GWL_STYLE),
      False, GetWindowLong(FHandle, GWL_EXSTYLE));
    SetRect(R, 0, 0,
      Width - R.Right + R.Left,
      Height - R.Bottom + R.Top);
  end
  else begin
    Windows.GetClientRect(FHandle, R);
  end;

  Result := Point(R.Right, R.Bottom);
end;

function TWindow.GetClientWidth: Integer;
begin
  Result := ClientSize.X;
end;

function TWindow.GetEnable: Boolean;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  Result := IsWindowEnabled(FHandle);
end;

function TWindow.GetTopMost: Boolean;
var
  ExStyle: LongWord;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  ExStyle := GetWindowLong(FHandle, GWL_EXSTYLE);
  Result := ExStyle and WS_EX_TOPMOST <> 0;
end;

function TWindow.GetVisible: Boolean;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  Result := IsWindowVisible(FHandle);
end;

function TWindow.GetWindowState: TWindowState;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if IsIconic(FHandle) then
    Result := wsMinimized
  else if IsZoomed(FHandle) then
    Result := wsMaximized
  else
    Result := wsNormal;
end;

procedure TWindow.Hide;
begin
  Visible := False;
end;

procedure TWindow.MainWndProc(var Message: TMessage);
begin
  WindowProc(Message);
end;

function TWindow.RegisterWndClass: Boolean;
var
  Reged: Boolean;
  wc: TWndClass;
  clsName: array[0..63] of Char;
begin
  Result := True;

  Reged :=  GetClassInfo(HInstance, cWndClassName, wc);
  if Reged and (wc.lpfnWndProc <> @InitWndProc) then
  begin
    Windows.UnregisterClass(cWndClassName, HInstance);
    Reged := False;
  end;

  if not Reged then
  begin
    StrCopy(clsName, cWndClassName);
    wc.style := CS_HREDRAW or CS_VREDRAW or CS_DBLCLKS;
    wc.lpfnWndProc := @InitWndProc;
    wc.cbClsExtra := 0;
    wc.cbWndExtra := 0;
    wc.hInstance := HInstance;
    wc.hIcon := 0;
    wc.hCursor := LoadCursor(0, IDC_ARROW);
    wc.hbrBackground := GetStockObject(WHITE_BRUSH);
    wc.lpszClassName := cWndClassName;

    Result := Windows.RegisterClass(wc) <> 0;
  end;
end;

procedure TWindow.SendToBack;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  SetWindowPos(FHandle, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE + SWP_NOSIZE);
end;

procedure TWindow.SetBorderIcons(const Value: TBorderIcons);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  SetWindowLong(FHandle, GWL_STYLE,
    BorderIconsToStyle(GetWindowLong(FHandle, GWL_STYLE), Value));
  // 改变
  SetWindowPos(FHandle, 0, 0, 0, 0, 0,
    SWP_NOMOVE or SWP_NOSIZE or SWP_NOZORDER or SWP_FRAMECHANGED);
end;

procedure TWindow.SetBorderStyle(const Value: TBorderStyle);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  SetWindowLong(FHandle, GWL_STYLE,
    BorderStyleToStyle(GetWindowLong(FHandle, GWL_STYLE), Value));
  // 改变
  SetWindowPos(FHandle, 0, 0, 0, 0, 0,
    SWP_FRAMECHANGED or SWP_NOMOVE or SWP_NOSIZE or SWP_NOZORDER);
end;

procedure TWindow.SetBounds(const Rect: TRect);
begin
  SetBounds(Rect.Left, Rect.Top, Rect.Right - Rect.Left, Rect.Bottom - Rect.Top);
end;

procedure TWindow.SetBounds(ALeft, ATop, AWidth, AHeight: Integer);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if (ALeft <> FLeft) or (ATop <> FTop) or
    (AWidth <> FWidth) or (AHeight <> FHeight) then
  begin
    if not IsIconic(FHandle) then
      SetWindowPos(FHandle, 0, ALeft, ATop, AWidth, AHeight,
        SWP_NOZORDER + SWP_NOACTIVATE);
  end;
end;

procedure TWindow.SetCaption(const Value: string);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  SetWindowText(FHandle, PChar(Value));
end;

procedure TWindow.SetClientHeight(const Value: Integer);
begin
  SetClientSize(Point(ClientHeight, Value));
end;

procedure TWindow.SetClientSize(Value: TPoint);
var
  Client: TPoint;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  Client := GetClientSize;
  SetBounds(FLeft, FTop, Width - Client.X + Value.X,  Height -
    Client.Y + Value.Y);
end;

procedure TWindow.SetClientWidth(const Value: Integer);
begin
  SetClientSize(Point(Value, ClientHeight));
end;

procedure TWindow.SetEnable(const Value: Boolean);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  EnableWindow(Fhandle, Value);
end;

procedure TWindow.SetHeight(const Value: Integer);
begin
  SetBounds(FLeft, FTop, FWidth, Value);
end;

procedure TWindow.SetLeft(const Value: Integer);
begin
  SetBounds(Value, FTop, FWidth, FHeight);
end;

procedure TWindow.SetTop(const Value: Integer);
begin
  SetBounds(FLeft, Value, FWidth, FHeight);
end;

procedure TWindow.SetTopMost(const Value: Boolean);
const
  ZOrder: array[Boolean] of LongWord = (HWND_NOTOPMOST, HWND_TOPMOST);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  SetWindowPos(FHandle, ZOrder[Value], 0, 0, 0, 0, SWP_NOMOVE or
    SWP_NOSIZE or SWP_NOACTIVATE);
end;

procedure TWindow.SetVisible(const Value: Boolean);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if not Value then
  begin
    DoHide;
    ShowWindow(FHandle, SW_HIDE);
  end
  else begin
    if IsIconic(FHandle) then
      ShowWindow(FHandle, SW_SHOWMINNOACTIVE)
    else if IsZoomed(FHandle) then
      ShowWindow(FHandle, SW_SHOWMAXIMIZED)
    else
      ShowWindow(FHandle, SW_SHOWNORMAL);
    DoShow;
  end;
end;

procedure TWindow.SetWidth(const Value: Integer);
begin
  SetBounds(FLeft, FTop, Value, FHeight);
end;

procedure TWindow.SetWindowState(const Value: TWindowState);
const
  ShowCommands: array[TWindowState] of Integer =
    (SW_SHOWNORMAL, SW_MINIMIZE, SW_SHOWMAXIMIZED);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if Visible then
    ShowWindow(FHandle, ShowCommands[Value]);
end;

procedure TWindow.Show(const WndState: TWindowState);
const
  ShowCommands: array[TWindowState] of Integer =
    (SW_SHOWNORMAL, SW_MINIMIZE, SW_SHOWMAXIMIZED);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  ShowWindow(FHandle, ShowCommands[WndState]);
  BringToFront;
  DoShow;
end;

function TWindow.StyleToBorderIcons(Style: LongWord): TBorderIcons;
begin
  Result := [];
  if Style and WS_SYSMENU <> 0 then
  begin
    Include(Result, biSystemMenu);
    if Style and WS_MAXIMIZEBOX <> 0 then
      Include(Result, biMaximize);
    if Style and WS_MINIMIZEBOX <> 0 then
      Include(Result, biMinimize);
  end;
end;

function TWindow.StyleToBorderStyle(Style: LongWord): TBorderStyle;
begin
  Result := bsNone;
  if Style and WS_CAPTION <> 0 then
  begin
    if Style and WS_THICKFRAME <> 0 then
      Result := bsSizeable
    else
      Result := bsSingle
  end
  else if Style and WS_POPUP <> 0 then
    Result := bsNone;
end;

procedure TWindow.UpdateBounds;
var
  Rect: TRect;
  WindowPlacement: TWindowPlacement;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if IsIconic(FHandle) then
  begin
    WindowPlacement.Length := SizeOf(WindowPlacement);
    GetWindowPlacement(FHandle, @WindowPlacement);
    if WPF_RESTORETOMAXIMIZED and WindowPlacement.flags <> 0 then
    begin
      Rect.TopLeft := WindowPlacement.ptMaxPosition;
      Rect.Right := Rect.Left + FWidth;
      Rect.Bottom := Rect.Top + FHeight;
    end
    else
      Rect := WindowPlacement.rcNormalPosition;
  end else
    GetWindowRect(FHandle, Rect);

  FLeft := Rect.Left;
  FTop := Rect.Top;
  FWidth := Rect.Right - Rect.Left;
  FHeight := Rect.Bottom - Rect.Top;
  DoSizeChange;
end;

procedure TWindow.WMClose(var Message: TWMClose);
begin
  Close;
end;

procedure TWindow.WMMove(var Message: TWMMove);
begin
  inherited;
  UpdateBounds;
end;

procedure TWindow.WMNCDestroy(var Message: TWMNCDestroy);
begin
  if not FDestroying then
  begin
    FDestroying := True;
    Free;
  end;
end;

procedure TWindow.WMSize(var Message: TWMSize);
begin
  UpdateBounds;
  inherited;
end;

procedure TWindow.WndProc(var Message: TMessage);
begin
  Dispatch(Message);
end;

{ TMainWindow }

procedure TMainWindow.DoClose;
begin
  inherited;
  PostQuitMessage(0);
end;

end.
