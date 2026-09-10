/*  ROBOTS.C
 *  Presentation Manager Robots v1.4
 *  Copyright (c) 1993,1994,2002 by Kent Lundberg
 *
 *  LICENSE: GNU GPL V3
 */

static const char bldlevel[] =
    "@#Kent Lundberg:1.4#@##1## 09 Jun 2023 20:00:00      "
    "ARCAOS:::0::::@@PM Robots Game for OS/2\r\n\x1a";

#define INCL_DOS
#define INCL_GPI
#define INCL_WIN
#include <os2.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "control.h"
#include "lang.h"

/* ------------------------------------------------------------------ */
/* Language string table                                               */
/* ------------------------------------------------------------------ */
int current_lang = LANG_EN;

const char *lang_strings[LANG_COUNT][STR_COUNT] = {
    /* LANG_EN */
    {
        "~Game",
        "~New Game",
        "~Sonic Screwdriver",
        "~Teleport",
        "~Wait",
        "~High Scores...",
        "~Clear Scores...",
        "~Pause\tCtrl+P",
        "E~xit\tCtrl+X",
        "~Options",
        "~Language",
        "~Save settings on exit",
        "~Background Run\tCtrl+B",
        "~Frame Controls\tCtrl+F",
        "~Help",
        "How to ~Play...",
        "~About PM Robots...",
    },
    /* LANG_ES */
    {
        "~Juego",
        "~Nuevo Juego",
        "~Destornillador Sonico",
        "~Teleportar",
        "~Esperar",
        "~Mejores Puntos...",
        "~Borrar Puntos...",
        "~Pausa\tCtrl+P",
        "~Salir\tCtrl+X",
        "~Opciones",
        "~Idioma",
        "~Guardar al salir",
        "~Fondo Activo\tCtrl+B",
        "~Controles Marco\tCtrl+F",
        "~Ayuda",
        "~Como Jugar...",
        "~Acerca de PM Robots...",
    },
    /* LANG_NL */
    {
        "~Spel",
        "~Nieuw Spel",
        "~Sonische Schroevendraaier",
        "~Teleporteren",
        "~Wachten",
        "~Topscores...",
        "~Wis Scores...",
        "~Pauze\tCtrl+P",
        "A~fsluiten\tCtrl+X",
        "~Opties",
        "~Taal",
        "~Bewaar bij afsluiten",
        "~Achtergrond Actief\tCtrl+B",
        "~Raambesturing\tCtrl+F",
        "~Hulp",
        "~Hoe te Spelen...",
        "~Over PM Robots...",
    },
    /* LANG_DE */
    {
        "~Spiel",
        "~Neues Spiel",
        "~Schraubenzieher",
        "~Teleportieren",
        "~Warten",
        "~Bestenliste...",
        "Liste ~loeschen...",
        "~Pause\tCtrl+P",
        "~Beenden\tCtrl+X",
        "~Optionen",
        "~Sprache",
        "Einstellungen ~speichern",
        "~Hintergrundlauf\tCtrl+B",
        "~Rahmensteuerung\tCtrl+F",
        "~Hilfe",
        "~Spielanleitung...",
        "~Ueber PM Robots...",
    },
    /* LANG_FR */
    {
        "~Jeu",
        "~Nouveau Jeu",
        "~Tournevis Sonique",
        "~Teleporter",
        "~Attendre",
        "~Meilleurs Scores...",
        "~Effacer Scores...",
        "~Pause\tCtrl+P",
        "~Quitter\tCtrl+X",
        "~Options",
        "~Langue",
        "~Sauvegarder a la sortie",
        "~Arriere-plan Actif\tCtrl+B",
        "~Controles Cadre\tCtrl+F",
        "~Aide",
        "~Comment Jouer...",
        "~A propos de PM Robots...",
    },
    /* LANG_IT */
    {
        "~Gioco",
        "~Nuovo Gioco",
        "~Cacciavite Sonico",
        "~Teleportare",
        "~Aspettare",
        "~Punteggi...",
        "~Cancella Punteggi...",
        "~Pausa\tCtrl+P",
        "~Esci\tCtrl+X",
        "~Opzioni",
        "~Lingua",
        "~Salva all uscita",
        "~Esecuzione Sfondo\tCtrl+B",
        "~Controlli Cornice\tCtrl+F",
        "~Aiuto",
        "~Come Giocare...",
        "~Informazioni su PM Robots...",
    },
};

/* ------------------------------------------------------------------ */
/* Global state                                                        */
/* ------------------------------------------------------------------ */
HAB     hab;
int     sMapSize = 16;

static BOOL saveonexit   = FALSE;
static BOOL bPaused      = FALSE;
static BOOL bBackgrndRun = FALSE;
static BOOL bFrameHidden = FALSE;
static BOOL bFocusPaused = FALSE;

/* ------------------------------------------------------------------ */
/* Forward declarations                                                */
/* ------------------------------------------------------------------ */
MRESULT EXPENTRY ClientWndProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY HelpDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY ClearDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY HiScoresDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY GetNameDlgProc(HWND, ULONG, MPARAM, MPARAM);

void load_settings(void);
void save_settings(void);
void set_language(HWND hwndMenu, int lang);

/* ------------------------------------------------------------------ */
/* Settings persistence                                                */
/* ------------------------------------------------------------------ */
void load_settings(void) {
    FILE *f = fopen("robots.cfg", "rb");
    if (f) {
        fread(&saveonexit,   sizeof(int), 1, f);
        fread(&current_lang, sizeof(int), 1, f);
        fclose(f);
        if (current_lang < 0 || current_lang >= LANG_COUNT) current_lang = 0;
    }
}

void save_settings(void) {
    FILE *f = fopen("robots.cfg", "wb");
    if (f) {
        fwrite(&saveonexit,   sizeof(int), 1, f);
        fwrite(&current_lang, sizeof(int), 1, f);
        fclose(f);
    }
}

/* ------------------------------------------------------------------ */
/* Language switching                                                  */
/* ------------------------------------------------------------------ */
void set_language(HWND hwndMenu, int lang) {
    MENUITEM mi;
    HWND hwndGame, hwndOpts, hwndLang, hwndHelp;
    int i;

    if (lang < 0 || lang >= LANG_COUNT) lang = 0;
    current_lang = lang;

    memset(&mi, 0, sizeof(mi));
    WinSendMsg(hwndMenu, MM_QUERYITEM,
               MPFROM2SHORT(IDM_GAME, TRUE), MPFROMP(&mi));
    hwndGame = mi.hwndSubMenu;

    memset(&mi, 0, sizeof(mi));
    WinSendMsg(hwndMenu, MM_QUERYITEM,
               MPFROM2SHORT(IDM_SUBMENU_OPTS, TRUE), MPFROMP(&mi));
    hwndOpts = mi.hwndSubMenu;

    hwndLang = NULLHANDLE;
    if (hwndOpts) {
        memset(&mi, 0, sizeof(mi));
        WinSendMsg(hwndOpts, MM_QUERYITEM,
                   MPFROM2SHORT(IDM_SUBMENU_LANG, TRUE), MPFROMP(&mi));
        hwndLang = mi.hwndSubMenu;
    }

    memset(&mi, 0, sizeof(mi));
    WinSendMsg(hwndMenu, MM_QUERYITEM,
               MPFROM2SHORT(IDM_SUBMENU_HELP, TRUE), MPFROMP(&mi));
    hwndHelp = mi.hwndSubMenu;

    /* Update top-level titles */
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_GAME),         MPFROMP(tr(STR_MENU_GAME)));
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_OPTS), MPFROMP(tr(STR_MENU_OPTIONS)));
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_HELP), MPFROMP(tr(STR_MENU_HELP)));

    /* Update Game submenu */
    if (hwndGame) {
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_NEW),    MPFROMP(tr(STR_MENU_NEW)));
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_SONIC),  MPFROMP(tr(STR_MENU_SONIC)));
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_TELEP),  MPFROMP(tr(STR_MENU_TELEP)));
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_WAIT),   MPFROMP(tr(STR_MENU_WAIT)));
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_HSCORE), MPFROMP(tr(STR_MENU_HSCORES)));
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_CLEAR),  MPFROMP(tr(STR_MENU_CLEAR)));
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_PAUSE),  MPFROMP(tr(STR_MENU_PAUSE)));
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_EXIT),   MPFROMP(tr(STR_MENU_EXIT)));
    }

    /* Update Options submenu */
    if (hwndOpts) {
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_SUBMENU_LANG), MPFROMP(tr(STR_MENU_LANGUAGE)));
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_SAVEONEXIT),   MPFROMP(tr(STR_MENU_SAVEONEXIT)));
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_BACKGRND),     MPFROMP(tr(STR_MENU_BACKGRND)));
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_FRAME_CTRL),   MPFROMP(tr(STR_MENU_FRAME)));
    }

    /* Update Help submenu */
    if (hwndHelp) {
        WinSendMsg(hwndHelp, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_HELP),  MPFROMP(tr(STR_MENU_HOWTOPLAY)));
        WinSendMsg(hwndHelp, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_ABOUT), MPFROMP(tr(STR_MENU_ABOUT)));
    }

    /* Update language checkmarks */
    if (hwndLang) {
        for (i = 0; i < LANG_COUNT; i++)
            WinCheckMenuItem(hwndLang, IDM_LANG_EN + i, (i == lang));
    }
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */
int main(void) {
    HWND  hwndFrame, hwndClient;
    HMQ   hmq;
    QMSG  qmsg;
    LONG  cxScreen, cyScreen, winW, winH, x, y;
    ULONG flStyle = (ULONG)(FCF_TITLEBAR | FCF_SYSMENU | FCF_TASKLIST |
                            FCF_BORDER | FCF_MINBUTTON | FCF_MENU | FCF_ICON |
                            FCF_ACCELTABLE);
    static char *szClassName = "Robots Buddy";
    static char *szWinTitle  = "Presentation Manager Robots";

    cxScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
    cyScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);

    if (cxScreen > 1000)
        sMapSize = 24;

    hab = WinInitialize(0);
    hmq = WinCreateMsgQueue(hab, 0);
    WinRegisterClass(hab, (PCSZ)szClassName, (PFNWP)ClientWndProc, CS_SIZEREDRAW, 0UL);

    hwndFrame = WinCreateStdWindow(HWND_DESKTOP, 0L, &flStyle,
                                   (PCSZ)szClassName, (PCSZ)"",
                                   0L, 0UL, ID_RESOURCE, &hwndClient);
    WinSetWindowText(hwndFrame, (PCSZ)szWinTitle);

    /* Size window to map grid and center on desktop */
    {
        RECTL rclTBar, rclMenu;
        HWND hwndTBarW = WinWindowFromID(hwndFrame, FID_TITLEBAR);
        HWND hwndMenuW = WinWindowFromID(hwndFrame, FID_MENU);
        WinQueryWindowRect(hwndTBarW, &rclTBar);
        WinQueryWindowRect(hwndMenuW, &rclMenu);
        winW = (LONG)sMapSize * 35L + 2L;
        winH = (LONG)sMapSize * 20L + 4L + rclTBar.yTop + rclMenu.yTop;
        x = (cxScreen - winW) / 2L;
        y = (cyScreen - winH) / 2L;
        if (x < 0L) x = 0L;
        if (y < 0L) y = 0L;
    }

    WinSetWindowPos(hwndFrame, HWND_TOP, x, y, winW, winH,
                    SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW);

    while (WinGetMsg(hab, &qmsg, 0, 0, 0))
        WinDispatchMsg(hab, &qmsg);

    WinDestroyWindow(hwndFrame);
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    return 0;
}


/* ------------------------------------------------------------------ */
/* Client window procedure                                             */
/* ------------------------------------------------------------------ */
MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
    static HPS  hpsMemory;
    static HDC  hdcMemory = NULLHANDLE;
    static HWND hwndMenu;
    static HWND hwndOpts;
    static HWND hwndGameSub;
    static HBITMAP hbmPlayer, hbmRobot, hbmPlayerD, hbmHeap, hbmDiamond;
    static HPOINTER ahptr[13];
    static HISCORES hs;
    static GAMESTATE gs;
    CHAR      szScore[40];
    HPS       hps;
    RECTL     rcl;
    POINTL    aptl[3] = {{0,0},{0,0},{0,0}};
    SIZEL     sizel = {0, 0};
    int       icx, icy = 0;

    switch (msg) {
        case WM_CREATE:
        {
            MENUITEM mi;
            hps = WinGetPS(hwnd);
            hwndMenu = WinWindowFromID(WinQueryWindow(hwnd, QW_PARENT), FID_MENU);
            hdcMemory = DevOpenDC(hab, OD_MEMORY, (PCSZ)"*", 0, NULL, NULLHANDLE);
            hpsMemory = GpiCreatePS(hab, hdcMemory, &sizel,
                                    PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC);
            if (sMapSize == 24) icy = 5;
            hbmPlayer  = GpiLoadBitmap(hpsMemory, NULLHANDLE, (IDB_PLAYER  + icy), 0, 0);
            hbmPlayerD = GpiLoadBitmap(hpsMemory, NULLHANDLE, (IDB_PLAYERD + icy), 0, 0);
            hbmRobot   = GpiLoadBitmap(hpsMemory, NULLHANDLE, (IDB_ROBOT   + icy), 0, 0);
            hbmHeap    = GpiLoadBitmap(hpsMemory, NULLHANDLE, (IDB_HEAP    + icy), 0, 0);
            hbmDiamond = GpiLoadBitmap(hpsMemory, NULLHANDLE, (IDB_DIAMOND + icy), 0, 0);
            for (icx = 2; icx < 25; icx += 2)
                ahptr[icx/2] = WinLoadPointer(HWND_DESKTOP, 0, icx);
            WinReleasePS(hps);
            srand(time(NULLHANDLE));

            load_settings();
            set_language(hwndMenu, current_lang);

            /* Cache Options and Game submenu handles for checkmarks */
            memset(&mi, 0, sizeof(mi));
            WinSendMsg(hwndMenu, MM_QUERYITEM,
                       MPFROM2SHORT(IDM_SUBMENU_OPTS, TRUE), MPFROMP(&mi));
            hwndOpts = mi.hwndSubMenu;

            memset(&mi, 0, sizeof(mi));
            WinSendMsg(hwndMenu, MM_QUERYITEM,
                       MPFROM2SHORT(IDM_GAME, TRUE), MPFROMP(&mi));
            hwndGameSub = mi.hwndSubMenu;

            /* Restore persisted checkmarks */
            if (hwndOpts) {
                WinCheckMenuItem(hwndOpts, IDM_SAVEONEXIT, saveonexit);
                WinCheckMenuItem(hwndOpts, IDM_BACKGRND,   bBackgrndRun);
                WinCheckMenuItem(hwndOpts, IDM_FRAME_CTRL, bFrameHidden);
            }

            LoadHiScores(&hs, &gs);
            NewGame(&gs);
            return 0L;
        }

        case WM_DESTROY:
            if (saveonexit)
                save_settings();
            for (icx = 2; icx < 25; icx += 2)
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
            WinQueryWindowRect(hwnd, &rcl);
            WinFillRect(hps, &rcl, CLR_BLACK);
            GpiSetBitmap(hpsMemory, hbmRobot);
            for (icx = 0; icx < 35; icx++)
                for (icy = 0; icy < 20; icy++)
                    if (gs.field[icx][icy] == ROBOT) {
                        aptl[0].x = icx*sMapSize;
                        aptl[0].y = icy*sMapSize;
                        aptl[1].x = icx*sMapSize + sMapSize;
                        aptl[1].y = icy*sMapSize + sMapSize;
                        GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
                    }
            GpiSetBitmap(hpsMemory, hbmHeap);
            for (icx = 0; icx < 35; icx++)
                for (icy = 0; icy < 20; icy++)
                    if (gs.field[icx][icy] == HEAP) {
                        aptl[0].x = icx*sMapSize;
                        aptl[0].y = icy*sMapSize;
                        aptl[1].x = icx*sMapSize + sMapSize;
                        aptl[1].y = icy*sMapSize + sMapSize;
                        GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
                    }
            aptl[0].x = gs.playx*sMapSize;
            aptl[0].y = gs.playy*sMapSize;
            aptl[1].x = gs.playx*sMapSize + sMapSize;
            aptl[1].y = gs.playy*sMapSize + sMapSize;
            if (gs.active)
                GpiSetBitmap(hpsMemory, hbmPlayer);
            else
                GpiSetBitmap(hpsMemory, hbmPlayerD);
            GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            GpiSetBitmap(hpsMemory, hbmDiamond);
            if (gs.moves & MOVE_UL) {
                aptl[0].x = gs.playx*sMapSize - sMapSize;
                aptl[0].y = gs.playy*sMapSize + sMapSize;
                aptl[1].x = gs.playx*sMapSize;
                aptl[1].y = gs.playy*sMapSize + sMapSize*2;
                GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            }
            if (gs.moves & MOVE_U) {
                aptl[0].x = gs.playx*sMapSize;
                aptl[0].y = gs.playy*sMapSize + sMapSize;
                aptl[1].x = gs.playx*sMapSize + sMapSize;
                aptl[1].y = gs.playy*sMapSize + sMapSize*2;
                GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            }
            if (gs.moves & MOVE_UR) {
                aptl[0].x = gs.playx*sMapSize + sMapSize;
                aptl[0].y = gs.playy*sMapSize + sMapSize;
                aptl[1].x = gs.playx*sMapSize + sMapSize*2;
                aptl[1].y = gs.playy*sMapSize + sMapSize*2;
                GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            }
            if (gs.moves & MOVE_L) {
                aptl[0].x = gs.playx*sMapSize - sMapSize;
                aptl[0].y = gs.playy*sMapSize;
                aptl[1].x = gs.playx*sMapSize;
                aptl[1].y = gs.playy*sMapSize + sMapSize;
                GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            }
            if (gs.moves & MOVE_R) {
                aptl[0].x = gs.playx*sMapSize + sMapSize;
                aptl[0].y = gs.playy*sMapSize;
                aptl[1].x = gs.playx*sMapSize + sMapSize*2;
                aptl[1].y = gs.playy*sMapSize + sMapSize;
                GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            }
            if (gs.moves & MOVE_DL) {
                aptl[0].x = gs.playx*sMapSize - sMapSize;
                aptl[0].y = gs.playy*sMapSize - sMapSize;
                aptl[1].x = gs.playx*sMapSize;
                aptl[1].y = gs.playy*sMapSize;
                GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            }
            if (gs.moves & MOVE_D) {
                aptl[0].x = gs.playx*sMapSize;
                aptl[0].y = gs.playy*sMapSize - sMapSize;
                aptl[1].x = gs.playx*sMapSize + sMapSize;
                aptl[1].y = gs.playy*sMapSize;
                GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            }
            if (gs.moves & MOVE_DR) {
                aptl[0].x = gs.playx*sMapSize + sMapSize;
                aptl[0].y = gs.playy*sMapSize - sMapSize;
                aptl[1].x = gs.playx*sMapSize + sMapSize*2;
                aptl[1].y = gs.playy*sMapSize;
                GpiBitBlt(hps, hpsMemory, 3L, aptl, ROP_SRCCOPY, 0);
            }
            WinEndPaint(hps);
            sprintf(szScore, " Score: %i  Level: %i", gs.score, gs.level);
            WinSendMsg(hwndMenu, MM_SETITEMTEXT,
                       MPFROM2SHORT(IDM_SCORE, FALSE), MPFROMP(szScore));
            if (RobotAdjacent(gs))
                WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_WAIT, TRUE),
                           MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
            else
                WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_WAIT, TRUE),
                           MPFROM2SHORT(MIA_DISABLED, 0));
            if (gs.sonic_reset) {
                WinSendMsg(hwndMenu, MM_SETITEMATTR,
                           MPFROM2SHORT(IDM_SONIC, TRUE),
                           MPFROM2SHORT(MIA_DISABLED, 0));
                gs.sonic_reset = FALSE;
            }
            return 0L;

        case WM_SETFOCUS:
            if (!SHORT1FROMMP(mp2)) {
                /* Losing focus */
                if (!bBackgrndRun && gs.active && !bPaused) {
                    bPaused      = TRUE;
                    bFocusPaused = TRUE;
                    if (hwndGameSub)
                        WinCheckMenuItem(hwndGameSub, IDM_PAUSE, TRUE);
                }
            } else {
                /* Gaining focus */
                if (bFocusPaused && gs.active && bPaused) {
                    bPaused = FALSE;
                    if (hwndGameSub)
                        WinCheckMenuItem(hwndGameSub, IDM_PAUSE, FALSE);
                }
                bFocusPaused = FALSE;
            }
            return 0L;

        case WM_MOUSEMOVE:
            icx = MOUSEMSG(&msg)->x;
            icy = MOUSEMSG(&msg)->y;
            WinSetPointer(HWND_DESKTOP, ahptr[Direction(gs, icx, icy, sMapSize)/2]);
            return 0L;

        case WM_BUTTON1UP:
            if (gs.active && !bPaused) {
                icx = MOUSEMSG(&msg)->x;
                icy = MOUSEMSG(&msg)->y;
                MovePlayer(&gs, Direction(gs, icx, icy, sMapSize));
                WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
            }
            return 0L;

        case WM_BUTTON2UP:
            if (gs.active && !bPaused) {
                if (!RobotAdjacent(gs))
                    do {
                        RobotChase(&gs);
                        WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
                        WinUpdateWindow(hwnd);
                    } while (!RobotAdjacent(gs) && !gs.newlevel);
            }
            return 0L;

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {

                case IDM_EXIT:
                case IDM_QUIT:
                    WinSendMsg(hwnd, WM_CLOSE, 0L, 0L);
                    return 0L;

                case IDM_PAUSE:
                    bPaused = !bPaused;
                    if (bPaused) bFocusPaused = FALSE;
                    if (hwndGameSub)
                        WinCheckMenuItem(hwndGameSub, IDM_PAUSE, bPaused);
                    return 0L;

                case IDM_BACKGRND:
                    bBackgrndRun = !bBackgrndRun;
                    if (hwndOpts)
                        WinCheckMenuItem(hwndOpts, IDM_BACKGRND, bBackgrndRun);
                    return 0L;

                case IDM_FRAME_CTRL:
                {
                    HWND hwndFr = WinQueryWindow(hwnd, QW_PARENT);
                    HWND hwndTB = WinWindowFromID(hwndFr, FID_TITLEBAR);
                    HWND hwndSM = WinWindowFromID(hwndFr, FID_SYSMENU);
                    HWND hwndMM = WinWindowFromID(hwndFr, FID_MINMAX);
                    HWND hwndMB = WinWindowFromID(hwndFr, FID_MENU);
                    HWND hwndTarget;

                    bFrameHidden = !bFrameHidden;
                    hwndTarget   = bFrameHidden ? HWND_OBJECT : hwndFr;

                    WinSetParent(hwndTB, hwndTarget, FALSE);
                    WinSetParent(hwndSM, hwndTarget, FALSE);
                    WinSetParent(hwndMM, hwndTarget, FALSE);
                    WinSetParent(hwndMB, hwndTarget, FALSE);

                    WinSendMsg(hwndFr, WM_UPDATEFRAME,
                               (MPARAM)(FCF_TITLEBAR | FCF_SYSMENU |
                                        FCF_MINBUTTON | FCF_MENU), NULL);
                    WinInvalidateRect(hwndFr, NULL, TRUE);
                    WinUpdateWindow(hwndFr);

                    if (hwndOpts)
                        WinCheckMenuItem(hwndOpts, IDM_FRAME_CTRL, bFrameHidden);
                    return 0L;
                }

                case IDM_SAVEONEXIT:
                    saveonexit = !saveonexit;
                    if (hwndOpts)
                        WinCheckMenuItem(hwndOpts, IDM_SAVEONEXIT, saveonexit);
                    return 0L;

                case IDM_LANG_EN:
                case IDM_LANG_ES:
                case IDM_LANG_NL:
                case IDM_LANG_DE:
                case IDM_LANG_FR:
                case IDM_LANG_IT:
                    set_language(hwndMenu,
                                 (int)COMMANDMSG(&msg)->cmd - IDM_LANG_EN);
                    return 0L;

                case IDM_NEW:
                    NewGame(&gs);
                    WinSendMsg(hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT(IDM_SONIC, TRUE),
                               MPFROM2SHORT(MIA_DISABLED, 0));
                    WinSendMsg(hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT(IDM_TELEP, TRUE),
                               MPFROM2SHORT(MIA_DISABLED, 0));
                    WinSendMsg(hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT(IDM_WAIT, TRUE),
                               MPFROM2SHORT(MIA_DISABLED, 0));
                    WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
                    return 0L;

                case IDM_SONIC:
                    if (bPaused) return 0L;
                    hps = WinGetPS(hwnd);
                    rcl.xLeft   = gs.playx*sMapSize - sMapSize;
                    rcl.yBottom = gs.playy*sMapSize - sMapSize;
                    rcl.xRight  = rcl.xLeft + sMapSize*3;
                    rcl.yTop    = rcl.yBottom + sMapSize*3;
                    WinFillRect(hps, &rcl, CLR_WHITE);
                    WinReleasePS(hps);
                    WinSendMsg(hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT(IDM_SONIC, TRUE),
                               MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
                    SonicScrew(&gs);
                    WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
                    return 0L;

                case IDM_TELEP:
                    if (bPaused) return 0L;
                    Teleport(&gs);
                    if (!gs.active) {
                        WinSendMsg(hwndMenu, MM_SETITEMATTR,
                                   MPFROM2SHORT(IDM_SONIC, TRUE),
                                   MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
                        WinSendMsg(hwndMenu, MM_SETITEMATTR,
                                   MPFROM2SHORT(IDM_TELEP, TRUE),
                                   MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
                        WinSendMsg(hwndMenu, MM_SETITEMATTR,
                                   MPFROM2SHORT(IDM_WAIT, TRUE),
                                   MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
                    }
                    WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
                    if (!gs.active && (gs.score > gs.hiscore)) {
                        gs.hiscore = gs.score;
                        WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)GetNameDlgProc,
                                  0, IDD_GETNAME, &gs);
                        SaveHiScores(&hs, &gs);
                        WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)HiScoresDlgProc,
                                  0, IDD_HISCORES, &hs);
                    }
                    return 0L;

                case IDM_WAIT:
                    if (bPaused) return 0L;
                    do {
                        RobotChase(&gs);
                        WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
                        WinUpdateWindow(hwnd);
                    } while (!RobotAdjacent(gs) && !gs.newlevel);
                    return 0L;

                case IDM_ABOUT:
                    WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)AboutDlgProc,
                              0, IDD_ABOUT, NULL);
                    return 0L;

                case IDM_HSCORE:
                    WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)HiScoresDlgProc,
                              0, IDD_HISCORES, &hs);
                    return 0L;

                case IDM_CLEAR:
                    WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)ClearDlgProc,
                              0, IDD_CLEAR, &hs);
                    if (hs.clear) {
                        SaveHiScores(&hs, &gs);
                        WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)HiScoresDlgProc,
                                  0, IDD_HISCORES, &hs);
                    }
                    return 0L;

                case IDM_HELP:
                    WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)HelpDlgProc,
                              0, IDD_HELP, NULL);
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
            WinSetDlgItemText(hwnd, IDE_GETNAME, (PCSZ)pgs->name);
            return 0L;
        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {
                case DID_OK:
                    WinQueryDlgItemText(hwnd, IDE_GETNAME, 20, (PSZ)pgs->name);
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
            for (ici = 0; ici < 10; ici++)
                WinSetDlgItemText(hwnd, IDE_HS + ici, (PCSZ)phs->name[ici]);
            for (ici = 0; ici < 10; ici++) {
                sprintf(sz, "%3i  ", phs->level[ici]);
                WinSetDlgItemText(hwnd, IDE_HS + 10 + ici, (PCSZ)sz);
            }
            for (ici = 0; ici < 10; ici++) {
                sprintf(sz, "%5i ", phs->score[ici]);
                WinSetDlgItemText(hwnd, IDE_HS + 20 + ici, (PCSZ)sz);
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
