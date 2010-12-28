object Form3: TForm3
  Left = 414
  Top = 211
  HelpContext = 30
  BiDiMode = bdLeftToRight
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Edit axes'
  ClientHeight = 332
  ClientWidth = 519
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  ParentBiDiMode = False
  Position = poMainFormCenter
  ShowHint = True
  OnShow = FormShow
  DesignSize = (
    519
    332)
  PixelsPerInch = 120
  TextHeight = 16
  object PageControl1: TPageControl
    Left = 12
    Top = 10
    Width = 498
    Height = 262
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    ActivePage = TabSheet1
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    object TabSheet1: TTabSheet
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'x-axis'
      object GridPanel1: TGridPanelEx
        Left = 0
        Top = 0
        Width = 490
        Height = 231
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Align = alClient
        BevelOuter = bvNone
        ColumnCollection = <
          item
            SizeStyle = ssAuto
            Value = 50.000000000000000000
          end
          item
            Value = 100.000000000000000000
          end
          item
            SizeStyle = ssAuto
          end
          item
            SizeStyle = ssAuto
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
          end
          item
            Column = 0
            Control = Label14
            Row = 3
          end
          item
            Column = 1
            Control = Edit4
            Row = 3
          end
          item
            Column = 2
            Control = CheckBox4
            Row = 2
          end
          item
            Column = 3
            Control = CheckBox6
            Row = 2
          end
          item
            Column = 2
            Control = CheckBox5
            Row = 3
          end
          item
            Column = 3
            Control = CheckBox7
            Row = 3
          end
          item
            Column = 2
            ColumnSpan = 2
            Control = Panel2
            Row = 1
          end
          item
            Column = 2
            ColumnSpan = 2
            Control = Panel1
            Row = 0
          end
          item
            Column = 0
            ColumnSpan = 4
            Control = CheckBox1
            Row = 4
          end
          item
            Column = 0
            ColumnSpan = 2
            Control = CheckBox2
            Row = 5
          end
          item
            Column = 2
            ColumnSpan = 2
            Control = CheckBox17
            Row = 5
          end>
        Padding.Left = 4
        Padding.Right = 2
        RowCollection = <
          item
            SizeStyle = ssAbsolute
            Value = 39.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 39.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 39.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 39.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 34.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 34.000000000000000000
          end>
        TabOrder = 0
        DesignSize = (
          490
          231)
        object Label1: TLabel
          Left = 8
          Top = 0
          Width = 56
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Minimum:'
          FocusControl = Edit1
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitHeight = 16
        end
        object Edit1: TMyEdit
          Left = 74
          Top = 7
          Width = 135
          Height = 24
          Hint = 'Minimum x-value.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = [akLeft, akTop, akRight, akBottom]
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
        end
        object Label2: TLabel
          Left = 8
          Top = 39
          Width = 60
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Maximum:'
          FocusControl = Edit2
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitHeight = 16
        end
        object Edit2: TMyEdit
          Left = 74
          Top = 46
          Width = 135
          Height = 24
          Hint = 'Maximum x-value.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = [akLeft, akTop, akRight, akBottom]
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
        end
        object Label3: TLabel
          Left = 8
          Top = 78
          Width = 52
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Tick unit:'
          FocusControl = Edit3
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitTop = 79
          ExplicitHeight = 16
        end
        object Edit3: TMyEdit
          Left = 74
          Top = 85
          Width = 135
          Height = 24
          Hint = 'Units between ticks on the x-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = [akLeft, akTop, akRight, akBottom]
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
        end
        object Label14: TLabel
          Left = 8
          Top = 117
          Width = 51
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Grid unit:'
          FocusControl = Edit4
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitTop = 118
          ExplicitHeight = 16
        end
        object Edit4: TMyEdit
          Left = 74
          Top = 124
          Width = 135
          Height = 24
          Hint = 'Units between the grid lines perpendicular to the x-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = [akLeft, akTop, akRight, akBottom]
          TabOrder = 3
        end
        object CheckBox4: TCheckBoxEx
          Left = 216
          Top = 78
          Width = 73
          Height = 39
          Hint = 'Determine the tick unit automatically.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Auto tick'
          TabOrder = 8
          OnClick = CheckBoxClick
        end
        object CheckBox6: TCheckBoxEx
          Left = 298
          Top = 78
          Width = 86
          Height = 39
          Hint = 'Show tick marks on the x-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Show ticks'
          TabOrder = 10
        end
        object CheckBox5: TCheckBoxEx
          Left = 216
          Top = 117
          Width = 76
          Height = 39
          Hint = 'Set the grid unit to the same as the tick unit.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Auto grid'
          TabOrder = 9
          OnClick = CheckBoxClick
        end
        object CheckBox7: TCheckBoxEx
          Left = 298
          Top = 117
          Width = 82
          Height = 39
          Hint = 'Show vertical grid lines.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Show grid'
          TabOrder = 11
        end
        object Panel2: TPanel
          Left = 216
          Top = 39
          Width = 270
          Height = 39
          Margins.Left = 0
          Margins.Top = 6
          Margins.Right = 0
          Margins.Bottom = 0
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 7
          object Label4: TLabel
            Left = 0
            Top = 0
            Width = 128
            Height = 39
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Align = alLeft
            Caption = 'The x-axis cross at y='
            FocusControl = Edit6
            Layout = tlCenter
            ExplicitHeight = 16
          end
          object Edit6: TMyEdit
            AlignWithMargins = True
            Left = 132
            Top = 6
            Width = 138
            Height = 26
            Hint = 'The x-axis will intersect with the y-axis at this value.'
            Margins.Left = 4
            Margins.Top = 6
            Margins.Right = 0
            Margins.Bottom = 7
            Align = alClient
            TabOrder = 0
            ExplicitHeight = 24
          end
        end
        object Panel1: TPanel
          Left = 216
          Top = 0
          Width = 270
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 6
          object CheckBox3: TCheckBoxEx
            Left = 0
            Top = 0
            Width = 60
            Height = 39
            Hint = 
              'Show the label at the right side of the graphing area, above the' +
              ' x-axis.'
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Align = alLeft
            Caption = 'Label:'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            OnClick = CheckBoxClick
          end
          object Edit5: TMyEdit
            AlignWithMargins = True
            Left = 60
            Top = 6
            Width = 210
            Height = 26
            Hint = 'Edit the label for the axis here.'
            Margins.Left = 0
            Margins.Top = 6
            Margins.Right = 0
            Margins.Bottom = 7
            Align = alClient
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            ExplicitHeight = 24
          end
        end
        object CheckBox1: TCheckBoxEx
          Left = 8
          Top = 156
          Width = 128
          Height = 34
          Hint = 'Use logarithmic scale on the x-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Logarithmic scale'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 4
          OnClick = CheckBox1Click
        end
        object CheckBox2: TCheckBoxEx
          Left = 8
          Top = 190
          Width = 111
          Height = 34
          Hint = 'Show units on the x-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Show numbers'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 5
          OnClick = CheckBoxClick
        end
        object CheckBox17: TCheckBoxEx
          Left = 216
          Top = 190
          Width = 155
          Height = 34
          Hint = 'Show units on the selected axis as a fraction multiplied by pi.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Show as a multiple of '#960
          TabOrder = 12
        end
      end
    end
    object TabSheet2: TTabSheet
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'y-axis'
      object GridPanel2: TGridPanelEx
        Left = 0
        Top = 0
        Width = 490
        Height = 231
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Align = alClient
        BevelEdges = []
        BevelOuter = bvNone
        ColumnCollection = <
          item
            SizeStyle = ssAuto
            Value = 25.000000000000000000
          end
          item
            Value = 100.000000000000000000
          end
          item
            SizeStyle = ssAuto
          end
          item
            SizeStyle = ssAuto
          end>
        ControlCollection = <
          item
            Column = 0
            Control = Label5
            Row = 0
          end
          item
            Column = 1
            Control = Edit7
            Row = 0
          end
          item
            Column = 0
            Control = Label6
            Row = 1
          end
          item
            Column = 1
            Control = Edit8
            Row = 1
          end
          item
            Column = 0
            Control = Label7
            Row = 2
          end
          item
            Column = 1
            Control = Edit9
            Row = 2
          end
          item
            Column = 0
            Control = Label9
            Row = 3
          end
          item
            Column = 1
            Control = Edit10
            Row = 3
          end
          item
            Column = 2
            ColumnSpan = 2
            Control = Panel3
            Row = 0
          end
          item
            Column = 2
            ColumnSpan = 2
            Control = Panel4
            Row = 1
          end
          item
            Column = 2
            Control = CheckBox11
            Row = 2
          end
          item
            Column = 3
            Control = CheckBox13
            Row = 2
          end
          item
            Column = 2
            Control = CheckBox12
            Row = 3
          end
          item
            Column = 3
            Control = CheckBox14
            Row = 3
          end
          item
            Column = 0
            ColumnSpan = 4
            Control = CheckBox8
            Row = 4
          end
          item
            Column = 0
            ColumnSpan = 2
            Control = CheckBox9
            Row = 5
          end
          item
            Column = 2
            ColumnSpan = 2
            Control = CheckBox18
            Row = 5
          end>
        Padding.Left = 4
        Padding.Right = 2
        RowCollection = <
          item
            SizeStyle = ssAbsolute
            Value = 39.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 39.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 39.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 39.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 34.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 34.000000000000000000
          end>
        TabOrder = 0
        DesignSize = (
          490
          231)
        object Label5: TLabel
          Left = 8
          Top = 0
          Width = 56
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Minimum:'
          FocusControl = Edit7
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitHeight = 16
        end
        object Edit7: TMyEdit
          Left = 74
          Top = 7
          Width = 135
          Height = 24
          Hint = 'Minimum y-value.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = [akLeft, akTop, akRight, akBottom]
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
        end
        object Label6: TLabel
          Left = 8
          Top = 39
          Width = 60
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Maximum:'
          FocusControl = Edit8
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitHeight = 16
        end
        object Edit8: TMyEdit
          Left = 74
          Top = 46
          Width = 135
          Height = 24
          Hint = 'Maximum y-value.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = [akLeft, akTop, akRight, akBottom]
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
        end
        object Label7: TLabel
          Left = 8
          Top = 78
          Width = 52
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Tick unit:'
          FocusControl = Edit9
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitTop = 79
          ExplicitHeight = 16
        end
        object Edit9: TMyEdit
          Left = 74
          Top = 85
          Width = 135
          Height = 24
          Hint = 'Units between ticks on the y-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = [akLeft, akTop, akRight, akBottom]
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
        end
        object Label9: TLabel
          Left = 8
          Top = 117
          Width = 51
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Grid unit:'
          FocusControl = Edit10
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitTop = 118
          ExplicitHeight = 16
        end
        object Edit10: TMyEdit
          Left = 74
          Top = 124
          Width = 135
          Height = 24
          Hint = 'Units between the grid lines perpendicular to the y-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = [akLeft, akTop, akRight, akBottom]
          TabOrder = 3
        end
        object Panel3: TPanel
          Left = 216
          Top = 0
          Width = 270
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = []
          BevelOuter = bvNone
          TabOrder = 6
          object CheckBox10: TCheckBoxEx
            Left = 0
            Top = 0
            Width = 60
            Height = 39
            Hint = 
              'Show the label at the top of the graphing area, to the right of ' +
              'the y-axis.'
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Align = alLeft
            Caption = 'Label:'
            TabOrder = 0
            OnClick = CheckBoxClick
          end
          object Edit11: TMyEdit
            AlignWithMargins = True
            Left = 60
            Top = 6
            Width = 210
            Height = 26
            Hint = 'Edit the label for the axis here.'
            Margins.Left = 0
            Margins.Top = 6
            Margins.Right = 0
            Margins.Bottom = 7
            Align = alClient
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            ExplicitHeight = 24
          end
        end
        object Panel4: TPanel
          Left = 216
          Top = 39
          Width = 270
          Height = 39
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Anchors = []
          BevelOuter = bvNone
          TabOrder = 7
          object Label8: TLabel
            Left = 0
            Top = 0
            Width = 128
            Height = 39
            Margins.Left = 4
            Margins.Top = 4
            Margins.Right = 4
            Margins.Bottom = 4
            Align = alLeft
            Caption = 'The y-axis cross at x='
            FocusControl = Edit12
            Layout = tlCenter
            ExplicitHeight = 16
          end
          object Edit12: TMyEdit
            AlignWithMargins = True
            Left = 132
            Top = 6
            Width = 138
            Height = 26
            Hint = 'The y-axis will intersect with the x-axis at this value.'
            Margins.Left = 4
            Margins.Top = 6
            Margins.Right = 0
            Margins.Bottom = 7
            Align = alClient
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            ExplicitHeight = 24
          end
        end
        object CheckBox11: TCheckBoxEx
          Left = 216
          Top = 78
          Width = 73
          Height = 39
          Hint = 'Determine the tick unit automatically.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Auto tick'
          TabOrder = 8
          OnClick = CheckBoxClick
        end
        object CheckBox13: TCheckBoxEx
          Left = 298
          Top = 78
          Width = 86
          Height = 39
          Hint = 'Show tick marks on the y-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Show ticks'
          TabOrder = 10
        end
        object CheckBox12: TCheckBoxEx
          Left = 216
          Top = 117
          Width = 76
          Height = 39
          Hint = 'Set the grid unit to the same as the tick unit.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Auto grid'
          TabOrder = 9
          OnClick = CheckBoxClick
        end
        object CheckBox14: TCheckBoxEx
          Left = 298
          Top = 117
          Width = 82
          Height = 39
          Hint = 'Show horizontal grid lines.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Show grid'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 11
        end
        object CheckBox8: TCheckBoxEx
          Left = 8
          Top = 156
          Width = 128
          Height = 34
          Hint = 'Use logarithmic scale for the y-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Logarithmic scale'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 4
          OnClick = CheckBox8Click
        end
        object CheckBox9: TCheckBoxEx
          Left = 8
          Top = 190
          Width = 111
          Height = 34
          Hint = 'Show units on the y-axis.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Show numbers'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 5
          OnClick = CheckBoxClick
        end
        object CheckBox18: TCheckBoxEx
          Left = 216
          Top = 190
          Width = 162
          Height = 34
          Hint = 'Show units on the selected axis as a fraction multiplied by pi.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Align = alLeft
          Caption = 'Show as a multiple of pi'
          TabOrder = 12
        end
      end
    end
    object TabSheet3: TTabSheet
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Settings'
      ImageIndex = 2
      DesignSize = (
        490
        231)
      object Label16: TLabel
        Left = 10
        Top = 15
        Width = 29
        Height = 16
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Title:'
        FocusControl = Edit13
      end
      object CheckBox15: TCheckBoxEx
        Left = 10
        Top = 49
        Width = 101
        Height = 21
        Hint = 'Select to show a legend in the coordinate system.'
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Show legend'
        TabOrder = 2
        OnClick = CheckBox15Click
      end
      object RadioGroup1: TRadioGroup
        Left = 187
        Top = 79
        Width = 143
        Height = 90
        Hint = 
          'Choose if you want to show the axes, and if you want to show the' +
          'm crossed or at the left and bottom of the image.'
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akTop, akRight]
        Caption = 'Axes style'
        ItemIndex = 1
        Items.Strings = (
          'None'
          'Crossed'
          'Boxed')
        TabOrder = 4
        OnClick = RadioGroup1Click
      end
      object RadioGroup2: TRadioGroup
        Left = 348
        Top = 79
        Width = 120
        Height = 70
        Hint = 
          'Choose if trigonometric functions should calculate in radians or' +
          ' degrees.'
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akTop, akRight]
        Caption = 'Trigonometry'
        ItemIndex = 0
        Items.Strings = (
          'Radian'
          'Degree')
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
      end
      object Edit13: TMyEdit
        Left = 49
        Top = 10
        Width = 409
        Height = 24
        Hint = 'Enter a title to show above the coordinate system.'
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
      end
      object BitBtn1: TBitBtn
        Left = 457
        Top = 10
        Width = 25
        Height = 26
        Hint = 'Select the font for the title.'
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akTop, akRight]
        DoubleBuffered = True
        Glyph.Data = {
          F6000000424DF600000000000000760000002800000010000000100000000100
          04000000000080000000230B0000230B00001000000010000000A64D4D00FF4D
          4D00D1D8DB00FFFFFF0000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000333333333333
          3333333000000000000333333333333333333330000033300003333330033333
          0033333333003333003311111330000000333311333300330033333113333003
          0033333311111100003333333113311000333333331131130033333333311113
          3333333333331113333333333333311333333333333333333333}
        ParentDoubleBuffered = False
        TabOrder = 1
        OnClick = BitBtn1Click
      end
      object RadioGroup3: TRadioGroup
        Left = 10
        Top = 79
        Width = 159
        Height = 109
        Hint = 'Choose where the legend should be placed.'
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Legend placement'
        Items.Strings = (
          'Top Right'
          'Bottom Right'
          'Top Left'
          'Bottom Left')
        TabOrder = 3
      end
      object CheckBox19: TCheckBoxEx
        Left = 187
        Top = 49
        Width = 213
        Height = 21
        Hint = 
          'Check this to use complex numbers for calculations. Notice that ' +
          'this will slow graphing down.'
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Calculate with complex numbers'
        TabOrder = 6
      end
    end
    object TabSheet4: TTabSheet
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Caption = 'Font and color'
      ImageIndex = 3
      DesignSize = (
        490
        231)
      object GroupBox3: TGroupBox
        Left = 10
        Top = 10
        Width = 237
        Height = 139
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Caption = 'Colors'
        TabOrder = 0
        object Label17: TLabel
          Left = 10
          Top = 101
          Width = 61
          Height = 16
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Grid color:'
          FocusControl = ExtColorBox3
        end
        object Label18: TLabel
          Left = 10
          Top = 62
          Width = 66
          Height = 16
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Axes color:'
          FocusControl = ExtColorBox2
        end
        object Label19: TLabel
          Left = 10
          Top = 22
          Width = 109
          Height = 16
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Background color:'
          FocusControl = ExtColorBox1
        end
        object ExtColorBox1: TExtColorBox
          Left = 128
          Top = 20
          Width = 100
          Height = 25
          Hint = 'Select the color of the image background.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          ItemHeight = 19
          TabOrder = 0
          AutoDroppedWidth = True
          Selected = clScrollBar
          DefaultName = 'Default'
          CustomName = 'Custom...'
        end
        object ExtColorBox2: TExtColorBox
          Left = 128
          Top = 59
          Width = 100
          Height = 25
          Hint = 'Select the color of the axes.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          ItemHeight = 19
          TabOrder = 1
          AutoDroppedWidth = True
          Selected = clScrollBar
          DefaultName = 'Default'
          CustomName = 'Custom...'
        end
        object ExtColorBox3: TExtColorBox
          Left = 128
          Top = 98
          Width = 100
          Height = 25
          Hint = 'Select the color of the grid lines.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          ItemHeight = 19
          TabOrder = 2
          AutoDroppedWidth = True
          Selected = clScrollBar
          DefaultName = 'Default'
          CustomName = 'Custom...'
        end
      end
      object GroupBox4: TGroupBox
        Left = 289
        Top = 10
        Width = 179
        Height = 139
        Margins.Left = 4
        Margins.Top = 4
        Margins.Right = 4
        Margins.Bottom = 4
        Anchors = [akTop, akRight]
        Caption = 'Fonts'
        TabOrder = 1
        object Label20: TLabel
          Left = 10
          Top = 62
          Width = 75
          Height = 16
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Number font:'
          FocusControl = BitBtn3
        end
        object Label21: TLabel
          Left = 10
          Top = 22
          Width = 61
          Height = 16
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Label font:'
          FocusControl = BitBtn2
        end
        object Label10: TLabel
          Left = 10
          Top = 98
          Width = 73
          Height = 16
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          Caption = 'Legend font:'
          FocusControl = BitBtn4
        end
        object BitBtn2: TBitBtn
          Left = 143
          Top = 17
          Width = 26
          Height = 26
          Hint = 'Select the font used to show the labels on the axes.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          DoubleBuffered = True
          Glyph.Data = {
            F6000000424DF600000000000000760000002800000010000000100000000100
            04000000000080000000130B0000130B00001000000010000000A64D4D00FF4D
            4D00D1D8DB000000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000222222222222
            2222222000000000000222222222222222222220000022200002222220022222
            0022222222002222002211111220000000222211222200220022222112222002
            0022222211111100002222222112211000222222221121120022222222211112
            2222222222221112222222222222211222222222222222222222}
          ParentDoubleBuffered = False
          TabOrder = 0
          OnClick = BitBtn2Click
        end
        object BitBtn3: TBitBtn
          Left = 143
          Top = 57
          Width = 26
          Height = 25
          Hint = 'Select the font used to show the numbers on the axes.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          DoubleBuffered = True
          Glyph.Data = {
            F6000000424DF600000000000000760000002800000010000000100000000100
            04000000000080000000130B0000130B00001000000010000000A64D4D00FF4D
            4D00D1D8DB000000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000222222222222
            2222222000000000000222222222222222222220000022200002222220022222
            0022222222002222002211111220000000222211222200220022222112222002
            0022222211111100002222222112211000222222221121120022222222211112
            2222222222221112222222222222211222222222222222222222}
          ParentDoubleBuffered = False
          TabOrder = 1
          OnClick = BitBtn3Click
        end
        object BitBtn4: TBitBtn
          Left = 143
          Top = 96
          Width = 26
          Height = 26
          Hint = 'Select the font used to show the legend text.'
          Margins.Left = 4
          Margins.Top = 4
          Margins.Right = 4
          Margins.Bottom = 4
          DoubleBuffered = True
          Glyph.Data = {
            F6000000424DF600000000000000760000002800000010000000100000000100
            04000000000080000000130B0000130B00001000000010000000A64D4D00FF4D
            4D00D1D8DB000000000000000000000000000000000000000000000000000000
            0000000000000000000000000000000000000000000000000000222222222222
            2222222000000000000222222222222222222220000022200002222220022222
            0022222222002222002211111220000000222211222200220022222112222002
            0022222211111100002222222112211000222222221121120022222222211112
            2222222222221112222222222222211222222222222222222222}
          ParentDoubleBuffered = False
          TabOrder = 2
          OnClick = BitBtn4Click
        end
      end
    end
  end
  object Button2: TButton
    Left = 309
    Top = 289
    Width = 92
    Height = 31
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object Button3: TButton
    Left = 417
    Top = 289
    Width = 93
    Height = 31
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Anchors = [akRight, akBottom]
    Caption = 'Help'
    TabOrder = 4
    OnClick = Button3Click
  end
  object CheckBox16: TCheckBoxEx
    Left = 10
    Top = 294
    Width = 116
    Height = 21
    Hint = 'Select this to use these settings as defaults in the future.'
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Anchors = [akLeft, akBottom]
    Caption = 'Save as default'
    TabOrder = 1
  end
  object Button1: TButton
    Left = 201
    Top = 289
    Width = 92
    Height = 31
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 2
    OnClick = Button1Click
  end
  object FontDialog1: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [fdEffects, fdForceFontExist]
    Left = 336
    Top = 192
  end
end
