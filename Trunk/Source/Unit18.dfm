object Form18: TForm18
  Left = 346
  Top = 182
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Image options'
  ClientHeight = 250
  ClientWidth = 304
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
    304
    250)
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TTntPageControl
    Left = 8
    Top = 8
    Width = 287
    Height = 193
    ActivePage = TabSheet2
    Anchors = [akLeft, akTop, akRight]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabIndex = 1
    TabOrder = 0
    object TabSheet1: TTntTabSheet
      Caption = 'Size'
      object Label1: TTntLabel
        Left = 8
        Top = 92
        Width = 60
        Height = 13
        Caption = 'Image width:'
      end
      object Label2: TTntLabel
        Left = 8
        Top = 124
        Width = 64
        Height = 13
        Caption = 'Image height:'
      end
      object Edit1: TTntEdit
        Left = 80
        Top = 88
        Width = 121
        Height = 21
        Hint = 'Set a custom image width.'
        TabOrder = 0
        OnKeyPress = EditKeyPress
      end
      object Edit2: TTntEdit
        Left = 80
        Top = 120
        Width = 121
        Height = 21
        Hint = 'Set a custom image height.'
        TabOrder = 1
        OnKeyPress = EditKeyPress
      end
      object RadioGroup3: TTntRadioGroup
        Left = 8
        Top = 8
        Width = 193
        Height = 65
        Hint = 
          'Choose to use the same size as shown on the screen or a custom i' +
          'mage size.'
        Caption = 'Image size'
        ItemIndex = 0
        Items.Strings = (
          'Screen size'
          'Custom size')
        TabOrder = 2
        OnClick = RadioGroup3Click
      end
    end
    object TabSheet2: TTntTabSheet
      Caption = 'JPEG'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImageIndex = 1
      ParentFont = False
      DesignSize = (
        279
        165)
      object RadioGroup1: TTntRadioGroup
        Left = 8
        Top = 72
        Width = 145
        Height = 73
        Hint = 
          'Progressive encoding allows viewers to see a rough copy of the i' +
          'mage while it is downloaded.'
        Caption = 'Encoding'
        ItemIndex = 0
        Items.Strings = (
          'Sequential'
          'Progressive')
        TabOrder = 0
      end
      object GroupBox1: TTntGroupBox
        Left = 8
        Top = 8
        Width = 263
        Height = 57
        Hint = 'Select a compression quality.'
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Quality'
        TabOrder = 1
        DesignSize = (
          263
          57)
        object TrackBar1: TTrackBar
          Left = 43
          Top = 12
          Width = 212
          Height = 40
          Anchors = [akLeft, akTop, akRight]
          Max = 99
          Min = 1
          Orientation = trHorizontal
          PageSize = 10
          Frequency = 5
          Position = 1
          SelEnd = 0
          SelStart = 0
          TabOrder = 0
          TickMarks = tmTopLeft
          TickStyle = tsAuto
          OnChange = TrackBar1Change
        end
        object Edit3: TEdit
          Left = 8
          Top = 20
          Width = 25
          Height = 21
          MaxLength = 2
          TabOrder = 1
          Text = '0'
          OnChange = Edit3Change
          OnKeyPress = EditKeyPress
        end
      end
    end
    object TabSheet3: TTntTabSheet
      Caption = 'PDF'
      ImageIndex = 2
      object Label3: TTntLabel
        Left = 8
        Top = 8
        Width = 52
        Height = 13
        Caption = 'Paper size:'
      end
      object RadioGroup2: TTntRadioGroup
        Left = 8
        Top = 40
        Width = 105
        Height = 65
        Hint = 'Choose paper orientation.'
        Caption = 'Orientation'
        ItemIndex = 0
        Items.Strings = (
          'Portrait'
          'Landscape')
        TabOrder = 0
      end
      object ComboBox1: TTntComboBox
        Left = 64
        Top = 4
        Width = 89
        Height = 21
        Hint = 'Choose paper size to use.'
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 2
        TabOrder = 1
        Text = 'A4'
        Items.Strings = (
          'A2'
          'A3'
          'A4'
          'A5'
          'A6'
          'Letter'
          'Legal'
          'Ledger')
      end
    end
  end
  object Button2: TTntButton
    Left = 222
    Top = 216
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object CheckBox1: TTntCheckBox
    Left = 8
    Top = 220
    Width = 129
    Height = 17
    Hint = 'Select this to use these settings as defaults in the future.'
    Caption = 'Save as default'
    TabOrder = 3
  end
  object Button1: TTntButton
    Left = 134
    Top = 216
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
end
