/*  HISCORES.C
 *  Presentation Manager Robots v1.4
 *  Copyright (c) 1993,1994,2002 by Kent Lundberg
 *
 *  LICENSE: GNU GPL V3
 */

#include <os2.h>
#include <string.h>
#include "control.h"

#define LOAD_ACTION OPEN_ACTION_OPEN_IF_EXISTS|OPEN_ACTION_FAIL_IF_NEW
#define SAVE_ACTION OPEN_ACTION_CREATE_IF_NEW|OPEN_ACTION_REPLACE_IF_EXISTS
#define READ_ATTRS  OPEN_FLAGS_NO_CACHE|OPEN_FLAGS_SEQUENTIAL|OPEN_SHARE_DENYWRITE|OPEN_ACCESS_READONLY
#define WRITE_ATTRS OPEN_FLAGS_NO_CACHE|OPEN_FLAGS_SEQUENTIAL|OPEN_SHARE_DENYREADWRITE|OPEN_ACCESS_WRITEONLY
#define HISCORESFILE "robots.sco"

void ClearHiScores(HISCORES *phs) {
   SHORT scI;

   for (scI = 0 ; scI < 10 ; scI++) {
      phs->score[scI] = 0;
      phs->level[scI] = 0;
      strcpy(phs->name[scI],"The Killer Robots");
   }
   phs->clear = FALSE;
}

void LoadHiScores(HISCORES *phs, GAMESTATE *pgs) {
   HFILE hfile;
   ULONG ulAction;
   ULONG ulBytesRead;
   ClearHiScores(phs);
   if (DosOpen((PCSZ) HISCORESFILE,&hfile,&ulAction,0,FILE_NORMAL,LOAD_ACTION,READ_ATTRS,NULL)) return;
   DosRead(hfile,phs,sizeof(*phs),&ulBytesRead);
   DosClose(hfile);
   pgs->hiscore = (phs->score[9] == 0) ? 1 : phs->score[9];
}

void SaveHiScores(HISCORES *phs, GAMESTATE *pgs) {
   HFILE hfile;
   ULONG ulAction;
   ULONG ulBytesRead;
   SHORT scI;

   if (phs->clear) {
      ClearHiScores(phs);
      pgs->hiscore = 1;
      if (DosOpen((PCSZ) HISCORESFILE,&hfile,&ulAction,sizeof(*phs),FILE_NORMAL,SAVE_ACTION,WRITE_ATTRS,NULL)) return;
      DosWrite(hfile,phs,sizeof(*phs),&ulBytesRead);
      DosClose(hfile);
   }
   else {
      scI = 9;
      while ((scI != 0)&&(pgs->score > phs->score[scI-1])) {
         phs->score[scI] = phs->score[scI-1];
         phs->level[scI] = phs->level[scI-1];
         strcpy(phs->name[scI],phs->name[scI-1]);
         scI--;
      }
      phs->score[scI] = pgs->score;
      phs->level[scI] = pgs->level;
      strcpy(phs->name[scI],pgs->name);
      pgs->hiscore = (phs->score[9] == 0) ? 1 : phs->score[9];
      if (DosOpen( (PCSZ) HISCORESFILE,&hfile,&ulAction,sizeof(*phs),FILE_NORMAL,SAVE_ACTION,WRITE_ATTRS,NULL)) return;
      DosWrite(hfile,phs,sizeof(*phs),&ulBytesRead);
      DosClose(hfile);
   }
}

