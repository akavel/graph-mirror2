object Form5: TForm5
  Left = 379
  Top = 228
  HelpContext = 50
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Insert function'
  ClientHeight = 424
  ClientWidth = 353
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  DesignSize = (
    353
    424)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TTntLabel
    Left = 16
    Top = 18
    Width = 67
    Height = 13
    Caption = 'Function type:'
    FocusControl = ComboBox1
  end
  object Button1: TTntButton
    Left = 89
    Top = 392
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 6
    OnClick = Button1Click
  end
  object Button2: TTntButton
    Left = 177
    Top = 392
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 7
  end
  object ComboBox1: TTntComboBox
    Left = 88
    Top = 16
    Width = 250
    Height = 21
    Hint = 'Select the function type to graph.'
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 0
    OnChange = ComboBox1Change
    OnKeyPress = ComboBox1KeyPress
    Items.Strings = (
      'Standard function   y=f(x)'
      'Parametric function x(t), y(t)'
      'Polar function         r=f(t)')
  end
  object GroupBox1: TTntGroupBox
    Left = 16
    Top = 48
    Width = 322
    Height = 73
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Function equation'
    TabOrder = 1
    DesignSize = (
      322
      73)
    object Label2: TTntLabel
      Left = 8
      Top = 20
      Width = 20
      Height = 13
      Caption = 'x(t)='
    end
    object Label3: TTntLabel
      Left = 8
      Top = 48
      Width = 20
      Height = 13
      Caption = 'y(t)='
    end
    object Edit1: TMyEdit
      Left = 32
      Top = 16
      Width = 282
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
    object Edit2: TMyEdit
      Left = 32
      Top = 44
      Width = 282
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
    end
  end
  object GroupBox2: TTntGroupBox
    Left = 16
    Top = 128
    Width = 322
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Argument range'
    TabOrder = 2
    DesignSize = (
      322
      49)
    object Label4: TTntLabel
      Left = 8
      Top = 20
      Width = 26
      Height = 13
      Caption = 'From:'
      FocusControl = Edit3
    end
    object Label5: TTntLabel
      Left = 124
      Top = 20
      Width = 16
      Height = 13
      Caption = 'To:'
      FocusControl = Edit4
    end
    object Label6: TTntLabel
      Left = 232
      Top = 20
      Width = 30
      Height = 13
      Caption = 'Steps:'
      FocusControl = Edit5
    end
    object Edit3: TTntEdit
      Left = 38
      Top = 18
      Width = 74
      Height = 21
      Hint = 'Start of range interval for the used variable.'
      TabOrder = 0
    end
    object Edit4: TTntEdit
      Left = 144
      Top = 18
      Width = 74
      Height = 21
      Hint = 'End of range interval for the used variable.'
      TabOrder = 1
    end
    object Edit5: TTntEdit
      Left = 264
      Top = 18
      Width = 49
      Height = 21
      Hint = 'Number of calculation steps used to draw the graph.'
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 2
      OnKeyPress = EditKeyPress
    end
  end
  object GroupBox4: TTntGroupBox
    Left = 16
    Top = 296
    Width = 322
    Height = 84
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Graph properties'
    TabOrder = 5
    DesignSize = (
      322
      84)
    object Label7: TTntLabel
      Left = 8
      Top = 20
      Width = 47
      Height = 13
      Caption = 'Line style:'
      FocusControl = LineSelect1
    end
    object Label8: TTntLabel
      Left = 176
      Top = 56
      Width = 31
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'Width:'
      FocusControl = Edit6
    end
    object Label9: TTntLabel
      Left = 8
      Top = 54
      Width = 27
      Height = 13
      Caption = 'Color:'
      FocusControl = ExtColorBox1
    end
    object Label13: TTntLabel
      Left = 176
      Top = 20
      Width = 51
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'Draw type:'
      FocusControl = ComboBox4
    end
    object Edit6: TTntEdit
      Left = 232
      Top = 50
      Width = 41
      Height = 21
      Hint = 'Width of graph in pixels.'
      Anchors = [akTop, akRight]
      TabOrder = 3
      Text = '1'
      OnKeyPress = EditKeyPress
    end
    object UpDown1: TUpDown
      Left = 273
      Top = 50
      Width = 15
      Height = 21
      Anchors = [akTop, akRight]
      Associate = Edit6
      Min = 1
      Max = 20
      Position = 1
      TabOrder = 4
      Wrap = False
    end
    object LineSelect1: TLineSelect
      Left = 64
      Top = 18
      Width = 81
      Height = 21
      Hint = 'Choose which line style to use for drawing the graph.'
      TabOrder = 0
      ItemHeight = 15
    end
    object ComboBox4: TTntComboBox
      Left = 232
      Top = 18
      Width = 81
      Height = 21
      Hint = 
        'Choose Lines if you want the calculated points for the graph to ' +
        'be connected. Choose Dots if you only want to show a dot at each' +
        ' point.'
      Style = csDropDownList
      Anchors = [akTop, akRight]
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 1
      Text = 'Automatic'
      OnChange = ComboBox4Change
      Items.Strings = (
        'Automatic'
        'Dots'
        'Lines')
    end
    object ExtColorBox1: TExtColorBox
      Left = 64
      Top = 50
      Width = 81
      Height = 22
      Hint = 'Select the color of the graph.'
      Selected = clDefault
      Style = [cbStandardColors, cbExtendedColors, cbCustomColor, cbPrettyNames]
      ItemHeight = 16
      TabOrder = 2
      AutoDroppedWidth = True
    end
  end
  object Button3: TTntButton
    Left = 265
    Top = 392
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Help'
    TabOrder = 8
    OnClick = Button3Click
  end
  object GroupBox3: TTntGroupBox
    Left = 16
    Top = 240
    Width = 321
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Legend text'
    TabOrder = 4
    DesignSize = (
      321
      49)
    object Label10: TTntLabel
      Left = 8
      Top = 20
      Width = 56
      Height = 13
      Caption = 'Description:'
      FocusControl = Edit7
    end
    object Edit7: TTntEdit
      Left = 72
      Top = 18
      Width = 241
      Height = 21
      Hint = 
        'Write a descriptive text to show in the legend. If empty the equ' +
        'ation will be used.'
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
  end
  object GroupBox5: TTntGroupBox
    Left = 16
    Top = 184
    Width = 321
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Endpoints'
    TabOrder = 3
    DesignSize = (
      321
      49)
    object Label11: TTntLabel
      Left = 8
      Top = 20
      Width = 25
      Height = 13
      Caption = 'Start:'
      FocusControl = ComboBox2
    end
    object Label12: TTntLabel
      Left = 216
      Top = 20
      Width = 22
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'End:'
      FocusControl = ComboBox3
    end
    object ComboBox2: TTntComboBox
      Left = 40
      Top = 16
      Width = 65
      Height = 22
      Hint = 
        'Select marker to show at the start of the interval. Only used if' +
        ' an argument range is specified.'
      Style = csOwnerDrawFixed
      ItemHeight = 16
      TabOrder = 0
      OnDrawItem = ComboBoxDrawItem
      Items.Strings = (
        ''
        ''
        ''
        ''
        ''
        '')
    end
    object ComboBox3: TTntComboBox
      Left = 248
      Top = 16
      Width = 65
      Height = 22
      Hint = 
        'Select marker to show at the end of the interval. Only used if a' +
        'n argument range is specified.'
      Style = csOwnerDrawFixed
      Anchors = [akTop, akRight]
      ItemHeight = 16
      TabOrder = 1
      OnDrawItem = ComboBoxDrawItem
      Items.Strings = (
        ''
        ''
        ''
        ''
        ''
        '')
    end
  end
end
