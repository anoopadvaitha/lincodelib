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
  Windows, Messages, SysUtils, Math, Classes, TypInfo;

type
  TMyWindow = class(TMainWindow)
  private
    FTimerVisible: Boolean;
    FTimerEnable: Boolean;
    FBIIndex: Integer;
    FBSIndex: Integer;
    FTimerWndState: Boolean;
    FWSIndex: Integer;
  protected
    procedure DoSizeChange; override;
  public
    procedure WMKeyDown(var message: TWMKey); message WM_KEYDOWN;
    procedure WMTimer(var message: TWMTimer); message WM_TIMER;
    procedure WMPaint(var Message: TWMPaint); message WM_PAINT;
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

  Wnd.Free;
  MsgLoop.Free;
end;


{ TMyWindow }

procedure TMyWindow.DoSizeChange;
begin
  inherited;
  InvalidateRect(Handle, nil, True);
end;

procedure TMyWindow.WMKeyDown(var message: TWMKey);
const
  BIS: array [0..3] of TBorderIcons =
    ([biSystemMenu, biMinimize, biMaximize],
     [biSystemMenu, biMinimize],
     [biSystemMenu, biMaximize],
     []);
  BSS: array [0..2] of  TBorderStyle =
    (bsSizeable, bsSingle, bsNone);
begin
  case message.CharCode of
    VK_F1: //  TopMost
    begin
      TopMost := not TopMost;
      InvalidateRect(Handle, nil, True);
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
      Inc(FBIIndex);
      if FBIIndex = 4 then
        FBIIndex := 0;
      BorderIcons := BIS[FBIIndex];
      InvalidateRect(Handle, nil, True);
    end;
    VK_F5: // BorderStyle
    begin
      Inc(FBSIndex);
      if FBSIndex = 3 then
        FBSIndex := 0;
      BorderStyle := BSS[FBSIndex];
      InvalidateRect(Handle, nil, True);
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
      InvalidateRect(Handle, nil, True);
    end;
    VK_F8: // left top width height
    begin
      Left := RandomRange(0, 600);
      Top := RandomRange(0, 500);
      Width := RandomRange(200, 800);
      Height := RandomRange(200, 700);
      InvalidateRect(Handle, nil, True);
    end;
    VK_F9: // SetBounds
    begin
      SetBounds(
        RandomRange(0, 600),
        RandomRange(0, 500),
        RandomRange(200, 800),
        RandomRange(200, 700)
        );
      InvalidateRect(Handle, nil, True);
    end;
    VK_NUMPAD0: // ClientWidth, ClientHeight
    begin
      ClientWidth := RandomRange(400, 800);
      ClientHeight := RandomRange(300, 600);
      InvalidateRect(Handle, nil, True);
    end;
    VK_NUMPAD1: // ClientSize
    begin
      ClientSize := Point(RandomRange(400, 800), RandomRange(300, 600));
      InvalidateRect(Handle, nil, True);
    end;
    VK_NUMPAD2: // AlignWindow
    begin
      AlignWindow(RandomRange(3, 7) * 10, RandomRange(3, 7) * 10);
      InvalidateRect(Handle, nil, True);
    end;
  end;
end;

procedure TMyWindow.WMPaint(var Message: TWMPaint);
var
  ps: TPaintStruct;
  dc: HDC;
  str: string;
  BIS: TBorderIcons;

  procedure PrintText(str: string; Line: Integer);
  begin
    TextOut(dc, 8, 8 + 18 * Line, PChar(str), Length(str));
  end;
begin
  dc := BeginPaint(Handle, ps);

  str := Format('F1: TopMost; 当前值: %s', [BoolToStr(TopMost, True)]);
  PrintText(str, 0);

  str := Format('F2: Visible; 当前值: %s', [BoolToStr(Visible, True)]);
  PrintText(str, 1);

  str := Format('F3: Enable; 当前值: %s', [BoolToStr(Enable, True)]);
  PrintText(str, 2);

  str := 'F4: BorderIcons; 当前值: ';
  BIS := BorderIcons;
  if biSystemMenu in BIS then
    str := str + 'biSystemMenu';
  if biMaximize in BIS then
    str := str + ', biMaximize';
  if biMinimize in BIS  then
    str := str + ', biMinimize';
  PrintText(str, 3);

  str := 'F5: BorderStyle; 当前值: ';
  str := str + GetEnumName(TypeInfo(TBorderStyle), Integer(BorderStyle));
  PrintText(str, 4);

  str := 'F6: WindowState; 当前值: ';
  str := str + GetEnumName(TypeInfo(TWindowState), Integer(WindowState));
  PrintText(str, 5);

  str := 'F7: Caption; 当前值: ' + Caption;
  PrintText(str, 6);

  str := Format('F8: Left, Top, Width, Height; 当前值: %d, %d, %d, %d', [Left, Top, Width,Height]);
  PrintText(str, 7);

  str := Format('F9: SetBounds; 当前值: %d, %d, %d, %d', [Left, Top, Width,Height]);
  PrintText(str, 8);

  str := Format('Num0: ClientWidth, ClientHeight; 当前值: %d, %d', [ClientWidth, ClientHeight]);
  PrintText(str, 9);

  str := Format('Num1: ClientSize; 当前值: %d, %d', [ClientWidth, ClientHeight]);
  PrintText(str, 10);

  str := Format('Num2: AlignWindow; 当前值: %d, %d, %d, %d', [Left, Top, Width,Height]);
  PrintText(str, 11);

  EndPaint(Handle, ps);
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

  InvalidateRect(Handle, nil, True);
end;

end.
