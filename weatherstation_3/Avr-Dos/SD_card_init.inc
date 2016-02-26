'*******************************************************************************
'
'Uni-TFT für Bascom AVR
'
'
'
'
'
'
'By Hkipnik
' © Copyright by H.Kipnik 2012
'Date 10.12.2015
'Hkipnik@aol.com
'
'*******************************************************************************
$nocompile

Sub Init_sd_card

Do
Incr Dum

Reset Mmc_cs
'Waitms 100

If Gbdriveerror <> 0 Then
 '  Print "Card Error " ; Gbdriveerror
Else
 '  Print "Karte erkannt!"
End If


Btemp1 = Initfilesystem(1)
If Btemp1 <> 0 Then
 '  Print "File System Error " ; Btemp1
   Sd_ok = 0
Else
 '  Print "Filesystem ok!"
   Sd_ok = 1
End If

Loop Until Btemp1 = 0 Or Dum > 10


End Sub