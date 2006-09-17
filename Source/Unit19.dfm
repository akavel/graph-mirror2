object Form19: TForm19
  Left = 409
  Top = 177
  HelpContext = 160
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Animate'
  ClientHeight = 280
  ClientWidth = 299
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
    Top = 20
    Width = 45
    Height = 13
    Caption = 'Constant:'
    FocusControl = ComboBox1
  end
  object ComboBox1: TTntComboBox
    Left = 56
    Top = 16
    Width = 137
    Height = 21
    Hint = 'Choose a constant to change between each frame.'
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
  end
  object Button1: TTntButton
    Left = 216
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Animate'
    Default = True
    TabOrder = 3
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
    TabOrder = 4
  end
  object Button4: TTntButton
    Left = 216
    Top = 136
    Width = 75
    Height = 25
    Caption = 'Options...'
    TabOrder = 5
    Visible = False
    OnClick = Button4Click
  end
  object Button3: TTntButton
    Left = 216
    Top = 96
    Width = 75
    Height = 25
    Caption = 'Help'
    TabOrder = 6
    OnClick = Button3Click
  end
  object TntGroupBox1: TTntGroupBox
    Left = 8
    Top = 48
    Width = 193
    Height = 105
    Caption = 'Range'
    TabOrder = 1
    object Label2: TTntLabel
      Left = 8
      Top = 20
      Width = 26
      Height = 13
      Caption = 'From:'
      FocusControl = Edit1
    end
    object Label3: TTntLabel
      Left = 8
      Top = 48
      Width = 16
      Height = 13
      Caption = 'To:'
      FocusControl = Edit2
    end
    object Label4: TTntLabel
      Left = 8
      Top = 76
      Width = 25
      Height = 13
      Caption = 'Step:'
      FocusControl = Edit3
    end
    object Edit1: TTntEdit
      Left = 96
      Top = 16
      Width = 89
      Height = 21
      Hint = 'The first value of the constant'
      TabOrder = 0
      Text = '1'
    end
    object Edit2: TTntEdit
      Left = 96
      Top = 44
      Width = 89
      Height = 21
      Hint = 'The last value of the constant.'
      TabOrder = 1
      Text = '10'
    end
    object Edit3: TTntEdit
      Left = 96
      Top = 72
      Width = 89
      Height = 21
      Hint = 'The chosen constant is changed by this value between each frame.'
      TabOrder = 2
      Text = '1'
    end
  end
  object TntGroupBox2: TTntGroupBox
    Left = 8
    Top = 168
    Width = 193
    Height = 105
    Caption = 'Image information'
    TabOrder = 2
    object Label5: TTntLabel
      Left = 8
      Top = 20
      Width = 60
      Height = 13
      Caption = 'Image width:'
      FocusControl = Edit4
    end
    object Label6: TTntLabel
      Left = 8
      Top = 48
      Width = 64
      Height = 13
      Caption = 'Image height:'
      FocusControl = Edit5
    end
    object Label7: TTntLabel
      Left = 8
      Top = 76
      Width = 77
      Height = 13
      Caption = 'Frames/second:'
      FocusControl = Edit6
    end
    object Edit4: TTntEdit
      Left = 96
      Top = 16
      Width = 89
      Height = 21
      TabOrder = 0
      Text = '320'
      OnKeyPress = TntEditKeyPress
    end
    object Edit5: TTntEdit
      Left = 96
      Top = 44
      Width = 89
      Height = 21
      TabOrder = 1
      Text = '320'
      OnKeyPress = TntEditKeyPress
    end
    object Edit6: TTntEdit
      Left = 96
      Top = 72
      Width = 89
      Height = 21
      Hint = 'Number of frames per second used when the animation is played.'
      TabOrder = 2
      Text = '1'
    end
  end
  object ProgressForm1: TProgressForm
    Caption = 'Working'
    Delay = 0
    ButtonCaption = 'Cancel'
    Left = 264
    Top = 240
  end
end
