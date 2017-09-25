object FDebug: TFDebug
  Left = 192
  Top = 107
  Width = 392
  Height = 289
  Caption = 'FDebug'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 16
  object Label1: TLabel
    Left = 32
    Top = 16
    Width = 105
    Height = 16
    Caption = 'Label1'
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 224
    Top = 16
  end
end
