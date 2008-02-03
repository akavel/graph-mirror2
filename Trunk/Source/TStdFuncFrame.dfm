object StdFuncFrame: TStdFuncFrame
  Left = 0
  Top = 0
  Width = 184
  Height = 167
  Anchors = [akLeft, akTop, akRight]
  TabOrder = 0
  DesignSize = (
    184
    167)
  object Label2: TLabel
    Left = 8
    Top = 42
    Width = 25
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'f(x)='
  end
  object Label3: TLabel
    Left = 8
    Top = 74
    Width = 25
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'f'#39'(x)='
  end
  object Label4: TLabel
    Left = 9
    Top = 106
    Width = 24
    Height = 13
    Alignment = taRightJustify
    Caption = 'f'#39#39'(x)='
  end
  object Label1: TLabel
    Left = 8
    Top = 10
    Width = 25
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'x='
  end
  object Label5: TTntLabel
    Left = 8
    Top = 140
    Width = 40
    Height = 13
    Caption = 'Snap to:'
    FocusControl = ComboBox1
  end
  object Edit1: TMyEdit
    Left = 40
    Top = 8
    Width = 135
    Height = 21
    Hint = 
      'Enter a value for which you want to evaluate the function, or cl' +
      'ick on the graph in the graphing area.'
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object ComboBox1: TTntComboBox
    Left = 64
    Top = 136
    Width = 111
    Height = 21
    Hint = 
      'Select where the cursor will be placed when you click on the ima' +
      'ge.'
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 4
    Text = 'Function'
    OnChange = ComboBox1Change
    Items.Strings = (
      'Function'
      'Intersection'
      'x-axis'
      'Extremum')
  end
  object Edit2: TTntEdit
    Left = 40
    Top = 40
    Width = 137
    Height = 23
    Hint = 'This is the function evaluated for the entered x-value.'
    Anchors = [akLeft, akTop, akRight]
    Color = clBtnFace
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Lucida Sans Unicode'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
  end
  object Edit3: TTntEdit
    Left = 40
    Top = 72
    Width = 137
    Height = 23
    Hint = 
      'This is the first derivative of the function evaluated for the e' +
      'ntered value.'
    Anchors = [akLeft, akTop, akRight]
    Color = clBtnFace
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Lucida Sans Unicode'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
  end
  object Edit4: TTntEdit
    Left = 40
    Top = 104
    Width = 137
    Height = 23
    Hint = 
      'This is the second derivative of the function evaluated for the ' +
      'entered value.'
    Anchors = [akLeft, akTop, akRight]
    Color = clBtnFace
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Lucida Sans Unicode'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 3
  end
end
