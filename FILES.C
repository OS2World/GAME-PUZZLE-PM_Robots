/*  FILES.C
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

#define INCL_DOSFILEMGR
#include <os2.h>
#include <string.h>

static HFILE hfile;
static ULONG ulAction;
static ULONG ulBytesRead;

#define LOAD_ACTION OPEN_ACTION_OPEN_IF_EXISTS|OPEN_ACTION_FAIL_IF_NEW 
#define SAVE_ACTION OPEN_ACTION_CREATE_IF_NEW|OPEN_ACTION_REPLACE_IF_EXISTS
#define READ_ATTRS  OPEN_FLAGS_NO_CACHE|OPEN_FLAGS_SEQUENTIAL|OPEN_SHARE_DENYWRITE|OPEN_ACCESS_READONLY   
#define WRITE_ATTRS OPEN_FLAGS_NO_CACHE|OPEN_FLAGS_SEQUENTIAL|OPEN_SHARE_DENYREADWRITE|OPEN_ACCESS_WRITEONLY      

void LoadMap(HWND hwnd) {
  if (DosOpen(fdg.szFullFile,&hfile,&ulAction,0,FILE_NORMAL,LOAD_ACTION,READ_ATTRS,NULL)) return;
  DosRead(hfile,amap,sizeof(amap),&ulBytesRead);
  DosClose(hfile);
}

void SaveMap(HWND hwnd) {
  if (DosOpen(fdg.szFullFile,&hfile,&ulAction,sizeof(amap),FILE_NORMAL,SAVE_ACTION,WRITE_ATTRS,NULL)) return;
  DosWrite(hfile,amap,sizeof(amap),&ulBytesRead);
  DosClose(hfile);
}
