/* main.cpp - DreamZZT: A reimplementation of the ZZT engine
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
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/eventcollector.h>
#include <Tiki/tikitime.h>
#include <Tiki/thread.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <Tiki/drawables/console.h>
#include <Tiki/oggvorbis.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Audio;
using namespace Tiki::Thread;

#if TIKI_PLAT == TIKI_WIN32
#include <wininet.h>
#endif

#include "window.h"
#include "sound.h"
#include "board.h"
#include "status.h"
#include "debug.h"
#include "editor.h"
#include "os.h"
#include "http.h"

Mutex zzt_screen_mutex;

extern struct world_header world;

#define MAIN_MENU "$Welcome to DreamZZT 3.0\r\r\
Please select an option from the\r\
menu below:\r\
\r\
!new;Start a New Game\r\
!restore;Restore a Saved Game\r\
!tutorial;DreamZZT Tutorial\r\
!edit;ZZT Editor\r\
!net;Homepage\r\
!credits;Credits\r\
!quit;Quit DreamZZT\r\
\r\
Copyright (C) 1999-2006 Sam Steele\r\
All Rights Reserved.\r"

#define CREDITS "$Programming\r\r\
Sam Steele\r\
http://www.c99.org/\r\r\
$Original ZZT\r\r\
Tim Sweeny - Epic Games\r\
http://www.epicgames.com/\r\r\
$ZZT file format specs\r\r\
Kev Vance\r\
http://www.kvance.com\r\r\
$Tiki\r\r\
Cryptic Allusion, LLC\r\
http://www.cadcdev.com/\r\r\
$Testing, Special Thanks and Shoutouts\r\r\
Jason Costa - Necrocosm Software\r\
http://www.necrocosm.com\r\r\
Chris 'Kilokahn' Haslage\r\
http://www.kkwow.net\r\r\
Brian Pinney\r\r\
$cURL File Transfer Library\r\
Copyright (c) 1996 - 2006, Daniel Stenberg\r\
<daniel@haxx.se>. All Rights Reserved.\r\r\
$Simple DirectMedia Layer\r\
This software may be dynamically linked\r\
to libSDL, an LGPL licensed library.\r\
http://www.libsdl.org/\r\r\
$DreamZZT is (C) 2000 - 2006 Sam Steele\r\
$For more information, please visit\r\
$http://www.c99.org/dc/dzzt/\r"

void play_zzt(const char *filename);
void net_menu();

ConsoleText *ct;
extern ConsoleText *dt;
extern struct board_info_node *board_list;
int switchbrd=-1;
extern Player *player;

void check_updates() {
#if (TIKI_PLAT != TIKI_OSX && defined(NET) && !defined(USE_SYSTEM_UPDATE_MANAGER))
	std::string ver;
#if TIKI_PLAT == TIKI_WIN32
	DWORD flags;
	if(!InternetGetConnectedState(&flags,0)) return;
#endif
	ver = http_get_string("http://www.c99.org/dc/dzzt/LATEST");

	Debug::printf("Latest version: %s\n", ver.c_str());
	
	if(ver != "3.0.4b2") {
		TUIWindow t("Update available");
		t.buildFromString("A new version of DreamZZT is available.\rPlease visit http://www.c99.org/dc/dzzt/\rfor more information.\r\r!ok;Ok\r");
		t.doMenu(ct);
	}
#endif
}

void menu_background() {
	int x,y;
	ct->color(1,0);
	for(y=0;y<BOARD_Y;y++) {
		for(x=0;x<BOARD_X;x++) {
			ct->locate(x,y);
			ct->printf("%c",177);
		}
	}
}	

#if TIKI_PLAT == TIKI_DC
//extern uint8 romdisk[];
//KOS_INIT_ROMDISK(romdisk);

pvr_init_params_t params = {
	{ PVR_BINSIZE_16, PVR_BINSIZE_8, PVR_BINSIZE_16, PVR_BINSIZE_8, PVR_BINSIZE_0 },
	512*1024,0,0
};
#endif

ZZTMusicStream *zm = NULL;
Tiki::Thread::Thread *render_thread;
Texture *zzt_font;

void render() {
#if 0
	int x,y,w,h;
	if(player!=NULL) {
		w=640;
		h=480;
		x=player->getPosition().x*8*-1 + (256/2) - 4;
		y=player->getPosition().y*20 - (192*2) - 8;
	}
#endif	
	zzt_screen_mutex.lock();
	Frame::begin();
	Frame::transEnable();
#if 0
	if(player!=NULL) {
		zzt_font->select();
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glViewport(x,y,w,h);
		ct->draw(Drawable::Opaque);
		ct->draw(Drawable::Trans);
	}		
	zzt_font->select();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glViewport(0,192,256/*+74*/,192);
#endif
	ct->draw(Drawable::Opaque);
	dt->draw(Drawable::Opaque);
	ct->draw(Drawable::Trans);
	dt->draw(Drawable::Trans);
	Frame::finish();
	zzt_screen_mutex.unlock();
}

extern "C" int tiki_main(int argc, char **argv) {
	TUIWindow *t, *c;
	srand(time(NULL));
		
	// Init Tiki
	Tiki::init(argc, argv);
	Tiki::setName("DreamZZT", NULL);
	//Hid::callbackReg(tkCallback, NULL);
	
	curl_global_init(CURL_GLOBAL_ALL);
	
#if TIKI_PLAT == TIKI_DC
	fs_chdir("/pc/users/sam/projects/dreamzzt/resources");
	
	pvr_init(&params);
#endif

	zm = new ZZTMusicStream;
	zm->setVolume(0.6);
	
	//initialize the screen		
	zzt_font = new Texture("zzt-ascii.png", true);
	ct = new ConsoleText(80,25, zzt_font);
	ct->setSize(640,480);
	ct->translate(Vector(320,240,0));
	
	debug_init();
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	menu_background();
	render();
	check_updates();
	
	//net_menu();
	
	if(argc > 1 && argv[argc-1][0] != '-') {
		play_zzt(argv[argc-1]);
	}
	
	while(switchbrd != -2) {
		ct->color(15,1);
		ct->clear();
		dzzt_logo();
		menu_background();

		t = new TUIWindow("Main Menu");
		t->buildFromString(MAIN_MENU);
		t->doMenu(ct);
		
		if(t->getLabel() == "quit" || t->getLabel() =="\0") {
			break;
		} else if(t->getLabel() == "new") {
			std::string s = os_select_file("Select a game","zzt");
			if(s!="")	play_zzt(s.c_str());
		} else if(t->getLabel() == "tutorial") {
			play_zzt("tutorial.zzt");
		} else if(t->getLabel() == "restore") {
			std::string s = os_select_file("Select a saved game","sav");
			if(s!="")	play_zzt(s.c_str());
		} else if(t->getLabel() == "edit") {
			new_world();
			edit_zzt();
		} else if(t->getLabel() == "net") {
			net_menu();
		} else if(t->getLabel() == "credits") {
			c = new TUIWindow("Credits");
			c->buildFromString(CREDITS);
			c->doMenu(ct);
			
			delete c;
		}
		
		delete t;
	}

	if(zm!=NULL && zm->isPlaying()) zm->stop();
	Tiki::shutdown();
  return 0;
}

extern Player *player;
extern struct board_info_node *currentbrd;
bool gameFrozen;
extern bool playerInputActive;

void titleHidCallback(const Event & evt, void * data) {
	if (evt.type == Hid::Event::EvtQuit) {
		switchbrd = -2;
	}
	if (evt.type == Hid::Event::EvtKeypress && evt.key == 13) {
		switchbrd = world.start;
	}
}	

void play_zzt(const char *filename) {
	int start;
	char tmp[50];
	int hidCookie = Hid::callbackReg(titleHidCallback, NULL);

	gameFrozen = false;
	playerInputActive = true;
	
	switchbrd=-1;
  if(load_zzt(filename,1)==-1) {
		TUIWindow t("Error");
		t.buildFromString("Unable to load world\r\r!ok;Ok");
		t.doMenu(ct);
		return;
	}
	start=world.start;
	switch_board(0);
	remove_from_board(currentbrd,player);
	player=NULL;
	ct->locate(BOARD_X+2,7);
  ct->color(14,1);
	ct->printf("   World: ");
	ct->color(15,1);
	ct->printf("%s",world.title.string);
	ct->locate(BOARD_X+2,9);
#ifdef DREAMCAST
	ct->printf("   Press Start");
#else
	ct->printf("   Press Enter");
#endif
	ct->locate(BOARD_X+2,10);
	ct->printf("    to begin!");
	draw_board();
	do {
		if(!gameFrozen) update_brd();
		draw_board();
		draw_msg();
		render();
		Time::sleep(60000);
	} while(world.saved==0 && switchbrd==-1);
	Hid::callbackUnreg(hidCookie);
	if(switchbrd==-2) return;
	switchbrd=-1;
	free_world();
	load_zzt(filename,0);
	start=world.start;
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
  draw_hud_ingame();
  switch_board(start);
  srand(time(0));
  draw_board();
  player->setFlag(F_SLEEPING);
  player->update();
  while(1) {
    if(!gameFrozen) update_brd();
		draw_board();
    draw_msg();
		render();
		if(world.health>0) Time::sleep(60000);
    
		if(switchbrd>-1) {
      switch_board(switchbrd);
			debug("\x1b[0;37mWarping to \x1b[1;37m%s\n",currentbrd->title);
      draw_board();
			redraw_status();
      if(player->getFlags()&F_SLEEPING) player->update();
      switchbrd=-1;
    } else if(switchbrd==-2) {
			break;
		} else if(switchbrd==-3) {
			//menu
			break;
		} else if(switchbrd==-4) {
			std::string s = os_save_file("Save a game","saved.sav","sav");
			if(s!="") save_game(s.c_str());
			switchbrd=-1;
		} else if(switchbrd==-5) {
			edit_zzt();
			switchbrd=-1;
			ct->color(15,1);
			ct->clear();
			dzzt_logo();
			draw_hud_ingame();
		}
  }
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	player=NULL;
	free_world();
}

#define DZZTNET_BASE "http://www.c99.org/dzztnet/"

void net_menu() {
	std::string url = "index.php";
	std::string tmp;
  do {
		ct->color(15,1);
		ct->clear();
		menu_background();
		dzzt_logo();
		
		if(url.find(".zzt") != std::string::npos) {
			http_get_file("/tmp/dzzthttp",DZZTNET_BASE + url);
			play_zzt("/tmp/dzzthttp");
			unlink("/tmp/dzzthttp");
			url = "index.php";
		} else {
			tmp = http_get_string(DZZTNET_BASE + url);
			if(tmp != "") {
				std::string title;
				if(tmp[0]=='$') { //The first line of the document is the window title
					title = tmp.substr(1,tmp.find("\n"));
					tmp.erase(0,tmp.find("\n")+1);
				} else {
					title = "Online Content";
				}
				TUIWindow t(title);
				t.buildFromString(tmp);
				t.doMenu(ct);
				url = t.getLabel();
			}
		}
	} while(url != "");
}
