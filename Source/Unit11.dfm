object Form11: TForm11
  Left = 342
  Top = 215
  HelpContext = 20
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Insert relation'
  ClientHeight = 226
  ClientWidth = 396
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
    396
    226)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TTntLabel
    Left = 8
    Top = 12
    Width = 42
    Height = 13
    Caption = 'Relation:'
  end
  object Label2: TTntLabel
    Left = 8
    Top = 44
    Width = 55
    Height = 13
    Caption = 'Constraints:'
  end
  object Button1: TTntButton
    Left = 137
    Top = 192
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TTntButton
    Left = 225
    Top = 192
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
  end
  object Button3: TTntButton
    Left = 313
    Top = 192
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Help'
    TabOrder = 6
    OnClick = Button3Click
  end
  object GroupBox1: TTntGroupBox
    Left = 8
    Top = 72
    Width = 378
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Legend text'
    TabOrder = 2
    DesignSize = (
      378
      49)
    object Label10: TTntLabel
      Left = 8
      Top = 20
      Width = 56
      Height = 13
      Caption = 'Description:'
      FocusControl = Edit3
    end
    object Edit3: TTntEdit
      Left = 72
      Top = 18
      Width = 298
      Height = 21
      Hint = 
        'Write a descriptive text to show in the legend. If empty the equ' +
        'ation will be used.'
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
  end
  object GroupBox2: TTntGroupBox
    Left = 9
    Top = 130
    Width = 379
    Height = 47
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Properties'
    TabOrder = 3
    object Label7: TTntLabel
      Left = 10
      Top = 20
      Width = 26
      Height = 13
      Alignment = taRightJustify
      Caption = 'Style:'
      FocusControl = ShadeSelect1
    end
    object Label9: TTntLabel
      Left = 144
      Top = 20
      Width = 27
      Height = 13
      Alignment = taRightJustify
      Caption = 'Color:'
      FocusControl = ExtColorBox1
    end
    object Label3: TTntLabel
      Left = 276
      Top = 20
      Width = 31
      Height = 13
      Alignment = taRightJustify
      Caption = 'Width:'
      FocusControl = Edit4
    end
    object ShadeSelect1: TShadeSelect
      Left = 40
      Top = 16
      Width = 81
      Height = 22
      Hint = 
        'Select the style used for drawing the inequality. Not used for e' +
        'quations.'
      TabOrder = 0
    end
    object Edit4: TTntEdit
      Left = 312
      Top = 16
      Width = 41
      Height = 21
      Hint = 
        'Width in pixels of the equation line or the borderline of the in' +
        'equality.'
      TabOrder = 2
      Text = '1'
      OnKeyPress = Edit4KeyPress
    end
    object UpDown1: TUpDown
      Left = 353
      Top = 16
      Width = 15
      Height = 21
      Associate = Edit4
      Min = 0
      Max = 20
      Position = 1
      TabOrder = 3
      Wrap = False
    end
    object ExtColorBox1: TExtColorBox
      Left = 176
      Top = 16
      Width = 81
      Height = 22
      Hint = 'Select the color for drawing the relation.'
      Selected = clDefault
      Style = [cbStandardColors, cbExtendedColors, cbCustomColor, cbPrettyNames]
      ItemHeight = 16
      TabOrder = 1
      DroppedWidth = 130
    end
  end
  object Edit1: TMyEdit
    Left = 64
    Top = 8
    Width = 322
    Height = 21
    Hint = 'Enter the relation, for example "x^2+y^2 = 25" or "y < sin(x)"'
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object Edit2: TMyEdit
    Left = 64
    Top = 40
    Width = 321
    Height = 21
    Hint = 
      'Enter constraints for the relation, for example "x > 0 and y < 5' +
      '"'
    TabOrder = 1
  end
end
