object Form9: TForm9
  Left = 449
  Top = 279
  BiDiMode = bdLeftToRight
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Evaluate'
  ClientHeight = 198
  ClientWidth = 174
  Color = clBtnFace
  DragKind = dkDock
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  ParentBiDiMode = False
  PopupMode = pmExplicit
  PopupParent = Form1.Owner
  Position = poMainFormCenter
  ShowHint = True
  OnEndDock = FormEndDock
  OnHide = FormHide
  OnResize = FormResize
  OnShortCut = FormShortCut
  OnStartDock = FormStartDock
  DesignSize = (
    174
    198)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 0
    Width = 174
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
    ExplicitWidth = 183
  end
  inline ParFuncFrame1: TParFuncFrame
    Left = 0
    Top = 0
    Width = 175
    Height = 230
    HelpContext = 101
    Anchors = [akLeft, akTop, akRight]
    ParentBackground = False
    TabOrder = 2
    Visible = False
    ExplicitWidth = 175
    inherited Label7: TLabel
      Width = 40
      ExplicitWidth = 40
    end
    inherited Edit4: TEdit [7]
      Width = 118
      ExplicitWidth = 118
    end
    inherited Edit5: TEdit [8]
      Width = 118
      ExplicitWidth = 118
    end
    inherited Edit6: TEdit [9]
      Width = 118
      ExplicitWidth = 118
    end
    inherited Edit1: TMyEdit [10]
      Width = 118
      OnChange = Edit1Change
      ExplicitWidth = 118
    end
    inherited Edit2: TEdit [11]
      Width = 118
      ExplicitWidth = 118
    end
    inherited Edit3: TEdit [12]
      Width = 118
      ExplicitWidth = 118
    end
    inherited ComboBox1: TComboBox
      Width = 102
      ExplicitWidth = 102
    end
  end
  inline PolFuncFrame1: TPolFuncFrame
    Left = 0
    Top = 0
    Width = 175
    Height = 230
    Hint = 
      'Select where the cursor will be placed when you click on the ima' +
      'ge.'
    Anchors = [akLeft, akTop, akRight]
    ParentBackground = False
    TabOrder = 3
    Visible = False
    ExplicitWidth = 175
    inherited Label7: TLabel
      Width = 40
      ExplicitWidth = 40
    end
    inherited Edit4: TEdit [7]
      Width = 118
      ExplicitWidth = 118
    end
    inherited Edit5: TEdit [8]
      Width = 118
      ExplicitWidth = 118
    end
    inherited Edit6: TEdit [9]
      Width = 118
      ExplicitWidth = 118
    end
    inherited Edit1: TMyEdit [10]
      Width = 118
      OnChange = Edit1Change
      ExplicitWidth = 118
    end
    inherited Edit2: TEdit [11]
      Width = 118
      ExplicitWidth = 118
    end
    inherited Edit3: TEdit [12]
      Width = 118
      ExplicitWidth = 118
    end
    inherited ComboBox1: TComboBox
      Width = 102
      ExplicitWidth = 102
    end
  end
  inline TanFrame1: TTanFrame
    Left = 0
    Top = 0
    Width = 175
    Height = 69
    HelpContext = 100
    Anchors = [akLeft, akTop, akRight]
    ParentBackground = False
    TabOrder = 1
    Visible = False
    ExplicitWidth = 175
    inherited Edit2: TEdit
      Width = 126
      ExplicitWidth = 126
    end
    inherited Edit1: TMyEdit
      Width = 126
      OnChange = Edit1Change
      ExplicitWidth = 126
    end
  end
  inline StdFuncFrame1: TStdFuncFrame
    Left = 0
    Top = 0
    Width = 175
    Height = 167
    HelpContext = 100
    Anchors = [akLeft, akTop, akRight]
    ParentBackground = False
    TabOrder = 0
    Visible = False
    ExplicitWidth = 175
    inherited Label4: TLabel
      Left = 9
      Width = 24
      ExplicitLeft = 9
      ExplicitWidth = 24
    end
    inherited Label5: TLabel
      Width = 40
      ExplicitWidth = 40
    end
    inherited ComboBox1: TComboBox [5]
      Width = 102
      ExplicitWidth = 102
    end
    inherited Edit1: TMyEdit [6]
      Width = 113
      OnChange = Edit1Change
      ExplicitWidth = 113
    end
    inherited Edit2: TEdit
      Width = 128
      ExplicitWidth = 128
    end
    inherited Edit3: TEdit
      Width = 128
      ExplicitWidth = 128
    end
    inherited Edit4: TEdit
      Width = 128
      ExplicitWidth = 128
    end
    inherited UpDown1: TUpDown
      Left = 153
      OnChangingEx = UpDownChangingEx
      ExplicitLeft = 153
    end
  end
  inline AreaFrame1: TAreaFrame
    Left = 0
    Top = 0
    Width = 175
    Height = 102
    Anchors = [akLeft, akTop, akRight]
    ParentBackground = False
    TabOrder = 4
    Visible = False
    ExplicitWidth = 175
    ExplicitHeight = 102
    inherited GridPanel1: TGridPanel
      Width = 175
      Height = 102
      ControlCollection = <
        item
          Column = 0
          Control = AreaFrame1.Label1
          Row = 0
        end
        item
          Column = 0
          Control = AreaFrame1.Label2
          Row = 1
        end
        item
          Column = 0
          Control = AreaFrame1.Label3
          Row = 2
        end
        item
          Column = 1
          Control = AreaFrame1.Edit3
          Row = 2
        end
        item
          Column = 1
          Control = AreaFrame1.Panel1
          Row = 0
        end
        item
          Column = 1
          Control = AreaFrame1.Panel2
          Row = 1
        end>
      ExplicitTop = 0
      ExplicitWidth = 175
      ExplicitHeight = 102
      inherited Label1: TLabel
        Width = 26
        ExplicitWidth = 26
      end
      inherited Label3: TLabel
        Width = 22
        ExplicitWidth = 22
      end
      inherited Edit3: TEdit
        Left = 34
        ExplicitLeft = 34
        ExplicitWidth = 137
      end
      inherited Panel1: TPanel
        Left = 34
        ExplicitLeft = 34
        ExplicitWidth = 137
        inherited Edit1: TMyEdit
          OnChange = Edit1Change
          ExplicitWidth = 121
        end
        inherited UpDown1: TUpDown
          OnChangingEx = UpDownChangingEx
          ExplicitLeft = 121
        end
      end
      inherited Panel2: TPanel
        Left = 34
        ExplicitLeft = 34
        ExplicitWidth = 137
        inherited Edit2: TMyEdit
          OnChange = Edit1Change
          ExplicitWidth = 121
        end
        inherited UpDown2: TUpDown
          OnChangingEx = UpDownChangingEx
          ExplicitLeft = 121
        end
      end
    end
  end
end
