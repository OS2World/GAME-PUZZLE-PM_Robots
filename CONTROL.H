/*  CONTROL.H
 *  Presentation Manager Robots v1.3
 *  Copyright (c) 1993,1994,2002 by Kent Lundberg
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details (file GPL.TXT).
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 */

#define ID_RESOURCE     1
#define IDD_ABOUT       2
#define IDD_HELP        3
#define IDD_HISCORES    4
#define IDE_HS          5
#define IDD_GETNAME     6
#define IDE_GETNAME     7
#define IDD_CLEAR       8

#define IDM_GAME        20
#define IDM_NEW         21
#define IDM_HELP        22
#define IDM_QUIT        23
#define IDM_HSCORE      24
#define IDM_ABOUT       25
#define IDM_TELEP       26
#define IDM_WAIT        27
#define IDM_SONIC       28
#define IDM_SCORE       29
#define IDM_CLEAR       30

#define IDB_PLAYER      40
#define IDB_PLAYERD     41
#define IDB_ROBOT       42
#define IDB_HEAP        43
#define IDB_DIAMOND     44
#define IDB_HPLAYER     45
#define IDB_HPLAYERD    46
#define IDB_HROBOT      47
#define IDB_HHEAP       48
#define IDB_HDIAMOND    49

/* The pointer load logic depends on these being bisequential   */
/* The UP DOWN RIGHT LEFT are bits, 6 14 22, non-directional    */
/* These all all offset from 1 because fucking ID_RESOURCE is 1 */
#define IDP_UP          2
#define IDP_DOWN        4
#define IDP_RIGHT       8
#define IDP_LEFT        16
#define IDP_UPRIGHT     10
#define IDP_DOWNRIGHT   12
#define IDP_UPLEFT      18
#define IDP_DOWNLEFT    20
#define IDP_STAY        6
#define IDP_INACTIVE    14
#define IDP_NOMOVE      22

#define EMPTY           0
#define ROBOT           1
#define HEAP            2

/* AvailableMoves bits */
#define MOVE_UL   1 
#define MOVE_U    2
#define MOVE_UR   4
#define MOVE_L    8
#define MOVE_R   16
#define MOVE_DL  32
#define MOVE_D   64
#define MOVE_DR 128
#define MOVE_S  256

typedef struct {
   int playx;
   int playy;
   int field[35][20];
   int score;
   int level;
   int robots;
   int active;
   int sonic_used; 
   int sonic_reset;
   int moves;
   int hiscore;
   int newlevel;
   char name[20];
} GAMESTATE;

typedef struct {
   int score[10];
   int level[10];
   char name[10][20];
   int clear;
} HISCORES;

/* We need these from robots.c to call from game.c in EndGame(); 
MRESULT EXPENTRY GetNameDlgProc(HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY HiScoresDlgProc(HWND, USHORT, MPARAM, MPARAM);  */

/* We need these from game.c to call from robots.c in ClientWndProc; */
void NewGame(GAMESTATE *);
void Teleport(GAMESTATE *);
void SonicScrew(GAMESTATE *);
void RobotChase(GAMESTATE *);
void MovePlayer(GAMESTATE *, int);
int RobotAdjacent(GAMESTATE);
int Direction(GAMESTATE, int, int, int);

/* We need these from hiscore.c to call from robots.c in ClientWndProc(); */
void LoadHiScores(HISCORES *, GAMESTATE *);
void SaveHiScores(HISCORES *, GAMESTATE *);
void ClearHiScores(HISCORES *);

