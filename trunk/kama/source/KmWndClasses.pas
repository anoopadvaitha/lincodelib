{*****************************************************************************
 * brief: Kama��Ϸ�����⣬���ں���Ϣ�ķ�װ��
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
  cWndClassName         = 'Kama.TWindow';     // ��������

  KMWM_BASE             = $A000;              // ��Ϣ��ֵ
  KMWM_RELEASE          = KMWM_BASE + $1;     // �ͷŴ���


type
  // ��Ϣ�����¼������������������Ϣ���ɷ�
  TMessageFilterEvent = procedure (var Msg: TMsg; var Handled: Boolean) of object;
  // �����¼�
  TIdleEvent = procedure (Sender: TObject; var Done: Boolean) of object;

  { ��Ϣѭ���� }
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
    // ��ʼִ����Ϣ��������TerminatedΪTrueʱ����
    // ��Terminated����������»�ΪTrue��
    // 1���յ�WM_QUITʱ��
    // 2���ֶ���ΪTrue��
    procedure Run; virtual;
    // ����һ����Ϣ���������Ϣ������Ϣ����û����Ϣʱ���ᴥ��һ��Idle�¼�
    procedure HandleMessage;
    // ѭ��������Ϣ���������Ϣ��ֱ������û����ϢΪֹ�����ᴥ��Idle�¼�
    procedure ProcessMessages;
    // ����һ����Ϣ���������Ϣ
    function ProcessMessage(var Msg: TMsg): Boolean;
    // ��������
    procedure Terminate;

    property OnIdle: TIdleEvent read FOnIdle write FOnIdle;
    property OnMessageFilter: TMessageFilterEvent read FOnMessageFilter write FOnMessageFilter;
    property Terminated: Boolean read FTerminated;
  end;

  // ��������ť
  TBorderIcon = (
    biSystemMenu,   // ϵͳ�˵�
    biMinimize,     // ��С����ť
    biMaximize      // ��󻯰�ť
  );

  // ��������ť����
  TBorderIcons = set of TBorderIcon;

  // �߿���
  TBorderStyle = (
    bsNone,          // �ޱ߿�
    bsSingle,        // ϸ�߿򣬲���������С
    bsSizeable       // ��߿򣬿���������С
    );

  // ����״̬
  TWindowState = (
    wsNormal,         // ����
    wsMinimized,      // ��С��
    wsMaximized       // ���
  );

  { ���ڷ�װ��:
    ����ֻ������Ϸ�������Ҫ���Զ��㴰�����򵥵ķ�װ
    �����봰�ھ������������һһ��Ӧ������
      ���ഴ��ʱ�����ھ��Ҳ����
      ��������ʱ�����ھ��Ҳ����
      ���ھ������ʱ������Ҳ����}
  TWindow = class
  private
    FDestroying: Boolean;
    FHandle: THandle;
    FHeight: Integer;
    FLeft: Integer;
    FTop: Integer;
    FWidth: Integer;
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
    function GetClientRect: TRect;
    function GetClientWidth: Integer;
    procedure SetClientHeight(const Value: Integer);
    procedure SetClientWidth(const Value: Integer);
    procedure SetClientSize(Value: TSize);
    procedure SetWindowState(const Value: TWindowState);
  protected
    FProcThunk: Pointer;
    FWindowProc: TWndMethod;

    function StyleToBorderStyle(Style: LongWord): TBorderStyle;
    function StyleToBorderIcons(Style: LongWord): TBorderIcons;
    function BorderIconsToStyle(OldStyle: LongWord; BIS: TBorderIcons): LongWord;
    function BorderStyleToStyle(OldStyle: LongWord; BS: TBorderStyle): LongWord;
    function RegisterWndClass: Boolean;
    function CreateWndHandle(ALeft, ATop, AWidth, AHeight: Integer;
      BIS: TBorderIcons; BS: TBorderStyle; Caption: string): Boolean;
    procedure MainWndProc(var Message: TMessage);
    procedure WndProc(var Message: TMessage); virtual;
    procedure KMWMRelease(var Message: TMessage); message KMWM_RELEASE;
    procedure WMClose(var Message: TWMClose); message WM_CLOSE;
    procedure WMSize(var Message: TWMSize); message WM_SIZE;
    procedure WMNCCalcSize(var Message: TWMNCCalcSize); message WM_NCCALCSIZE;
    procedure WMMove(var Message: TWMMove); message WM_MOVE;
    procedure WMNCDestroy(var Message: TWMNCDestroy); message WM_NCDESTROY;
    procedure UpdateBounds;
  public
    // ���캯�������õ�ʱ�򴰿ھʹ����ˣ������ܶ࣬�������������
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
    procedure SetBounds(ALeft, ATop, AWidth, AHeight: Integer);

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
    property ClientRect: TRect read GetClientRect;
    property ClientHeight: Integer read GetClientHeight write SetClientHeight;
    property ClientWidth: Integer read GetClientWidth write SetClientWidth;
  end;

  { �����ڣ����ٵ�ʱ��������� }
  TMainWindow = class(TWindow)
  public
    destructor Destroy; override;
  end;

implementation

threadvar
  CreatingWnd: TWindow;     // �̰߳�ȫ�Ŀ���

function InitWndProc(hwnd: THandle; Message, WParam: Longint;
  LParam: Longint): Longint; stdcall;
var
  Wnd: TWindow;
begin
  //��Message�Ǵ��ڵĵ�һ����Ϣ
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

  PostMessage(FHandle, KMWM_RELEASE, 0, 0);
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
  Result := ClientRect.Bottom;
end;

function TWindow.GetClientRect: TRect;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if IsIconic(Handle) then
  begin
    SetRect(Result, 0, 0, 0, 0);
    AdjustWindowRectEx(Result, GetWindowLong(FHandle, GWL_STYLE),
      False, GetWindowLong(FHandle, GWL_EXSTYLE));
    SetRect(Result, 0, 0,
      Width - Result.Right + Result.Left,
      Height - Result.Bottom + Result.Top);
  end
  else
    Windows.GetClientRect(FHandle, Result);
end;

function TWindow.GetClientWidth: Integer;
begin
  Result := ClientRect.Right;
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

procedure TWindow.KMWMRelease(var Message: TMessage);
begin
  Free;
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
  // �ı�
  SetWindowPos(FHandle, 0, 0, 0, 0, 0,
    SWP_NOMOVE or SWP_NOSIZE or SWP_NOZORDER or SWP_FRAMECHANGED);
end;

procedure TWindow.SetBorderStyle(const Value: TBorderStyle);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  SetWindowLong(FHandle, GWL_STYLE,
    BorderStyleToStyle(GetWindowLong(FHandle, GWL_STYLE), Value));
  // �ı�
  SetWindowPos(FHandle, 0, 0, 0, 0, 0,
    SWP_FRAMECHANGED or SWP_NOMOVE or SWP_NOSIZE or SWP_NOZORDER);
end;

procedure TWindow.SetBounds(ALeft, ATop, AWidth, AHeight: Integer);
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  if (ALeft <> FLeft) or (ATop <> FTop) or
    (AWidth <> FWidth) or (AHeight <> FHeight) then
  begin
    if IsIconic(FHandle) then
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
var
  Size: TSize;
begin
  Size.cx := ClientHeight;
  Size.cy := Value;
  SetClientSize(Size);
end;

procedure TWindow.SetClientSize(Value: TSize);
var
  Client: TRect;
begin
  Assert(FHandle <> 0, 'FHandle has not created');

  Client := GetClientRect;
  SetBounds(FLeft, FTop, Width - Client.Right + Value.cx, Height -
    Client.Bottom + Value.cy);
end;

procedure TWindow.SetClientWidth(const Value: Integer);
var
  Size: TSize;
begin
  Size.cx := Value;
  Size.cy := ClientHeight;
  SetClientSize(Size);
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
    ShowWindow(FHandle, SW_HIDE)
  else if IsIconic(FHandle) then
    ShowWindow(FHandle, SW_SHOWMINNOACTIVE)
  else if IsZoomed(FHandle) then
    ShowWindow(FHandle, SW_SHOWMAXIMIZED)
  else
    ShowWindow(FHandle, SW_SHOWNORMAL);
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
    if Style and WS_BORDER <> 0 then
      Result := bsSingle
    else if Style and WS_THICKFRAME <> 0 then
      Result := bsSizeable;
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

procedure TWindow.WMNCCalcSize(var Message: TWMNCCalcSize);
begin
  inherited;
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

destructor TMainWindow.Destroy;
begin
  PostQuitMessage(0);
  inherited;
end;

end.
