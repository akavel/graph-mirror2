object Form4: TForm4
  Left = 428
  Top = 192
  HelpContext = 40
  BiDiMode = bdLeftToRight
  BorderIcons = [biMinimize]
  BorderStyle = bsDialog
  Caption = 'Options'
  ClientHeight = 328
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  ParentBiDiMode = False
  Position = poMainFormCenter
  ShowHint = True
  DesignSize = (
    320
    328)
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 62
    Top = 295
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 150
    Top = 295
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object Button3: TButton
    Left = 238
    Top = 295
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Help'
    TabOrder = 3
    OnClick = Button3Click
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 305
    Height = 273
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
    DesignSize = (
      305
      273)
    object Label1: TLabel
      Left = 16
      Top = 20
      Width = 46
      Height = 13
      Caption = 'Decimals:'
      FocusControl = Edit1
    end
    object Label2: TLabel
      Left = 16
      Top = 52
      Width = 59
      Height = 13
      Caption = 'Recent files:'
      FocusControl = Edit2
    end
    object Label3: TLabel
      Left = 16
      Top = 84
      Width = 78
      Height = 13
      Caption = 'Max undo steps:'
      FocusControl = Edit3
    end
    object Label5: TLabel
      Left = 16
      Top = 116
      Width = 52
      Height = 13
      Caption = 'Font scale:'
      FocusControl = ComboBox1
    end
    object Label4: TLabel
      Left = 16
      Top = 148
      Width = 51
      Height = 13
      Caption = 'Language:'
      FocusControl = ComboBox2
    end
    object Edit1: TEdit
      Left = 102
      Top = 16
      Width = 43
      Height = 21
      Hint = 'Number of decimals used when results are shown.'
      Anchors = [akTop, akRight]
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      Text = '4'
    end
    object UpDown1: TUpDown
      Left = 145
      Top = 16
      Width = 15
      Height = 21
      Associate = Edit1
      Max = 15
      Position = 4
      TabOrder = 1
    end
    object RadioGroup1: TRadioGroup
      Left = 170
      Top = 12
      Width = 117
      Height = 73
      Hint = 'Format used when complex numbers are shown.'
      Anchors = [akTop, akRight]
      Caption = 'Complex form'
      ItemIndex = 1
      Items.Strings = (
        'Real'
        'Rectangular'
        'Polar')
      ParentShowHint = False
      ShowHint = True
      TabOrder = 12
    end
    object UpDown2: TUpDown
      Left = 145
      Top = 48
      Width = 15
      Height = 21
      Associate = Edit2
      Max = 9
      Position = 4
      TabOrder = 3
    end
    object Edit2: TEdit
      Left = 102
      Top = 48
      Width = 43
      Height = 21
      Hint = 'The number of most recent used files shown in the File menu.'
      Anchors = [akTop, akRight]
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      Text = '4'
    end
    object Edit3: TEdit
      Left = 102
      Top = 80
      Width = 43
      Height = 21
      Hint = 
        'Maximum number of undo steps saved. Set this to 0 to prevent und' +
        'o.'
      Anchors = [akTop, akRight]
      TabOrder = 4
      Text = '50'
    end
    object UpDown3: TUpDown
      Left = 145
      Top = 80
      Width = 15
      Height = 21
      Associate = Edit3
      Max = 1000
      Increment = 10
      Position = 50
      TabOrder = 5
    end
    object ComboBox1: TComboBox
      Left = 102
      Top = 112
      Width = 123
      Height = 21
      Hint = 
        'This is used to scale the user interface if you are having probl' +
        'ems reading the text.'
      AutoComplete = False
      Anchors = [akTop, akRight]
      ItemHeight = 13
      ItemIndex = 1
      TabOrder = 6
      Text = '100%'
      OnKeyPress = ComboBox1KeyPress
      Items.Strings = (
        '75%'
        '100%'
        '125%'
        '150%'
        '200%')
    end
    object ComboBox2: TComboBox
      Left = 102
      Top = 144
      Width = 123
      Height = 21
      Hint = 'Select the language to use for the user interface.'
      Style = csDropDownList
      Anchors = [akTop, akRight]
      ItemHeight = 0
      Sorted = True
      TabOrder = 7
    end
    object CheckBox1: TCheckBox
      Left = 16
      Top = 176
      Width = 274
      Height = 17
      Hint = 'Associate *.grf files with Graph.'
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Associate .grf files'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 8
    end
    object CheckBox2: TCheckBox
      Left = 16
      Top = 200
      Width = 274
      Height = 17
      Hint = 'Show the tooltip when the cursor is over an item.'
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Show tooltip'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 9
    end
    object CheckBox3: TCheckBox
      Left = 16
      Top = 224
      Width = 274
      Height = 17
      Hint = 
        'Save the size of the main window when the program exits. This is' +
        ' used the next time Graph is run.'
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Save workspace on exit'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 10
    end
    object CheckBox4: TCheckBox
      Left = 16
      Top = 248
      Width = 274
      Height = 17
      Hint = 
        'If checked the program will contact a web server when the progra' +
        'm is started to see if a newer version is available.'
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Check for new version of Graph at startup'
      TabOrder = 11
    end
  end
end
