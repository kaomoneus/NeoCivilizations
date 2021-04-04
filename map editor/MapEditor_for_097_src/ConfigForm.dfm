object CfgForm: TCfgForm
  Left = 452
  Top = 292
  Width = 700
  Height = 542
  Caption = 'Configuration'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDeactivate = FormDeactivate
  PixelsPerInch = 96
  TextHeight = 13
  object LTeamNum: TLabel
    Left = 24
    Top = 24
    Width = 90
    Height = 18
    AutoSize = False
    Caption = 'Номер команды'
  end
  object Label1: TLabel
    Left = 232
    Top = 80
    Width = 169
    Height = 17
    AutoSize = False
    Caption = 'Редактирование ландшафта'
  end
  object ETeam: TEdit
    Left = 184
    Top = 24
    Width = 42
    Height = 21
    TabOrder = 0
    Text = '0'
  end
  object BTeamNum: TButton
    Left = 304
    Top = 432
    Width = 50
    Height = 18
    Caption = 'Ввод'
    TabOrder = 1
    OnClick = BTeamNumClick
  end
  object BClose: TButton
    Left = 360
    Top = 432
    Width = 57
    Height = 17
    Caption = 'Закрыть'
    TabOrder = 2
    OnClick = BCloseClick
  end
  object CfgBrush: TGroupBox
    Left = 232
    Top = 96
    Width = 185
    Height = 217
    Caption = 'Кисть'
    TabOrder = 3
    object Label3: TLabel
      Left = 8
      Top = 104
      Width = 36
      Height = 13
      Caption = 'Радиус'
    end
    object Label4: TLabel
      Left = 8
      Top = 160
      Width = 78
      Height = 13
      Caption = 'Интенсивность'
    end
    object BPick: TRadioButton
      Left = 8
      Top = 16
      Width = 113
      Height = 17
      Caption = 'Рисование пик'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object BSoft: TRadioButton
      Left = 8
      Top = 48
      Width = 113
      Height = 17
      Caption = 'Рисование холмов'
      TabOrder = 1
    end
    object BHard: TRadioButton
      Left = 8
      Top = 80
      Width = 113
      Height = 17
      Caption = 'Резкий подьем'
      TabOrder = 2
    end
    object RadiusUpDown: TUpDown
      Left = 81
      Top = 120
      Width = 12
      Height = 21
      Associate = BRadius
      Min = 0
      Position = 0
      TabOrder = 3
      Wrap = False
    end
    object BRadius: TEdit
      Left = 8
      Top = 120
      Width = 73
      Height = 21
      TabOrder = 4
      Text = '0'
    end
    object BDelta: TEdit
      Left = 8
      Top = 176
      Width = 73
      Height = 21
      TabOrder = 5
      Text = '0'
    end
    object UpDown1: TUpDown
      Left = 81
      Top = 176
      Width = 12
      Height = 21
      Associate = BDelta
      Min = 0
      Position = 0
      TabOrder = 6
      Wrap = False
    end
  end
  object GroupMode: TGroupBox
    Left = 24
    Top = 80
    Width = 185
    Height = 105
    Caption = 'Режим редактора'
    TabOrder = 4
    object LSMode: TRadioButton
      Left = 8
      Top = 24
      Width = 161
      Height = 17
      Caption = 'Редактировать ландшафт'
      TabOrder = 0
    end
    object PlaceMode: TRadioButton
      Left = 8
      Top = 48
      Width = 161
      Height = 17
      Caption = 'Редактировать местность'
      TabOrder = 1
    end
    object ObjectsMode: TRadioButton
      Left = 8
      Top = 72
      Width = 153
      Height = 17
      Caption = 'Размещать объекты'
      TabOrder = 2
    end
  end
end
