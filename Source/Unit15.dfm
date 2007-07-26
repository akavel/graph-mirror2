object Form15: TForm15
  Left = 412
  Top = 162
  HelpContext = 150
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Calculate table'
  ClientHeight = 469
  ClientWidth = 322
  Color = clBtnFace
  Constraints.MinHeight = 496
  Constraints.MinWidth = 330
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnResize = FormResize
  DesignSize = (
    322
    469)
  PixelsPerInch = 96
  TextHeight = 13
  object Label4: TTntLabel
    Left = 16
    Top = 50
    Width = 17
    Height = 13
    Caption = 'dx='
  end
  object Label1: TTntLabel
    Left = 16
    Top = 18
    Width = 26
    Height = 13
    Caption = 'From:'
  end
  object Label2: TTntLabel
    Left = 138
    Top = 18
    Width = 16
    Height = 13
    Caption = 'To:'
  end
  object Edit3: TTntEdit
    Left = 48
    Top = 48
    Width = 57
    Height = 21
    Hint = 
      'The step value. This is the distance between the x-values in the' +
      ' interval.'
    TabOrder = 2
    Text = '0.1'
  end
  object Button1: TTntButton
    Left = 56
    Top = 432
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Close'
    ModalResult = 2
    TabOrder = 5
  end
  object Button2: TTntButton
    Left = 232
    Top = 32
    Width = 75
    Height = 25
    Caption = 'Calc'
    Default = True
    TabOrder = 3
    OnClick = Button2Click
  end
  object Edit1: TTntEdit
    Left = 48
    Top = 16
    Width = 57
    Height = 21
    Hint = 'The first x-value in the interval.'
    TabOrder = 0
    Text = '-10'
  end
  object Edit2: TTntEdit
    Left = 160
    Top = 16
    Width = 57
    Height = 21
    Hint = 'The last x-value in the interval.'
    TabOrder = 1
    Text = '10'
  end
  object Button3: TTntButton
    Left = 152
    Top = 432
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Help'
    TabOrder = 6
    OnClick = Button3Click
  end
  object Grid1: TGrid
    Left = 16
    Top = 80
    Width = 289
    Height = 337
    ColCount = 4
    DefaultRowHeight = 16
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goThumbTracking]
    PopupMenu = PopupMenu1
    TabOrder = 4
    MinColWidth = 64
    ColWidths = (
      64
      64
      64
      64)
  end
  object PopupMenu1: TTntPopupMenu
    Left = 8
    Top = 432
    object Popup1_Show_df: TTntMenuItem
      AutoCheck = True
      Caption = 'Show %s'
      Checked = True
      OnClick = Popup1_Show
    end
    object Popup1_Show_ddf: TTntMenuItem
      AutoCheck = True
      Caption = 'Show %s'
      Checked = True
      OnClick = Popup1_Show
    end
    object N1: TTntMenuItem
      Caption = '-'
    end
    object Popup1_Copy: TTntMenuItem
      Caption = 'Copy'
      Hint = 'Copy selected cells to the clipboard.'
      ShortCut = 16451
      OnClick = Popup1_CopyClick
    end
    object Popup1_Export: TTntMenuItem
      Caption = 'Export to file...'
      Hint = 'Saves the selected data to a file.'
      OnClick = Popup1_ExportClick
    end
    object Popup1_Select: TTntMenuItem
      Caption = 'Select all'
      Hint = 'Select all cells.'
      ShortCut = 16449
      OnClick = Popup1_SelectClick
    end
  end
  object ProgressForm1: TProgressForm
    Caption = 'Calculating'
    Step = 1000
    ButtonCaption = 'Cancel'
    Left = 248
    Top = 432
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'csv'
    Filter = 'CSV (Comma delimited) [*.csv]|*.csv|Text [*.txt]|*.txt'
    Options = [ofOverwritePrompt, ofPathMustExist, ofNoReadOnlyReturn, ofEnableSizing]
    Left = 288
    Top = 432
  end
end
