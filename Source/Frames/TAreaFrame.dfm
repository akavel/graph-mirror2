object AreaFrame: TAreaFrame
  Left = 0
  Top = 0
  Width = 149
  Height = 106
  Anchors = [akLeft, akTop, akRight]
  TabOrder = 0
  ExplicitWidth = 150
  object GridPanel1: TGridPanel
    Left = 0
    Top = 0
    Width = 137
    Height = 97
    ColumnCollection = <
      item
        SizeStyle = ssAuto
        Value = 50.000000000000000000
      end
      item
        SizeStyle = ssAuto
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
      137
      97)
    object Label1: TLabel
      Left = 5
      Top = 11
      Width = 28
      Height = 13
      Anchors = []
      Caption = 'From:'
      ExplicitLeft = -5
      ExplicitTop = 16
    end
    object Edit1: TMyEdit
      Left = 35
      Top = 5
      Width = 92
      Height = 19
      Hint = 
        'Enter the start coordinate or mark the range on the graph itself' +
        ' with the mouse.'
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
    object Label2: TLabel
      Left = 11
      Top = 41
      Width = 16
      Height = 13
      Anchors = []
      Caption = 'To:'
      ExplicitLeft = 14
      ExplicitTop = 23
    end
    object Edit2: TMyEdit
      Left = 35
      Top = 35
      Width = 92
      Height = 20
      Hint = 
        'Enter the end coordinate or mark the range on the graph itself w' +
        'ith the mouse.'
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
    end
    object Label3: TLabel
      Left = 7
      Top = 71
      Width = 23
      Height = 13
      Anchors = []
      Caption = 'Area'
      ExplicitLeft = 0
      ExplicitTop = 68
    end
    object Edit3: TEdit
      Left = 35
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
