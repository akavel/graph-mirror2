object Form11: TForm11
  Left = 342
  Top = 215
  HelpContext = 20
  BiDiMode = bdLeftToRight
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Insert relation'
  ClientHeight = 243
  ClientWidth = 396
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
  OnShow = FormShow
  DesignSize = (
    396
    243)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 12
    Width = 42
    Height = 13
    Caption = 'Relation:'
    FocusControl = Edit1
  end
  object Label2: TLabel
    Left = 8
    Top = 44
    Width = 55
    Height = 13
    Caption = 'Constraints:'
    FocusControl = Edit2
  end
  object Button1: TButton
    Left = 137
    Top = 211
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 225
    Top = 211
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
  end
  object Button3: TButton
    Left = 313
    Top = 211
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Help'
    TabOrder = 6
    OnClick = Button3Click
  end
  object GroupBox1: TGroupBox
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
    object Label10: TLabel
      Left = 8
      Top = 20
      Width = 56
      Height = 13
      Caption = 'Description:'
      FocusControl = Edit3
    end
    object Edit3: TMyEdit
      Left = 72
      Top = 18
      Width = 298
      Height = 21
      Hint = 
        'Write a descriptive text to show in the legend and function list' +
        '. If empty the formula will be used.'
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 127
    Width = 379
    Height = 78
    Margins.Right = 8
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Properties'
    TabOrder = 3
    object Label7: TLabel
      AlignWithMargins = True
      Left = 6
      Top = 22
      Width = 26
      Height = 13
      Margins.Left = 6
      AutoSize = False
      Caption = 'Style:'
      FocusControl = ShadeSelect1
      Layout = tlCenter
    end
    object Label9: TLabel
      AlignWithMargins = True
      Left = 228
      Top = 22
      Width = 27
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Color:'
      FocusControl = ExtColorBox1
      Layout = tlCenter
    end
    object Label3: TLabel
      AlignWithMargins = True
      Left = 228
      Top = 50
      Width = 31
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Width:'
      FocusControl = Edit4
      Layout = tlCenter
    end
    object Label4: TLabel
      Left = 8
      Top = 50
      Width = 47
      Height = 13
      Caption = 'Line style:'
      FocusControl = LineSelect1
    end
    object ShadeSelect1: TShadeSelect
      AlignWithMargins = True
      Left = 61
      Top = 18
      Width = 81
      Height = 22
      Hint = 
        'Select the style used for drawing the inequality. Not used for e' +
        'quations.'
      TabOrder = 0
    end
    object ExtColorBox1: TExtColorBox
      AlignWithMargins = True
      Left = 274
      Top = 18
      Width = 81
      Height = 22
      Hint = 'Select the color for drawing the relation.'
      Margins.Right = 12
      TabOrder = 1
      AutoDroppedWidth = True
      Selected = clScrollBar
      DefaultName = 'Default'
      CustomName = 'Custom...'
    end
    object LineSelect1: TLineSelect
      Left = 61
      Top = 46
      Width = 81
      Height = 22
      Hint = 
        'Select line style used to draw equations and the border of inequ' +
        'alities.'
      TabOrder = 2
    end
    object Edit4: TEdit
      Left = 274
      Top = 46
      Width = 41
      Height = 21
      Hint = 
        'Width in pixels of the equation line or the borderline of the in' +
        'equality.'
      TabOrder = 3
      Text = '1'
      OnKeyPress = Edit4KeyPress
    end
    object UpDown1: TUpDown
      Left = 315
      Top = 46
      Width = 15
      Height = 21
      Associate = Edit4
      Max = 20
      Position = 1
      TabOrder = 4
    end
  end
  object Edit1: TMyEdit
    Left = 64
    Top = 8
    Width = 322
    Height = 21
    Hint = 'Enter the relation, for example "x^2+y^2 = 25" or "y < sin(x)"'
    Anchors = [akLeft, akTop, akRight]
    BiDiMode = bdLeftToRight
    ParentBiDiMode = False
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
    BiDiMode = bdLeftToRight
    ParentBiDiMode = False
    TabOrder = 1
  end
end
