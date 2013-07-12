object TestForm: TTestForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Test form'
  ClientHeight = 151
  ClientWidth = 204
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 12
    Width = 31
    Height = 13
    Caption = 'Name:'
  end
  object Label2: TLabel
    Left = 16
    Top = 38
    Width = 43
    Height = 13
    Caption = 'Address:'
  end
  object Label3: TLabel
    Left = 16
    Top = 65
    Width = 23
    Height = 13
    Caption = 'City:'
  end
  object Edit1: TEdit
    Left = 65
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 0
  end
  object Edit2: TEdit
    Left = 65
    Top = 35
    Width = 121
    Height = 21
    TabOrder = 1
  end
  object Edit3: TEdit
    Left = 65
    Top = 62
    Width = 121
    Height = 21
    TabOrder = 2
  end
  object Button1: TButton
    Left = 16
    Top = 104
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object Button2: TButton
    Left = 111
    Top = 104
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
end
