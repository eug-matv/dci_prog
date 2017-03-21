object Form2: TForm2
  Left = 627
  Top = 84
  Width = 549
  Height = 435
  BorderIcons = [biSystemMenu]
  Caption = 'Options'
  Color = clBtnFace
  Constraints.MaxHeight = 435
  Constraints.MaxWidth = 549
  Constraints.MinHeight = 435
  Constraints.MinWidth = 549
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 533
    Height = 359
    ActivePage = TabSheet1
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    MultiLine = True
    ParentFont = False
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Azimuth-range scale'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      object PageControl2: TPageControl
        Left = 0
        Top = 0
        Width = 525
        Height = 209
        ActivePage = TabSheet7
        Align = alTop
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clBlack
        Font.Height = -15
        Font.Name = 'Times'
        Font.Style = []
        ParentFont = False
        TabIndex = 0
        TabOrder = 0
        object TabSheet7: TTabSheet
          Caption = 'Azimuth'
          ImageIndex = 1
          object GroupBox13: TGroupBox
            Left = 1
            Top = 69
            Width = 522
            Height = 56
            Caption = 'Additional azimuth lines'
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            object Label46: TLabel
              Left = 8
              Top = 26
              Width = 24
              Height = 13
              Caption = 'Color'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object SDLines: TShape
              Left = 74
              Top = 20
              Width = 25
              Height = 25
              OnMouseDown = SClrbackgroundMouseDown
            end
            object Label47: TLabel
              Left = 150
              Top = 26
              Width = 28
              Height = 13
              Caption = 'Width'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label48: TLabel
              Left = 357
              Top = 26
              Width = 23
              Height = 13
              Caption = 'Style'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object EDLines: TEdit
              Left = 76
              Top = 23
              Width = 53
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = 'EDLines'
              Visible = False
              OnChange = EClrbackgroundChange
              OnDblClick = EClrbackgroundDblClick
              OnKeyPress = EClrbackgroundKeyPress
            end
            object Edit15: TEdit
              Left = 240
              Top = 23
              Width = 44
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              MaxLength = 1
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnKeyPress = RadCir1KeyPress
            end
            object UpDown19: TUpDown
              Left = 284
              Top = 23
              Width = 16
              Height = 21
              Associate = Edit15
              Min = 0
              Max = 3
              Position = 0
              TabOrder = 2
              Wrap = False
            end
            object ComboBox10: TComboBox
              Left = 427
              Top = 23
              Width = 89
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemHeight = 13
              ParentFont = False
              TabOrder = 3
              Text = 'PS_SOLID'
              OnExit = ComboBox1Exit
              Items.Strings = (
                'PS_SOLID'
                'PS_DASH'
                'PS_DOT'
                'PS_DASHDOT'
                'PS_DASHDOTDOT'
                'PS_NULL'
                'PS_INSIDEFRAME')
            end
          end
          object GroupBox12: TGroupBox
            Left = 1
            Top = 8
            Width = 522
            Height = 56
            Caption = 'Reference azimuth lines'
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            object Label43: TLabel
              Left = 8
              Top = 26
              Width = 24
              Height = 13
              Caption = 'Color'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object SLines: TShape
              Left = 74
              Top = 20
              Width = 25
              Height = 25
              OnMouseDown = SClrbackgroundMouseDown
            end
            object Label44: TLabel
              Left = 150
              Top = 26
              Width = 28
              Height = 13
              Caption = 'Width'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label45: TLabel
              Left = 357
              Top = 26
              Width = 23
              Height = 13
              Caption = 'Style'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object ELines: TEdit
              Left = 76
              Top = 23
              Width = 53
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = 'ELines'
              Visible = False
              OnChange = EClrbackgroundChange
              OnDblClick = EClrbackgroundDblClick
              OnKeyPress = EClrbackgroundKeyPress
            end
            object Edit13: TEdit
              Left = 240
              Top = 23
              Width = 44
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              MaxLength = 1
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnKeyPress = RadCir1KeyPress
            end
            object UpDown18: TUpDown
              Left = 284
              Top = 23
              Width = 16
              Height = 21
              Associate = Edit13
              Min = 0
              Max = 3
              Position = 0
              TabOrder = 2
              Wrap = False
            end
            object ComboBox9: TComboBox
              Left = 427
              Top = 23
              Width = 89
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemHeight = 13
              ParentFont = False
              TabOrder = 3
              Text = 'PS_SOLID'
              OnExit = ComboBox1Exit
              Items.Strings = (
                'PS_SOLID'
                'PS_DASH'
                'PS_DOT'
                'PS_DASHDOT'
                'PS_DASHDOTDOT'
                'PS_NULL'
                'PS_INSIDEFRAME')
            end
          end
          object TGroupBox
            Left = 1
            Top = 126
            Width = 522
            Height = 47
            TabOrder = 2
            object Label1: TLabel
              Left = 8
              Top = 12
              Width = 37
              Height = 13
              Caption = 'Azimuth'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object SAzimuth: TShape
              Left = 74
              Top = 14
              Width = 26
              Height = 25
              OnMouseDown = SClrbackgroundMouseDown
            end
            object Label2: TLabel
              Left = 150
              Top = 20
              Width = 60
              Height = 13
              Caption = 'Azimuth step'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Transparent = True
            end
            object Label4: TLabel
              Left = 9
              Top = 25
              Width = 26
              Height = 13
              Caption = 'value'
              Font.Charset = RUSSIAN_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object EAzimuth: TEdit
              Left = 76
              Top = 17
              Width = 53
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = 'EAzimuth'
              Visible = False
              OnChange = EClrbackgroundChange
              OnDblClick = EClrbackgroundDblClick
              OnKeyPress = EClrbackgroundKeyPress
            end
            object ComboBox2: TComboBox
              Left = 240
              Top = 15
              Width = 60
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemHeight = 13
              ParentFont = False
              TabOrder = 1
              Text = '0'
              Items.Strings = (
                '10'
                '15'
                '30'
                '45')
            end
          end
        end
        object TabSheet6: TTabSheet
          Caption = 'Range'
          object GroupBox10: TGroupBox
            Left = 1
            Top = 8
            Width = 522
            Height = 56
            Caption = 'Reference range circles'
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            object Label27: TLabel
              Left = 8
              Top = 26
              Width = 24
              Height = 13
              Caption = 'Color'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object SClrcircle: TShape
              Left = 74
              Top = 20
              Width = 25
              Height = 25
              OnMouseDown = SClrbackgroundMouseDown
            end
            object Label28: TLabel
              Left = 148
              Top = 26
              Width = 28
              Height = 13
              Caption = 'Width'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label39: TLabel
              Left = 355
              Top = 26
              Width = 23
              Height = 13
              Caption = 'Style'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object EClrcircle: TEdit
              Left = 76
              Top = 23
              Width = 53
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = 'EClrcircle'
              Visible = False
              OnChange = EClrbackgroundChange
              OnDblClick = EClrbackgroundDblClick
              OnKeyPress = EClrbackgroundKeyPress
            end
            object Edit9: TEdit
              Left = 238
              Top = 23
              Width = 44
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              MaxLength = 1
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnKeyPress = RadCir1KeyPress
            end
            object UpDown10: TUpDown
              Left = 282
              Top = 23
              Width = 16
              Height = 21
              Associate = Edit9
              Min = 0
              Max = 3
              Position = 0
              TabOrder = 2
              Wrap = False
            end
            object ComboBox7: TComboBox
              Left = 425
              Top = 23
              Width = 89
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemHeight = 13
              ParentFont = False
              TabOrder = 3
              Text = 'PS_SOLID'
              OnExit = ComboBox1Exit
              Items.Strings = (
                'PS_SOLID'
                'PS_DASH'
                'PS_DOT'
                'PS_DASHDOT'
                'PS_DASHDOTDOT'
                'PS_NULL'
                'PS_INSIDEFRAME')
            end
          end
          object GroupBox11: TGroupBox
            Left = 1
            Top = 67
            Width = 522
            Height = 56
            Caption = 'Additional range circles'
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            object Label40: TLabel
              Left = 8
              Top = 26
              Width = 24
              Height = 13
              Caption = 'Color'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label41: TLabel
              Left = 355
              Top = 26
              Width = 23
              Height = 13
              Caption = 'Style'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label42: TLabel
              Left = 148
              Top = 26
              Width = 28
              Height = 13
              Caption = 'Width'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object SDClrcircle: TShape
              Left = 74
              Top = 20
              Width = 25
              Height = 25
              OnMouseDown = SClrbackgroundMouseDown
            end
            object ComboBox8: TComboBox
              Left = 425
              Top = 23
              Width = 89
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemHeight = 13
              ParentFont = False
              TabOrder = 0
              Text = 'PS_SOLID'
              OnExit = ComboBox1Exit
              Items.Strings = (
                'PS_SOLID'
                'PS_DASH'
                'PS_DOT'
                'PS_DASHDOT'
                'PS_DASHDOTDOT'
                'PS_NULL'
                'PS_INSIDEFRAME')
            end
            object UpDown11: TUpDown
              Left = 282
              Top = 23
              Width = 16
              Height = 21
              Associate = Edit10
              Min = 0
              Max = 3
              Position = 0
              TabOrder = 1
              Wrap = False
            end
            object Edit10: TEdit
              Left = 238
              Top = 23
              Width = 44
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              MaxLength = 1
              ParentFont = False
              TabOrder = 2
              Text = '0'
              OnKeyPress = RadCir1KeyPress
            end
            object EDClrcircle: TEdit
              Left = 76
              Top = 23
              Width = 53
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              Text = 'EDClrcircle'
              Visible = False
              OnChange = EClrbackgroundChange
              OnDblClick = EClrbackgroundDblClick
              OnKeyPress = EClrbackgroundKeyPress
            end
          end
          object TGroupBox
            Left = 1
            Top = 125
            Width = 522
            Height = 47
            TabOrder = 2
            object Label3: TLabel
              Left = 8
              Top = 12
              Width = 32
              Height = 13
              Caption = 'Range'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object SDalnost: TShape
              Left = 74
              Top = 14
              Width = 25
              Height = 25
              OnMouseDown = SClrbackgroundMouseDown
            end
            object Label5: TLabel
              Left = 7
              Top = 25
              Width = 26
              Height = 13
              Caption = 'value'
              Font.Charset = RUSSIAN_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object EDalnost: TEdit
              Left = 76
              Top = 18
              Width = 53
              Height = 21
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = 'EDalnost'
              Visible = False
              OnChange = EClrbackgroundChange
              OnDblClick = EClrbackgroundDblClick
              OnKeyPress = EClrbackgroundKeyPress
            end
          end
        end
      end
      object GroupBox16: TGroupBox
        Left = 4
        Top = 265
        Width = 525
        Height = 47
        TabOrder = 1
        object LClrbackground: TLabel
          Left = 8
          Top = 21
          Width = 84
          Height = 13
          Caption = 'Background color'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object SClrbackground: TShape
          Left = 100
          Top = 14
          Width = 26
          Height = 25
          OnMouseDown = SClrbackgroundMouseDown
        end
        object EClrbackground: TEdit
          Left = 112
          Top = 17
          Width = 53
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = 'EClrbackground'
          Visible = False
          OnChange = EClrbackgroundChange
          OnDblClick = EClrbackgroundDblClick
          OnKeyPress = EClrbackgroundKeyPress
        end
      end
      object GroupBox15: TGroupBox
        Left = 4
        Top = 209
        Width = 525
        Height = 56
        Caption = 'Scale area'
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        object Label52: TLabel
          Left = 8
          Top = 26
          Width = 24
          Height = 13
          Caption = 'Color'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object SScale: TShape
          Left = 76
          Top = 20
          Width = 25
          Height = 25
          OnMouseDown = SClrbackgroundMouseDown
        end
        object Label53: TLabel
          Left = 152
          Top = 26
          Width = 28
          Height = 13
          Caption = 'Width'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label54: TLabel
          Left = 359
          Top = 26
          Width = 23
          Height = 13
          Caption = 'Style'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object EScale: TEdit
          Left = 80
          Top = 23
          Width = 53
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = 'EScale'
          Visible = False
          OnChange = EClrbackgroundChange
          OnDblClick = EClrbackgroundDblClick
          OnKeyPress = EClrbackgroundKeyPress
        end
        object Edit19: TEdit
          Left = 243
          Top = 23
          Width = 44
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 1
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnKeyPress = RadCir1KeyPress
        end
        object UpDown21: TUpDown
          Left = 287
          Top = 23
          Width = 16
          Height = 21
          Associate = Edit19
          Min = 0
          Max = 3
          Position = 0
          TabOrder = 2
          Wrap = False
        end
        object ComboBox12: TComboBox
          Left = 429
          Top = 23
          Width = 89
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 13
          ParentFont = False
          TabOrder = 3
          Text = 'PS_SOLID'
          OnExit = ComboBox1Exit
          Items.Strings = (
            'PS_SOLID'
            'PS_DASH'
            'PS_DOT'
            'PS_DASHDOT'
            'PS_DASHDOTDOT'
            'PS_NULL'
            'PS_INSIDEFRAME')
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Plots'
      ImageIndex = 1
      object GroupBox3: TGroupBox
        Left = 4
        Top = 8
        Width = 525
        Height = 65
        Caption = '         Primary plot'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object Label7: TLabel
          Left = 35
          Top = 26
          Width = 20
          Height = 13
          Caption = 'Size'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label8: TLabel
          Left = 430
          Top = 26
          Width = 24
          Height = 13
          Caption = 'Color'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object SClrcircle1: TShape
          Left = 475
          Top = 20
          Width = 25
          Height = 25
          OnMouseDown = SClrbackgroundMouseDown
        end
        object Label11: TLabel
          Left = 205
          Top = 26
          Width = 51
          Height = 13
          Caption = 'Line width '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Image1: TImage
          Left = 8
          Top = -3
          Width = 25
          Height = 25
          Constraints.MaxHeight = 25
          Constraints.MaxWidth = 25
          Constraints.MinHeight = 25
          Constraints.MinWidth = 25
          Picture.Data = {
            07544269746D6170FA090000424DFA0900000000000036000000280000001900
            0000190000000100200000000000C4090000120B0000120B0000000000000000
            0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF0024344F0024344F0024344F0024344F00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F0024344F0024344F002434
            4F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF0024344F0024344F0024344F0024344F0024344F00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF0024344F0024344F0024344F0024344F0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F0024344F0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F0024344F0024344F0024344F0024344F00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF0024344F0024344F0024344F002434
            4F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF0024344F0024344F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF002434
            4F0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF0024344F0024344F0024344F0024344F00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF0024344F0024344F0024344F00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0024344F0024344F0024344F00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00}
          Transparent = True
        end
        object EClrcircle1: TEdit
          Left = 485
          Top = 23
          Width = 53
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = 'EClrcircle1'
          Visible = False
          OnChange = EClrbackgroundChange
          OnDblClick = EClrbackgroundDblClick
          OnKeyPress = EClrbackgroundKeyPress
        end
        object WidthCir1: TEdit
          Left = 305
          Top = 23
          Width = 44
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 1
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnKeyPress = RadCir1KeyPress
        end
        object UpDown3: TUpDown
          Left = 132
          Top = 23
          Width = 15
          Height = 21
          Associate = RadCir1
          Min = 1
          Max = 10
          Position = 1
          TabOrder = 2
          Wrap = False
        end
        object UpDown4: TUpDown
          Left = 349
          Top = 23
          Width = 15
          Height = 21
          Associate = WidthCir1
          Min = 0
          Max = 3
          Position = 0
          TabOrder = 3
          Wrap = False
        end
        object RadCir1: TEdit
          Left = 88
          Top = 23
          Width = 44
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          ParentShowHint = False
          ShowHint = False
          TabOrder = 4
          Text = '1'
          OnKeyPress = RadCir1KeyPress
        end
      end
      object GroupBox4: TGroupBox
        Left = 4
        Top = 84
        Width = 525
        Height = 65
        Caption = '       Secondary plot'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        object Label13: TLabel
          Left = 35
          Top = 26
          Width = 20
          Height = 13
          Caption = 'Size'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label14: TLabel
          Left = 430
          Top = 26
          Width = 24
          Height = 13
          Caption = 'Color'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object SClrcircle2: TShape
          Left = 475
          Top = 20
          Width = 25
          Height = 25
          OnMouseDown = SClrbackgroundMouseDown
        end
        object Label17: TLabel
          Left = 205
          Top = 26
          Width = 51
          Height = 13
          Caption = 'Line width '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Image2: TImage
          Left = 8
          Top = -3
          Width = 25
          Height = 25
          Constraints.MaxHeight = 25
          Constraints.MaxWidth = 25
          Constraints.MinHeight = 25
          Constraints.MinWidth = 25
          Picture.Data = {
            07544269746D6170FA090000424DFA0900000000000036000000280000001900
            0000190000000100200000000000C4090000120B0000120B0000000000000000
            0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00314059003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00314059003140590031405900FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00314059003140590031405900FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00314059003140590031405900FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0031405900314059003140
            5900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00314059003140
            590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF003140590031405900314059003140590031405900314059003140
            5900314059003140590031405900314059003140590031405900314059003140
            5900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF0031405900314059003140590031405900314059003140
            5900314059003140590031405900314059003140590031405900314059003140
            590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00314059003140590031405900314059003140
            5900314059003140590031405900314059003140590031405900314059003140
            59003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00314059003140590031405900FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00314059003140590031405900FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00314059003140590031405900FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0031405900314059003140
            5900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00314059003140
            590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF003140
            59003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00}
          Transparent = True
        end
        object EClrcircle2: TEdit
          Left = 490
          Top = 23
          Width = 53
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = 'EClrcircle2'
          Visible = False
          OnChange = EClrbackgroundChange
          OnDblClick = EClrbackgroundDblClick
          OnKeyPress = EClrbackgroundKeyPress
        end
        object WidthCir2: TEdit
          Left = 305
          Top = 23
          Width = 44
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 1
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnKeyPress = RadCir1KeyPress
        end
        object UpDown5: TUpDown
          Left = 132
          Top = 23
          Width = 15
          Height = 21
          Associate = RadCir2
          Min = 1
          Max = 10
          Position = 1
          TabOrder = 2
          Wrap = False
        end
        object UpDown6: TUpDown
          Left = 349
          Top = 23
          Width = 15
          Height = 21
          Associate = WidthCir2
          Min = 0
          Max = 3
          Position = 0
          TabOrder = 3
          Wrap = False
        end
        object RadCir2: TEdit
          Left = 88
          Top = 23
          Width = 44
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          ParentShowHint = False
          ShowHint = False
          TabOrder = 4
          Text = '1'
          OnKeyPress = RadCir1KeyPress
        end
      end
      object GroupBox5: TGroupBox
        Left = 4
        Top = 160
        Width = 525
        Height = 65
        Caption = '       Combined plots'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        object Label19: TLabel
          Left = 35
          Top = 26
          Width = 20
          Height = 13
          Caption = 'Size'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label20: TLabel
          Left = 430
          Top = 26
          Width = 24
          Height = 13
          Caption = 'Color'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object SClrcircle3: TShape
          Left = 475
          Top = 20
          Width = 25
          Height = 25
          OnMouseDown = SClrbackgroundMouseDown
        end
        object Label23: TLabel
          Left = 205
          Top = 26
          Width = 51
          Height = 13
          Caption = 'Line width '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Image3: TImage
          Left = 8
          Top = -2
          Width = 25
          Height = 25
          Constraints.MaxHeight = 25
          Constraints.MaxWidth = 25
          Constraints.MinHeight = 25
          Constraints.MinWidth = 25
          Picture.Data = {
            07544269746D6170FA090000424DFA0900000000000036000000280000001900
            0000190000000100200000000000C4090000120B0000120B0000000000000000
            0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF0031405900314059003140590031405900314059003140
            5900314059003140590031405900314059003140590031405900314059003140
            590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00314059003140590031405900314059003140
            5900314059003140590031405900314059003140590031405900314059003140
            59003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0031405900314059003A4B69003A4B
            69003A4B69003A4B69003A4B69003A4B69003A4B69003A4B69003A4B69003A4B
            69003A4B69003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0031405900314059003A4B
            69004D638C004D638C004D638C004D638C004D638C004D638C004D638C004D63
            8C004D638C003A4B69003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00314059003140
            59003A4B69004D638C004D638C004D638C004D638C004D638C004D638C004D63
            8C004D638C004D638C003A4B69003140590031405900FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF003140
            5900314059003A4B69004D638C004D638C004D638C004D638C004D638C004D63
            8C004D638C004D638C004D638C003A4B69003140590031405900FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF0031405900314059003A4B69004D638C004D638C004D638C004D638C004D63
            8C004D638C004D638C004D638C004D638C003A4B69003140590031405900FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF0031405900314059003A4B69004D638C004D638C004D638C004D63
            8C004D638C004D638C004D638C004D638C004D638C003A4B6900314059003140
            5900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF0031405900314059003A4B69004D638C004D638C004D63
            8C004D638C004D638C004D638C004D638C004D638C004D638C003A4B69003140
            590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF0031405900314059003A4B69004D638C004D63
            8C004D638C004D638C004D638C004D638C004D638C004D638C004D638C003A4B
            69003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0031405900314059003A4B69004D63
            8C004D638C004D638C004D638C004D638C004D638C004D638C004D638C004D63
            8C003A4B69003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0031405900314059003A4B
            69004D638C004D638C004D638C004D638C004D638C004D638C004D638C004D63
            8C004D638C003A4B69003140590031405900FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00314059003140
            59003A4B69003A4B69003A4B69003A4B69003A4B69003A4B69003A4B69003A4B
            69003A4B69003A4B69003A4B69003140590031405900FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF003140
            5900314059003140590031405900314059003140590031405900314059003140
            5900314059003140590031405900314059003140590031405900FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00314059003140590031405900314059003140590031405900314059003140
            590031405900314059003140590031405900314059003140590031405900FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
            FF00FFFFFF00}
          Transparent = True
        end
        object EClrcircle3: TEdit
          Left = 485
          Top = 23
          Width = 53
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = 'EClrcircle3'
          Visible = False
          OnChange = EClrbackgroundChange
          OnDblClick = EClrbackgroundDblClick
          OnKeyPress = EClrbackgroundKeyPress
        end
        object RadCir3: TEdit
          Left = 90
          Top = 23
          Width = 44
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 2
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          Text = '1'
          OnKeyPress = RadCir1KeyPress
        end
        object WidthCir3: TEdit
          Left = 305
          Top = 23
          Width = 44
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 1
          ParentFont = False
          TabOrder = 2
          Text = '0'
          OnKeyPress = RadCir1KeyPress
        end
        object UpDown7: TUpDown
          Left = 134
          Top = 23
          Width = 15
          Height = 21
          Associate = RadCir3
          Min = 1
          Max = 10
          Position = 1
          TabOrder = 3
          Wrap = False
        end
        object UpDown8: TUpDown
          Left = 349
          Top = 23
          Width = 15
          Height = 21
          Associate = WidthCir3
          Min = 0
          Max = 3
          Position = 0
          TabOrder = 4
          Wrap = False
        end
      end
      object GroupBox7: TGroupBox
        Left = 5
        Top = 239
        Width = 524
        Height = 65
        Caption = 'Track parameters'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        object Label29: TLabel
          Left = 205
          Top = 26
          Width = 60
          Height = 13
          Caption = 'Track length'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object CheckBox2: TCheckBox
          Left = 8
          Top = 26
          Width = 121
          Height = 17
          Caption = 'Display track'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object Edit7: TEdit
          Left = 305
          Top = 23
          Width = 44
          Height = 21
          Hint = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1072#1103' '#1076#1083#1080#1085#1072' '#1089#1083#1077#1076#1072': 200'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 3
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          Text = '0'
          OnKeyPress = RadCir1KeyPress
        end
        object UpDown22: TUpDown
          Left = 349
          Top = 23
          Width = 16
          Height = 21
          Associate = Edit7
          Min = 0
          Max = 200
          Position = 0
          TabOrder = 2
          Wrap = False
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Label'
      ImageIndex = 2
      object GroupBox6: TGroupBox
        Left = 4
        Top = 46
        Width = 525
        Height = 107
        Caption = 'Label parameters'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object Label25: TLabel
          Left = 8
          Top = 26
          Width = 42
          Height = 13
          Caption = 'Font size'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label26: TLabel
          Left = 8
          Top = 60
          Width = 24
          Height = 13
          Caption = 'Color'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object SClrShrift: TShape
          Left = 106
          Top = 54
          Width = 25
          Height = 25
          OnMouseDown = SClrbackgroundMouseDown
        end
        object Label38: TLabel
          Left = 238
          Top = 60
          Width = 31
          Height = 13
          Caption = 'Height'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object EClrShrift: TEdit
          Left = 104
          Top = 56
          Width = 49
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = 'EClrShrift'
          Visible = False
          OnChange = EClrbackgroundChange
          OnDblClick = EClrbackgroundDblClick
          OnKeyPress = EClrbackgroundKeyPress
        end
        object SizeShrift: TEdit
          Left = 104
          Top = 23
          Width = 44
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 2
          ParentFont = False
          TabOrder = 1
          Text = '4'
          OnKeyPress = RadCir1KeyPress
        end
        object UpDown9: TUpDown
          Left = 148
          Top = 23
          Width = 15
          Height = 21
          Associate = SizeShrift
          Min = 4
          Max = 16
          Position = 4
          TabOrder = 2
          Wrap = False
        end
        object fWidth: TEdit
          Left = 306
          Top = 23
          Width = 60
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Text = '0'
          Visible = False
          OnKeyPress = RadCir1KeyPress
        end
        object fHeight: TEdit
          Left = 306
          Top = 56
          Width = 60
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Text = '0'
          Visible = False
          OnKeyPress = RadCir1KeyPress
        end
        object UpDown16: TUpDown
          Left = 366
          Top = 23
          Width = 15
          Height = 21
          Associate = fWidth
          Min = 0
          Position = 0
          TabOrder = 5
          Visible = False
          Wrap = False
        end
        object UpDown17: TUpDown
          Left = 366
          Top = 56
          Width = 16
          Height = 21
          Associate = fHeight
          Min = 0
          Position = 0
          TabOrder = 6
          Visible = False
          Wrap = False
        end
        object CheckBox1: TCheckBox
          Left = 248
          Top = 21
          Width = 169
          Height = 25
          Caption = 'Display frame'
          TabOrder = 7
        end
      end
      object gbDataOutOfFormulyar: TGroupBox
        Left = 4
        Top = 166
        Width = 525
        Height = 141
        Caption = 'Display data in label'
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 1
        object cbNomer: TCheckBox
          Left = 8
          Top = 26
          Width = 177
          Height = 17
          Caption = 'Number'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object cbVysota: TCheckBox
          Left = 8
          Top = 50
          Width = 169
          Height = 17
          Caption = 'Altitude'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object cbOstatokTopliva: TCheckBox
          Left = 205
          Top = 98
          Width = 177
          Height = 17
          Caption = #1054#1089#1090#1072#1090#1086#1082' '#1090#1086#1087#1083#1080#1074#1072
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Visible = False
        end
        object cbTime: TCheckBox
          Left = 8
          Top = 74
          Width = 185
          Height = 17
          Caption = 'Time'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object cbVeroyatnostPervKanal: TCheckBox
          Left = 205
          Top = 28
          Width = 292
          Height = 17
          Caption = 'Probability of detection by the primary channel'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object cbSkorost: TCheckBox
          Left = 205
          Top = 75
          Width = 252
          Height = 17
          Caption = 'Speed'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object cbAzimutDalnost: TCheckBox
          Left = 8
          Top = 98
          Width = 161
          Height = 17
          Caption = 'Azimuth and range'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object cbVeroyatnostVtorKanal: TCheckBox
          Left = 205
          Top = 52
          Width = 268
          Height = 17
          Caption = 'Probability of detection by the secondary channel'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
      end
      object cbFormulyar: TCheckBox
        Left = 8
        Top = 12
        Width = 289
        Height = 17
        Caption = 'Display label'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        State = cbChecked
        TabOrder = 2
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Port parameters'
      ImageIndex = 3
      object RGBaudrate: TRadioGroup
        Left = 4
        Top = 156
        Width = 525
        Height = 158
        Caption = 'Read rate'
        Columns = 5
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemIndex = 6
        Items.Strings = (
          '110'
          '300'
          '600'
          '1200'
          '2400'
          '4800'
          '9600'
          '14400'
          '19200'
          '38400'
          '56000'
          '57600'
          '115200'
          '128000'
          '256000')
        ParentFont = False
        TabOrder = 0
      end
      object GBPort: TGroupBox
        Left = 5
        Top = 8
        Width = 523
        Height = 145
        Caption = 'Port'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        object Label6: TLabel
          Left = 8
          Top = 24
          Width = 90
          Height = 16
          Caption = 'Name of port'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object CBPortName: TComboBox
          Left = 104
          Top = 24
          Width = 185
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 13
          ParentFont = False
          TabOrder = 0
          Text = 'CBPortName'
        end
        object RGChisloStopBits: TRadioGroup
          Left = 8
          Top = 56
          Width = 193
          Height = 81
          Caption = 'Stop bits'
          ItemIndex = 1
          Items.Strings = (
            '1 '
            '1.5'
            '2 ')
          TabOrder = 1
        end
        object cbFParity: TCheckBox
          Left = 208
          Top = 64
          Width = 121
          Height = 17
          Caption = 'fParity'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
        end
        object RGParityMode: TRadioGroup
          Left = 328
          Top = 16
          Width = 177
          Height = 121
          Caption = 'Parity scheme'
          ItemIndex = 0
          Items.Strings = (
            'NOPARITY'
            'ODDPARITY'
            'EVENPARITY'
            'MARKPARTITY'
            'SPACEPARITY')
          TabOrder = 3
        end
      end
      object StaticText2: TStaticText
        Left = 6
        Top = 154
        Width = 73
        Height = 20
        Caption = 'Read rate'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'Parameters of saving to file'
      ImageIndex = 4
      DesignSize = (
        525
        331)
      object GroupBox8: TGroupBox
        Left = 4
        Top = 8
        Width = 525
        Height = 65
        Caption = 'Folder for saving data'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        DesignSize = (
          525
          65)
        object EFullPathToData: TEdit
          Left = 8
          Top = 26
          Width = 506
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = 'EFullPathToData'
        end
      end
      object CB_IsSaveDataFile: TCheckBox
        Left = 11
        Top = 131
        Width = 529
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Save data'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        Visible = False
      end
      object CB_IsFilterDataToFile: TCheckBox
        Left = 11
        Top = 151
        Width = 529
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = #1060#1080#1083#1100#1090#1088
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        Visible = False
      end
      object CB_IsSaveSectorsToFile: TCheckBox
        Left = 11
        Top = 83
        Width = 529
        Height = 17
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Save data on received sectors'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 359
    Width = 533
    Height = 38
    Align = alBottom
    TabOrder = 1
    DesignSize = (
      533
      38)
    object Button2: TButton
      Left = 343
      Top = 6
      Width = 78
      Height = 26
      Cursor = crHandPoint
      Anchors = [akTop]
      Caption = 'OK'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = Button2Click
    end
    object Button1: TButton
      Left = 437
      Top = 6
      Width = 78
      Height = 26
      Caption = 'Cancel'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = Button1Click
    end
    object Button3: TButton
      Left = 24
      Top = 6
      Width = 97
      Height = 26
      Cursor = crHandPoint
      Anchors = [akTop]
      Caption = 'Default... '
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = Button3Click
    end
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
  end
end
