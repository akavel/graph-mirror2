object AreaFrame: TAreaFrame
  Left = 0
  Top = 0
  Width = 221
  Height = 106
  Anchors = [akLeft, akTop, akRight]
  TabOrder = 0
  object GridPanel1: TGridPanel
    Left = 0
    Top = 3
    Width = 185
    Height = 97
    ColumnCollection = <
      item
        SizeStyle = ssAuto
        Value = 50.000000000000000000
      end
      item
        Value = 100.000000000000000000
      end>
    ControlCollection = <
      item
        Column = 0
        Control = Label1
        Row = 0
      end
      item
        Column = 1
        Control = Edit1
        Row = 0
      end
      item
        Column = 0
        Control = Label2
        Row = 1
      end
      item
        Column = 1
        Control = Edit2
        Row = 1
      end
      item
        Column = 0
        Control = Label3
        Row = 2
      end
      item
        Column = 1
        Control = Edit3
        Row = 2
      end>
    Padding.Left = 2
    Padding.Top = 2
    Padding.Bottom = 2
    RowCollection = <
      item
        SizeStyle = ssAbsolute
        Value = 30.000000000000000000
      end
      item
        SizeStyle = ssAbsolute
        Value = 30.000000000000000000
      end
      item
        SizeStyle = ssAbsolute
        Value = 30.000000000000000000
      end>
    TabOrder = 0
    DesignSize = (
      185
      97)
    object Label1: TLabel
      Left = 5
      Top = 5
      Width = 28
      Height = 26
      Align = alLeft
      Caption = 'From:'
      Layout = tlCenter
      ExplicitHeight = 13
    end
    object Edit1: TMyEdit
      Left = 35
      Top = 5
      Width = 105
      Height = 26
      Hint = 
        'Enter the start coordinate or mark the range on the graph itself' +
        ' with the mouse.'
      Align = alLeft
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      ExplicitWidth = 109
      ExplicitHeight = 21
    end
    object Label2: TLabel
      Left = 5
      Top = 35
      Width = 16
      Height = 26
      Align = alLeft
      Caption = 'To:'
      Layout = tlCenter
      ExplicitHeight = 13
    end
    object Edit2: TMyEdit
      Left = 35
      Top = 35
      Width = 92
      Height = 26
      Hint = 
        'Enter the end coordinate or mark the range on the graph itself w' +
        'ith the mouse.'
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 21
    end
    object Label3: TLabel
      Left = 5
      Top = 65
      Width = 23
      Height = 26
      Align = alLeft
      Caption = 'Area'
      Layout = tlCenter
      ExplicitHeight = 13
    end
    object Edit3: TEdit
      Left = 63
      Top = 65
      Width = 92
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
end
