object Form9: TForm9
  Left = 449
  Top = 279
  BiDiMode = bdLeftToRight
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Evaluate'
  ClientHeight = 198
  ClientWidth = 183
  Color = clBtnFace
  DragKind = dkDock
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  ParentBiDiMode = False
  Position = poMainFormCenter
  ShowHint = True
  OnEndDock = FormEndDock
  OnHide = FormHide
  OnResize = FormResize
  OnShortCut = FormShortCut
  OnStartDock = FormStartDock
  DesignSize = (
    183
    198)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TTntLabel
    Left = 0
    Top = 0
    Width = 183
    Height = 198
    Align = alClient
    AutoSize = False
    Caption = 'Select a function or tangent to evaluate.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  inline ParFuncFrame1: TParFuncFrame
    Left = 0
    Top = 0
    Width = 184
    Height = 230
    HelpContext = 101
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
    Visible = False
    inherited Edit4: TTntEdit [7]
    end
    inherited Edit5: TTntEdit [8]
    end
    inherited Edit6: TTntEdit [9]
    end
    inherited Edit1: TMyEdit [10]
      OnChange = Edit1Change
    end
    inherited Edit2: TTntEdit [11]
    end
    inherited Edit3: TTntEdit [12]
    end
  end
  inline AreaFrame1: TAreaFrame
    Left = 0
    Top = 0
    Width = 184
    Height = 102
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 4
    Visible = False
    inherited Edit1: TMyEdit
      OnChange = Edit1Change
    end
    inherited Edit2: TMyEdit
      OnChange = Edit1Change
    end
  end
  inline PolFuncFrame1: TPolFuncFrame
    Left = 0
    Top = 0
    Width = 184
    Height = 230
    Hint = 
      'Select where the cursor will be placed when you click on the ima' +
      'ge.'
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 3
    Visible = False
    inherited Edit4: TTntEdit [7]
    end
    inherited Edit5: TTntEdit [8]
    end
    inherited Edit6: TTntEdit [9]
    end
    inherited Edit1: TMyEdit [10]
      OnChange = Edit1Change
    end
    inherited Edit2: TTntEdit [11]
    end
    inherited Edit3: TTntEdit [12]
    end
  end
  inline TanFrame1: TTanFrame
    Left = 0
    Top = 0
    Width = 184
    Height = 69
    HelpContext = 100
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
    Visible = False
    inherited Edit1: TMyEdit
      OnChange = Edit1Change
    end
  end
  inline StdFuncFrame1: TStdFuncFrame
    Left = 0
    Top = 0
    Width = 184
    Height = 167
    HelpContext = 100
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
    Visible = False
    inherited ComboBox1: TTntComboBox [5]
    end
    inherited Edit1: TMyEdit [6]
      OnChange = Edit1Change
    end
  end
end
