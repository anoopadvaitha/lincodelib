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
  Windows, Messages;

type
  TMessageEvent = procedure (var Msg: TMsg; var Handled: Boolean) of object;
  TIdleEvent = procedure (Sender: TObject; var Done: Boolean) of object;

  { 消息循环类 }
  TMessageLoop = class
  private
    FOnIdle: TIdleEvent;
    FOnMessage: TMessageEvent;
    FTerminated: Boolean;
  protected
    procedure DoMessage(var Msg: TMsg; var Handled: Boolean); virtual;
    procedure DoIdle(var Done: Boolean); virtual;
    procedure Idle(var Msg: TMsg); virtual;
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

    property OnIdle: TIdleEvent read FOnIdle write FOnIdle;
    property OnMessage: TMessageEvent read FOnMessage write FonMessage;
    property Terminated: Boolean read FTerminated write FTerminated;
  end;

implementation


{ TMessageLoop }

procedure TMessageLoop.DoIdle(var Done: Boolean);
begin
  if Assigned(FOnIdle) then
    FOnIdle(Self, Done);
end;

procedure TMessageLoop.DoMessage(var Msg: TMsg; var Handled: Boolean);
begin
  if Assigned(FOnMessage) then
    FOnMessage(Msg, Handled);
end;

procedure TMessageLoop.HandleMessage;
var
  Msg: TMsg;
begin
  if not ProcessMessage(Msg) then Idle(Msg);
end;

procedure TMessageLoop.Idle(var Msg: TMsg);
var
  Done: Boolean;
begin
  Done := True;
  DoIdle(Done);
  if Done then WaitMessage;
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
      DoMessage(Msg, Handled);
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

end.
