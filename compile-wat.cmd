@echo off
rem compile-wat.cmd - PM Robots Open Watcom build script

set LOGFILE=compile-wat.log
echo Build started > %LOGFILE%

rem Detect WATCOM installation
set WATCOM=c:\watcom
if exist c:\watcom2\binp\wcc386.exe set WATCOM=c:\watcom2

if not exist %WATCOM%\binp\wcc386.exe goto nowatcom

rem Set default OS2TK if not already set
if "%OS2TK%" == "" set OS2TK=c:\os2tk45

echo WATCOM=%WATCOM% >> %LOGFILE%
echo OS2TK=%OS2TK% >> %LOGFILE%

set PATH=%WATCOM%\binp;%WATCOM%\binw;%PATH%
set INCLUDE=%WATCOM%\h;%WATCOM%\h\os2
set LIB=%WATCOM%\lib386;%WATCOM%\lib386\os2

wmake -f makefile.wat clean 2>&1 | tee -a %LOGFILE%
wmake -f makefile.wat all 2>&1 | tee -a %LOGFILE%

if exist bin\Robots.exe goto buildok

echo BUILD FAILED - see %LOGFILE%
echo BUILD FAILED >> %LOGFILE%
goto end

:buildok
echo BUILD OK
echo BUILD OK >> %LOGFILE%
goto end

:nowatcom
echo ERROR: Open Watcom not found at c:\watcom or c:\watcom2
echo ERROR: Open Watcom not found >> %LOGFILE%

:end
