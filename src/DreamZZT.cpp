/* DreamZZT.cpp - Tiki main function
 * Copyright (C) 2000 - 2007 Sam Steele
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
#include <Tiki/hid.h>
#include <Tiki/eventcollector.h>
#include <Tiki/thread.h>
#include <Tiki/tikitime.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Thread;

#include <stdarg.h>
#include <string.h>

#if TIKI_PLAT == TIKI_SDL
#include <iostream>
#include <qt3/qapplication.h>
#include <qt3/qfiledialog.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "os.h"

#if TIKI_PLAT == TIKI_DC
#include <Tiki/hid.h>
#include <Tiki/eventcollector.h>
#include <Tiki/drawables/console.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;

#include <dc/vmu_pkg.h>
#include "vmu.h"
#include "window.h"
#include "board.h"

extern ConsoleText *ct;
extern world_header world;
#endif

#if TIKI_PLAT == TIKI_DC || TIKI_PLAT == TIKI_NDS
#include "window.h"
#endif

#if TIKI_PLAT == TIKI_WIN32
#include <windows.h>
#include "pch.h"
#endif

#if TIKI_PLAT != TIKI_NDS && !(defined(USE_SDL) && TIKI_PLAT == TIKI_WIN32)

#if TIKI_PLAT == TIKI_WIN32
static char szAppName[] = "DreamZZT";
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else

extern "C" int tiki_main(int argc, char *argv[]);
int main(int argc, char *argv[])
#endif
{
#if TIKI_PLAT != TIKI_WIN32
#if TIKI_PLAT == TIKI_SDL
#if defined(DATA_PATH)
	chdir(DATA_PATH);
#else
	//Locate our data files by searching PATH for our binary
	//and following the symblink to our installed location
	char text[200];
	char text2[200];
	int len,cnt=0;

	if(readlink(argv[0],text,100)==-1)
		strcpy(text,argv[0]);

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

	QApplication app(argc,argv); //Initialize QT
#endif

	return tiki_main(argc, argv);
#else
	return Tiki::DoMain(szAppName, hInst, hPrevInstance, lpCmdLine, nCmdShow);
#endif
}
#endif

#if TIKI_PLAT == TIKI_WIN32
std::string os_select_file(std::string title, std::string filter) {
	OPENFILENAME ofn;			 // common dialog box structure
	char szFile[260];			 // buffer for file name
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
	OPENFILENAME ofn;			 // common dialog box structure
	char szFile[260];			 // buffer for file name
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

#if TIKI_PLAT == TIKI_SDL
std::string os_select_file(std::string title, std::string filter) {
	QString f = QFileDialog::getOpenFileName( QString::null, (std::string("*.") + filter).c_str());
	std::string output;

	if( !f.isEmpty() ) {
		output = f.latin1();
	} else {
		output = "";
	}

	return output;
}

std::string os_save_file(std::string title, std::string filename, std::string filter) {
	QString f = QFileDialog::getOpenFileName( filename.c_str(), (std::string("*.") + filter).c_str());
	std::string output;

	if( !f.isEmpty() ) {
		output = f.latin1();
	} else {
		output = "";
	}

	return output;
}
#endif

#if TIKI_PLAT == TIKI_DC
std::string os_select_file(std::string title, std::string filter) {
	dirent_t *de;
	uint32 d;
	TUIWindow t(title);
	std::string tmp = title + std::string(":\n");

	if(filter == "sav") {
		d = fs_open("/vmu/a1",O_RDONLY|O_DIR);
	} else {
		return "monster.szt";
		d = fs_open(fs_getwd(),O_RDONLY|O_DIR);
	}

	spinner("Working");

	while ( (de=fs_readdir(d)) ) {
		if((de->name[strlen(de->name)-3]==filter[0] || de->name[strlen(de->name)-3]==(filter[0] - 32)) &&
		        (de->name[strlen(de->name)-2]==filter[1] || de->name[strlen(de->name)-2]==(filter[1] - 32)) &&
		        (de->name[strlen(de->name)-1]==filter[2] || de->name[strlen(de->name)-1]==(filter[2] - 32))) {
			if(filter=="sav") {
				char name[128];
				char info[256];
				char ampm[3];
				dzzt_vmu_hdr hdr;
				vmu_hdr_t pkg;
				struct tm *time;
				int fd = fs_open((std::string("/vmu/a1/") + std::string(de->name)).c_str(), O_RDONLY);
				fs_read(fd,&pkg,sizeof(vmu_hdr_t));
				fs_seek(fd,128 + (pkg.icon_cnt * 512), SEEK_SET);
				fs_read(fd,&hdr,sizeof(dzzt_vmu_hdr));
				fs_close(fd);
				if(std::string(pkg.app_id) == "DZZT3") {
					time = localtime(&hdr.time);
					sprintf(name,"%s: %s",hdr.world,hdr.board);
					if(time->tm_hour < 12) {
						strcpy(ampm,"AM");
					} else {
						time->tm_hour -= 12;
						strcpy(ampm,"PM");
					}
					if(time->tm_hour == 0)
						time->tm_hour = 12;
					if(time->tm_min < 10) {
						sprintf(info,"Score: \x1b[1;37m%i    \x1b[1;33m%i/%i/%i    %i:0%i %s\n\x1b[1;31m\x03 \x1b[1;33mHealth: \x1b[1;37m%i  \x1b[0;44;36m\x84 \x1b[1;33mAmmo: \x1b[1;37m%i  \x1b[1;32m\x04 \x1b[1;33mGems: \x1b[1;37m%i",hdr.score,time->tm_mon+1,time->tm_mday,1900+time->tm_year,time->tm_hour,time->tm_min,ampm,hdr.health,hdr.ammo,hdr.gems);
					} else {
						sprintf(info,"Score: \x1b[1;37m%i    \x1b[1;33m%i/%i/%i    %i:%i %s\n\x1b[1;31m\x03 \x1b[1;33mHealth: \x1b[1;37m%i  \x1b[0;44;36m\x84 \x1b[1;33mAmmo: \x1b[1;37m%i  \x1b[1;32m\x04 \x1b[1;33mGems: \x1b[1;37m%i",hdr.score,time->tm_mon+1,time->tm_mday,1900+time->tm_year,time->tm_hour,time->tm_min,ampm,hdr.health,hdr.ammo,hdr.gems);
					}
					tmp += std::string("\n!") + std::string(de->name) + std::string(";") + std::string(name) + std::string("\n");
					tmp += std::string(info) + std::string("\n");
				}
			} else {
				tmp += std::string("!") + std::string(de->name) + std::string(";") + std::string(de->name) + std::string("\n");
			}
		}
	}
	fs_close(d);
	spinner_clear();
	t.buildFromString(tmp,true);
	t.doMenu();
	return t.getLabel();
}

std::string os_save_file(std::string title, std::string filename, std::string filter) {
	dirent_t *de;
	uint32 d,fd;
	TUIWindow t(title);
	int slot = 0;
	char strslot[12] = "0";
	char name[128];
	char info[256];
	char ampm[3];
	int blocks;
	stat_t rv;
	dzzt_vmu_hdr hdr;
	vmu_hdr_t pkg;
	struct tm *time;
	std::string tmp = title + std::string(":\n");

	save_game("/ram/tmp.sav");
	vmuify("/ram/tmp.sav","/ram/tmp.vms","/ram/tmp.vms","DreamZZT Saved Game");
	fs_unlink("/ram/tmp.sav");

	spinner("Working");

	fs_stat("/vmu/a1", &rv);
	fd=fs_open("/ram/tmp.vms",O_RDONLY);
	blocks = (fs_total(fd) / 512) + 1;
	fs_close(fd);
	fs_unlink("/ram/tmp.vms");
	printf("%i blocks required (%i blocks free)\n",blocks,rv.size);

	if(blocks < rv.size) {
		strcpy(name,(const char *)world.title.c_str());
		name[6] = '\0';
		strcat(name,strslot);
		strcat(name,".sav");

		while(fd = fs_open((std::string("/vmu/a1/") + std::string(name)).c_str(), O_RDONLY) >= 0) {
			printf("fd: %i\n",fd);
			fs_close(fd);
			slot++;
			sprintf(strslot,"%i",slot);
			strcpy(name,(const char *)world.title.c_str());
			name[6] = '\0';
			strcat(name,strslot);
			strcat(name,".sav");
		}
		fs_close(fd);
		printf("New name: %s\n",name);

		tmp += std::string("\n!") + std::string(name) + std::string("; Create a new save\n");
	} else {
		tmp += std::string("\n$Not enough free blocks to create a new\n$file.  Please select an existing file\n$to overwrite.\n");
	}

	d = fs_open("/vmu/a1",O_RDONLY|O_DIR);

	while ( (de=fs_readdir(d)) ) {
		printf("Filename: %s\n",de->name);
		if((de->name[strlen(de->name)-3]==filter[0] || de->name[strlen(de->name)-3]==(filter[0] - 32)) &&
		        (de->name[strlen(de->name)-2]==filter[1] || de->name[strlen(de->name)-2]==(filter[1] - 32)) &&
		        (de->name[strlen(de->name)-1]==filter[2] || de->name[strlen(de->name)-1]==(filter[2] - 32))) {
			if(filter=="sav") {
				int fd = fs_open((std::string("/vmu/a1/") + std::string(de->name)).c_str(), O_RDONLY);
				fs_read(fd,&pkg,sizeof(vmu_hdr_t));
				fs_seek(fd,128 + (pkg.icon_cnt * 512), SEEK_SET);
				fs_read(fd,&hdr,sizeof(dzzt_vmu_hdr));
				fs_close(fd);
				if(std::string(pkg.app_id) == "DZZT3" && std::string(hdr.world) == std::string((const char *)world.title.c_str())) {
					time = localtime(&hdr.time);
					sprintf(name,"%s: %s",hdr.world,hdr.board);
					name[38] = '\0';
					if(time->tm_hour < 12) {
						strcpy(ampm,"AM");
					} else {
						time->tm_hour -= 12;
						strcpy(ampm,"PM");
					}
					if(time->tm_hour == 0)
						time->tm_hour = 12;
					if(time->tm_min < 10) {
						sprintf(info,"Score: \x1b[1;37m%i    \x1b[1;33m%i/%i/%i    %i:0%i %s\n\x1b[1;31m\x03 \x1b[1;33mHealth: \x1b[1;37m%i  \x1b[0;44;36m\x84 \x1b[1;33mAmmo: \x1b[1;37m%i  \x1b[1;32m\x04 \x1b[1;33mGems: \x1b[1;37m%i",hdr.score,time->tm_mon+1,time->tm_mday,1900+time->tm_year,time->tm_hour,time->tm_min,ampm,hdr.health,hdr.ammo,hdr.gems);
					} else {
						sprintf(info,"Score: \x1b[1;37m%i    \x1b[1;33m%i/%i/%i    %i:%i %s\n\x1b[1;31m\x03 \x1b[1;33mHealth: \x1b[1;37m%i  \x1b[0;44;36m\x84 \x1b[1;33mAmmo: \x1b[1;37m%i  \x1b[1;32m\x04 \x1b[1;33mGems: \x1b[1;37m%i",hdr.score,time->tm_mon+1,time->tm_mday,1900+time->tm_year,time->tm_hour,time->tm_min,ampm,hdr.health,hdr.ammo,hdr.gems);
					}
					tmp += std::string("\n!") + std::string(de->name) + std::string(";") + std::string(name) + std::string("\n");
					tmp += std::string(info) + std::string("\n");
					printf("tmp: %s\n",tmp.c_str());
				}
			} else {
				tmp += std::string("!") + std::string(de->name) + std::string(";") + std::string(de->name) + std::string("\n");
			}
		}
	}
	fs_close(d);
	spinner_clear();
	t.buildFromString(tmp,true);
	t.doMenu();
	printf("Returning: %s\n",t.getLabel().c_str());
	return t.getLabel();
}
#endif

#if TIKI_PLAT == TIKI_NDS
#include <nds.h>
#include <fat.h>
#include <sys/dir.h>

#include "board.h"

extern ConsoleText *ct;
extern struct world_header world;
extern "C" int tiki_main(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	return tiki_main(argc, argv);
}

std::string os_select_file(std::string title, std::string filter) {
	struct stat st;
	char name[256]; // to hold a full filename and string terminator
	DIR_ITER* dir;
	std::string selection;
	std::string cur_path = "/";
	std::string dirs;
	std::string files;
	int ndx;

	while(true) {
		TUIWindow t(title);

		dir = diropen(cur_path.c_str());
		if(dir == NULL) {
			return "";
		}

		dirs = "";
		files = "";

		// if we're not in the root, make the first thing in the list a link to ".."
		if(cur_path != std::string("/"))
			dirs += std::string("!/..;[..]\n");

		while (dirnext(dir, name, &st) == 0) {
			if(st.st_mode & S_IFDIR && std::string(name) != std::string(".") && std::string(name) != std::string("..")) {
				// this is a directory (other than "." or "..")
				dirs += std::string("!/") + std::string(name) + std::string(";[") + std::string(name) + std::string("]\n");
			} else if((name[strlen(name)-3]==filter[0] || name[strlen(name)-3]==(filter[0] - 32)) &&
			          (name[strlen(name)-2]==filter[1] || name[strlen(name)-2]==(filter[1] - 32)) &&
			          (name[strlen(name)-1]==filter[2] || name[strlen(name)-1]==(filter[2] - 32))) {
				// this is a regular file matching our desired extension
				files += std::string("!") + std::string(name) + std::string(";") + std::string(name) + std::string("\n");
				/*
					TODO: Parse file header and display Dreamcast-style save selector
				*/
			}
		}

		dirclose(dir);

		t.buildFromString(title + std::string(":\n[") + cur_path + std::string("]\n") + dirs + files,true);
		t.doMenu();
		selection = t.getLabel();

		if(selection == std::string("")) {
			// they hit escape; bail out
			return selection;
		} else if(selection == std::string("/..")) {
			// trim the last dir off our path
			ndx = cur_path.rfind('/', cur_path.length() - 1);
			if(ndx)
				cur_path = cur_path.substr(0, cur_path.rfind('/', cur_path.length() - 1));
			else
				cur_path = std::string("/");
		} else if(selection.at(0) == '/') {
			// add the selected dir to our path
			// if the dir is in the root, avoid adding an extra slash
			if(cur_path == std::string("/"))
				cur_path = selection;
			else
				cur_path += selection;
		} else {
			// this is a file; return the full path to it.
			// if the file is in the root, avoid adding an extra slash
			if(cur_path == std::string("/"))
				return cur_path + selection;
			else
				return cur_path + std::string("/") + selection;
		}
	}
}
std::string os_save_file(std::string title, std::string filename, std::string filter) {
	TUIWindow t(title);
	t.addWidget(new TUILabel("Enter a filename:",false,false));
	t.addWidget(new TUITextInput("",&filename,false,false));
	t.addWidget(new TUIHyperLink("Save","Save Game"));
	t.doMenu();
	return filename;
}
#endif
