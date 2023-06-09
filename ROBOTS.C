/*  ROBOTS.C
 *  Presentation Manager Robots v1.4
 *  Copyright (c) 1993,1994,2002 by Kent Lundberg
 *
 *  LICENSE: GNU GPL V3
 */

#define INCL_DOS
#define INCL_GPI
#define INCL_WIN
#include <os2.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "control.h"

MRESULT EXPENTRY ClientWndProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY HelpDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY ClearDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY HiScoresDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY GetNameDlgProc(HWND, ULONG, MPARAM, MPARAM);
HAB     hab;
int     sMapSize = 16;

int main(void) {
    HWND  hwndFrame, hwndClient, hwndMenu, hwndTBar;
    HMQ   hmq;
    QMSG  qmsg;
    RECTL rclTBar, rclMenu;
    ULONG flStyle = (ULONG) (FCF_TITLEBAR | FCF_SYSMENU | FCF_TASKLIST |
                         FCF_BORDER | FCF_MINBUTTON | FCF_MENU | FCF_ICON);
    static char *szClassName="Robots Buddy";
    static char *szWinTitle="Presentation Manager Robots";

    if (WinQuerySysValue(HWND_DESKTOP,SV_CXSCREEN) > 1000)
       sMapSize = 24;
    hab=WinInitialize(0);
    hmq=WinCreateMsgQueue(hab,0);
    WinRegisterClass(hab, (PCSZ) szClassName,(PFNWP) ClientWndProc,CS_SIZEREDRAW,0UL);
    hwndFrame=WinCreateStdWindow(HWND_DESKTOP,0,&flStyle,(PCSZ) szClassName, (PCSZ) "",
                                 0L,0UL,ID_RESOURCE,&hwndClient);
    WinSetWindowText(hwndFrame,(PCSZ) szWinTitle);
    WinSetWindowPos(hwndFrame,0,40,40, sMapSize*35, sMapSize*20, SWP_ACTIVATE | SWP_MOVE | SWP_SHOW | SWP_SIZE);
    hwndTBar=WinWindowFromID(hwndFrame,FID_TITLEBAR);
    WinQueryWindowRect(hwndTBar,&rclTBar);
    hwndMenu=WinWindowFromID(hwndFrame,FID_MENU);
    WinQueryWindowRect(hwndMenu,&rclMenu);
    WinSetWindowPos(hwndFrame,0,0,0, sMapSize*35 + 2, sMapSize*20 + 4 + rclTBar.yTop + rclMenu.yTop, SWP_SIZE);

    while (WinGetMsg(hab,&qmsg,0,0,0)) WinDispatchMsg(hab,&qmsg);

    WinDestroyWindow(hwndFrame);
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    return 0;
}


MRESULT EXPENTRY ClientWndProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2) {
    static HPS hpsMemory;
    static HDC hdcMemory = NULLHANDLE;
    static HWND hwndMenu; //, hwndScore;
    static HBITMAP hbmPlayer, hbmRobot, hbmPlayerD, hbmHeap, hbmDiamond;
    static HPOINTER ahptr[13];
    static HISCORES hs;
    static GAMESTATE gs;
    CHAR      szScore[40];
    HPS       hps;
    RECTL     rcl;
    POINTL    aptl[3] = {{0,0},{0,0},{0,0}};
    SIZEL     sizel={0,0};
    int       icx, icy = 0;

    switch (msg) {
        case WM_CREATE:
            hps = WinGetPS(hwnd);
            hwndMenu=WinWindowFromID(WinQueryWindow(hwnd, QW_PARENT), FID_MENU);
            hdcMemory=DevOpenDC(hab,OD_MEMORY, (PCSZ) "*",0,NULL,NULLHANDLE);
            hpsMemory=GpiCreatePS(hab,hdcMemory,&sizel,PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC);
            if (sMapSize == 24)
               icy = 5;
            hbmPlayer  = GpiLoadBitmap(hpsMemory,NULLHANDLE,(IDB_PLAYER  + icy),0,0);
            hbmPlayerD = GpiLoadBitmap(hpsMemory,NULLHANDLE,(IDB_PLAYERD + icy),0,0);
            hbmRobot   = GpiLoadBitmap(hpsMemory,NULLHANDLE,(IDB_ROBOT   + icy),0,0);
            hbmHeap    = GpiLoadBitmap(hpsMemory,NULLHANDLE,(IDB_HEAP    + icy),0,0);
            hbmDiamond = GpiLoadBitmap(hpsMemory,NULLHANDLE,(IDB_DIAMOND + icy),0,0);
            for (icx = 2 ; icx < 25 ; icx += 2)
               ahptr[icx/2]=WinLoadPointer(HWND_DESKTOP, 0, icx);
            WinReleasePS(hps);
            srand(time(NULLHANDLE));
            LoadHiScores(&hs, &gs);
            NewGame(&gs);
            return 0L;

        case WM_DESTROY:
            for (icx = 2 ; icx < 25 ; icx += 2)
               WinDestroyPointer(ahptr[icx/2]);
            GpiDeleteBitmap(hbmPlayer);
            GpiDeleteBitmap(hbmPlayerD);
            GpiDeleteBitmap(hbmRobot);
            GpiDeleteBitmap(hbmHeap);
            GpiDeleteBitmap(hbmDiamond);
            GpiDestroyPS(hpsMemory);
            DevCloseDC(hdcMemory);
            return 0L;

        case WM_PAINT:
            hps = WinBeginPaint(hwnd, 0UL, NULL);
            WinQueryWindowRect(hwnd,&rcl);
            WinFillRect(hps,&rcl,CLR_BLACK);
            GpiSetBitmap(hpsMemory,hbmRobot);
            for (icx = 0 ; icx < 35 ; icx++)
               for  (icy = 0 ; icy < 20 ; icy++)
                  if (gs.field[icx][icy] == ROBOT) {
                     aptl[0].x = icx*sMapSize;
                     aptl[0].y = icy*sMapSize;
                     aptl[1].x = icx*sMapSize + sMapSize;
                     aptl[1].y = icy*sMapSize + sMapSize;
                     GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
                  }
            GpiSetBitmap(hpsMemory,hbmHeap);
            for (icx = 0 ; icx < 35 ; icx++)
               for  (icy = 0 ; icy < 20 ; icy++)
                  if (gs.field[icx][icy] == HEAP) {
                     aptl[0].x = icx*sMapSize;
                     aptl[0].y = icy*sMapSize;
                     aptl[1].x = icx*sMapSize + sMapSize;
                     aptl[1].y = icy*sMapSize + sMapSize;
                     GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
                  }
            aptl[0].x = gs.playx*sMapSize;
            aptl[0].y = gs.playy*sMapSize;
            aptl[1].x = gs.playx*sMapSize + sMapSize;
            aptl[1].y = gs.playy*sMapSize + sMapSize;
            if (gs.active)
               GpiSetBitmap(hpsMemory,hbmPlayer);
            else
               GpiSetBitmap(hpsMemory,hbmPlayerD);
            GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            GpiSetBitmap(hpsMemory,hbmDiamond);
            if (gs.moves&MOVE_UL) {
               aptl[0].x = gs.playx*sMapSize - sMapSize;
               aptl[0].y = gs.playy*sMapSize + sMapSize;
               aptl[1].x = gs.playx*sMapSize;
               aptl[1].y = gs.playy*sMapSize + sMapSize*2;
               GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            }
            if (gs.moves&MOVE_U) {
               aptl[0].x = gs.playx*sMapSize;
               aptl[0].y = gs.playy*sMapSize + sMapSize;
               aptl[1].x = gs.playx*sMapSize + sMapSize;
               aptl[1].y = gs.playy*sMapSize + sMapSize*2;
               GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            }
            if (gs.moves&MOVE_UR) {
               aptl[0].x = gs.playx*sMapSize + sMapSize;
               aptl[0].y = gs.playy*sMapSize + sMapSize;
               aptl[1].x = gs.playx*sMapSize + sMapSize*2;
               aptl[1].y = gs.playy*sMapSize + sMapSize*2;
               GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            }
            if (gs.moves&MOVE_L) {
               aptl[0].x = gs.playx*sMapSize - sMapSize;
               aptl[0].y = gs.playy*sMapSize;
               aptl[1].x = gs.playx*sMapSize;
               aptl[1].y = gs.playy*sMapSize + sMapSize;
               GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            }
            if (gs.moves&MOVE_R) {
               aptl[0].x = gs.playx*sMapSize + sMapSize;
               aptl[0].y = gs.playy*sMapSize;
               aptl[1].x = gs.playx*sMapSize + sMapSize*2;
               aptl[1].y = gs.playy*sMapSize + sMapSize;
               GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            }
            if (gs.moves&MOVE_DL) {
               aptl[0].x = gs.playx*sMapSize - sMapSize;
               aptl[0].y = gs.playy*sMapSize - sMapSize;
               aptl[1].x = gs.playx*sMapSize;
               aptl[1].y = gs.playy*sMapSize;
               GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            }
            if (gs.moves&MOVE_D) {
               aptl[0].x = gs.playx*sMapSize;
               aptl[0].y = gs.playy*sMapSize - sMapSize;
               aptl[1].x = gs.playx*sMapSize + sMapSize;
               aptl[1].y = gs.playy*sMapSize;
               GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            }
            if (gs.moves&MOVE_DR) {
               aptl[0].x = gs.playx*sMapSize + sMapSize;
               aptl[0].y = gs.playy*sMapSize - sMapSize;
               aptl[1].x = gs.playx*sMapSize + sMapSize*2;
               aptl[1].y = gs.playy*sMapSize;
               GpiBitBlt(hps,hpsMemory,3L,aptl,ROP_SRCCOPY,0);
            }

            WinEndPaint(hps);
            sprintf( szScore," Score: %i  Level: %i",
                        gs.score, gs.level);
            WinSendMsg(hwndMenu, MM_SETITEMTEXT,
                        MPFROM2SHORT(IDM_SCORE, FALSE),
                        MPFROMP(szScore));
            if (RobotAdjacent(gs))
               WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_WAIT, TRUE),
                           MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));
            else
               WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_WAIT, TRUE),
                           MPFROM2SHORT(MIA_DISABLED,0));
            if (gs.sonic_reset) {
               WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_SONIC, TRUE),
                           MPFROM2SHORT(MIA_DISABLED,0));
               gs.sonic_reset = FALSE;
            }
            return 0L;

        case WM_MOUSEMOVE:
            icx = MOUSEMSG(&msg)->x;
            icy = MOUSEMSG(&msg)->y;
            WinSetPointer(HWND_DESKTOP, ahptr[Direction(gs,icx,icy,sMapSize)/2]);
            return 0L;

        case WM_BUTTON1UP:
            if (gs.active) {
               icx = MOUSEMSG(&msg)->x;
               icy = MOUSEMSG(&msg)->y;
               MovePlayer(&gs,Direction(gs,icx,icy,sMapSize));
               WinInvalidateRect(hwnd,NULLHANDLE,FALSE);
            }
            return 0L;

        case WM_BUTTON2UP:
            if (gs.active) {
               if (!RobotAdjacent(gs))
                  do {
                     RobotChase(&gs);
                     WinInvalidateRect(hwnd,NULLHANDLE,FALSE);
                     WinUpdateWindow(hwnd);
                  } while (!RobotAdjacent(gs)&&!gs.newlevel);
            }
            return 0L;

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {
                case IDM_NEW:
                    NewGame(&gs);
                    WinSendMsg(hwndMenu, MM_SETITEMATTR,
                       MPFROM2SHORT(IDM_SONIC, TRUE),
                       MPFROM2SHORT(MIA_DISABLED,0));
                    WinSendMsg(hwndMenu, MM_SETITEMATTR,
                       MPFROM2SHORT(IDM_TELEP, TRUE),
                       MPFROM2SHORT(MIA_DISABLED,0));
                    WinSendMsg(hwndMenu, MM_SETITEMATTR,
                       MPFROM2SHORT(IDM_WAIT, TRUE),
                       MPFROM2SHORT(MIA_DISABLED,0));
                    WinInvalidateRect(hwnd,NULLHANDLE,FALSE);
                    return 0L;

                case IDM_SONIC:
                    hps = WinGetPS(hwnd);
                    rcl.xLeft = gs.playx*sMapSize - sMapSize;
                    rcl.yBottom = gs.playy*sMapSize - sMapSize;
                    rcl.xRight = rcl.xLeft + sMapSize*3;
                    rcl.yTop = rcl.yBottom + sMapSize*3;
                    WinFillRect(hps,&rcl,CLR_WHITE);
                    WinReleasePS(hps);
                    WinSendMsg(hwndMenu, MM_SETITEMATTR,
                        MPFROM2SHORT(IDM_SONIC, TRUE),
                        MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));
                    SonicScrew(&gs);
                    WinInvalidateRect(hwnd,NULLHANDLE,FALSE);
                    return 0L;

                case IDM_TELEP:
                    Teleport(&gs);
                    if (!gs.active) {
                        WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_SONIC, TRUE),
                           MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));
                        WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_TELEP, TRUE),
                           MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));
                        WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_WAIT, TRUE),
                           MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));
                    }
                    WinInvalidateRect(hwnd,NULLHANDLE,FALSE);
                    if (!gs.active&&(gs.score > gs.hiscore)) {
                       gs.hiscore = gs.score;
                       WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP) GetNameDlgProc, 0, IDD_GETNAME, &gs);
                       SaveHiScores(&hs, &gs);
                       WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP) HiScoresDlgProc, 0, IDD_HISCORES, &hs);
                    }
                    return 0L;

                case IDM_WAIT:
                    do {
                       RobotChase(&gs);
                       WinInvalidateRect(hwnd,NULLHANDLE,FALSE);
                       WinUpdateWindow(hwnd);
                    } while (!RobotAdjacent(gs)&&!gs.newlevel);
                    return 0L;

                case IDM_QUIT:
                    WinSendMsg(hwnd, WM_CLOSE, 0L, 0L);
                    return 0L;

                case IDM_ABOUT:
                    WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP) AboutDlgProc, 0, IDD_ABOUT, NULL);
                    return 0L;

                case IDM_HSCORE:
                    WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP) HiScoresDlgProc, 0, IDD_HISCORES, &hs);
                    return 0L;

                case IDM_CLEAR:
                    WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP) ClearDlgProc, 0, IDD_CLEAR, &hs);
                    if (hs.clear) {
                       SaveHiScores(&hs, &gs);
                       WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP) HiScoresDlgProc, 0, IDD_HISCORES, &hs);
                    }
                    return 0L;

                case IDM_HELP:
                    WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP) HelpDlgProc, 0, IDD_HELP, NULL);
                    return 0L;

            }
    }
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_COMMAND:
         switch (COMMANDMSG(&msg)->cmd) {
            case DID_OK:
            case DID_CANCEL:
               WinDismissDlg(hwnd, TRUE);
               return 0L;
            }
      }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY HelpDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_COMMAND:
         switch (COMMANDMSG(&msg)->cmd) {
            case DID_OK:
            case DID_CANCEL:
               WinDismissDlg(hwnd, TRUE);
               return 0L;
            }
      }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY ClearDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   static HISCORES *phs;

   switch (msg) {
      case WM_INITDLG:
         phs = PVOIDFROMMP(mp2);
         return 0L;
      case WM_COMMAND:
         switch (COMMANDMSG(&msg)->cmd) {
            case DID_OK:
               WinDismissDlg(hwnd, TRUE);
               phs->clear = TRUE;
               return 0L;
            case DID_CANCEL:
               WinDismissDlg(hwnd, TRUE);
               return 0L;
            }
      }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY GetNameDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   static GAMESTATE *pgs;

   switch (msg) {
      case WM_INITDLG:
         pgs = PVOIDFROMMP(mp2);
         WinSetDlgItemText(hwnd, IDE_GETNAME, (PCSZ) pgs->name);
         return 0L;
      case WM_COMMAND:
         switch (COMMANDMSG(&msg)->cmd) {
            case DID_OK:
               WinQueryDlgItemText(hwnd, IDE_GETNAME, 20, (PSZ) pgs->name);
               WinDismissDlg(hwnd, TRUE);
               return 0L;
            case DID_CANCEL:
               WinDismissDlg(hwnd, TRUE);
               return 0L;
            }
      }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY HiScoresDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   HISCORES *phs;
   char sz[10];
   int ici;

   switch (msg) {
      case WM_INITDLG:
         phs = PVOIDFROMMP(mp2);
         for (ici = 0 ; ici < 10 ; ici++)
            WinSetDlgItemText(hwnd, IDE_HS+ici, (PCSZ) phs->name[ici]);
         for (ici = 0 ; ici < 10 ; ici++) {
            sprintf(sz,"%3i  ",phs->level[ici]);
            WinSetDlgItemText(hwnd, IDE_HS+10+ici, (PCSZ) sz);
         }
         for (ici = 0 ; ici < 10 ; ici++) {
            sprintf(sz,"%5i ",phs->score[ici]);
            WinSetDlgItemText(hwnd, IDE_HS+20+ici, (PCSZ) sz);
         }
         return 0L;
      case WM_COMMAND:
         switch (COMMANDMSG(&msg)->cmd) {
            case DID_OK:
            case DID_CANCEL:
               WinDismissDlg(hwnd, TRUE);
               return 0L;
            }
      }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


