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
#include "os.h"

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

#ifdef WIN32
std::string os_select_file(std::string title, std::string filter) {
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	std::string output;
	char szFilter[260];

	sprintf(szFilter,"*.%s%c*.%s%cAll Files (*.*)%c*.*%c",filter.c_str(),'\0',filter.c_str(),'\0','\0','\0');

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = filter.c_str();
	ofn.lpstrTitle = title.c_str();

	if (GetOpenFileName(&ofn)==TRUE) {
		output = ofn.lpstrFile;
	} else {
		output = "";
	}

	return output;
}

std::string os_save_file(std::string title, std::string filename, std::string filter) {
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	std::string output;
	char szFilter[260];

	sprintf(szFilter,"*.%s%c*.%s%cAll Files (*.*)%c*.*%c",filter.c_str(),'\0',filter.c_str(),'\0','\0','\0');

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = filter.c_str();
	ofn.lpstrTitle = title.c_str();

	if (GetSaveFileName(&ofn)==TRUE) {
		output = ofn.lpstrFile;
	} else {
		output = "";
	}

	return output;
}
#endif