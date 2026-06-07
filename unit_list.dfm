object window_session_list: Twindow_session_list
  Left = 53
  Top = 119
  ActiveControl = checkbox_list_main
  BorderStyle = bsToolWindow
  Caption = #1057#1086#1093#1088#1072#1085#1103#1077#1084#1099#1077' '#1089#1090#1088#1072#1085#1080#1094#1099
  ClientHeight = 400
  ClientWidth = 521
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesktopCenter
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object txt_info: TLabel
    Left = 9
    Top = 9
    Width = 324
    Height = 13
    Caption = #1040#1076#1088#1077#1089#1072', '#1082#1086#1090#1086#1088#1099#1077' '#1073#1091#1076#1091#1090' '#1089#1086#1093#1088#1072#1085#1077#1085#1099' '#1074' '#1086#1090#1076#1077#1083#1100#1085#1099#1077' MHTML-'#1092#1072#1081#1083#1099
    WordWrap = True
  end
  object txt_count: TLabel
    Left = 9
    Top = 27
    Width = 47
    Height = 13
    Caption = 'txt_count'
  end
  object txt_result: TLabel
    Left = 9
    Top = 363
    Width = 55
    Height = 13
    Caption = #1057#1086#1093#1088#1072#1085#1077#1085#1086
  end
  object txt_size: TLabel
    Left = 9
    Top = 378
    Width = 69
    Height = 13
    Caption = #1054#1073#1097#1080#1081' '#1086#1073#1098#1077#1084
    Enabled = False
  end
  object btn_go: TButton
    Left = 438
    Top = 366
    Width = 75
    Height = 25
    Caption = #1057#1090#1072#1088#1090
    TabOrder = 0
    OnClick = btn_goClick
  end
  object checkbox_list_main: TCheckListBox
    Left = 9
    Top = 45
    Width = 502
    Height = 310
    ItemHeight = 13
    TabOrder = 1
  end
  object btn_sel_all: TButton
    Left = 357
    Top = 366
    Width = 75
    Height = 25
    Caption = #1054#1073#1088#1072#1090#1080#1090#1100
    Enabled = False
    TabOrder = 2
    OnClick = btn_sel_allClick
  end
end
