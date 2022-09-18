@echo off
chcp 1252

title Kompilator
set dateformat=%time:~0,2%:%time:~3,2%:%time:~6,2%
echo Start pracy: %date% - %dateformat%

del "C:\Users\mateu\Desktop\Elita-MP\ElitaMP\source\version.c"
cd "C:\Users\mateu\Desktop\Elita-MP\ElitaMP\source"

set /a totalNumLines = 0
set /a files = 0
for /r %1 %%F in (*.lib *.c) do (
    set /a files+=1
    for /f %%N in ('find /v /c "" ^<"%%F"') do set /a totalNumLines+=%%N
)

cd "C:\Users\mateu\Desktop\Elita-MP\ElitaMP"

echo Linijki kodu:  %totalNumLines%
echo Ilosc plikow:  %files%

echo #define DATE   "%date%, %dateformat%" >> "source\version.c"
echo #define LINES  "%totalNumLines%" >> "source\version.c"
echo #define FILES  "%files%" >> "source\version.c"

set fileName="map.lib"
(
    ECHO.Option Explicit
    ECHO.Dim Fso, Text, x
    ECHO.ReDim Lines^(0^)
    ECHO.Set Fso = CreateObject^("Scripting.FileSystemObject"^)
    ECHO.Set Text = Fso.OpenTextFile^(%fileName%^)
    ECHO.While Not Text.AtEndOfStream
    ECHO.    x = Text.Line - 1
    ECHO.    ReDim Preserve Lines^(x^)
    ECHO.    Lines^(x^) = Text.ReadLine^(^)
    ECHO.    If InStr^(Lines^(x^), "const buildNumber = "^) = 1 Then
    ECHO.        Lines^(x^) = "const buildNumber = " ^& CInt^(Mid^(Lines^(x^), 21, Len^(Lines^(x^)^) - 21^)^) + 1 ^& ";"
    ECHO.    End If 
    ECHO.Wend
    ECHO.Text.Close^(^)
    ECHO.Fso.DeleteFile^(%fileName%^)
    ECHO.Set Text = Fso.CreateTextFile^(%fileName%^)
    ECHO.For Each x In Lines
    ECHO.    Text.WriteLine^(x^)
    ECHO.Next
    ECHO.Text.Close^(^)
) > Modify.vbs
CSCRIPT //NOLOGO Modify.vbs
DEL Modify.vbs

"pawncc.exe" "map.lib" -;+ -(+ -d3 -r
move /Y map.amx server/gamemodes/map.amx

endlocal