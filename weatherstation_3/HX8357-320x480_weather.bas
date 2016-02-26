

'Weather station version 3.0

'Ben Zijlstra en Theo Kleijn

'Display Driver IC HX8357B or C 480x320 Pixel
'Modus 16Bit
'HVGA 480x320 3,6
'
'Bascom version 2.0.7.9
'
'Autor hx8357c driver: Hkipnik@aol.com
'
'Copyright by Hkipnik@aol.com

'Arduino Mega 2560

$regfile = "m2560def.dat"
$crystal = 16000000
$hwstack = 200
$swstack = 200
$framesize = 200
'serial connection to the nodemcu
Config Com1 = 115200 , Synchrone = 0 , Parity = None , Stopbits = 1 , Databits = 8 , Clockpol = 0
'serial connection to a debug PC
Config Com2 = 9600 , Synchrone = 0 , Parity = None , Stopbits = 1 , Databits = 8 , Clockpol = 0
'create a buffered serial connection to the nodemcu
Config Serialin1 = Buffered , Size = 254
Config Submode = New

'Dims for the weatherstation
'See for hx8357-dims the include file hx8357_declarations.inc

Dim Temperature As String * 7
Dim Humidity As String * 7
Dim Pressure As String * 7
Dim Windspeed As String * 7
Dim Windbearing As String * 7
Dim Cloudcover As String * 7

Dim Tempstring As String * 7
Dim Templen As Byte

Dim Teller As Word

Dim Buffer As String * 80
Dim A As Byte
Dim X As Byte
Dim Info(10) As String * 80
Dim Buffer_len As Byte
Dim Info_len As Byte

Dim Count As Byte
Dim Ar(2) As String * 40
Dim Ad(3) As String * 4

'*******************************************************************************
'Display Mode
'*******************************************************************************
Const Sd_card = 0                                           'with SD Card = 1 -- without SD Card = 0
Const Lcd_mode = 1                                          '1=Portrait 2=Portrait 180° 3=landscape 4=landscape 180°
Const Lcd_driver = 2                                        '1=HX8357B  2=HX8357C
'*******************************************************************************
#if Sd_card = 1
   $include "Avr-Dos\Config_MMCSD_HC.bas"
   $include "Avr-Dos\Config_AVR-DOS.bas"
#endif

'*******************************************************************************
$include "TFTDriver\HX8357_declarations.inc"

Config Timer1 = Timer , Prescale = 256
On Ovf1 Tim1_isr

Config Portb.4 = Output                                     'reset from Nodemcu by Arduino
Reset Portb.4

'*******************************************************************************
'Init the Display
'*******************************************************************************
Call Lcd_init()
Call Lcd_clear(black)

'*******************************************************************************
' SD Card Info
'*******************************************************************************
#if Sd_card = 1
   Call Init_sd_card()                                      'Init SD Card
   Temp_str = "SD Card= " + Str(sd_ok)
   Call Lcd_text(temp_str , 1 , 1 , 2 , Yellow , Black , 1)
'   1 OK
'   0 Error

   Temp_str = "SD Card Typ= " + Str(mmcsd_cardtype)
   Call Lcd_text(temp_str , 1 , 20 , 2 , Yellow , Black , 1)
'   0 can't init the Card
'   1 MMC
'   2 SDSC Spec. 1.x
'   4 SDSC Spec. 2.0 or later
'  12 SDHC Spec. 2.0 or later

   Wait 2
   Call Lcd_clear(black)
#endif

Teller = 460                                                'to get the first time a quick update

'Open both com-ports
Open "com1:" For Binary As #1
Open "com2:" For Binary As #2

Enable Interrupts
Enable Timer1

Print #1 , "Start program"

Set Portb.4                                                 ' reset nodemcu
Wait 1
Reset Portb.4

'*******************************************************************************
' Display Portrait
'*******************************************************************************
#if Lcd_mode = 1 Or Lcd_mode = 2

Call Lcd_clear(black)
Call Lcd_text( "BASCOM-AVR Weatherstation" , 5 , 1 , 2 , Dodgerblue , Black , 1)
Call Lcd_line(5 , 23 , 315 , 23 , 1 , Yellow)
Call Lcd_text( "Location:         Tilburg" , 5 , 30 , 2 , Floralwhite , Black , 1)
Call Lcd_text( "Fetching" , 92 , 200 , 2 , Crimson , Black , 1)
Call Lcd_text( "weather info" , 68 , 220 , 2 , Crimson , Black , 1)
Call Lcd_line(5 , 412 , 315 , 412 , 1 , Yellow)
Call Lcd_text( "Powered by: eventcaster.nl" , 5 , 460 , 2 , Dodgerblue , Black , 1)

Do
   Buffer = ""
   'split all weatherinfo in seperate info array
   Do

      A = Inkey(#2)
      If A > 0 Then
         Print #1 , Chr(a);
         Buffer = Buffer + Chr(a)
         If A = 13 Then
            X = Instr(buffer , "closing connection")
            If X > 0 Then
               Exit Do
            End If
            X = Instr(buffer , "CURRENT_DATETIME=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Count = Split(ar(2) , Ar(1) , " ")
               Count = Split(ar(1) , Ad(1) , "-")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Ar(2) = Left(ar(2) , Buffer_len)
               Info(1) = Ad(3) + "-" + Ad(2) + "-" + Ad(1)
               Info(1) = Info(1) + " " + Ar(2) + " GMT+1"
            End If
            X = Instr(buffer , "CURRENT_TEMP=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Info(2) = Left(ar(2) , Buffer_len)
            End If
            X = Instr(buffer , "CURRENT_HUMIDITY=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Info(3) = Left(ar(2) , Buffer_len)
            End If
            X = Instr(buffer , "CURRENT_ICON=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Info(4) = Left(ar(2) , Buffer_len)
            End If
            X = Instr(buffer , "CURRENT_PRESSURE=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Info(5) = Left(ar(2) , Buffer_len)
            End If
            X = Instr(buffer , "CURRENT_WINDSPEED=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Info(6) = Left(ar(2) , Buffer_len)
            End If
            X = Instr(buffer , "CURRENT_SUMMARY=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Info(7) = Left(ar(2) , Buffer_len)
            End If
            X = Instr(buffer , "CURRENT_WINDBEARING=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Info(8) = Left(ar(2) , Buffer_len)
            End If
            X = Instr(buffer , "CURRENT_CLOUDCOVER=")
            If X > 0 Then
               Count = Split(buffer , Ar(1) , "=")
               Buffer_len = Len(ar(2))
               Buffer_len = Buffer_len - 1
               Info(9) = Left(ar(2) , Buffer_len)
            End If
            Buffer = ""
         End If
      End If
   Loop

      '*******************
   Call Lcd_text( "Fetching" , 92 , 200 , 2 , Black , Black , 1)
   Call Lcd_text( "weather info" , 68 , 220 , 2 , Black , Black , 1)

   'place it on the display
   Select Case Info(4)
      Case "clear-day"
         Restore Clear_day
      Case "clear-night"
         Restore Clear_night
      Case "cloudy"
         Restore Cloudy
      Case "rain"
         Restore Rain
      Case "wind"
         Restore Wind
      Case "snow"
         Restore Snoww
      Case "fog"
         Restore Fog
      Case "partly-cloudy-day"
         Restore Partly_cloudy_day
      Case "partly-cloudy-night"
         Restore Partly_cloudy_night
      Case "sleet"
         Restore Sleet
   End Select

   Call Lcd_show_bgc(20 , 54)

   Call Lcd_text( "Temperature:" , 5 , 120 , 2 , Crimson , Black , 1)
   Tempstring = Info(2)
   Temperature = Space(7)
   Templen = 8 - Len(tempstring)
   Mid(temperature , Templen) = Tempstring
   Call Lcd_text(temperature , 160 , 104 , 4 , Yellow , Black , 1)

   Call Lcd_text( "Humidity:" , 5 , 158 , 2 , Crimson , Black , 1)
   Tempstring = Info(3)
   Humidity = Space(7)
   Templen = 8 - Len(tempstring)
   Mid(humidity , Templen) = Tempstring
   Call Lcd_text(humidity , 160 , 142 , 4 , Yellow , Black , 1)

   Call Lcd_text( "Pressure:" , 5 , 196 , 2 , Crimson , Black , 1)
   Tempstring = Info(5)
   Humidity = Space(7)
   Templen = 8 - Len(tempstring)
   Mid(pressure , Templen) = Tempstring
   Call Lcd_text(pressure , 160 , 180 , 4 , Yellow , Black , 1)

   Call Lcd_text( "Windspeed m/s:" , 5 , 234 , 2 , Crimson , Black , 1)
   Tempstring = Info(6)
   Windspeed = Space(6)
   Templen = 7 - Len(tempstring)
   Mid(windspeed , Templen) = Tempstring
   Call Lcd_text(windspeed , 180 , 218 , 4 , Yellow , Black , 1)

   Call Lcd_text( "Windbearing:" , 5 , 272 , 2 , Crimson , Black , 1)
   Tempstring = Info(8)
   Windbearing = Space(7)
   Templen = 8 - Len(tempstring)
   Mid(windbearing , Templen) = Tempstring
   Call Lcd_text(windbearing , 160 , 256 , 4 , Yellow , Black , 1)

   Call Lcd_text( "Cloudcover:" , 5 , 310 , 2 , Crimson , Black , 1)
   Tempstring = Info(9)
   Cloudcover = Space(7)
   Templen = 8 - Len(tempstring)
   Mid(cloudcover , Templen) = Tempstring
   Call Lcd_text(cloudcover , 160 , 294 , 4 , Yellow , Black , 1)

   'check the longest text that can be displayed
   Call Lcd_text(info(7) , 5 , 340 , 2 , Yellow , Black , 1)

   Call Lcd_line(5 , 412 , 315 , 412 , 1 , Yellow)

   Call Lcd_text( "Last update:" , 5 , 420 , 2 , Crimson , Black , 1)
   Call Lcd_text(info(1) , 5 , 440 , 2 , Floralwhite , Black , 1)
   Call Lcd_text( "Powered by: eventcaster.nl" , 5 , 460 , 2 , Dodgerblue , Black , 1)

#endif

Loop

Close #1
Close #2

End

Tim1_isr:
   Teller = Teller + 1
   If Teller = 480 Then
      Stop Timer1
      Lcd_clear(black)
     'some steady info on the display
      Call Lcd_text( "BASCOM-AVR Weatherstation" , 5 , 1 , 2 , Dodgerblue , Black , 1)       'zo laten Call Lcd_text( "Weatherstation" , 31 , 27 , 2 , Crimson , Black , 1)
      Call Lcd_line(5 , 23 , 315 , 23 , 1 , Yellow)
      Call Lcd_text( "Location:         Tilburg" , 5 , 30 , 2 , Floralwhite , Black , 1)
      Call Lcd_line(5 , 412 , 315 , 412 , 1 , Yellow)

      Call Lcd_text( "Powered by: eventcaster.nl" , 5 , 460 , 2 , Dodgerblue , Black , 1)

      Call Lcd_text( "Fetching" , 92 , 200 , 2 , Crimson , Black , 1)
      Call Lcd_text( "weather info" , 68 , 220 , 2 , Crimson , Black , 1)
      Set Portb.4                                           ' reset nodemcu
      Wait 1
      Reset Portb.4
      Teller = 0
      Start Timer1
   End If
Return

'*******************************************************************************

#if Sd_card = 1
   $include "Avr-Dos\SD_card_init.inc"
#endif
$include "TFTDriver\HX8357_functions.inc"

$include "Font\Digital20x32.font"                           '7 segments
$include "Font\font12x16.font"
$include "Font\font25x32.font"
$include "Font\Arial11x14.font"

'icons used
Wind:
   $bgf "Bilder\wind.bgc"
Sunset:
   $bgf "Bilder\sunset.bgc"
Snoww:
   $bgf "Bilder\snoww.bgc"
Sleet:
   $bgf "Bilder\sleet.bgc"
Rain:
   $bgf "Bilder\rain.bgc"
Partly_cloudy_night:
   $bgf "Bilder\pcn.bgc"
Partly_cloudy_day:
   $bgf "Bilder\pcd.bgc"
Fog:
   $bgf "Bilder\fog.bgc"
Cloudy:
   $bgf "Bilder\cloudy.bgc"
Clear_night:
   $bgf "Bilder\clear_night.bgc"
Clear_day:
   $bgf "Bilder\clear_day.bgc"