/* DreamZZT.cpp - Tiki main function
 * Copyright (c) 2000-2006 Sam Steele
 *
 * This file is part of DreamZZT.
 *
 * DreamZZT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * DreamZZT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */ 

#include <Tiki/tiki.h>

#if TIKI_PLAT == TIKI_WIN32
#include <windows.h>

#include "pch.h"

static char szAppName[] = "DreamZZT";
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
#if TIKI_PLAT == TIKI_SDL
#include <sys/stat.h>
#include <unistd.h>
#endif

extern "C" int tiki_main(int argc, char *argv[]);
int main(int argc, char *argv[])
#endif
{
#if TIKI_PLAT != TIKI_WIN32
#if TIKI_PLAT == TIKI_SDL
//Locate our data files by searching PATH for our binary
//and following the symblink to our installed location
  char text[200];
  char text2[200];
  int len,cnt=0;

  if(readlink(argv[0],text,100)==-1) strcpy(text,argv[0]);

  if(text[0]!='/' && text[0]!='.') {
    char *val;
    struct stat s;

    val=strtok(getenv("PATH"),":");
    do {
      sprintf(text2,"%s/%s",val,argv[0]);

      if(lstat(text2,&s)!=-1) {
        if(S_ISLNK(s.st_mode)) {
          readlink(text2,text,100);
        } else {
          strcpy(text,text2);
        }
        break;
      }
      val=strtok(NULL,":");
    } while(val!=NULL);
  }

  for(len=strlen(text);len>=0;len--) {
    if(text[len]=='/') {
      text[len]='\0';
      break;
    }
  }

  chdir(text);
#endif

  return tiki_main(argc, argv);
#else
  return Tiki::DoMain(szAppName, hInst, hPrevInstance, lpCmdLine, nCmdShow);
#endif
}
