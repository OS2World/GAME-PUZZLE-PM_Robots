@echo off
rem compile-wat.cmd - PM Robots Open Watcom build script
rem Redirects all output to compile-wat.log

set LOGFILE=compile-wat.log
echo Build started > %LOGFILE%
date /t >> %LOGFILE%
time /t >> %LOGFILE%

rem Detect WATCOM installation
if exist c:\watcom\binp\wcc386.exe (
    set WATCOM=c:\watcom
) else if exist c:\watcom2\binp\wcc386.exe (
    set WATCOM=c:\watcom2
) else (
    echo ERROR: Open Watcom not found at c:\watcom or c:\watcom2 >> %LOGFILE%
    echo ERROR: Open Watcom not found at c:\watcom or c:\watcom2
    exit /b 1
)

rem Set default OS2TK if not already set
if not defined OS2TK set OS2TK=c:\os2tk45

echo WATCOM=%WATCOM% >> %LOGFILE%
echo OS2TK=%OS2TK% >> %LOGFILE%

set PATH=%WATCOM%\binp;%WATCOM%\binw;%PATH%

rem Clean and build
wmake -f makefile.wat clean >> %LOGFILE%
wmake -f makefile.wat all >> %LOGFILE%

if exist bin\Robots.exe (
    echo BUILD OK
    echo BUILD OK >> %LOGFILE%
) else (
    echo BUILD FAILED - see compile-wat.log
    echo BUILD FAILED >> %LOGFILE%
    exit /b 1
)
