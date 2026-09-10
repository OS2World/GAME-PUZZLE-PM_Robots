/*  FILES.C
 *  Presentation Manager Robots v1.4
 *  Copyright (c) 1993,1994,2002 by Kent Lundberg
 *
 *  LICENSE: GNU GPL V3
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
