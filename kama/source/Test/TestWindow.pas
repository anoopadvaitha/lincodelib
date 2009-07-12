{*****************************************************************************
 * brief: Kama游戏开发库，测试窗口类
 *
 * autor: linzhenqun
 * date:  2009-7-12
 * email: linzhenqun@gmail.com
*****************************************************************************}
unit TestWindow;

interface
uses
  KmWndClasses;

procedure DoTestWindow;

implementation

uses
  Windows, Messages, SysUtils;

type
  TMyWindow = class(TMainWindow)
  private
    FTimerVisible: Boolean;
    FTimerEnable: Boolean;
    FBIIndex: Integer;
    FBSIndex: Integer;
    FTimerWndState: Boolean;
    FWSIndex: Integer;
  public
    procedure WMKeyDown(var message: TWMKey); message WM_KEYDOWN;
    procedure WMTimer(var message: TWMTimer); message WM_TIMER;
  end;


procedure DoTestWindow;
var
  Wnd: TWindow;
  MsgLoop: TMessageLoop;
begin
  Wnd := TMyWindow.Create(100, 100, 600, 480,
    [biSystemMenu, biMaximize, biMinimize],
    bsSizeable, 'mywindow', LoadIcon(0, IDI_INFORMATION));
  MsgLoop := TMessageLoop.Create;

  Wnd.Show;
  MsgLoop.Run;

  MsgLoop.Free;
end;


{ TMyWindow }

procedure TMyWindow.WMKeyDown(var message: TWMKey);
const
  BIS: array [0..6] of TBorderIcons =
    ([biSystemMenu, biMinimize, biMaximize],
     [biSystemMenu, biMinimize],
     [biSystemMenu, biMaximize],
     [biMinimize],
     [biMaximize],
     [biMaximize, biMinimize],
     []);
  BSS: array [0..2] of  TBorderStyle =
    (bsSizeable, bsSingle, bsNone);
begin
  case message.CharCode of
    VK_F1: //  TopMost
    begin
      TopMost := not TopMost;
    end;
    VK_F2: // Visible
    begin
      if not FTimerVisible then
      begin
        FTimerVisible := True;
        SetTimer(Handle, 1, 2000, nil);
      end
      else begin
        FTimerVisible := False;
        KillTimer(Handle, 1);
      end;
    end;
    VK_F3: // Enable
    begin
      if not FTimerEnable then
      begin
        FTimerEnable := True;
        SetTimer(Handle, 2, 2000, nil);
      end
      else begin
        FTimerEnable := False;
        KillTimer(Handle, 2);
      end;
    end;
    VK_F4: // BorderIcons
    begin
      BorderIcons := BIS[FBIIndex];
      Inc(FBIIndex);
      if FBIIndex = 7 then
        FBIIndex := 0;
    end;
    VK_F5: // SetBorderStyle
    begin
      BorderStyle := BSS[FBSIndex];
      Inc(FBSIndex);
      if FBSIndex = 3 then
        FBSIndex := 0;
    end;
    VK_F6: // WindowState
    begin
      if not FTimerWndState then
      begin
        FTimerWndState := True;
        SetTimer(Handle, 3, 2000, nil);
      end
      else begin
        FTimerWndState := False;
        KillTimer(Handle, 3);
      end;
    end;
    VK_F7: // SetCaption
    begin
      Caption := DateTimeToStr(Now);
    end;
  end;
end;

procedure TMyWindow.WMTimer(var message: TWMTimer);
const
  WndStates: array [0..2] of TWindowState = (wsNormal, wsMinimized, wsMaximized);
begin
  if message.TimerID = 1 then
    Visible := not Visible
  else if message.TimerID = 2 then
    Enable := not Enable
  else if message.TimerID = 3 then
  begin
    WindowState := WndStates[FWSIndex];
    Inc(FWSIndex);
    if FWSIndex = 3 then
      FWSIndex := 0;
  end;
end;

end.
