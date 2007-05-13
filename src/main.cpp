/* main.cpp - DreamZZT: A reimplementation of the ZZT engine
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
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/eventcollector.h>
#include <Tiki/tikitime.h>
#include <Tiki/thread.h>
#include <string.h>
#include <time.h>
#ifdef NET
#include <curl/curl.h>
#endif
#include <Tiki/drawables/console.h>
#include <Tiki/oggvorbis.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Audio;
using namespace Tiki::Thread;

#if TIKI_PLAT == TIKI_WIN32
#include <wininet.h>
#include <shlobj.h>
#endif

#include "window.h"
#include "sound.h"
#include "board.h"
#include "status.h"
#include "debug.h"
#include "editor.h"
#include "os.h"
#include "http.h"
#include "task.h"
#include "word.h"
#include "bugreport.h"
#if TIKI_PLAT == TIKI_DC
#include "vmu.h"

#ifdef DEBUG
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
#endif
#endif

Mutex zzt_screen_mutex;

extern struct world_header world;
extern struct board_info_node *currentbrd;
bool gameFrozen;
ConsoleText *ct;
extern ConsoleText *dt;
extern ConsoleText *st;
extern struct board_info_node *board_list;
int switchbrd=-1;
extern Player *player;
extern EventCollector *playerEventCollector;
extern std::string curl_auth_string;
ZZTMusicStream *zm = NULL;
Tiki::Thread::Thread *render_thread;
Texture *zzt_font;

#define SCREEN_X 640
#if TIKI_PLAT == TIKI_DC
#define SCREEN_Y 424
#else
#define SCREEN_Y 480
#endif

#define GAMESPEED_ALIVE 160000
#define GAMESPEED_DEAD 10000

float zoom = 1;

#define VERSION "3.0.6.1"

std::string MAIN_MENU = std::string(std::string("$Welcome to DreamZZT ") + std::string(VERSION) + "\r\r\
Please select an option from the\r\
menu below:\r\
\r\
!new;Start a New Game\r\
!restore;Restore a Saved Game\r\
!tutorial;Tutorial\r") +
#ifdef NET
std::string("!net;DreamZZT Online\r") +
#endif
#if TIKI_PLAT != TIKI_DC
std::string("!edit;Editor\r") +
#endif
std::string("!credits;Credits\r\
!quit;Quit DreamZZT\r\
\r\
Copyright (C) 2000 - 2007 Sam Steele\r\
All Rights Reserved.\r");

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
$DreamZZT is (C) 2000 - 2007 Sam Steele\r\
$For more information, please visit\r\
$http://dev.c99.org/DreamZZT/\r"

void play_zzt(const char *filename, bool tempFile=false);
void net_menu();

void check_updates() {
#if (TIKI_PLAT != TIKI_OSX && defined(NET) && !defined(USE_SYSTEM_UPDATE_MANAGER))
	std::string ver;
#if TIKI_PLAT == TIKI_WIN32
	DWORD flags;
	if(!InternetGetConnectedState(&flags,0)) return;
#endif
	ver = http_get_string("http://dev.c99.org/DreamZZT/LATEST");

	if(ver != VERSION) {
		TUIWindow t("Update available");
		t.buildFromString("A new version of DreamZZT is available.\rPlease visit http://dev.c99.org/DreamZZT/\rfor more information.\r\r!ok;Ok\r");
		t.doMenu(ct);
	}
#endif
}

bool submit_bug(std::string email, std::string summary, std::string description) {
#ifdef NET
	std::string ver = VERSION;
#if TIKI_PLAT == TIKI_WIN32
	std::string plat = "Windows";
#elif TIKI_PLAT == TIKI_OSX
	std::string plat = "Macintosh";
#elif TIKI_PLAT == TIKI_SDL
	std::string plat = "Linux";
#elif TIKI_PLAT == TIKI_DC
	std::string plat = "Dreamcast"; //How did you get here, anyway?
#else
	std::string plat = "Other";
#endif	
	TracBug bug;
	
	bug.setProperty("reporter", email, "string");
	bug.setProperty("summary", summary, "string");
	bug.setProperty("description", description, "string");
	bug.setProperty("version", ver, "string");
	bug.setProperty("platform", plat, "string");

	if(currentbrd!=NULL) {
		bug.setProperty("game", std::string((const char*)world.title.string), "string");
		bug.setProperty("board", ToString(currentbrd->num) + ": " + currentbrd->title, "string");
	}
	
	
	if(bug.create()) {
		if(currentbrd != NULL) {
			std::string filename;
#if TIKI_PLAT == TIKI_WIN32
			char path[128];
			GetTempPath(128,path);
			filename = path + std::string("bugreport.sav");
#else 
			filename = "/tmp/bugreport.sav";
#endif
			save_game(filename.c_str());
			bug.attach(filename, "Current game state");
#if TIKI_PLAT == TIKI_WIN32
			 _unlink(filename.c_str());
#else
			 unlink(filename.c_str());
#endif
		}
		return true;
	}
#endif
	return false;
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

void render() {
#if 0
	float x,y,w,h;

	if(player!=NULL) {
		x=((BOARD_X*4)*zoom)+((BOARD_X*4)-player->getPosition().x*(8*zoom));
		y=((SCREEN_Y/2*zoom)+(SCREEN_Y/2-player->getPosition().y*((SCREEN_Y/ 25) * zoom)));
		if(x>(BOARD_X*4*zoom)) x = (BOARD_X*4*zoom);
		if(y>((SCREEN_Y/2)*zoom)) y = ((SCREEN_Y/2)*zoom);
		// TODO: test maximum values too
		ct->setTranslate(Vector(x, y, 0));
		ct->setSize((BOARD_X*8) * zoom, SCREEN_Y * zoom);
	}
#endif
	zzt_screen_mutex.lock();
	Frame::begin();
#if 0
	if(player!=NULL) {
		zzt_font->select();
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glViewport(x,y,w,h);
		ct->drawAll(Drawable::Opaque);
		ct->drawAll(Drawable::Trans);
	}		
	zzt_font->select();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glViewport(0,192,256/*+74*/,192);
#endif
	ct->drawAll(Drawable::Opaque);
	dt->drawAll(Drawable::Opaque);
	st->drawAll(Drawable::Opaque);
	Frame::transEnable();
	ct->drawAll(Drawable::Trans);
	dt->drawAll(Drawable::Trans);
	st->drawAll(Drawable::Trans);
	Frame::finish();
#if TIKI_PLAT == TIKI_DC
	update_lcds();
#endif
	zzt_screen_mutex.unlock();
}

extern "C" int tiki_main(int argc, char **argv) {
	TUIWindow *t, *c;
	srand((unsigned int)time(NULL));
		
	// Init Tiki
	Tiki::init(argc, argv);
	Tiki::setName("DreamZZT", NULL);
	//Hid::callbackReg(tkCallback, NULL);
	
#ifdef NET
	curl_global_init(CURL_GLOBAL_ALL);
	
	char authtmp[256];
	std::string filename;
	File f;
#if TIKI_PLAT == TIKI_WIN32
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL,CSIDL_LOCAL_APPDATA,NULL,0,szPath); 
	filename = std::string(szPath) + std::string("\\dzztauth.dat");
#else
	filename = std::string(getenv("HOME")) + std::string("/.dzztauth");
#endif
	f.open(filename.c_str(),"rb");
	if(f.isValid()) {
		int len = f.read(authtmp,256);
		if(len > 0) {
			authtmp[len]='\0';
			curl_auth_string = authtmp;
		}
		f.close();
	}
#endif	
	
#if TIKI_PLAT == TIKI_DC
#ifdef DEBUG
	//fs_chdir("/pc/users/sam/projects/dreamzzt/resources");
	fs_chdir("/rd");
#else
	fs_chdir("/cd");
#endif
	zzt_vmu_init();
#endif
	
	zm = new ZZTMusicStream;
	zm->setVolume(0.4f);
	
	//initialize the screen		
	zzt_font = new Texture("zzt-ascii.png", true);
	ct = new ConsoleText(BOARD_X, 25, zzt_font);
	ct->setSize(BOARD_X * 8,SCREEN_Y);
	ct->translate(Vector(BOARD_X * 4,240,0));
	
	st = new ConsoleText(80 - BOARD_X, 25, zzt_font);
	st->setSize((80 - BOARD_X) * 8, SCREEN_Y);
	st->setTranslate(Vector(640 - ((80 - BOARD_X) * 4), 240, 0.9f));
	
	debug_init();
	ct->color(15,1);
	ct->clear();
	st->color(15,1);
	st->clear();
	dzzt_logo();
	menu_background();
	render();
	check_updates();

	world.online=0;
	
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
			if(s!="")  play_zzt(s.c_str());
		} else if(t->getLabel() == "tutorial") {
			play_zzt("tutorial.zzt");
		} else if(t->getLabel() == "restore") {
			std::string s = os_select_file("Select a saved game","sav");
			if(s!="")  {
#if TIKI_PLAT == TIKI_DC
				unvmuify((std::string("/vmu/a1/") + s).c_str(),"/ram/tmp.sav");
				play_zzt("/ram/tmp.sav",true);
#else
				play_zzt(s.c_str());
#endif
			}
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

#if TIKI_PLAT == TIKI_DC
	arch_exit(0);
#endif
	
	if(zm!=NULL && zm->isPlaying()) zm->stop();
#ifdef NET
#if TIKI_PLAT == TIKI_WIN32
	SHGetFolderPath(NULL,CSIDL_LOCAL_APPDATA,NULL,0,szPath); 
	filename = std::string(szPath) + std::string("\\dzztauth.dat");
#else
	filename = std::string(getenv("HOME")) + std::string("/.dzztauth");
#endif
	f.open(filename.c_str(),"wb");
	if(f.isValid()) {
		f.write(curl_auth_string.c_str(),(int)curl_auth_string.length()+1);
		f.close();
	}
#endif	
	Tiki::shutdown();
	
	exit(0); //Win32 wont quit when this thread ends
	return 0;
}

void play_zzt(const char *filename, bool tempFile) {
	int start,tasktype,complete;
	std::string tmp;
	std::vector<std::string> tasks;
	std::vector<std::string> params;
	std::vector<std::string>::iterator tasks_iter;
	Player *titlePlayer;
	uint64 ticker=Time::gettime(), gamespeed = GAMESPEED_ALIVE;
	gameFrozen = false;
	int speedmod = 4;
	int volmod = 4;
	Event evt;
	TUISlider sm("", &speedmod);
	TUISlider vm("", &volmod);
	
	switchbrd=-1;
	if(load_zzt(filename,0)==-1) {
		TUIWindow t("Error");
		t.buildFromString("Unable to load world\r\r!ok;Ok");
		t.doMenu(ct);
		return;
	}
	
	if(tempFile) {
		#if TIKI_PLAT == TIKI_WIN32
			 _unlink(filename);
		#else
			 unlink(filename);
		#endif
	}
	
#ifdef NET
	if(std::string((const char *)world.title.string) != "") {
		std::list<TracBug> bugs = search_tickets("status!=closed&amp;game=" + std::string((const char *)world.title.string));
		if(bugs.size() > 0) {
			std::string bugWarning = "The following bugs have been reported for\rthis game:\r\r";
			
			for(std::list<TracBug>::iterator bi = bugs.begin(); bi != bugs.end(); bi++) {
				bugWarning += std::string("!") + ToString((*bi).getNum()) + std::string(";") + (*bi).getProperty("summary") + "\r";
			}
			
			bugWarning += "\r\
These bugs may affect your ability to\r\
complete this game.\r\
\r\
!continue;Continue to game\r\
!quit;Return to menu\r";
			TUIWindow t("Warning");
			t.buildFromString(bugWarning);
			do {
				t.doMenu(ct);
				
				if(t.getLabel() != "" && atoi(t.getLabel().c_str()) > 0) {
					for(std::list<TracBug>::iterator bi = bugs.begin(); bi != bugs.end(); bi++) {
						if((*bi).getNum() == atoi(t.getLabel().c_str())) {
							std::string bug = std::string("Reporter: ") + (*bi).getProperty("reporter") + "\r";
							bug += std::string("Platform: ") + (*bi).getProperty("platform") + "\r";
							bug += std::string("Version: ") + (*bi).getProperty("version") + "\r";
							bug += std::string("Game: ") + (*bi).getProperty("game") + "\r";
							bug += std::string("Board: ") + (*bi).getProperty("board") + "\r";					
							bug += "\r";
							bug += (*bi).getProperty("description") + "\r";
							bug += "\r";
							bug += "!return;Return to bug list\r";
							
							TUIWindow bw((*bi).getProperty("summary"));
							bw.buildFromString(bug);
							bw.doMenu(ct);
						}
					}
				} else if(t.getLabel() == "quit") {
					free_world();
					return;
				}
			} while(t.getLabel() != "" && t.getLabel() != "continue");
		}
	}
#endif
	
	start=world.start;
	EventCollector ec;
	if(filename[strlen(filename)-1]!='v' && filename[strlen(filename)-1]!='V') {
		switch_board(0);
		player->setShape(ZZT_EMPTY_SHAPE);
		player->setColor(0,0);
		playerEventCollector->stop();
		titlePlayer = player;
		player=NULL;
		st->locate(1,7);
		st->color(14,1);
		st->printf("World: ");
		st->color(15,1);
		st->printf("%s",world.title.string);
		st->locate(2,9);
	#if TIKI_PLAT == TIKI_DC
		st->printf("   Press Start");
	#else
		st->printf("   Press Enter");
	#endif
		st->locate(2,10);
		st->printf("    to begin!");
		draw_board();
		do {
			if(!gameFrozen && (Time::gettime() - ticker) > gamespeed) {
				update_brd();
				sm.update();				
				gamespeed = (uint64)(GAMESPEED_DEAD + ((float)GAMESPEED_ALIVE * (8.0f - (float)speedmod) / 8.0f));
				vm.update();
				zm->setVolume((float)volmod / 8.0f);
				draw_board();
				draw_msg();
				st->locate(2,19);
				st->color(0,3);
				st->printf(" S ");
				st->color(15,1);
				st->printf(" Speed:");
				st->locate(4,20);
				sm.draw(st);
				st->locate(2,22);
				st->color(0,7);
				st->printf(" V ");
				st->color(15,1);
				st->printf(" Volume:");
				st->locate(4,23);
				vm.draw(st);
				ticker = Time::gettime();
			}
			render();
			while(ec.getEvent(evt)) {
				if (evt.type == Hid::Event::EvtQuit) {
					switchbrd = -2;
				}
				if (evt.type == Hid::Event::EvtKeypress && evt.key == 13) {
					if(sm.getFocus() || vm.getFocus()) {
						sm.focus(false);
						vm.focus(false);
					} else {
						switchbrd = world.start;
					}
				} else if(evt.type == Hid::Event::EvtKeypress && evt.key == 's') {
					sm.focus(true);
				} else if(evt.type == Hid::Event::EvtKeypress && evt.key == 'v') {
					vm.focus(true);
				}
			}
		} while(switchbrd==-1);
		if(switchbrd==-2) return;
	}
	switchbrd=-1;

	titlePlayer->setShape(ZZT_PLAYER_SHAPE);
	titlePlayer->setColor(15,1);

#ifdef NET
	if(world.online==1) {
		tmp = http_get_string(DZZTNET_HOST + DZZTNET_HOME + std::string("?PostBackAction=Tasks&GameID=") + std::string((const char *)world.title.string));
		tasks = wordify(tmp,'\n');
		for(tasks_iter=tasks.begin(); tasks_iter!=tasks.end(); tasks_iter++) {
			params = wordify((*tasks_iter),'|');
			if(params.size() > 2) {
				tasktype = atoi(params[0].c_str());
				complete = atoi(params[1].c_str());
				params.erase(params.begin(), params.begin() + 2);			
				switch(tasktype) {
					case 0: //End of list
						break;
					case TASK_COLLECT:
						add_task(new TaskCollect(params), !!complete);
						break;
					case TASK_TORCH:
						add_task(new TaskUseTorch(params), !!complete);
						break;
					case TASK_KILL_ENEMY:
						add_task(new TaskKillEnemy(params), !!complete);
						break;
					case TASK_KILL_OBJECT:
						add_task(new TaskKillObject(params), !!complete);
						break;
					case TASK_TOUCH_OBJECT:
						add_task(new TaskTouchObject(params), !!complete);
						break;
					case TASK_SHOOT_OBJECT:
						add_task(new TaskShootObject(params), !!complete);
						break;
					case TASK_PLAYER_POSITION:
						add_task(new TaskPlayerPosition(params), !!complete);
						break;
					default:
						Debug::printf("Warning: unknown task type: %i\n", tasktype);
						break;
				}
			}
		}
	}	
#endif

	ct->color(15,1);
	ct->clear();
	st->color(15,1);
	st->clear();
	dzzt_logo();
	draw_hud_ingame();
	switch_board(start);
	if(!playerEventCollector->listening()) playerEventCollector->start();
	if(currentbrd->reenter_x == 254 && player!=NULL) currentbrd->reenter_x = (unsigned char)player->getPosition().x;
	if(currentbrd->reenter_y == 254 && player!=NULL) currentbrd->reenter_y = (unsigned char)player->getPosition().y;	
	srand((unsigned int)time(0));
	draw_board();
	if(player!=NULL) player->setFlag(F_SLEEPING);
	if(player!=NULL) player->update();
	while(1) {
		if(!gameFrozen && (Time::gettime() - ticker) > ((world.health>0)?gamespeed:GAMESPEED_DEAD)) {
			check_tasks();
			update_brd();
			draw_board();
			draw_msg();
			ticker = Time::gettime();
		}
		render();
		
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
			TUIWindow t("Game Menu");
			t.buildFromString(
#ifdef NET
std::string("!bugreport;Report a bug\r") +
#endif
"!save;Save Game\r\
!game;Return to Game\r\
!quit;Return to Main Menu\r");
			t.doMenu(ct);
			if(t.getLabel() == "bugreport") {
				TUIWindow bugReport("Bug Report");
				std::string email;
				std::string summary;
				std::string description1;
				std::string description2;
				std::string description3;
				bugReport.addWidget(new TUITextInput("Email: ", &email));
				bugReport.addWidget(new TUITextInput("Summary: ", &summary));
				bugReport.addWidget(new TUILabel("Description:"));
				bugReport.addWidget(new TUITextInput("",&description1));
				bugReport.addWidget(new TUITextInput("",&description2));
				bugReport.addWidget(new TUITextInput("",&description3));
				bugReport.addWidget(new TUILabel(""));
				bugReport.addWidget(new TUIHyperLink("submit","Submit bug report"));
				bugReport.doMenu(ct);
				
				if(bugReport.getLabel() == "submit") {
					submit_bug(email, summary, description1 + std::string("\n") + description2 + std::string("\n") + description3);
				}
			} else if(t.getLabel() == "quit") {
				break;
			}
			switchbrd=-1;
		} else if(switchbrd==-4) {
			world.saved=1;	
			if(world.online) {
#ifdef NET
				std::string filename;
#if TIKI_PLAT == TIKI_WIN32
				char path[128];
				GetTempPath(128,path);
				filename = path + std::string("saved.sav");
#else 
				filename = "/tmp/saved.sav";
#endif
				save_game(filename.c_str());
				std::string s = http_post_file(filename,"application/x-zzt-save", DZZTNET_HOST + DZZTNET_HOME + std::string("?PostBackAction=ProcessSave"));
				if(s!="OK") {
					TUIWindow t("");
					t.buildFromString(s);
					t.doMenu(ct);
				}
#endif
			} else {
				std::string s = os_save_file("Save a game","saved.sav","sav");
				if(s!="") {
#if TIKI_PLAT == TIKI_DC
					save_game("/ram/tmp.sav");
					spinner("Saving");
					vmuify("/ram/tmp.sav",(std::string("/vmu/a1/") + s).c_str(),s.c_str(),"DreamZZT Saved Game");
					fs_unlink("/ram/tmp.sav");
					spinner_clear();
#else
					save_game(s.c_str());
#endif
				}
			}
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
	delete playerEventCollector;
	playerEventCollector=NULL;
#ifdef NET
	if(world.online && switchbrd != -2) {
		std::string url = DZZTNET_HOST + DZZTNET_HOME + "?PostBackAction=SubmitScore";
		url += "&GameID=" + std::string((const char *)world.title.string);
		url += "&Score=" + ToString((int)world.score);
		std::string tmp = http_get_string(url);
		if(tmp!="OK") {
			TUIWindow *t;
			std::string title;
			if(tmp[0]=='$') { //The first line of the document is the window title
				title = tmp.substr(1,tmp.find("\n"));
				tmp.erase(0,tmp.find("\n")+1);
			} else {
				title = "Score Submission Error";
			}
			t = new TUIWindow(title);
			t->buildFromString(tmp);
			t->doMenu(ct);
		}
	}
#endif

	free_world();
}

void net_menu() {
#ifdef NET
	std::string url = DZZTNET_HOST + DZZTNET_HOME;
	std::string tmp,filename;
	
	/*if(curl_auth_string != "") {
		url = DZZTNET_HOST + DZZTNET_HOME + "?PostBackAction=AuthTest";
		tmp = http_get_string(url);
		if(tmp!="OK") {
			curl_auth_string = "";
		}
	}*/
	if(curl_auth_string == "") {
		TUIWindow *t;
		t = new TUIWindow("DreamZZT Online");
		t->buildFromString("DreamZZT Online allows you to compete\n\
against players around the world to\n\
get the highest score.	Before you can\n\
access DreamZZT Online, you'll need a\n\
C99.ORG Forums account.\n\
\n\
!Create;Create new account\n\
!Existing;Use existing acount\n");
		t->doMenu(ct);
		if(switchbrd==-2 || t->getLabel() == "") return;
		
		if(t->getLabel() == "Existing") {
			do {
				std::string user="",pass="";
				delete t;
				t = new TUIWindow("DreamZZT Online");
				t->addWidget(new TUILabel("Please enter your C99.ORG details:"));
				t->addWidget(new TUIWidget());
				t->addWidget(new TUITextInput("      Username: ",&user));
				t->addWidget(new TUIPasswordInput("      Password: ",&pass));
				t->addWidget(new TUIWidget());
				t->addWidget(new TUIHyperLink("login","Login to C99.ORG"));
				t->addWidget(new TUIHyperLink("cancel","Return to menu"));
				t->doMenu(ct);
				if(switchbrd==-2 || t->getLabel() == "cancel" || t->getLabel() == "") return;
				curl_auth_string = user + std::string(":") + pass;
				url = DZZTNET_HOST + DZZTNET_HOME + "?PostBackAction=AuthTest";
				tmp = http_get_string(url);
				if(tmp!="OK") {
					delete t;
					std::string title;
					if(tmp[0]=='$') { //The first line of the document is the window title
						title = tmp.substr(1,tmp.find("\n"));
						tmp.erase(0,tmp.find("\n")+1);
					} else {
						title = "Authentication Error";
					}
					t = new TUIWindow(title);
					t->buildFromString(tmp);
					t->doMenu(ct);
					if(switchbrd==-2) return;
				}
			} while(tmp != "OK" && switchbrd != -2);
		} else if(t->getLabel() == "Create") {
			std::string user="",first="",last="",pass1="",pass2="",email="";
			bool useEmail=false,useName=false,acceptTOS=false;
			do {
				delete t;
				t = new TUIWindow("DreamZZT Online");
				t->addWidget(new TUILabel("Please enter your information:"));
				t->addWidget(new TUIWidget());
				t->addWidget(new TUITextInput    ("         * Username: ",&user));
				t->addWidget(new TUIPasswordInput("         * Password: ",&pass1));
				t->addWidget(new TUIPasswordInput(" * Confirm Password: ",&pass2));
				t->addWidget(new TUITextInput    ("    * Email Address: ",&email));
				t->addWidget(new TUITextInput    ("         First Name: ",&first));
				t->addWidget(new TUITextInput    ("          Last Name: ",&last));
				t->addWidget(new TUIWidget());
				t->addWidget(new TUICheckBox		 ("Display full name",&useName));
				t->addWidget(new TUICheckBox		 ("Display email address",&useEmail));
				t->addWidget(new TUICheckBox		 ("I accept the terms of service",&acceptTOS));
				t->addWidget(new TUIWidget());
				t->addWidget(new TUIHyperLink("register","Register account"));
				t->addWidget(new TUIHyperLink("cancel","Return to menu"));
				t->addWidget(new TUIWidget());
				t->addWidget(new TUILabel("* Indicates a required field",true));
				t->addWidget(new TUIWidget());
				t->addWidget(new TUILabel("The terms of service can be viewed at",true));
				t->addWidget(new TUILabel("http://forums.c99.org/",true));
				t->doMenu(ct);
				if(switchbrd==-2 || t->getLabel() == "cancel" || t->getLabel() == "") return;
				url = DZZTNET_HOST + DZZTNET_HOME + "?PostBackAction=Register";
				url += "&Name=" + user;
				url += "&NewPassword=" + pass1;
				url += "&ConfirmPassword=" + pass2;
				url += "&Email=" + email;
				url += "&FirstName=" + first;
				url += "&LastName=" + last;
				url += "&ShowName=" + ToString((int)useName);
				url += "&UtilizeEmail=" + ToString((int)useEmail);
				url += "&AgreeToTerms=" + ToString((int)acceptTOS);
				
				tmp = http_get_string(url);
				if(tmp!="OK") {
					delete t;
					std::string title;
					if(tmp[0]=='$') { //The first line of the document is the window title
						title = tmp.substr(1,tmp.find("\n"));
						tmp.erase(0,tmp.find("\n")+1);
					} else {
						title = "Registration Error";
					}
					t = new TUIWindow(title);
					t->buildFromString(tmp);
					t->doMenu(ct);
					if(switchbrd==-2) return;
				}
			} while(tmp != "OK" && switchbrd != -2);
			if(tmp=="OK") curl_auth_string = user + ":" + pass1;
		} else {
			return; 
		}
	}
	
	url = DZZTNET_HOST + DZZTNET_HOME;
	
	do {
		ct->color(15,1);
		ct->clear();
		menu_background();
		dzzt_logo();
		
		if((url.find(".zzt") != std::string::npos) || (url.find(".ZZT") != std::string::npos) || (url.find(".sav") != std::string::npos) || (url.find(".SAV") != std::string::npos)) {
#ifdef DEBUG
			Debug::printf("Downloading: %s\n", url.c_str());
#endif
#if TIKI_PLAT == TIKI_WIN32
			char path[128];
			GetTempPath(128,path);
			filename = path + std::string("dzzthttp");
#else 
			filename = "/tmp/dzzthttp";
#endif
			http_get_file(filename, url);
			world.online=1;
			play_zzt(filename.c_str());
			world.online=0;

			#if TIKI_PLAT == TIKI_WIN32
				 _unlink(filename.c_str());
			#else
				 unlink(filename.c_str());
			#endif

			url = DZZTNET_HOST + DZZTNET_HOME;
		} else {
#ifdef DEBUG
			Debug::printf("Loading: %s\n", url.c_str());
#endif
			tmp = http_get_string(url);
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
				if(url != "") {
					if(url[0] == '/') {
						url = DZZTNET_HOST + url;
					} else if(url == "logout") {
						curl_auth_string="";
						url = "";
					} else {
						url = DZZTNET_HOST + DZZTNET_HOME + "?PostBackAction=" + url;
					}
				}
			}
		}
	} while(url != "" && switchbrd != -2);

#endif
}
