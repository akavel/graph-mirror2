object AreaFrame: TAreaFrame
  Left = 0
  Top = 0
  Width = 184
  Height = 102
  Anchors = [akLeft, akTop, akRight]
  TabOrder = 0
  DesignSize = (
    184
    102)
  object Label1: TTntLabel
    Left = 8
    Top = 10
    Width = 26
    Height = 13
    Caption = 'From:'
    FocusControl = Edit1
  end
  object Label2: TTntLabel
    Left = 8
    Top = 42
    Width = 16
    Height = 13
    Caption = 'To:'
    FocusControl = Edit2
  end
  object Label3: TTntLabel
    Left = 8
    Top = 74
    Width = 22
    Height = 13
    Caption = 'Area'
    FocusControl = Edit3
  end
  object Edit1: TMyEdit
    Left = 48
    Top = 8
    Width = 127
    Height = 21
    Hint = 
      'Enter a start coordinate or mark the range on the image with the' +
      ' mouse.'
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object Edit2: TMyEdit
    Left = 48
    Top = 40
    Width = 127
    Height = 21
    Hint = 
      'Enter the end coordinate or mark the range on the image with the' +
      ' mouse.'
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
  end
  object Edit3: TTntEdit
    Left = 48
    Top = 72
    Width = 127
    Height = 21
    Hint = 
      'Area or length of path calculated for the given range using Simp' +
      'son'#39's formula.'
    Anchors = [akLeft, akTop, akRight]
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 2
  end
end
