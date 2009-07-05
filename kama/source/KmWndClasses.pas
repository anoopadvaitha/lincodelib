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
  Windows, Messages;

type
  TMessageEvent = procedure (var Msg: TMsg; var Handled: Boolean) of object;
  TIdleEvent = procedure (Sender: TObject; var Done: Boolean) of object;

  { ��Ϣѭ���� }
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
