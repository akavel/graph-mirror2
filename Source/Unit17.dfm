object Form17: TForm17
  Left = 343
  Top = 154
  HelpContext = 90
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Custom functions/constants'
  ClientHeight = 364
  ClientWidth = 442
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
    442
    364)
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TTntButton
    Left = 92
    Top = 331
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TTntButton
    Left = 180
    Top = 331
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object Button4: TTntButton
    Left = 356
    Top = 331
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Help'
    TabOrder = 4
    OnClick = Button4Click
  end
  object Button3: TTntButton
    Left = 268
    Top = 331
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Apply'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Grid1: TGrid
    Left = 16
    Top = 16
    Width = 413
    Height = 300
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 2
    DefaultRowHeight = 16
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goColSizing, goEditing, goTabs, goAlwaysShowEditor]
    PopupMenu = PopupMenu1
    TabOrder = 0
    AutoAddRows = True
    ExportFixedRows = False
    ReplaceDecimalSeparator = False
    EditorPopupMenu = PopupMenu1
    TitleCaptions.Strings = (
      'Names'
      'Definition')
    OnEditorKeyPress = Grid1EditorKeyPress
    ColWidths = (
      76
      328)
  end
  object PopupMenu1: TTntPopupMenu
    Top = 328
    object Popup_Cut: TTntMenuItem
      Caption = 'Cut'
      Hint = 'Cut the selected text to the clipboard'
      ShortCut = 16472
      OnClick = Popup_CutClick
    end
    object Popup_Copy: TTntMenuItem
      Caption = 'Copy'
      Hint = 'Copy the selected text to the clipboard.'
      ShortCut = 16451
      OnClick = Popup_CopyClick
    end
    object Popup_Paste: TTntMenuItem
      Caption = 'Paste'
      Hint = 
        'Paste text from the clipboard into the grid. Other cells may be ' +
        'overwritten by the text from the clipboard.'
      ShortCut = 16470
      OnClick = Popup_PasteClick
    end
    object Popup_Delete: TTntMenuItem
      Caption = 'Delete'
      Hint = 'Erase selected cells.'
      ShortCut = 16430
      OnClick = Popup_DeleteClick
    end
    object N1: TTntMenuItem
      Caption = '-'
    end
    object Popup_Insert: TTntMenuItem
      Caption = 'Insert row'
      OnClick = Popup_InsertClick
    end
    object Popup_Remove: TTntMenuItem
      Caption = 'Remove row'
      OnClick = Popup_RemoveClick
    end
    object N2: TTntMenuItem
      Caption = '-'
    end
    object Popup_Select: TTntMenuItem
      Caption = 'Select all'
      Hint = 'Select all cells.'
      ShortCut = 16449
      OnClick = Popup_SelectClick
    end
    object Popup_Import: TTntMenuItem
      Caption = 'Import from file...'
      Hint = 'Import data from a file and place it at the selected cell.'
      OnClick = Popup_ImportClick
    end
    object Popup_Export: TTntMenuItem
      Caption = 'Export to file...'
      Hint = 'Export selected data to a file.'
      OnClick = Popup_ExportClick
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 32
    Top = 328
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'csv'
    Options = [ofOverwritePrompt, ofPathMustExist, ofNoReadOnlyReturn, ofEnableSizing]
    Left = 64
    Top = 328
  end
end
