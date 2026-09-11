# PM Robots - Open Watcom wmake build file
#
# Usage: wmake -f makefile.wat all
#        wmake -f makefile.wat clean

CC     = wcc386
RC     = wrc
LINK   = wlink

!ifndef OS2TK
OS2TK  = c:\os2tk45
!endif

CFLAGS  = -bt=os2 -mf -5 -fpi -Oaxt -W3 -ze -d0 -i=$(OS2TK)\h -i=src
RCFLAGS = -i=$(OS2TK)\h -i=src

all : bin\Robots.exe

bin\Robots.exe : bin\robots.obj bin\game.obj bin\hiscores.obj bin\robots.res
    $(LINK) system os2v2 pm option stack=65536 option heapsize=4096 option map=bin\Robots.map name bin\Robots.exe file bin\robots.obj, bin\game.obj, bin\hiscores.obj
    $(RC) bin\robots.res bin\Robots.exe

bin\robots.obj : src\ROBOTS.C src\CONTROL.H src\lang.h
    $(CC) $(CFLAGS) src\ROBOTS.C -fo=bin\robots.obj

bin\game.obj : src\GAME.C src\CONTROL.H
    $(CC) $(CFLAGS) src\GAME.C -fo=bin\game.obj

bin\hiscores.obj : src\HISCORES.C src\CONTROL.H
    $(CC) $(CFLAGS) src\HISCORES.C -fo=bin\hiscores.obj

bin\robots.res : src\ROBOTS.RC src\CONTROL.H
    $(RC) $(RCFLAGS) -r src\ROBOTS.RC -fo=bin\robots.res

clean : .SYMBOLIC
    -del bin\robots.obj 2>NUL
    -del bin\game.obj 2>NUL
    -del bin\hiscores.obj 2>NUL
    -del bin\robots.res 2>NUL
    -del bin\Robots.exe 2>NUL
    -del bin\Robots.map 2>NUL
