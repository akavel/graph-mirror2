object AreaFrame: TAreaFrame
  Left = 0
  Top = 0
  Width = 182
  Height = 101
  Anchors = [akLeft, akTop, akRight]
  ParentBackground = False
  TabOrder = 0
  object GridPanel1: TGridPanel
    Left = 0
    Top = 0
    Width = 182
    Height = 101
    Align = alClient
    BevelOuter = bvNone
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
    Padding.Top = 4
    Padding.Right = 2
    Padding.Bottom = 5
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
    object Label1: TLabel
      Left = 4
      Top = 8
      Width = 28
      Height = 21
      Align = alLeft
      Caption = 'From:'
      Layout = tlCenter
      ExplicitHeight = 13
    end
    object Edit1: TMyEdit
      Left = 34
      Top = 8
      Width = 146
      Height = 21
      Hint = 
        'Enter the start coordinate or mark the range on the graph itself' +
        ' with the mouse.'
      Align = alClient
      TabOrder = 0
      ExplicitLeft = 36
      ExplicitWidth = 142
    end
    object Label2: TLabel
      Left = 4
      Top = 38
      Width = 16
      Height = 21
      Align = alLeft
      Caption = 'To:'
      Layout = tlCenter
      ExplicitHeight = 13
    end
    object Edit2: TMyEdit
      Left = 34
      Top = 38
      Width = 146
      Height = 21
      Hint = 
        'Enter the end coordinate or mark the range on the graph itself w' +
        'ith the mouse.'
      Align = alClient
      TabOrder = 1
      ExplicitLeft = 36
      ExplicitWidth = 142
    end
    object Label3: TLabel
      Left = 4
      Top = 68
      Width = 23
      Height = 21
      Align = alLeft
      Caption = 'Area'
      Layout = tlCenter
      ExplicitHeight = 13
    end
    object Edit3: TEdit
      Left = 34
      Top = 68
      Width = 146
      Height = 21
      Hint = 
        'Area or length of path calculated for the given range using Simp' +
        'son'#39's formula.'
      Align = alClient
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 2
      ExplicitLeft = 36
      ExplicitWidth = 142
    end
  end
end
