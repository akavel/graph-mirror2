object Form8: TForm8
  Left = 408
  Top = 227
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Add model'
  ClientHeight = 298
  ClientWidth = 324
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TTntLabel
    Left = 8
    Top = 12
    Width = 31
    Height = 13
    Caption = 'Name:'
    FocusControl = Edit1
  end
  object Edit1: TTntEdit
    Left = 48
    Top = 8
    Width = 265
    Height = 21
    TabOrder = 0
  end
  object ValueListEditor1: TValueListEditor
    Left = 8
    Top = 40
    Width = 306
    Height = 212
    TabOrder = 1
    TitleCaptions.Strings = (
      'Constant'
      'Value')
    OnKeyPress = ValueListEditor1KeyPress
    ColWidths = (
      112
      188)
  end
  object Button1: TTntButton
    Left = 152
    Top = 264
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TTntButton
    Left = 240
    Top = 264
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
