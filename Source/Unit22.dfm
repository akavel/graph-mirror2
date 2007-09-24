object Form22: TForm22
  Left = 335
  Top = 276
  Width = 506
  Height = 230
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = 'Python interpreter'
  Color = clBtnFace
  DragKind = dkDock
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object IRichEdit1: TIRichEdit
    Left = 0
    Top = 0
    Width = 498
    Height = 203
    WordWrap = False
    HideScrollBars = False
    ScrollBars = ssVertical
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnKeyDown = IRichEdit1KeyDown
    OnProtectChange = IRichEdit1ProtectChange
  end
end
