object Form4: TForm4
  Left = 428
  Top = 192
  HelpContext = 40
  BorderIcons = [biMinimize]
  BorderStyle = bsDialog
  Caption = 'Options'
  ClientHeight = 332
  ClientWidth = 317
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  DesignSize = (
    317
    332)
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 300
    Height = 281
    Anchors = [akLeft, akTop, akRight, akBottom]
    Shape = bsFrame
  end
  object Label1: TTntLabel
    Left = 24
    Top = 28
    Width = 46
    Height = 13
    Caption = 'Decimals:'
    FocusControl = Edit1
  end
  object Label2: TTntLabel
    Left = 24
    Top = 60
    Width = 59
    Height = 13
    Caption = 'Recent files:'
    FocusControl = Edit2
  end
  object Label3: TTntLabel
    Left = 24
    Top = 92
    Width = 78
    Height = 13
    Caption = 'Max undo steps:'
    FocusControl = Edit3
  end
  object Label4: TTntLabel
    Left = 24
    Top = 156
    Width = 51
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = 'Language:'
    FocusControl = ComboBox1
  end
  object Label5: TTntLabel
    Left = 24
    Top = 124
    Width = 52
    Height = 13
    Caption = 'Font scale:'
    FocusControl = ComboBox2
  end
  object Edit1: TTntEdit
    Left = 110
    Top = 24
    Width = 43
    Height = 21
    Hint = 'Number of decimals used when results are shown.'
    Anchors = [akTop, akRight]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    Text = '4'
  end
  object Button1: TTntButton
    Left = 59
    Top = 299
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 10
    OnClick = Button1Click
  end
  object Button2: TTntButton
    Left = 147
    Top = 299
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 11
  end
  object CheckBox1: TTntCheckBox
    Left = 24
    Top = 184
    Width = 274
    Height = 17
    Hint = 'Associate *.grf files with Graph.'
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Associate .grf files'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
  end
  object Edit2: TTntEdit
    Left = 110
    Top = 56
    Width = 43
    Height = 21
    Hint = 'The number of most recent used files shown in the File menu.'
    Anchors = [akTop, akRight]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    Text = '4'
  end
  object CheckBox4: TTntCheckBox
    Left = 24
    Top = 232
    Width = 274
    Height = 17
    Hint = 
      'Save window position when the program quits. This setting is use' +
      'd the next time the program is used.|Save window state on exit.'
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Save workspace on exit'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 7
  end
  object Button3: TTntButton
    Left = 235
    Top = 299
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Help'
    TabOrder = 12
    OnClick = Button3Click
  end
  object RadioGroup1: TTntRadioGroup
    Left = 178
    Top = 16
    Width = 117
    Height = 73
    Hint = 'Format used when complex numbers are shown.'
    Anchors = [akTop, akRight]
    Caption = 'Complex format'
    ItemIndex = 1
    Items.Strings = (
      'Real'
      'Rectangular'
      'Polar')
    ParentShowHint = False
    ShowHint = True
    TabOrder = 9
  end
  object CheckBox2: TTntCheckBox
    Left = 24
    Top = 208
    Width = 274
    Height = 17
    Hint = 'Show the tooltip when the cursor is over an item.'
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Show tooltip'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 6
  end
  object Edit3: TTntEdit
    Left = 110
    Top = 88
    Width = 43
    Height = 21
    Hint = 
      'Maximum number of undo steps saved. Set this to 0 to prevent und' +
      'o.'
    Anchors = [akTop, akRight]
    TabOrder = 2
    Text = '50'
  end
  object CheckBox7: TTntCheckBox
    Left = 24
    Top = 256
    Width = 282
    Height = 17
    Hint = 
      'If checked the program will contact a web server when the progra' +
      'm is started to see if a newer version is available.'
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Check for new version of Graph at startup'
    TabOrder = 8
  end
  object ComboBox1: TTntComboBox
    Left = 110
    Top = 152
    Width = 97
    Height = 21
    Hint = 'Select the language to use for the user interface.'
    Style = csDropDownList
    Anchors = [akTop, akRight]
    ItemHeight = 13
    Sorted = True
    TabOrder = 4
  end
  object UpDown1: TUpDown
    Left = 153
    Top = 24
    Width = 15
    Height = 21
    Anchors = [akTop, akRight]
    Associate = Edit1
    Min = 0
    Max = 15
    Position = 4
    TabOrder = 13
    Wrap = False
  end
  object UpDown2: TUpDown
    Left = 153
    Top = 56
    Width = 15
    Height = 21
    Anchors = [akTop, akRight]
    Associate = Edit2
    Min = 0
    Max = 9
    Position = 4
    TabOrder = 14
    Wrap = False
  end
  object UpDown3: TUpDown
    Left = 153
    Top = 88
    Width = 15
    Height = 21
    Anchors = [akTop, akRight]
    Associate = Edit3
    Min = 0
    Max = 1000
    Increment = 10
    Position = 50
    TabOrder = 15
    Wrap = False
  end
  object ComboBox2: TTntComboBox
    Left = 110
    Top = 120
    Width = 97
    Height = 21
    Hint = 
      'This is used to scale the user interface if you are having probl' +
      'ems reading the text.'
    AutoComplete = False
    Anchors = [akTop, akRight]
    ItemHeight = 13
    ItemIndex = 1
    TabOrder = 3
    Text = '100%'
    OnKeyPress = ComboBox2KeyPress
    Items.Strings = (
      '75%'
      '100%'
      '125%'
      '150%'
      '200%')
  end
end
