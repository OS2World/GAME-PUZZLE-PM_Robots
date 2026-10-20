/*  ROBOTS.C
 *  Presentation Manager Robots v1.6
 *  Copyright (c) 1993,1994,2002 by Kent Lundberg
 *
 *  LICENSE: GNU GPL V3
 */

#pragma off(unreferenced)
static const char bldlevel[] =
    "@#Kent Lundberg:1.6#@##1## 20 Oct 2026 20:00:00      "
    "ARCAOS:::0::::@@PM Robots Game for OS/2\r\n\x1a";
#pragma on(unreferenced)

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
        "~New Game\tCtrl+N",
        "~Sonic Screwdriver\tS",
        "~Teleport\tT",
        "~Wait\tW",
        "~High Scores...",
        "~Clear Scores...",
        "E~xit\tCtrl+X",
        "~Options",
        "~Language",
        "~Frame Controls\tCtrl+F",
        "~Help",
        "How to ~Play...",
        "~About...",
        "~Key Mapping...",
        "Key Mapping",
        "Click the box, then click it again with the mouse button to use.",
        "Move toward the pointer",
        "Wait for the robots",
        "Left button",
        "Right button",
        "Middle button",
        "Default",
        "OK",
        "Cancel",
        "Click the box, then press the key to use.",
        "Wait",
        "Sonic Screwdriver",
        "Teleport",
        "~Save settings on exit",
    },

    /* LANG_ES */
    {
        "~Juego",
        "~Nuevo Juego\tCtrl+N",
        "~Destornillador Sonico\tS",
        "~Teleportar\tT",
        "~Esperar\tW",
        "~Mejores Puntos...",
        "~Borrar Puntos...",
        "~Salir\tCtrl+X",
        "~Opciones",
        "~Idioma",
        "~Controles Marco\tCtrl+F",
        "~Ayuda",
        "~Como Jugar...",
        "~About...",
        "Asignacion de ~teclas...",
        "Asignacion de teclas",
        "Haga clic en el cuadro y de nuevo con el boton del raton que quiera usar.",
        "Mover hacia el puntero",
        "Esperar a los robots",
        "Boton izquierdo",
        "Boton derecho",
        "Boton central",
        "Por defecto",
        "Aceptar",
        "Cancelar",
        "Haga clic en el cuadro y pulse la tecla que quiera usar.",
        "Esperar",
        "Destornillador Sonico",
        "Teleportar",
        "~Guardar ajustes al salir",
    },

    /* LANG_NL */
    {
        "~Spel",
        "~Nieuw Spel\tCtrl+N",
        "~Sonische Schroevendraaier\tS",
        "~Teleporteren\tT",
        "~Wachten\tW",
        "~Topscores...",
        "~Wis Scores...",
        "A~fsluiten\tCtrl+X",
        "~Opties",
        "~Taal",
        "~Raambesturing\tCtrl+F",
        "~Hulp",
        "~Hoe te Spelen...",
        "~About...",
        "Toetsen toe~wijzen...",
        "Toetsen toewijzen",
        "Klik op het vak en klik er nogmaals op met de muisknop die u wilt gebruiken.",
        "Naar de aanwijzer bewegen",
        "Op de robots wachten",
        "Linkerknop",
        "Rechterknop",
        "Middelste knop",
        "Standaard",
        "OK",
        "Annuleren",
        "Klik op het vak en druk op de toets die u wilt gebruiken.",
        "Wachten",
        "Sonische Schroevendraaier",
        "Teleporteren",
        "~Instellingen bewaren bij afsluiten",
    },

    /* LANG_DE */
    {
        "~Spiel",
        "~Neues Spiel\tCtrl+N",
        "~Schraubenzieher\tS",
        "~Teleportieren\tT",
        "~Warten\tW",
        "~Bestenliste...",
        "Liste ~loeschen...",
        "~Beenden\tCtrl+X",
        "~Optionen",
        "~Sprache",
        "~Rahmensteuerung\tCtrl+F",
        "~Hilfe",
        "~Spielanleitung...",
        "~About...",
        "~Tastenzuordnung...",
        "Tastenzuordnung",
        "Klicken Sie auf das Feld und dann erneut mit der gewuenschten Maustaste.",
        "Zum Zeiger bewegen",
        "Auf die Roboter warten",
        "Linke Taste",
        "Rechte Taste",
        "Mittlere Taste",
        "Standard",
        "OK",
        "Abbrechen",
        "Klicken Sie auf das Feld und druecken Sie die gewuenschte Taste.",
        "Warten",
        "Schraubenzieher",
        "Teleportieren",
        "~Einstellungen beim Beenden speichern",
    },

    /* LANG_FR */
    {
        "~Jeu",
        "~Nouveau Jeu\tCtrl+N",
        "~Tournevis Sonique\tS",
        "~Teleporter\tT",
        "~Attendre\tW",
        "~Meilleurs Scores...",
        "~Effacer Scores...",
        "~Quitter\tCtrl+X",
        "~Options",
        "~Langue",
        "~Controles Cadre\tCtrl+F",
        "~Aide",
        "~Comment Jouer...",
        "~About...",
        "Affectation des ~touches...",
        "Affectation des touches",
        "Cliquez sur la case, puis cliquez dessus avec le bouton de souris voulu.",
        "Aller vers le pointeur",
        "Attendre les robots",
        "Bouton gauche",
        "Bouton droit",
        "Bouton central",
        "Defaut",
        "OK",
        "Annuler",
        "Cliquez sur la case, puis appuyez sur la touche voulue.",
        "Attendre",
        "Tournevis Sonique",
        "Teleporter",
        "~Sauvegarder a la fermeture",
    },

    /* LANG_IT */
    {
        "~Gioco",
        "~Nuovo Gioco\tCtrl+N",
        "~Cacciavite Sonico\tS",
        "~Teleportare\tT",
        "~Aspettare\tW",
        "~Punteggi...",
        "~Cancella Punteggi...",
        "~Esci\tCtrl+X",
        "~Opzioni",
        "~Lingua",
        "~Controlli Cornice\tCtrl+F",
        "~Aiuto",
        "~Come Giocare...",
        "~About...",
        "Assegnazione ~tasti...",
        "Assegnazione tasti",
        "Fai clic sulla casella, poi di nuovo con il pulsante del mouse da usare.",
        "Muovere verso il puntatore",
        "Aspettare i robot",
        "Pulsante sinistro",
        "Pulsante destro",
        "Pulsante centrale",
        "Predefinito",
        "OK",
        "Annulla",
        "Fai clic sulla casella, poi premi il tasto da usare.",
        "Aspettare",
        "Cacciavite Sonico",
        "Teleportare",
        "~Salva alla chiusura",
    },

};

/* ------------------------------------------------------------------ */
/* Global state                                                        */
/* ------------------------------------------------------------------ */
HAB     hab;
int     sMapSize = 16;

static BOOL bFrameHidden = FALSE;
static BOOL bSaveOnExit  = TRUE;     /* "Save settings on exit": on by default */

/* Mouse buttons (1 left, 2 right, 3 middle) of the two actions (Key Mapping) */
#define KM_MOVE   0      /* move toward the pointer */
#define KM_WAIT   1      /* wait for the robots     */
#define KM_NACT   2
static int g_btn[KM_NACT] = { 1, 2 };

/* Keys of the three keyboard actions (Key Mapping): an upper-case character
   code, or 0x100 | virtual key for F-keys, arrows, etc. */
#define KK_WAIT   0
#define KK_SONIC  1
#define KK_TELEP  2
#define KK_NACT   3
static const int kkDefault[KK_NACT] = { 'W', 'S', 'T' };
static int g_key[KK_NACT]           = { 'W', 'S', 'T' };

/* Key code of a WM_CHAR key-down without Ctrl/Alt, or 0 if not mappable */
static int KeyOfChar(MPARAM mp1, MPARAM mp2) {
    USHORT fs = SHORT1FROMMP(mp1);
    USHORT ch = SHORT1FROMMP(mp2);
    USHORT vk = SHORT2FROMMP(mp2);

    if (fs & (KC_KEYUP | KC_CTRL | KC_ALT)) return 0;
    if ((fs & KC_CHAR) && ch >= 32 && ch < 127)
        return (ch >= 'a' && ch <= 'z') ? ch - 32 : ch;
    if (fs & KC_VIRTUALKEY) {
        if ((vk >= VK_F1 && vk <= VK_F24) ||
            vk == VK_LEFT || vk == VK_RIGHT || vk == VK_UP || vk == VK_DOWN ||
            vk == VK_PAGEUP || vk == VK_PAGEDOWN || vk == VK_HOME ||
            vk == VK_END || vk == VK_INSERT || vk == VK_DELETE)
            return 0x100 | vk;
    }
    return 0;
}

static void KeyName(int k, char *out) {
    static const struct { int vk; const char *n; } t[] = {
        { VK_LEFT, "Left" }, { VK_RIGHT, "Right" }, { VK_UP, "Up" }, { VK_DOWN, "Down" },
        { VK_PAGEUP, "PgUp" }, { VK_PAGEDOWN, "PgDn" }, { VK_HOME, "Home" },
        { VK_END, "End" }, { VK_INSERT, "Ins" }, { VK_DELETE, "Del" } };
    int i;

    if (k & 0x100) {
        k &= 0xFF;
        if (k >= VK_F1 && k <= VK_F24) { sprintf(out, "F%d", k - VK_F1 + 1); return; }
        for (i = 0; i < (int)(sizeof(t) / sizeof(t[0])); i++)
            if (t[i].vk == k) { strcpy(out, t[i].n); return; }
        strcpy(out, "?");
    } else if (k == ' ') {
        strcpy(out, "Space");
    } else {
        out[0] = (char)k; out[1] = 0;
    }
}

/* Menu text with the mapped key after the tab */
static void SetMenuKeyText(HWND hMenu, USHORT id, int str, int act) {
    char buf[64];
    char *p;

    strncpy(buf, tr(str), 40);
    buf[40] = 0;
    p = strchr(buf, '\t');
    if (p) *p = 0;
    strcat(buf, "\t");
    KeyName(g_key[act], buf + strlen(buf));
    WinSendMsg(hMenu, MM_SETITEMTEXT, MPFROMSHORT(id), MPFROMP(buf));
}

/* ------------------------------------------------------------------ */
/* Forward declarations                                                */
/* ------------------------------------------------------------------ */
MRESULT EXPENTRY ClientWndProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY HelpDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY ClearDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY HiScoresDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY GetNameDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY KeyMapDlgProc(HWND, ULONG, MPARAM, MPARAM);

void load_settings(void);
void save_settings(void);
void set_language(HWND hwndMenu, int lang);

/* ------------------------------------------------------------------ */
/* Settings persistence                                                */
/* ------------------------------------------------------------------ */
void load_settings(void) {
    FILE *f = fopen("robots.cfg", "rb");
    if (f) {
        int b[KM_NACT];
        fread(&current_lang, sizeof(int), 1, f);
        if (fread(b, sizeof(int), KM_NACT, f) == KM_NACT &&
            b[0] >= 1 && b[0] <= 3 && b[1] >= 1 && b[1] <= 3 && b[0] != b[1]) {
            g_btn[KM_MOVE] = b[0];
            g_btn[KM_WAIT] = b[1];
            {
                int k[KK_NACT];
                if (fread(k, sizeof(int), KK_NACT, f) == KK_NACT &&
                    k[0] != k[1] && k[0] != k[2] && k[1] != k[2] &&
                    k[0] > 0 && k[1] > 0 && k[2] > 0) {
                    g_key[0] = k[0]; g_key[1] = k[1]; g_key[2] = k[2];
                }
                if (fread(&k[0], sizeof(int), 1, f) == 1 && (k[0] == 0 || k[0] == 1))
                    bSaveOnExit = (BOOL)k[0];
            }
        }
        fclose(f);
        if (current_lang < 0 || current_lang >= LANG_COUNT) current_lang = 0;
    }
}

void save_settings(void) {
    FILE *f = fopen("robots.cfg", "wb");
    if (f) {
        fwrite(&current_lang, sizeof(int), 1, f);
        fwrite(g_btn, sizeof(int), KM_NACT, f);
        fwrite(g_key, sizeof(int), KK_NACT, f);
        { int sv = (int)bSaveOnExit; fwrite(&sv, sizeof(int), 1, f); }
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
                   MPFROMSHORT(IDM_NEW),   MPFROMP(tr(STR_MENU_NEW)));
        SetMenuKeyText(hwndGame, IDM_SONIC, STR_MENU_SONIC, KK_SONIC);
        SetMenuKeyText(hwndGame, IDM_TELEP, STR_MENU_TELEP, KK_TELEP);
        SetMenuKeyText(hwndGame, IDM_WAIT,  STR_MENU_WAIT,  KK_WAIT);
        WinSendMsg(hwndGame, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_EXIT),  MPFROMP(tr(STR_MENU_EXIT)));
    }

    /* Update Options submenu */
    if (hwndOpts) {
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_KEYMAP),       MPFROMP(tr(STR_MENU_KEYMAP)));
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_SUBMENU_LANG), MPFROMP(tr(STR_MENU_LANGUAGE)));
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_HSCORE),       MPFROMP(tr(STR_MENU_HSCORES)));
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_CLEAR),        MPFROMP(tr(STR_MENU_CLEAR)));
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_FRAME_CTRL),   MPFROMP(tr(STR_MENU_FRAME)));
        WinSendMsg(hwndOpts, MM_SETITEMTEXT,
                   MPFROMSHORT(IDM_SAVEONEXIT),   MPFROMP(tr(STR_MENU_SAVEONEXIT)));
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
    WinRegisterClass(hab, (PSZ)szClassName, (PFNWP)ClientWndProc, CS_SIZEREDRAW, 0UL);

    hwndFrame = WinCreateStdWindow(HWND_DESKTOP, 0L, &flStyle,
                                   (PSZ)szClassName, (PSZ)"",
                                   0L, 0UL, ID_RESOURCE, &hwndClient);
    WinSetWindowText(hwndFrame, (PSZ)szWinTitle);

    /* Size window to map grid and center on desktop */
    {
        LONG cyTitle  = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
        LONG cyMenu   = WinQuerySysValue(HWND_DESKTOP, SV_CYMENU);
        LONG cyBorder = WinQuerySysValue(HWND_DESKTOP, SV_CYBORDER);
        LONG cxBorder = WinQuerySysValue(HWND_DESKTOP, SV_CXBORDER);
        winW = (LONG)sMapSize * 35L + 2L * cxBorder;
        winH = (LONG)sMapSize * 20L + cyTitle + cyMenu + 2L * cyBorder;
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
    static HWND hwndFrCache, hwndTB, hwndSM, hwndMM;
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
            hwndFrCache = WinQueryWindow(hwnd, QW_PARENT);
            hwndMenu = WinWindowFromID(hwndFrCache, FID_MENU);
            hwndTB   = WinWindowFromID(hwndFrCache, FID_TITLEBAR);
            hwndSM   = WinWindowFromID(hwndFrCache, FID_SYSMENU);
            hwndMM   = WinWindowFromID(hwndFrCache, FID_MINMAX);
            hdcMemory = DevOpenDC(hab, OD_MEMORY, (PSZ)"*", 0, NULL, NULLHANDLE);
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

            /* Cache Options submenu handle for checkmarks */
            memset(&mi, 0, sizeof(mi));
            WinSendMsg(hwndMenu, MM_QUERYITEM,
                       MPFROM2SHORT(IDM_SUBMENU_OPTS, TRUE), MPFROMP(&mi));
            hwndOpts = mi.hwndSubMenu;

            /* Right-justify the Score item in the menu bar */
            memset(&mi, 0, sizeof(mi));
            WinSendMsg(hwndMenu, MM_QUERYITEM,
                       MPFROM2SHORT(IDM_SCORE, FALSE), MPFROMP(&mi));
            mi.afStyle |= 0x4000;   /* MIS_RIGHTJUSTIFY */
            WinSendMsg(hwndMenu, MM_SETITEM,
                       MPFROM2SHORT(IDM_SCORE, FALSE), MPFROMP(&mi));

            /* Restore persisted checkmarks */
            if (hwndOpts) {
                WinCheckMenuItem(hwndOpts, IDM_FRAME_CTRL, bFrameHidden);
                WinCheckMenuItem(hwndOpts, IDM_SAVEONEXIT, bSaveOnExit);
            }

            LoadHiScores(&hs, &gs);
            NewGame(&gs);
            return 0L;
        }

        case WM_DESTROY:
            if (bSaveOnExit) save_settings();
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

        case WM_MOUSEMOVE:
            icx = MOUSEMSG(&msg)->x;
            icy = MOUSEMSG(&msg)->y;
            WinSetPointer(HWND_DESKTOP, ahptr[Direction(gs, icx, icy, sMapSize)/2]);
            return 0L;

        case WM_CHAR:
        {
            int key = KeyOfChar(mp1, mp2);
            int a, id = 0;

            if (key) {
                for (a = 0; a < KK_NACT; a++)
                    if (g_key[a] == key)
                        id = (a == KK_WAIT) ? IDM_WAIT : (a == KK_SONIC) ? IDM_SONIC : IDM_TELEP;
                if (id) {
                    /* a disabled menu item disables its key too */
                    ULONG at = (ULONG)WinSendMsg(hwndMenu, MM_QUERYITEMATTR,
                                   MPFROM2SHORT(id, TRUE), MPFROMSHORT(MIA_DISABLED));
                    if (!(at & MIA_DISABLED))
                        WinSendMsg(hwnd, WM_COMMAND, MPFROMSHORT(id),
                                   MPFROM2SHORT(CMDSRC_ACCELERATOR, FALSE));
                    return (MRESULT)TRUE;
                }
            }
            break;
        }

        case WM_BUTTON1UP:
        case WM_BUTTON2UP:
        case WM_BUTTON3UP:
        {
            int iBtn = (msg == WM_BUTTON1UP) ? 1 : (msg == WM_BUTTON2UP) ? 2 : 3;

            if (iBtn == g_btn[KM_MOVE] && gs.active) {
                icx = MOUSEMSG(&msg)->x;
                icy = MOUSEMSG(&msg)->y;
                MovePlayer(&gs, Direction(gs, icx, icy, sMapSize));
                WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
            }
            else if (iBtn == g_btn[KM_WAIT] && gs.active) {
                if (!RobotAdjacent(gs))
                    do {
                        RobotChase(&gs);
                        WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
                        WinUpdateWindow(hwnd);
                    } while (!RobotAdjacent(gs) && !gs.newlevel);
            }
            return 0L;
        }

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {

                case IDM_EXIT:
                case IDM_QUIT:
                    WinSendMsg(hwnd, WM_CLOSE, 0L, 0L);
                    return 0L;

                case IDM_FRAME_CTRL:
                {
                    HWND hwndTarget;
                    bFrameHidden = !bFrameHidden;
                    hwndTarget   = bFrameHidden ? HWND_OBJECT : hwndFrCache;
                    WinSetParent(hwndTB,   hwndTarget, FALSE);
                    WinSetParent(hwndSM,   hwndTarget, FALSE);
                    WinSetParent(hwndMM,   hwndTarget, FALSE);
                    WinSetParent(hwndMenu, hwndTarget, FALSE);
                    WinSendMsg(hwndFrCache, WM_UPDATEFRAME,
                               (MPARAM)(FCF_TITLEBAR | FCF_SYSMENU |
                                        FCF_MINBUTTON | FCF_MENU), NULL);
                    WinInvalidateRect(hwndFrCache, NULL, TRUE);
                    WinUpdateWindow(hwndFrCache);
                    if (hwndOpts)
                        WinCheckMenuItem(hwndOpts, IDM_FRAME_CTRL, bFrameHidden);
                    return 0L;
                }

                case IDM_SAVEONEXIT:
                    bSaveOnExit = !bSaveOnExit;
                    if (hwndOpts)
                        WinCheckMenuItem(hwndOpts, IDM_SAVEONEXIT, bSaveOnExit);
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
                    do {
                        RobotChase(&gs);
                        WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
                        WinUpdateWindow(hwnd);
                    } while (!RobotAdjacent(gs) && !gs.newlevel);
                    return 0L;

                case IDM_KEYMAP:
                    if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)KeyMapDlgProc,
                                  0, IDD_KEYMAP, NULL) == TRUE)
                        set_language(hwndMenu, current_lang);
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


/* ------------------------------------------------------------------ */
/* Key Mapping dialog (mouse buttons)                                  */
/* ------------------------------------------------------------------ */
#define WM_KM_INPUT   (WM_USER + 2)

#define KM_ROWS  (KM_NACT + KK_NACT)     /* rows 0..1 mouse, 2..4 keyboard */

static PFNWP pfnKmEntry[KM_ROWS];

/* Capture box: mouse rows - the first click only focuses it, a click on the
   focused box assigns the button used; keyboard rows - a key press assigns
   the key */
static MRESULT EXPENTRY KmEntryProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
    int   row = (int)WinQueryWindowUShort(hwnd, QWS_ID) - IDC_KM_ENTRY;
    SHORT btn = 0;

    switch (msg) {
        case WM_BUTTON1DOWN: if (row < KM_NACT) btn = 1; break;
        case WM_BUTTON2DOWN: if (row < KM_NACT) btn = 2; break;
        case WM_BUTTON3DOWN: if (row < KM_NACT) btn = 3; break;
        case WM_CHAR:
            if ((SHORT1FROMMP(mp1) & KC_VIRTUALKEY) &&
                (SHORT2FROMMP(mp2) == VK_TAB   || SHORT2FROMMP(mp2) == VK_BACKTAB ||
                 SHORT2FROMMP(mp2) == VK_ENTER || SHORT2FROMMP(mp2) == VK_NEWLINE ||
                 SHORT2FROMMP(mp2) == VK_ESC))
                break;                          /* the dialog handles these */
            if (row >= KM_NACT) {
                int key = KeyOfChar(mp1, mp2);
                if (key)
                    WinPostMsg(WinQueryWindow(hwnd, QW_PARENT), WM_KM_INPUT,
                               MPFROMLONG(row), MPFROMLONG(key));
            }
            return (MRESULT)TRUE;               /* swallow every other key  */
    }
    if (btn && WinQueryFocus(HWND_DESKTOP) == hwnd) {
        WinPostMsg(WinQueryWindow(hwnd, QW_PARENT), WM_KM_INPUT,
                   MPFROMLONG(row), MPFROMLONG(btn));
        return (MRESULT)TRUE;
    }
    return pfnKmEntry[row](hwnd, msg, mp1, mp2);
}

static void km_show(HWND hwnd, const int *btn, const int *key) {
    int  i;
    char sz[16];

    for (i = 0; i < KM_NACT; i++)
        WinSetDlgItemText(hwnd, IDC_KM_ENTRY + i, (PSZ)tr(STR_KM_BTN1 + btn[i] - 1));
    for (i = 0; i < KK_NACT; i++) {
        KeyName(key[i], sz);
        WinSetDlgItemText(hwnd, IDC_KM_ENTRY + KM_NACT + i, (PSZ)sz);
    }
}

MRESULT EXPENTRY KeyMapDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
    static int tmp[KM_NACT];
    static int tmpk[KK_NACT];
    int i;

    switch (msg) {
        case WM_INITDLG:
            for (i = 0; i < KM_NACT; i++) tmp[i]  = g_btn[i];
            for (i = 0; i < KK_NACT; i++) tmpk[i] = g_key[i];
            WinSetWindowText(hwnd, (PSZ)tr(STR_KM_TITLE));
            WinSetDlgItemText(hwnd, IDC_KM_HINT,      (PSZ)tr(STR_KM_HINT));
            WinSetDlgItemText(hwnd, IDC_KM_HINTK,     (PSZ)tr(STR_KM_HINTK));
            for (i = 0; i < KM_ROWS; i++)
                WinSetDlgItemText(hwnd, IDC_KM_LABEL + i, (PSZ)tr(i < KM_NACT ? STR_KM_ACT0 + i : STR_KM_ACT2 + i - KM_NACT));
            WinSetDlgItemText(hwnd, DID_OK,           (PSZ)tr(STR_DLG_OK));
            WinSetDlgItemText(hwnd, DID_CANCEL,       (PSZ)tr(STR_DLG_CANCEL));
            WinSetDlgItemText(hwnd, IDC_KM_DEFAULT,   (PSZ)tr(STR_KM_DEFAULT));
            for (i = 0; i < KM_ROWS; i++)
                pfnKmEntry[i] = WinSubclassWindow(WinWindowFromID(hwnd, IDC_KM_ENTRY + i), KmEntryProc);
            km_show(hwnd, tmp, tmpk);
            return (MRESULT)FALSE;

        case WM_KM_INPUT: {
            int row = (int)LONGFROMMP(mp1);
            int val = (int)LONGFROMMP(mp2);
            int r;

            if (row < KM_NACT) {                /* mouse: the two actions keep different buttons (swap) */
                int other = 1 - row;
                if (tmp[other] == val) tmp[other] = tmp[row];
                tmp[row] = val;
            } else {                            /* keyboard: the three actions keep different keys (swap) */
                row -= KM_NACT;
                for (r = 0; r < KK_NACT; r++)
                    if (r != row && tmpk[r] == val) tmpk[r] = tmpk[row];
                tmpk[row] = val;
            }
            km_show(hwnd, tmp, tmpk);
            return 0L;
        }

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {
                case DID_OK:
                    for (i = 0; i < KM_NACT; i++) g_btn[i] = tmp[i];
                    for (i = 0; i < KK_NACT; i++) g_key[i] = tmpk[i];
                    WinDismissDlg(hwnd, TRUE);
                    return 0L;
                case DID_CANCEL:
                    WinDismissDlg(hwnd, FALSE);
                    return 0L;
                case IDC_KM_DEFAULT:
                    tmp[KM_MOVE] = 1;  tmp[KM_WAIT] = 2;
                    for (i = 0; i < KK_NACT; i++) tmpk[i] = kkDefault[i];
                    km_show(hwnd, tmp, tmpk);
                    return 0L;
            }
            break;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
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
            WinSetDlgItemText(hwnd, IDE_GETNAME, pgs->name);
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
                WinSetDlgItemText(hwnd, IDE_HS + ici, phs->name[ici]);
            for (ici = 0; ici < 10; ici++) {
                sprintf(sz, "%3i  ", phs->level[ici]);
                WinSetDlgItemText(hwnd, IDE_HS + 10 + ici, sz);
            }
            for (ici = 0; ici < 10; ici++) {
                sprintf(sz, "%5i ", phs->score[ici]);
                WinSetDlgItemText(hwnd, IDE_HS + 20 + ici, sz);
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
