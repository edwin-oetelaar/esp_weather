'*******************************************************************************
'Display Driver IC HX8357B or C 480x320 Pixel
'Modus 16Bit
'HVGA 480x320 3,6
'
'Bascom version 2.0.7.9
'Date 01.03.2014
'
'Autor: Hkipnik@aol.com
'
'Copyright by Hkipnik@aol.com
'
'
' der RD Pin vom Display an 3,3V legen
' Test whith Levelshifter
'*******************************************************************************

$nocompile


'*******************************************************************************
'Colors
'*******************************************************************************
'neue Farben
Const Transparent = &H1000
Const Aliceblue = &HF7DF
Const Antiquewhite = &HFF5A
Const Aqua = &H07FF
Const Aquamarine = &H7FFA
Const Azure = &HF7FF
Const Beige = &HF7BB
Const Bisque = &HFF38
Const Black = &H0000
Const Blanchedalmond = &HFF59
Const Blue = &H001F
Const Blueviolet = &H895C
Const Brown = &HA145
Const Burlywood = &HDDD0
Const Cadetblue = &H5CF4
Const Chartreuse = &H7FE0
Const Chocolate = &HD343
Const Coral = &HFBEA
Const Cornflowerblue = &H64BD
Const Cornsilk = &HFFDB
Const Crimson = &HD8A7
Const Cyan = &H07FF
Const Darkblue = &H0011
Const Darkcyan = &H0451
Const Darkgoldenrod = &HBC21
Const Darkgray = &HAD55
Const Darkgreen = &H0320
Const Darkkhaki = &HBDAD
Const Darkmagenta = &H8811
Const Darkolivegreen = &H5345
Const Darkorange = &HFC60
Const Darkorchid = &H9999
Const Darkred = &H8800
Const Darksalmon = &HECAF
Const Darkseagreen = &H8DF1
Const Darkslateblue = &H49F1
Const Darkslategray = &H2A69
Const Darkturquoise = &H067A
Const Darkviolet = &H901A
Const Deeppink = &HF8B2
Const Deepskyblue = &H05FF
Const Dimgray = &H6B4D
Const Dodgerblue = &H1C9F
Const Firebrick = &HB104
Const Floralwhite = &HFFDE
Const Forestgreen = &H2444
Const Fuchsia = &HF81F
Const Gainsboro = &HDEFB
Const Ghostwhite = &HFFDF
Const Gold = &HFEA0
Const Goldenrod = &HDD24
Const Gray = &H8410
Const Green = &H07E0
Const Green1 = &H0700
Const Greenyellow = &HAFE5
Const Honeydew = &HF7FE
Const Hotpink = &HFB56
Const Indianred = &HCAEB
Const Indigo = &H4810
Const Ivory = &HFFFE
Const Khaki = &HF731
Const Lavender = &HE73F
Const Lavenderblush = &HFF9E
Const Lawngreen = &H7FE0
Const Lemonchiffon = &HFFD9
Const Lightblue = &HAEDC
Const Lightcoral = &HF410
Const Lightcyan = &HE7FF
Const Lightgold = &HFFDA
Const Lightgreen = &H9772
Const Lightgrey = &HD69A
Const Lightpink = &HFDB8
Const Lightsalmon = &HFD0F
Const Lightseagreen = &H2595
Const Lightskyblue = &H867F
Const Lightslategray = &H7453
Const Lightsteelblue = &HB63B
Const Lightyellow = &HFFFC
Const Lime = &H07E0
Const Limegreen = &H3666
Const Linen = &HFF9C
Const Magenta = &HF81F
Const Maroon = &H8000
Const Mediumaquamarine = &H6675
Const Mediumblue = &H0019
Const Mediumorchid = &HBABA
Const Mediumpurple = &H939B
Const Mediumseagreen = &H3D8E
Const Mediumslateblue = &H7B5D
Const Mediumspringgreen = &H07D3
Const Mediumturquoise = &H4E99
Const Mediumvioletred = &HC0B0
Const Midnightblue = &H18CE
Const Mintcream = &HF7FF
Const Mistyrose = &HFF3C
Const Moccasin = &HFF36
Const Navajowhite = &HFEF5
Const Navy = &H0010
Const Oldlace = &HFFBC
Const Olive = &H8400
Const Olivedrab = &H6C64
Const Orange = &HFD20
Const Orangered = &HFA20
Const Orchid = &HDB9A
Const Palegoldenrod = &HEF55
Const Palegreen = &H9FD3
Const Paleturquoise = &HAF7D
Const Palevioletred = &HDB92
Const Papayawhip = &HFF7A
Const Peachpuff = &HFED7
Const Peru = &HCC27
Const Plum = &HDD1B
Const Powderblue = &HB71C
Const Purple = &H8010
Const Red = &HF800
Const Rosybrown = &HBC71
Const Royalblue = &H435C
Const Saddlebrown = &H8A22
Const Salmon = &HFC0E
Const Sandybrown = &HF52C
Const Seagreen = &H2C4A
Const Seashell = &HFFBD
Const Sienna = &HA285
Const Silver = &HC618
Const Skyblue = &H867D
Const Slateblue = &H6AD9
Const Slategray = &H7412
Const Snow = &HFFDF
Const Springgreen = &H07EF
Const Steelblue = &H4416
Const Teal = &H0410
Const Thistle = &HDDFB
Const Tomato = &HFB08
Const Turquoise = &H471A
Const Violet = &HEC1D
Const Wheat = &HF6F6
Const White = &HFFFF
Const Whitesmoke = &HF7BE
Const Yellow = &HFFE0
Const Yellowgreen = &H9E66




#if Lcd_mode = 1
  Const X_size = 319
  Const Y_size = 479
#elseif Lcd_mode = 2
  Const X_size = 319
  Const Y_size = 479
#elseif Lcd_mode = 3
  Const X_size = 479
  Const Y_size = 319
#elseif Lcd_mode = 4
  Const X_size = 479
  Const Y_size = 319
#endif


Config Portc = Output
Data_disp_low Alias Portc                                   'DB0 --> DB7

Config Porta = Output                                       'DB8 --> DB15
Data_disp Alias Porta

Rs_disp Alias Portd.7                                       'Display DC
Cs_disp Alias Portg.1                                       'Display CS
Res_disp Alias Portg.0                                      'Display Reset

Wr_disp Alias Portg.2                                       'Display WR
Port_wr Alias Portg                                         'WR Port
Const Wrpin = 2                                             'WR pin
'RD Pin pull it high                  RD Pin vom Display fest auf high legen

Config Rs_disp = Output
Config Cs_disp = Output
Config Wr_disp = Output
Config Res_disp = Output

Data_disp_low = 0
Data_disp = 0
Wr_disp = 1


Dim Rgb16 As Word
Dim Rgb16_low As Byte At Rgb16 Overlay
Dim Rgb16_high As Byte At Rgb16 + 1 Overlay
Dim Hoehe As Word
Dim Breit As Word
Dim Zeichenarray(100) As Byte

Declare Sub Lcd_init()
Declare Sub Lcd_write_reg(byval Index As Byte , Byval Daten As Word)
Declare Sub Lcd_write_data(byval Daten As Word)
Declare Sub Lcd_select_reg(byval Index As Byte )
Declare Sub Lcd_write_byte(byval Daten As Byte)

Declare Sub Lcd_set_window(byval Xpos As Word , Byval Ypos As Word , Byval Xsize As Word , Byval Ysize As Word)
Declare Sub Lcd_set_pixel(byval X1 As Word , Byval Y1 As Word , Byval Color As Word)
Declare Sub Lcd_line(byval X1 As Word , Byval Y1 As Word , Byval X2 As Word , Byval Y2 As Word , Byval Pen_width As Byte , Byval Color As Word)
'Declare Sub Lcd_box(byval Xstart As Word , Byval Ystart As Word , Byval X_length As Word , Byval Y_height As Word , Byval Fill As Byte , Byval Color As Word , Byval Bordercolor As Word)
Declare Sub Lcd_box(byval Xstart As Word , Byval Ystart As Word , Byval Xend As Word , Byval Yend As Word , Byval Fill As Byte , Byval Color As Word , Byval Bordercolor As Word)

Declare Sub Fill_circle(byval X As Word , Byval Y As Word , Byval Radius As Word , Byval Color1 As Word)
Declare Sub Lcd_circle(byval X As Word , Byval Y As Word , Byval Radius As Word , Byval Color As Word)

Declare Sub Lcd_show_bgc(byval Xoffset As Word , Byval Yoffset As Word)
Declare Sub Lcd_clear(byval Color As Word)
Declare Sub Lcd_pic_sram(byval Xs As Word , Byval Ys As Word , Byval Breite As Word , Byval Height As Word )
Declare Sub Lcd_text(byval Lcd_text As String , Byval Lcd_posx As Word , Byval Lcd_posy As Word , Byval Fontset As Byte , Byval Forecolor As Word , Byval Backcolor As Word , Byval Bold As Byte)

#if Sd_card = 1
Declare Sub Lcd_draw_bmp(byval Filename As String , Byval Xpos As Word , Byval Ypos As Word)
Declare Sub Lcd_pic_binsd(byval Filename As String , Byval Xpos As Word , Byval Ypos As Word , Byval Width As Word , Byval Height As Word)
 Dim Rgb_data(1440) As Byte
 Dim Line_byte As Word
#endif