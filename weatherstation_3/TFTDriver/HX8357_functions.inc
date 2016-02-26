'*******************************************************************************
'Display Driver IC HX8357B or C 480x320 Pixel
'Modus 16Bit
'HVGA 480x320 3,6
'
'Bascom version 2.0.7.8
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
' Draw BMP Picture with 24Bit Color
'*******************************************************************************
#if Sd_card = 1
   Sub Lcd_draw_bmp(byval Filename As String , Byval Xpos As Word , Byval Ypos As Word)

      Local X As Word
      Local Line_len As Word
      Local Y As Single , Z As Word                         ', Line_count As Word
      Local Bmp_read_pos As Long
      Local Xad As Word

      Local Yad As Word

      Local Bfoffbits As Long

      Dim Biwidth As Word                                   'normaly long, but we can't handle bigger bmp's!
      Dim Biheight As Word                                  'normaly long, but we can't handle bigger bmp's!

      Local Bibitcount As Word


      Reset Mmc_cs
      Open Filename For Binary As #10



      Get #10 , Bfoffbits , &H0B                            '&H0A + 1
      Get #10 , Biwidth , &H013                             '&H12 + 1
      Get #10 , Biheight , &H017                            '&H16 + 1
      Get #10 , Bibitcount , &H1D                           '&H1C + 1


      Xad = Biwidth + Xpos
      Yad = Biheight + Ypos
      Decr Xad
      Decr Yad

      Call Lcd_set_window(xpos , Ypos , Xad , Yad)


      Config Single = Scientific , Digits = 1

     '  Incr Biheight
      Incr Biheight
      If Bibitcount = 24 Then
        ' position behind BMP Header Data
         Bmp_read_pos = Bfoffbits + 1
        ' we need line length divideable by 4!
         Line_byte = Biwidth * 3
         Line_len = Biwidth Mod 4
         Line_len = Line_byte + Line_len
      '  Print "MOD 4  Line-len : " ; Line_len
         Set Rs_disp
     '    Reset Cs_disp


         !LDS     R30,       {Biheight}                     'Anzahl lines in y-Register
         !LDS     R31,       {Biheight+1}
         !SBIW    R30, 1
         Loop_rows:
            !push    R30
            !push    R31
            Get #10 , Rgb_data(1) , Bmp_read_pos , Line_len

           'THX MWS!!!!
            !LDS     R30,       {Biwidth}                   'Anzahl Pixel/line in Z-Register
            !LDS     R31,       {Biwidth+1}

            Loadadr Rgb_data(1) , X

         Loop_line:
            !LD      R16,        X+                         ' blau in R16
            !LD      R17,        X+                         ' gruen in R17
            !LD      R18,        X+                         ' rot in R18
            !ANDI    R18,        &b11111000                 ' unnoetiges ausmaskieren

               'GRÜN
            !ROL     R17                                    ' 4 mal rotieren, da erstes Bit zuerst ins Carry geht
            !ROL     R17
            !ROL     R17
            !ROL     R17                                    ' Bits 7..5 von gruen sind nun an Bit 2..0 fuer Rgb_HB
            !MOV     R24,        R17                        ' nach R24 sichern
            !ROR     R17                                    ' 1 mal aus dem Carry zurück rotieren, um die Bits 4..2 auf 7..5 zu bekommen
            !ANDI    R17,        &b11100000                 ' Bit 7..5 von gruen maskieren, fuer Rgb_LB
            !ANDI    R24,        &b00000111                 ' Bit 2..0 maskieren fuer Rgb_HB

               'BLAU
            !LSR     R16                                    ' R16, blau 3mal rechts schieben
            !LSR     R16
            !LSR     R16
            !OR      R24,        R18
            !OUT    Data_disp, R24
            !OR     R17,        R16
            !OUT    Data_disp_low, R17
            !cBI Port_wr, Wrpin                             'WR Pin
            !SBI Port_wr, Wrpin
            !SBIW  R30, 1                                   'decr Z-Register
            !BRNE Loop_line

            Bmp_read_pos = Bmp_read_pos + Line_len

            !pop    R31
            !pop    R30
            !SBIW   R30, 1                                  'decr Z-Register
            !BREq   End_rows
            !Rjmp   Loop_rows
         End_rows:

     '   Set Cs_disp
      End If

      Close #10
      Set Mmc_cs
      Config Single = Scientific , Digits = 7

   End Sub

'*******************************************************************************
' Draw pic from SD Card with Bin File
' Bin File created with IMG2LCD Program
'*******************************************************************************
   Sub Lcd_pic_binsd(byval Filename As String , Byval Xpos As Word , Byval Ypos As Word , Byval Width As Word , Byval Height As Word)
      Local Xend As Word , Yend As Word , Linelen As Word
 '  Local Read_pos As Dword

      Linelen = 2 * Width
      Xend = Xpos + Width
      Yend = Ypos + Height
      Breit = Width
      Hoehe = Height
      Decr Xend
      Decr Yend
      Call Lcd_set_window(xpos , Ypos , Xend , Yend)        'Mode1

      Cs_disp = 0
      Rs_disp = 1
  ' Read_pos = 1
      Reset Mmc_cs
      Open Filename For Binary As #10

      !LDS     R30,       {Hoehe}                           'Anzahl lines
      !LDS     R31,       {Hoehe+1}
      Loop_2:
         !push    R30
         !push    R31

         Get #10 , Rgb_data(1) , , Linelen                  'Read_pos

         !LDS     R30,       {breit}                        'Anzahl breite
         !LDS     R31,       {breit+1}

         Loadadr Rgb_data(1) , X
      Loop_1:
         !LD R16, X+
         !LD R17, X+
         !OUT    Data_disp, R16
         !OUT    Data_disp_low, R17
         !cBI Port_wr, Wrpin                                'WR Pin
         !SBI Port_wr, Wrpin
         !SBIW  R30, 1                                      'decr breit   30
         !BRNE Loop_1
   '  Read_pos = Read_pos + Linelen
         !pop    R31
         !pop    R30
         !SBIW   R30, 1                                     'decr lines
         !BREq   End_1
         !Rjmp   Loop_2
      End_1:

         Close #10
         Set Mmc_cs
   End Sub

#endif
'*******************************************************************************
' Draw Pic x start -- y start -- Breite -- Höhe from SRAM  Bin file
' before Call Sub you must Restore the Label
'*******************************************************************************
Sub Lcd_pic_sram(byval Xs As Word , Byval Ys As Word , Byval Breite As Word , Byval Height As Word )
   Local Zal As Word , Xe As Word , Yss As Word
   Local R1 As Byte , R2 As Byte
   Local Lang As Word
   Lang = Breite * Height
   Xe = Xs + Breite
   Yss = Ys + Height
   Decr Xe
   Call Lcd_set_window(xs , Ys , Xe , Yss)
' Cs_disp = 0
   Rs_disp = 1
   For Zal = 1 To Lang
      Read R1
      Read R2
      Data_disp = R1
      Data_disp_low = R2
      Wr_disp = 0
      Wr_disp = 1
   Next
'   Cs_disp = 1
End Sub
'*******************************************************************************
' Set Window Area to write Data
'*******************************************************************************
Sub Lcd_set_window(byval Xpos As Word , Byval Ypos As Word , Byval Xsize As Word , Byval Ysize As Word)

   Rs_disp = 0                                              'command
   !ldi R16,$2a
   !Out Data_disp_low , r16
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin


   !ldd R26, Y+6                                            ' xpos
   !Ldd R27, Y+7
   !LD R18, X+
   !LD R17, X

   Rs_disp = 1
   !Out Data_disp_low , R17
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin
   !Out Data_disp_low , R18
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin


   !ldd R26, Y+2                                            ' xsize
   !Ldd R27, Y+3
   !LD R18, X+
   !LD R17, X
   !Out Data_disp_low , R17
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin
   !Out Data_disp_low , R18
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

   Rs_disp = 0                                              'command
   !ldi R16,$2b
   !Out Data_disp_low , r16
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

   !ldd R26, Y+4                                            ' ypos
   !Ldd R27, Y+5
   !LD R18, X+
   !LD R17, X
   Rs_disp = 1
   !Out Data_disp_low , R17
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin
   !Out Data_disp_low , R18
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin


   !ldd R26, Y+0                                            ' ysize
   !Ldd R27, Y+1
   !LD R18, X+
   !LD R17, X
   !Out Data_disp_low , R17
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin
   !Out Data_disp_low , R18
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

   Rs_disp = 0                                              'command
   !ldi R16,$2c
   !Out Data_disp_low , r16
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

End Sub
'*******************************************************************************
' Set_pixel  Set one Pixel in given color
'*******************************************************************************
Sub Lcd_set_pixel(byval X1 As Word , Y1 As Word , Color As Word)



   Rs_disp = 0                                              'command
   !ldi R16,$2a
   !Out Data_disp_low , r16
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

   !ldd R26, Y+4
   !Ldd R27, Y+5
   !LD R10, X+
   !LD R11, X
   Rs_disp = 1
   !Out Data_disp_low , R11
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin
   !Out Data_disp_low , R10
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

   !Out Data_disp_low , R11
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin
   !Out Data_disp_low , R10
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

   Rs_disp = 0
   !ldi R16,$2b
   !Out Data_disp_low , r16
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin


   !ldd R26, Y+2
   !Ldd R27, Y+3
   !LD R10, X+
   !LD R11, X
   Rs_disp = 1
   !Out Data_disp_low , R11
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin
   !Out Data_disp_low , R10
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

   !Out Data_disp_low , R11
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin
   !Out Data_disp_low , R10
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin


   Rs_disp = 0
   !ldi R16,$2c
   !Out Data_disp_low , r16
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin


   !ldd R26, Y+0
   !Ldd R27, Y+1
   !LD R10, X+
   !LD R11, X
   Rs_disp = 1
   !Out Data_disp , R11
   !Out Data_disp_low , R10
   !cBI Port_wr, Wrpin
   !SBI Port_wr, Wrpin

End Sub
'*******************************************************************************
' Draw Text String  x y Start - Font Nr - Forecolor - Backcolor - Bold Nr 1 - 2 - 3 -4  1 is normal
'*******************************************************************************
Sub Lcd_text(byval Lcd_text As String , Byval Lcd_posx As Word , Byval Lcd_posy As Word , Byval Fontset As Byte , Byval Forecolor As Word , Byval Backcolor As Word , Byval Bold As Byte)

   Local Pzeichen As String * 1
   Local Zeichen_row As Word
   Local Ptemp As Word
   Local Ptemp2 As Word
   Local Px As Word
   Local Py As Word
   Local Char_rows As Byte
   Local Char_row_len As Byte
   Local Char_bytes As Byte
   Local Dummy As Byte
   Local String_pos As Byte
   Local Zeichen_code As Word
   Local Fcolh As Byte , Fcoll As Byte , Bacolh As Byte , Bacoll As Byte
   Local Doph As Word , Zbv As Byte
   Fcolh = High(forecolor)
   Fcoll = Low(forecolor)
   Bacolh = High(backcolor)
   Bacoll = Low(backcolor)
   Cs_disp = 0
   Select Case Fontset
      Case 2:
         Char_rows = Lookup(0 , Font12x16)
         Char_row_len = Lookup(1 , Font12x16)
         Char_bytes = Lookup(2 , Font12x16)
         Dummy = Lookup(3 , Font12x16)
      Case 1:
         Char_rows = Lookup(0 , Font25x32)
         Char_row_len = Lookup(1 , Font25x32)
         Char_bytes = Lookup(2 , Font25x32)
         Dummy = Lookup(3 , Font25x32)
      Case 3:
         Char_rows = Lookup(0 , Arial_11x14)
         Char_row_len = Lookup(1 , Arial_11x14)
         Char_bytes = Lookup(2 , Arial_11x14)
         Dummy = Lookup(3 , Arial_11x14)
      Case 4:
         Char_rows = Lookup(0 , Digital20x32)
         Char_row_len = Lookup(1 , Digital20x32)
         Char_bytes = Lookup(2 , Digital20x32)
         Dummy = Lookup(3 , Digital20x32)
   End Select

   For String_pos = 1 To Len(lcd_text)                      'Für jedes Zeichen im String
      Pzeichen = Mid(lcd_text , String_pos , 1)             'Ein Zeichen aus String holen
      Zeichen_code = Asc(pzeichen) - 32                     'Font files start with caracter 32
      Zeichen_code = Zeichen_code * Char_bytes

      Doph = Zeichen_code
      Doph = Doph + 4
      For Px = 1 To Char_bytes

         Select Case Fontset
            Case 1 : Zeichenarray(px) = Lookup(doph , Font25x32)
            Case 2 : Zeichenarray(px) = Lookup(doph , Font12x16)
            Case 3 : Zeichenarray(px) = Lookup(doph , Arial_11x14)       
            Case 4 : Zeichenarray(px) = Lookup(doph , Digital20x32)
         End Select
         Incr Doph

      Next Px

      Py = Char_rows * 8
      Py = Py * Bold
      Py = Py + Lcd_posy

      Px = String_pos - 1
      Px = Px * Char_row_len
      Px = Px + Lcd_posx

      Select Case Bold
         Case 1:
            Ptemp = Px + Char_row_len
         Case 2:
            Px = Px * Bold                                  '* 2
            Px = Px - Lcd_posx
            Ptemp = Px + Char_row_len
            Ptemp = Ptemp + Char_row_len
         Case 3:
            Px = Px * Bold                                  '* 3
            Px = Px - Lcd_posx
            Px = Px - Lcd_posx
            Ptemp = Px + Char_row_len
            Ptemp = Ptemp + Char_row_len
            Ptemp = Ptemp + Char_row_len
         Case 4:
            Px = Px * Bold                                  '* 4
            Px = Px - Lcd_posx
            Px = Px - Lcd_posx
            Px = Px - Lcd_posx
            Ptemp = Px + Char_row_len
            Ptemp = Ptemp + Char_row_len
            Ptemp = Ptemp + Char_row_len
            Ptemp = Ptemp + Char_row_len
      End Select

      Decr Ptemp

      Call Lcd_set_window(px , Lcd_posy , Ptemp , Py)
  '    Cs_disp = 0
      Set Rs_disp

      For Py = 1 To Char_rows
         Ptemp = Py - 1
         Ptemp = Ptemp * Char_row_len

         For Zeichen_row = 0 To 7
            For Doph = 1 To Bold
               For Px = 1 To Char_row_len
                  Ptemp2 = Ptemp + Px
                  If Zeichenarray(ptemp2).zeichen_row = 1 Then

                     For Zbv = 1 To Bold
                        Data_disp = Fcolh                   'Rgb16_high
                        Data_disp_low = Fcoll
                        !cBI Port_wr, Wrpin                 'WR Pin
                        !SBI Port_wr, Wrpin
                     Next Zbv

                  Else

                     For Zbv = 1 To Bold
                        Data_disp = Bacolh                  'Rgb16_high
                        Data_disp_low = Bacoll
                        !cBI Port_wr, Wrpin                 'WR Pin
                        !SBI Port_wr, Wrpin
                     Next Zbv

                  End If
               Next Px
            Next Doph
         Next Zeichen_row
      Next Py


   Next String_pos

End Sub
'*******************************************************************************
' Draw line X - Y Start to X - Y End - Pen Width - Color
'*******************************************************************************
Sub Lcd_line(byval X1 As Word , Byval Y1 As Word , Byval X2 As Word , Byval Y2 As Word , Byval Pen_width As Byte , Byval Color As Word)
   Local Y As Word , X As Word , X_diff As Single , Y_diff As Single , Pos As Word
   Local X_factor As Single , X_pos As Word , Y_pos As Word , Base As Word , Pen_count As Byte


   Y_diff = Y2 - Y1
   X_diff = X2 - X1
   Pos = 0

   X_factor = Abs(y_diff)
   Y = X_factor
   X_factor = Abs(x_diff)
   X = X_factor

   If Y > X Then
      X_factor = X_diff / Y_diff
      If Y1 > Y2 Then
         Swap Y1 , Y2
         Base = X2
      Else
         Base = X1
      End If
      For Y = Y1 To Y2
         X_diff = Pos * X_factor
         X_pos = X_diff
         X_pos = X_pos + Base
         For Pen_count = 1 To Pen_width
            Call Lcd_set_pixel(x_pos , Y , Color)
            Incr X_pos
         Next Pen_count
         Incr Pos
      Next Y
   Else
      X_factor = Y_diff / X_diff
      If X1 > X2 Then
         Swap X1 , X2
         Base = Y2
      Else
         Base = Y1
      End If
      For X = X1 To X2
         Y_diff = Pos * X_factor
         Y_pos = Y_diff
         Y_pos = Y_pos + Base
         For Pen_count = 1 To Pen_width
            Call Lcd_set_pixel(x , Y_pos , Color)
            Incr Y_pos
         Next Pen_count
         Incr Pos
      Next X
   End If
End Sub
'*******************************************************************************
' Draw Box x start -- y start -- xend -- yend -- 1=fill 2=fill with Border 3=no fill -- color -- bordercolor
'*******************************************************************************
Sub Lcd_box(byval Xstart As Word , Byval Ystart As Word , Byval Xend As Word , Byval Yend As Word , Byval Fill As Byte , Byval Color As Word , Byval Bordercolor As Word)

   Hoehe = Yend - Ystart
   Breit = Xend - Xstart
   Decr Xend
   If Fill = 1 Then
      Lcd_set_window Xstart , Ystart , Xend , Yend


      Rs_disp = 1

      ldd R26, Y+2
      Ldd R27, Y+3
      LD R10, X+
      LD R11, X

      !LDS     R24,       {Hoehe}                           'Anzahl lines
      !LDS     R25,       {Hoehe+1}
      Lo_rows:

         !LDS     R30,       {Breit}                        'Anzahl Pixel pro line
         !LDS     R31,       {Breit+1}

      Lo_line:
         !Out Data_disp , R11

         !Out Data_disp_low , R10
         !cBI Port_wr, Wrpin
         !SBI Port_wr, Wrpin

         !SBIW  R30, 1                                      'decr Pixel
         !BRNE Lo_line

         !SBIW   R24, 1                                     'decr höhe
         !brne lo_rows


   Elseif Fill = 2 Then
      Lcd_set_window Xstart , Ystart , Xend , Yend

      Rs_disp = 1

      ldd R26, Y+2
      Ldd R27, Y+3
      LD R10, X+
      LD R11, X

      !LDS     R24,       {Hoehe}                           'Anzahl lines
      !LDS     R25,       {Hoehe+1}
      Lo_rows1:

         !LDS     R30,       {Breit}                        'Anzahl Pixel pro line
         !LDS     R31,       {Breit+1}

      Lo_line1:
         !Out Data_disp , R11

         !Out Data_disp_low , R10
         !cBI Port_wr, Wrpin
         !SBI Port_wr, Wrpin

         !SBIW  R30, 1                                      'decr Pixel
         !BRNE Lo_line1

         !SBIW   R24, 1                                     'decr höhe
         !brne lo_rows1

         Decr Yend
         Lcd_line Xstart , Ystart , Xend , Ystart , 2 , Bordercolor
         Lcd_line Xstart , Yend , Xend , Yend , 2 , Bordercolor
         Lcd_line Xstart , Ystart , Xstart , Yend , 2 , Bordercolor
         Decr Xend
         Lcd_line Xend , Ystart , Xend , Yend , 2 , Bordercolor
   Elseif Fill = 3 Then
      Lcd_line Xstart , Ystart , Xend , Ystart , 2 , Bordercolor
      Lcd_line Xstart , Yend , Xend , Yend , 2 , Bordercolor
      Lcd_line Xstart , Ystart , Xstart , Yend , 2 , Bordercolor
      Decr Xend
      Lcd_line Xend , Ystart , Xend , Yend , 2 , Bordercolor

   End If

End Sub
'*******************************************************************************
' Lcd show BGC File  X Start  Y Start
' Important that you first RESTORE the image before calling this sub
'*******************************************************************************
Sub Lcd_show_bgc(byval Xoffset As Word , Byval Yoffset As Word)

   Local Ysize As Byte , Xsize As Byte
   Local Imagesize As Word
   Local L As Word , Pixel As Byte ,
   Local Pixelword As Word , Prevpixel As Word , Repeat As Byte
   Local X1size As Word , Y1size As Word , Yss As Word


   Read Ysize                                               'Read the first 4 bytes from the BGC, this contains info about the image
   Read Xsize
   Read Imagesize                                           'Byte 3&4 contains the image size

   X1size = Xsize + Xoffset
   Y1size = Ysize + Yoffset
   Decr X1size


   Call Lcd_set_window(xoffset , Yoffset , X1size , Y1size)

   Rs_disp = 1
   Do
      Read Pixel                                            'Read the value for the pixel
      If Pixel = &HAA Then                                  'BGC file is rle coded &HAA represents this, the byte after &HAA is the number of times that the byte before &HAA wil be repeated
         Read Repeat                                        'Byte after &HAA contains the number of times to repeat
         Repeat = Repeat - 1
         For L = 1 To Repeat                                'Loop the repeat
           ' Data_disp = Rgb16_high
           ' Data_disp_low = Rgb16_low
            !cBI Port_wr, Wrpin                             'Port_wr, Wrpin                                'WR Pin
            !SBI Port_wr, Wrpin                             'Port_wr, Wrpin
            Decr Imagesize                                  'Decrease the bytes still to be processed
         Next L                                             'Next repeat loop
      Else
         Rgb16 = 0
         If Pixel.0 = 1 Then Rgb16_low = Rgb16_low Or 10    'Blue
         If Pixel.1 = 1 Then Rgb16_low = Rgb16_low Or 21
         If Pixel.2 = 1 Then Rgb16 = Rgb16 Or 288           'Green
         If Pixel.3 = 1 Then Rgb16 = Rgb16 Or 576
         If Pixel.4 = 1 Then Rgb16 = Rgb16 Or 1152
         If Pixel.5 = 1 Then Rgb16_high = Rgb16_high Or 32  'Red
         If Pixel.6 = 1 Then Rgb16_high = Rgb16_high Or 72
         If Pixel.7 = 1 Then Rgb16_high = Rgb16_high Or 144
         Data_disp = Rgb16_high
         Data_disp_low = Rgb16_low
         !cBI Port_wr, Wrpin                                'Port_wr, Wrpin                                'WR Pin
         !SBI Port_wr, Wrpin                                'Port_wr, Wrpin
         Decr Imagesize
      End If
   Loop Until Imagesize = 0                                 'Do until all bytes are processed

End Sub

'*******************************************************************************
' Clear LCD with color
'*******************************************************************************
Sub Lcd_clear(byval Color As Word)
   Local Ili9325_count_color As Long

   Call Lcd_set_window(0 , 0 , X_size , Y_size)

   Rgb16 = Color
 '  Cs_disp = 0
   Rs_disp = 1
   Data_disp = Rgb16_high
   Data_disp_low = Rgb16_low
   !ldi  R16, $06                                           '153600
   Wgloop0:
      !ldi  R17, $A0
   Wgloop1:
      !ldi  R18, $A0
   Wgloop2:
         '  Data_disp = Rgb16_high
         '  Data_disp_low = Rgb16_low
      !cBI Port_wr, Wrpin
      !SBI Port_wr, Wrpin
      !dec  R18
      !brne WGLOOP2

      !dec  R17
      !brne WGLOOP1

      !dec  R16
      !brne WGLOOP0
  ' Cs_disp = 1
End Sub
'*******************************************************************************
' draw Circle  X - Y Center - Radius - Color
'*******************************************************************************
Sub Lcd_circle(byval X As Word , Byval Y As Word , Byval Radius As Word , Byval Color As Word)
   Local X0 As Word , Y0 As Word , Error As Integer
   Local Xp As Word , Yp As Word , Xe As Word , Ye As Word
   Error = -radius
   Xp = Radius
   Yp = 0

   While Xp >= Yp
      X0 = X + Xp : Y0 = Y + Yp
      Call Lcd_set_pixel(x0 , Y0 , Color)
      X0 = X - Xp : Y0 = Y + Yp
      Call Lcd_set_pixel(x0 , Y0 , Color)
      X0 = X + Xp : Y0 = Y - Yp
      Call Lcd_set_pixel(x0 , Y0 , Color)
      X0 = X - Xp : Y0 = Y - Yp
      Call Lcd_set_pixel(x0 , Y0 , Color)

      X0 = X + Yp : Y0 = Y + Xp
      Call Lcd_set_pixel(x0 , Y0 , Color)
      X0 = X - Yp : Y0 = Y + Xp
      Call Lcd_set_pixel(x0 , Y0 , Color)
      X0 = X + Yp : Y0 = Y - Xp
      Call Lcd_set_pixel(x0 , Y0 , Color)
      X0 = X - Yp : Y0 = Y - Xp
      Call Lcd_set_pixel(x0 , Y0 , Color)
      Error = Error + Yp
      Incr Yp
      Error = Error + Yp

      If Error >= 0 Then
         Decr Xp
         Error = Error - Xp
         Error = Error - Xp
      End If
   Wend
End Sub
'*******************************************************************************
'LCD Fill Circle X Y center -- Radius -- Color
'*******************************************************************************
Sub Fill_circle(byval X As Word , Byval Y As Word , Byval Radius As Word , Byval Color1 As Word)
   Local Count_y As Integer , Count_x As Integer
   Local Y1 As Integer , X1 As Integer , Xy_radius As Integer
   Local Y11 As Integer , X11 As Integer , Xy As Integer , Tempxy As Word , Tempx As Word

   Rgb16 = Color1
   Xy_radius = Radius * Radius
   Y1 = -radius
   X1 = -radius
   For Count_y = Y1 To Radius
      Y11 = Count_y * Count_y
      Tempxy = Y + Count_y

      For Count_x = X1 To Radius
         X11 = Count_x * Count_x
         Xy = X11 + Y11
         If Xy <= Xy_radius Then
            Tempx = X + Count_x
       '  Call Lcd_set_pixel(tempx , Tempxy , Color1)
            Rgb16 = Tempx

            Rs_disp = 0                                     'command
            Data_disp = 0
            Data_disp_low = &H2A
            Wr_disp = 0
            Wr_disp = 1

            Rs_disp = 1                                     'data
            Data_disp_low = Rgb16_high
            Wr_disp = 0
            Wr_disp = 1
            Data_disp_low = Rgb16_low
            Wr_disp = 0
            Wr_disp = 1
            Data_disp_low = Rgb16_high
            Wr_disp = 0
            Wr_disp = 1
            Data_disp_low = Rgb16_low
            Wr_disp = 0
            Wr_disp = 1

            Rgb16 = Tempxy
            Rs_disp = 0
            Data_disp_low = &H2B
            Wr_disp = 0
            Wr_disp = 1

            Rs_disp = 1                                     'command
            Data_disp_low = Rgb16_high
            Wr_disp = 0
            Wr_disp = 1
            Data_disp_low = Rgb16_low
            Wr_disp = 0
            Wr_disp = 1
            Data_disp_low = Rgb16_high
            Wr_disp = 0
            Wr_disp = 1
            Data_disp_low = Rgb16_low
            Wr_disp = 0
            Wr_disp = 1

            Rs_disp = 0
            Data_disp_low = &H2C
            Wr_disp = 0
            Wr_disp = 1

            Rgb16 = Color1
   '
            Rs_disp = 1
            Data_disp = Rgb16_high                          'High(daten)
            Data_disp_low = Rgb16_low                       'Low(daten
            Wr_disp = 0
            Wr_disp = 1
         End If
      Next Count_x
   Next Count_y
End Sub
'*******************************************************************************
' INIT
'*******************************************************************************
Sub Lcd_init()
'reset
   Res_disp = 1
   Waitms 5
   Res_disp = 0
   Waitms 100
   Res_disp = 1
   Waitms 50
   Cs_disp = 0                                              'reset CS Pin
'HX 8357B
   #if Lcd_driver = 1
      Call Lcd_select_reg(&H11)                             'Exit_sleep_mode
      Waitms 100

      Call Lcd_select_reg(&Hee)                             'Set EQ function
      Call Lcd_write_byte(&H02)
      Call Lcd_write_byte(&H01)
      Call Lcd_write_byte(&H02)
      Call Lcd_write_byte(&H01)

      Call Lcd_select_reg(&Hb4)                             'Set Display mode
      Call Lcd_write_byte(&H00)


      Call Lcd_select_reg(&Hc0)                             'Set Panel Driving
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H3b)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H02)
      Call Lcd_write_byte(&H11)

      Call Lcd_select_reg(&Hc1)                             'Set display timing for Normal mode
      Call Lcd_write_byte(&H10)

      Call Lcd_select_reg(&Hc8)                             'Set Gamma
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H46)
      Call Lcd_write_byte(&H12)
      Call Lcd_write_byte(&H20)
      Call Lcd_write_byte(&H0c)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H56)
      Call Lcd_write_byte(&H12)
      Call Lcd_write_byte(&H67)
      Call Lcd_write_byte(&H02)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H0c)



      Call Lcd_select_reg(&Hd0)                             'Set Power
      Call Lcd_write_byte(&H44)
      Call Lcd_write_byte(&H42)
      Call Lcd_write_byte(&H06)

      Call Lcd_select_reg(&Hd1)                             'Set VCOM
      Call Lcd_write_byte(&H43)
      Call Lcd_write_byte(&H16)

      Call Lcd_select_reg(&Hd2)                             'Set Power for Normal mode
      Call Lcd_write_byte(&H04)
      Call Lcd_write_byte(&H22)

      Call Lcd_select_reg(&Hd3)                             'Set Power for Partial mode
      Call Lcd_write_byte(&H04)
      Call Lcd_write_byte(&H12)

      Call Lcd_select_reg(&Hd4)                             'Set Power for Idle mode
      Call Lcd_write_byte(&H07)
      Call Lcd_write_byte(&H12)

      Call Lcd_select_reg(&He9)                             'Set Panel related
      Call Lcd_write_byte(&H00)

      Call Lcd_select_reg(&Hc5)                             'Set display frame
      Call Lcd_write_byte(&H08)

      Call Lcd_select_reg(&H36)                             'Set_address_mode

      #if Lcd_mode = 1
         Lcd_write_byte(&H0a)
      #elseif Lcd_mode = 2
         Lcd_write_byte(&Hca)
      #elseif Lcd_mode = 3
         Lcd_write_byte(&H3b)
      #elseif Lcd_mode = 4
         Lcd_write_byte(&Hfb)
      #endif

      Call Lcd_select_reg(&H3a)                             'Set_pixel_format
      Call Lcd_write_data(&H55)


      Call Lcd_select_reg(&H2a)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H01)
      Call Lcd_write_byte(&H3f)                             '319
      Call Lcd_select_reg(&H2b)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H01)
      Call Lcd_write_byte(&He0)

      Call Lcd_select_reg(&H35)                             'Set_tear_on
      Call Lcd_write_byte(&H00)

      Call Lcd_select_reg(&H51)                             'Display Brightness
      Call Lcd_write_byte(&H20)                             '00h value means the lowest brightness and FFh value means the highest

      Call Lcd_select_reg(&H29)
   #endif


   #if Lcd_driver = 2                                       'HX8357C
      Call Lcd_select_reg(&Hb9)                             'Set EXTC
      Call Lcd_write_byte(&Hff)
      Call Lcd_write_byte(&H83)
      Call Lcd_write_byte(&H57)
      Waitms 50
      Call Lcd_select_reg(&Hb6)                             'Set VCOMDC
      Call Lcd_write_byte(&H2c)                             '52 For Hsd 3.0 ""
      Call Lcd_select_reg(&H11)
      Waitms 200
      Call Lcd_select_reg(&H35)                             'TE ON
      Call Lcd_select_reg(&H3a)
      Call Lcd_write_byte(&H55)                             '65K

      Call Lcd_select_reg(&Hcc)                             'set panel
      Call Lcd_select_reg(&H09)

      Call Lcd_select_reg(&Hb1)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H15)
      Call Lcd_write_byte(&H0d)
      Call Lcd_write_byte(&H0d)
      Call Lcd_write_byte(&H83)
      Call Lcd_write_byte(&H48)

      Call Lcd_select_reg(&Hc0)
      Call Lcd_write_byte(&H24)
      Call Lcd_write_byte(&H24)
      Call Lcd_write_byte(&H01)
      Call Lcd_write_byte(&H3c)
      Call Lcd_write_byte(&Hc8)
      Call Lcd_write_byte(&H08)

      Call Lcd_select_reg(&Hb4)
      Call Lcd_write_byte(&H02)
      Call Lcd_write_byte(&H40)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H2a)
      Call Lcd_write_byte(&H2a)
      Call Lcd_write_byte(&H0d)
      Call Lcd_write_byte(&H4f)

      Call Lcd_select_reg(&He0)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H15)
      Call Lcd_write_byte(&H1d)
      Call Lcd_write_byte(&H2a)
      Call Lcd_write_byte(&H31)
      Call Lcd_write_byte(&H42)
      Call Lcd_write_byte(&H4c)
      Call Lcd_write_byte(&H53)
      Call Lcd_write_byte(&H45)
      Call Lcd_write_byte(&H40)
      Call Lcd_write_byte(&H2a)
      Call Lcd_write_byte(&H3b)
      Call Lcd_write_byte(&H32)
      Call Lcd_write_byte(&H2e)
      Call Lcd_write_byte(&H28)

      Call Lcd_write_byte(&H24)
      Call Lcd_write_byte(&H03)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H15)
      Call Lcd_write_byte(&H1d)
      Call Lcd_write_byte(&H2a)
      Call Lcd_write_byte(&H31)
      Call Lcd_write_byte(&H42)
      Call Lcd_write_byte(&H4c)
      Call Lcd_write_byte(&H53)
      Call Lcd_write_byte(&H45)
      Call Lcd_write_byte(&H40)
      Call Lcd_write_byte(&H3b)
      Call Lcd_write_byte(&H32)
      Call Lcd_write_byte(&H2e)
      Call Lcd_write_byte(&H28)
      Call Lcd_write_byte(&H24)
      Call Lcd_write_byte(&H03)
      Call Lcd_write_byte(&H00)
      Call Lcd_write_byte(&H01)

      Call Lcd_select_reg(&H36)                             'Set_address_mode

      #if Lcd_mode = 1
         Lcd_write_byte(&H88)
      #elseif Lcd_mode = 2
         Lcd_write_byte(&H48)
      #elseif Lcd_mode = 3
         Lcd_write_byte(&H38)
      #elseif Lcd_mode = 4
         Lcd_write_byte(&Hf8)
      #endif


      Waitms 20
      Call Lcd_select_reg(&H21)                             'display inversion on
      Waitms 20
      Call Lcd_select_reg(&H29)                             'display on
      Waitms 120
   #endif

End Sub

'-------------------------------------------------------------------------------
' Select an index register
'-------------------------------------------------------------------------------
Sub Lcd_select_reg(byval Index As Byte )
    '  Cs_disp = 0
   Rs_disp = 0
   Data_disp = 0
   Data_disp_low = Index
   Wr_disp = 0
   Wr_disp = 1
    '  Cs_disp = 1
End Sub

Sub Lcd_write_byte(byval Daten As Byte)
   '  Cs_disp = 0
   Rs_disp = 1
   Data_disp = 0
   Data_disp_low = Daten                                    'Low(daten
   Wr_disp = 0
   Wr_disp = 1
   '  Cs_disp = 1
End Sub
'-------------------------------------------------------------------------------
' Write 16 Bit Data to Display
'-------------------------------------------------------------------------------
Sub Lcd_write_data(byval Daten As Word)
   Rgb16 = Daten
   '  Cs_disp = 0
   Rs_disp = 1
   Data_disp = Rgb16_high                                   ' High(daten)
   Data_disp_low = Rgb16_low                                'Low(daten
   Wr_disp = 0
   Wr_disp = 1
   '  Cs_disp = 1
End Sub
'-------------------------------------------------------------------------------
' Write 16 Bit Data to Index Register
'-------------------------------------------------------------------------------
Sub Lcd_write_reg(byval Index As Byte , Daten As Word)
   Rgb16 = Daten

    '  Cs_disp = 0

   Rs_disp = 0
   Data_disp = &H00                                         'VH
   Data_disp_low = Index                                    'VL
   Wr_disp = 0
   Wr_disp = 1
   Rs_disp = 1
   Data_disp = Rgb16_high                                   'VH
   Data_disp_low = Rgb16_low                                'VL
   Wr_disp = 0
   Wr_disp = 1

   '   Cs_disp = 1
End Sub