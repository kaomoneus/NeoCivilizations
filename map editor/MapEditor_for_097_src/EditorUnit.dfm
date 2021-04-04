object Form1: TForm1
  Left = 64
  Top = 140
  Width = 1058
  Height = 612
  Caption = 'Map Editor'
  Color = clBtnFace
  DefaultMonitor = dmPrimary
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Visible = True
  WindowState = wsMaximized
  OnActivate = FormActivate
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 88
    Top = 16
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Label2: TLabel
    Left = 904
    Top = 496
    Width = 32
    Height = 13
    Caption = 'Label2'
  end
  object MEListBox: TListBox
    Left = 904
    Top = 384
    Width = 137
    Height = 97
    ItemHeight = 13
    TabOrder = 0
  end
  object ImagePanel: TPanel
    Left = 904
    Top = 264
    Width = 137
    Height = 113
    Caption = 'ImagePanel'
    TabOrder = 1
  end
  object BRefresh: TButton
    Left = 904
    Top = 520
    Width = 137
    Height = 17
    Caption = 'Обновить'
    TabOrder = 2
    OnClick = BRefreshClick
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 24
    object MenuFile: TMenuItem
      Caption = '&File'
      object Open1: TMenuItem
        Caption = 'Open'
        OnClick = Open1Click
      end
      object Save1: TMenuItem
        Caption = 'Save'
        OnClick = Save1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
      end
    end
    object MenuEditMap: TMenuItem
      Caption = 'EditMap'
      object MenuChTeam: TMenuItem
        Caption = 'EditConfiguration'
        OnClick = MenuChTeamClick
      end
    end
    object ViewMenu: TMenuItem
      Caption = 'View'
      OnClick = ViewMenuClick
      object ChangeMinMap: TMenuItem
        Caption = 'MiniMap ON'
        OnClick = ChangeMinMapClick
      end
    end
    object MenuHelp: TMenuItem
      Caption = '&Help'
    end
  end
  object Timer1: TTimer
    Interval = 75
    OnTimer = Timer1Timer
    Left = 56
    Top = 59
  end
  object MiniMapTimer: TTimer
    Interval = 50
    Left = 816
    Top = 16
  end
end
