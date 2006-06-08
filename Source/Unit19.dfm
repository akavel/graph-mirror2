object Form19: TForm19
  Left = 409
  Top = 177
  HelpContext = 160
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Animate'
  ClientHeight = 176
  ClientWidth = 300
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TTntLabel
    Left = 8
    Top = 20
    Width = 50
    Height = 13
    Caption = 'Constants:'
    FocusControl = ComboBox1
  end
  object Label2: TTntLabel
    Left = 8
    Top = 52
    Width = 26
    Height = 13
    Caption = 'From:'
    FocusControl = Edit1
  end
  object Label3: TTntLabel
    Left = 8
    Top = 84
    Width = 16
    Height = 13
    Caption = 'To:'
    FocusControl = Edit2
  end
  object Label4: TTntLabel
    Left = 8
    Top = 116
    Width = 25
    Height = 13
    Caption = 'Step:'
    FocusControl = Edit3
  end
  object Label5: TTntLabel
    Left = 8
    Top = 148
    Width = 77
    Height = 13
    Caption = 'Frames/second:'
    FocusControl = Edit4
  end
  object ComboBox1: TTntComboBox
    Left = 96
    Top = 16
    Width = 105
    Height = 21
    Hint = 'Choose a constant to change between each frame.'
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
  end
  object Edit1: TTntEdit
    Left = 96
    Top = 48
    Width = 105
    Height = 21
    Hint = 'The first value of the constant'
    TabOrder = 1
    Text = '1'
  end
  object Edit2: TTntEdit
    Left = 96
    Top = 80
    Width = 105
    Height = 21
    Hint = 'The last value of the constant.'
    TabOrder = 2
    Text = '10'
  end
  object Edit3: TTntEdit
    Left = 96
    Top = 112
    Width = 105
    Height = 21
    Hint = 'The chosen constant is changed by this value between each frame.'
    TabOrder = 3
    Text = '1'
  end
  object Edit4: TTntEdit
    Left = 96
    Top = 144
    Width = 105
    Height = 21
    Hint = 'Number of frames per second used when the animation is played.'
    TabOrder = 4
    Text = '1'
  end
  object Button1: TTntButton
    Left = 216
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Animate'
    Default = True
    TabOrder = 5
    OnClick = Button1Click
  end
  object Button2: TTntButton
    Left = 216
    Top = 56
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Close'
    ModalResult = 2
    TabOrder = 6
  end
  object Button3: TTntButton
    Left = 216
    Top = 136
    Width = 75
    Height = 25
    Caption = 'Options...'
    TabOrder = 7
    Visible = False
  end
  object Button4: TTntButton
    Left = 216
    Top = 96
    Width = 75
    Height = 25
    Caption = 'Help'
    TabOrder = 8
    OnClick = Button4Click
  end
  object ProgressForm1: TProgressForm
    Caption = 'Working'
    Delay = 0
    ButtonCaption = 'Cancel'
    Left = 272
    Top = 144
  end
end
