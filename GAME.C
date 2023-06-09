/*  GAME.C
 *  Presentation Manager Robots v1.4
 *  Copyright (c) 1993,1994,2002 by Kent Lundberg
 *
 *  LICENSE: GNU GPL V3
 */

#define TRUE 1
#define FALSE 0
#include <stdlib.h>
#include "control.h"

void NewLevel(GAMESTATE *);
int RobotAdjacent(GAMESTATE);
int AvailableMoves(GAMESTATE);

void NewGame(GAMESTATE *pgs) {
   pgs->score = 0;
   pgs->level = 0;
   pgs->active = TRUE;
   pgs->sonic_used = TRUE;
   pgs->sonic_reset = FALSE;
   NewLevel(pgs);
}

void EndGame(GAMESTATE *pgs) {
   pgs->active = FALSE;
   pgs->moves = 0;
   if (pgs->score > pgs->hiscore) {
   }
}

void NewLevel(GAMESTATE *pgs) {
   int icx, icy, icrobots;

   pgs->newlevel = TRUE;
   pgs->playx = rand()%35;
   pgs->playy = rand()%20;
   if (!pgs->sonic_used)
      pgs->score += 50;
   else {
      pgs->sonic_used = FALSE;
      pgs->sonic_reset = TRUE;
   }
   pgs->robots = icrobots = (++(pgs->level))*5;
   for (icx = 0 ; icx < 35 ; icx++)
      for (icy = 0 ; icy < 20 ; icy++) {
         pgs->field[icx][icy] = EMPTY;
   }
   pgs->field[pgs->playx][pgs->playy] = ROBOT;
   while(icrobots--) {
      icx = rand()%35;
      icy = rand()%20;
      if (pgs->field[icx][icy] == ROBOT) icrobots++;
      pgs->field[icx][icy] = ROBOT;
   }
   pgs->field[pgs->playx][pgs->playy] = EMPTY;
   pgs->moves = AvailableMoves(*pgs);
}

void Teleport(GAMESTATE *pgs) {
   do {
      pgs->playx = rand()%35;
      pgs->playy = rand()%20;
   } while (pgs->field[pgs->playx][pgs->playy] != EMPTY) ;
   if (RobotAdjacent(*pgs))
      EndGame(pgs);
   else {
      pgs->score += pgs->robots;
      RobotChase(pgs);
   }
}

void SonicScrew(GAMESTATE *pgs) {
   int icx, icy, istartx, istarty, iendx, iendy;

   istartx = pgs->playx - 1;
   istarty = pgs->playy - 1;
   iendx = pgs->playx + 1;
   iendy = pgs->playy + 1;
   if (istartx == -1) istartx = 0;
   if (istarty == -1) istarty = 0;
   if (iendx == 35) iendx = 34;
   if (iendy == 20) iendy = 19;
   pgs->sonic_used = TRUE;
   for (icx = istartx ; icx <= iendx ; icx++ )
      for (icy = istarty ; icy <= iendy ; icy++ )
         if (pgs->field[icx][icy] == ROBOT) {
             pgs->field[icx][icy] = EMPTY;
             pgs->robots--;
             pgs->score++;
         }
   RobotChase(pgs);
}

void RobotChase(GAMESTATE *pgs) {
   GAMESTATE old;
   int icx, icy, icxnew, icynew;

   pgs->newlevel = FALSE;
   for (icx = 0 ; icx < 35 ; icx++)
      for (icy = 0 ; icy < 20 ; icy++) {
         old.field[icx][icy] = pgs->field[icx][icy];
         if (pgs->field[icx][icy] != HEAP)
            pgs->field[icx][icy] = EMPTY;
      }
   for (icx = 0 ; icx < 35 ; icx++)
      for (icy = 0 ; icy < 20 ; icy++)
         if (old.field[icx][icy] == ROBOT) {
            if (icx > pgs->playx )
               icxnew = icx - 1;
            else if (icx < pgs->playx )
               icxnew = icx + 1;
            else
               icxnew = icx;
            if (icy > pgs->playy )
               icynew = icy - 1;
            else if (icy < pgs->playy )
               icynew = icy + 1;
            else
               icynew = icy;
            if (pgs->field[icxnew][icynew] == ROBOT) {
               pgs->field[icxnew][icynew] = HEAP;
               pgs->score += 20;
               pgs->robots -= 2;
            }
            else if (pgs->field[icxnew][icynew] == HEAP) {
               pgs->score += 10;
               pgs->robots -= 1;
            }
            else
               pgs->field[icxnew][icynew] = ROBOT;
         }
   if (pgs->robots == 0) NewLevel(pgs);
   pgs->moves = AvailableMoves(*pgs);
}

void MovePlayer(GAMESTATE *pgs, int direction) {
   switch (direction) {
      case IDP_NOMOVE:
         return;
      case IDP_STAY:
         RobotChase(pgs);
         return;
      case IDP_UP:
         pgs->playy++;
         RobotChase(pgs);
         return;
      case IDP_DOWN:
         pgs->playy--;
         RobotChase(pgs);
         return;
      case IDP_LEFT:
         pgs->playx--;
         RobotChase(pgs);
         return;
      case IDP_RIGHT:
         pgs->playx++;
         RobotChase(pgs);
         return;
      case IDP_UPRIGHT:
         pgs->playx++;
         pgs->playy++;
         RobotChase(pgs);
         return;
      case IDP_UPLEFT:
         pgs->playx--;
         pgs->playy++;
         RobotChase(pgs);
         return;
      case IDP_DOWNRIGHT:
         pgs->playx++;
         pgs->playy--;
         RobotChase(pgs);
         return;
      case IDP_DOWNLEFT:
         pgs->playx--;
         pgs->playy--;
         RobotChase(pgs);
         return;
   }
}

int RobotAdjacent(GAMESTATE gs) {
   if (((gs.playx != 0 )&&(gs.field[gs.playx-1][gs.playy] == ROBOT))
       || ((gs.playx != 34)&&(gs.field[gs.playx+1][gs.playy] == ROBOT))
       || ((gs.playy != 0 )&&(gs.field[gs.playx][gs.playy-1] == ROBOT))
       || ((gs.playy != 19)&&(gs.field[gs.playx][gs.playy+1] == ROBOT))
       || ((gs.playx != 0 )&&(gs.playy != 0 )&&(gs.field[gs.playx-1][gs.playy-1] == ROBOT))
       || ((gs.playx != 0 )&&(gs.playy != 19)&&(gs.field[gs.playx-1][gs.playy+1] == ROBOT))
       || ((gs.playx != 34)&&(gs.playy != 0 )&&(gs.field[gs.playx+1][gs.playy-1] == ROBOT))
       || ((gs.playx != 34)&&(gs.playy != 19)&&(gs.field[gs.playx+1][gs.playy+1] == ROBOT)))
      return TRUE;
   else
      return FALSE;
}

int Direction(GAMESTATE gs, int cx, int cy, int sMapSize) {
   int result,
       cpx = cx - sMapSize*gs.playx - sMapSize/2,
       cpy = cy - sMapSize*gs.playy - sMapSize/2;

   if ( cpx*cpx + cpy*cpy < 256)
      return (gs.moves&MOVE_S ? IDP_STAY : IDP_NOMOVE);
   result = 0;
   if ( cpy > abs(cpx/2) )
      result |= IDP_UP;
   else if ( cpy < -abs(cpx/2) )
      result |= IDP_DOWN;
   if ( cpx > abs(cpy/2) )
      result |= IDP_RIGHT;
   else if ( cpx < -abs(cpy/2) )
      result |= IDP_LEFT;
   switch (result) {
      case IDP_UP:
         return (gs.moves&MOVE_U ? IDP_UP : IDP_NOMOVE);
      case IDP_DOWN:
         return (gs.moves&MOVE_D ? IDP_DOWN : IDP_NOMOVE);
      case IDP_LEFT:
         return (gs.moves&MOVE_L ? IDP_LEFT : IDP_NOMOVE);
      case IDP_RIGHT:
         return (gs.moves&MOVE_R ? IDP_RIGHT : IDP_NOMOVE);
      case IDP_UPLEFT:
         return (gs.moves&MOVE_UL ? IDP_UPLEFT : IDP_NOMOVE);
      case IDP_UPRIGHT:
         return (gs.moves&MOVE_UR ? IDP_UPRIGHT : IDP_NOMOVE);
      case IDP_DOWNLEFT:
         return (gs.moves&MOVE_DL ? IDP_DOWNLEFT : IDP_NOMOVE);
      case IDP_DOWNRIGHT:
         return (gs.moves&MOVE_DR ? IDP_DOWNRIGHT : IDP_NOMOVE);
   }
   return 0;
}

int AvailableMoves(GAMESTATE gs) {
   int result = 511;

   if (gs.playx == 0)
      result &= ~(MOVE_UL | MOVE_L | MOVE_DL);
   if (gs.playx == 34)
      result &= ~(MOVE_UR | MOVE_R | MOVE_DR);
   if (gs.playy == 0)
      result &= ~(MOVE_DL | MOVE_D | MOVE_DR);
   if (gs.playy == 19)
      result &= ~(MOVE_UL | MOVE_U | MOVE_UR);
   if ((gs.playx > 1)&&(gs.playy < 18))
      if (gs.field[gs.playx-2][gs.playy+2] == ROBOT)
         result &= ~MOVE_UL;
   if (gs.playy < 18) {
      if (gs.playx > 0)
         if (gs.field[gs.playx-1][gs.playy+2] == ROBOT)
            result &= ~(MOVE_UL | MOVE_U);
      if (gs.field[gs.playx][gs.playy+2] == ROBOT)
         result &= ~(MOVE_UL | MOVE_U | MOVE_UR);
      if (gs.playx < 34)
         if (gs.field[gs.playx+1][gs.playy+2] == ROBOT)
            result &= ~(MOVE_U | MOVE_UR); }
   if ((gs.playx < 33)&&(gs.playy < 18))
      if (gs.field[gs.playx+2][gs.playy+2] == ROBOT)
         result &= ~MOVE_UR;
   if (gs.playy < 19) {
      if (gs.playx > 1)
         if (gs.field[gs.playx-2][gs.playy+1] == ROBOT)
            result &= ~(MOVE_L | MOVE_UL);
      if (gs.playx > 0)
         if (gs.field[gs.playx-1][gs.playy+1] == ROBOT)
            result &= ~(MOVE_L | MOVE_UL | MOVE_U | MOVE_S);
      if (gs.field[gs.playx][gs.playy+1] == ROBOT)
         result &= ~(MOVE_L | MOVE_UL | MOVE_U | MOVE_UR | MOVE_R | MOVE_S);
      if (gs.playx < 34)
         if (gs.field[gs.playx+1][gs.playy+1] == ROBOT)
            result &= ~(MOVE_U | MOVE_UR | MOVE_R | MOVE_S);
      if (gs.playx < 33)
         if (gs.field[gs.playx+2][gs.playy+1] == ROBOT)
            result &= ~(MOVE_UR | MOVE_R); }
   if (gs.playx > 1)
      if (gs.field[gs.playx-2][gs.playy] == ROBOT)
         result &= ~(MOVE_L | MOVE_UL | MOVE_DL);
   if (gs.playx > 0)
      if (gs.field[gs.playx-1][gs.playy] == ROBOT)
         result &= ~(MOVE_L | MOVE_UL | MOVE_DL | MOVE_U | MOVE_D | MOVE_S);
   if (gs.playx < 34)
      if (gs.field[gs.playx+1][gs.playy] == ROBOT)
         result &= ~(MOVE_U | MOVE_D | MOVE_DR | MOVE_UR | MOVE_R | MOVE_S);
   if (gs.playx < 33)
      if (gs.field[gs.playx+2][gs.playy] == ROBOT)
         result &= ~(MOVE_UR | MOVE_R | MOVE_DR);
   if (gs.playy > 0) {
      if (gs.playx > 1)
         if (gs.field[gs.playx-2][gs.playy-1] == ROBOT)
            result &= ~(MOVE_L | MOVE_DL);
      if (gs.playx > 0)
         if (gs.field[gs.playx-1][gs.playy-1] == ROBOT)
            result &= ~(MOVE_L | MOVE_DL | MOVE_D | MOVE_S);
      if (gs.field[gs.playx][gs.playy-1] == ROBOT)
         result &= ~(MOVE_L | MOVE_DL | MOVE_D | MOVE_DR | MOVE_R | MOVE_S);
      if (gs.playx < 34)
         if (gs.field[gs.playx+1][gs.playy-1] == ROBOT)
            result &= ~(MOVE_D | MOVE_DR | MOVE_R | MOVE_S);
      if (gs.playx < 33)
         if (gs.field[gs.playx+2][gs.playy-1] == ROBOT)
            result &= ~(MOVE_DR | MOVE_R); }
   if ((gs.playx > 1)&&(gs.playy > 1))
      if (gs.field[gs.playx-2][gs.playy-2] == ROBOT)
         result &= ~MOVE_DL;
   if (gs.playy > 1) {
      if (gs.playx > 0)
         if (gs.field[gs.playx-1][gs.playy-2] == ROBOT)
            result &= ~(MOVE_DL | MOVE_D);
      if (gs.field[gs.playx][gs.playy-2] == ROBOT)
         result &= ~(MOVE_DL | MOVE_D | MOVE_DR);
      if (gs.playx < 34)
         if (gs.field[gs.playx+1][gs.playy-2] == ROBOT)
            result &= ~(MOVE_D | MOVE_DR); }
   if ((gs.playx < 33)&&(gs.playy > 1))
      if (gs.field[gs.playx+2][gs.playy-2] == ROBOT)
         result &= ~MOVE_DR;
   if (gs.playy < 19) {
      if (gs.playx > 0)
         if (gs.field[gs.playx-1][gs.playy+1] == HEAP)
            result &= ~MOVE_UL;
      if (gs.field[gs.playx][gs.playy+1] == HEAP)
         result &= ~MOVE_U;
      if (gs.playx < 34)
         if (gs.field[gs.playx+1][gs.playy+1] == HEAP)
            result &= ~MOVE_UR; }
   if (gs.playx > 0)
      if (gs.field[gs.playx-1][gs.playy] == HEAP)
         result &= ~MOVE_L;
   if (gs.playx < 34)
      if (gs.field[gs.playx+1][gs.playy] == HEAP)
         result &= ~MOVE_R;
   if (gs.playy > 0) {
      if (gs.playx > 0)
         if (gs.field[gs.playx-1][gs.playy-1] == HEAP)
            result &= ~MOVE_DL;
      if (gs.field[gs.playx][gs.playy-1] == HEAP)
         result &= ~MOVE_D;
      if (gs.playx < 34)
         if (gs.field[gs.playx+1][gs.playy-1] == HEAP)
            result &= ~MOVE_DR; }
   return result;
}

