/*
 *  main.cpp
 *  DreamZZT - a reimplementation of the ZZT engine
 *
 *  Copyright 2000 - 2006 Sam Steele. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/tikitime.h>
#include <Tiki/thread.h>
#include <string.h>
#include <time.h>

#include <Tiki/drawables/console.h>
#include "board.h"
#include "status.h"
#include "debug.h"
#include <Tiki/oggvorbis.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Audio;
using namespace Tiki::Thread;

#include "sound.h"

Mutex zzt_screen_mutex;

extern struct world_header world;

#define MAIN_MENU "$Welcome to DreamZZT 3.0\r\r\
Please select an option from the\r\
menu below:\r\
\r\
!new;Start a New Game\r\
!tutorial;DreamZZT Tutorial\r\
!restore;Restore a Saved Game\r\
!net;Homepage\r\
!credits;Credits\r\
!quit;Quit DreamZZT\r\
\r\
(C) 2003 Sam Steele"

#define CREDITS "$Programming\r\r\
Sam Steele\r\
http://www.c99.org/dc/\r\r\
$Original ZZT\r\r\
Tim Sweeny - Epic Games\r\
http://www.epicgames.com/\r\r\
$ZZT file format specs\r\r\
Kev Vance\r\r\
$KallistiOS - Dreamcast\r\r\
Cryptic Allusion, LLC\r\
http://www.cadcdev.com/\r\r\
$Simple DirectMedia Layer - Linux\r\r\
This software may be dynamically linked\r\
to libSDL, an LGPL licensed library.\r\
http://www.libsdl.org/\r\r\
$Testing, Special Thanks and Shoutouts\r\r\
Jason Costa - Necrocosm Software\r\
http://www.necrocosm.com\r\r\
James Futhey\r\
http://www.dreamgamer.com/\r\r\
Gunnar (G-Force)\r\
http://G-Force420.cjb.net\r\r\
Chris 'Kilokahn' Haslage\r\
http://www.kkwow.net\r\r\
Brian Pinney\r\r\
$DreamZZT is (C) 2003 Sam Steele\r\
$For more information, please visit\r\
$http://www.c99.org/dc/\r"

void play_zzt(char *filename);
void net_menu();

ConsoleText *ct;
extern ConsoleText *dt;
extern struct board_info_node *board_list;
int switchbrd=-1;

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
Thread::Thread *render_thread;

void render() {
	zzt_screen_mutex.lock();
	Frame::begin();
	ct->draw(Drawable::Opaque);
	dt->draw(Drawable::Opaque);
	Frame::transEnable();
	ct->draw(Drawable::Trans);
	dt->draw(Drawable::Trans);
	Frame::finish();
	zzt_screen_mutex.unlock();
}

extern "C" int tiki_main(int argc, char **argv) {
  char tmp[50];
	srand(time(NULL));
		
	// Init Tiki
	Tiki::init(argc, argv);
	Tiki::setName("DreamZZT", NULL);
	//Hid::callbackReg(tkCallback, NULL);
	
#if TIKI_PLAT == TIKI_DC
	fs_chdir("/pc/users/sam/projects/dreamzzt/resources");
	
	pvr_init(&params);
#endif

	zm = new ZZTMusicStream;
	zm->setTune("cdefgab+c");
	zm->start();
	zm->setVolume(100);
	
	//initialize the screen		
	ct = new ConsoleText(80,25,new Texture("zzt-ascii.png", true));
	ct->setSize(640,480);
	ct->translate(Vector(320,240,0));
	
	debug_init();
	
  ct->color(15,1);
  ct->clear();
  dzzt_logo();
	menu_background();
	
	//TextWindow t("Main Menu",MAIN_MENU);
	//t.doMenu();

	/*menu_background();
	
	Frame::begin();
	ct->draw(Drawable::Opaque);
	Frame::transEnable();
	ct->draw(Drawable::Trans);
	Frame::finish();
	
	Time::sleep(800000);
	
	TextWindow t("Main Menu",MAIN_MENU);
	t.doMenu();*/
	
	play_zzt("town.zzt");
	if(zm!=NULL && zm->isPlaying()) zm->stop();
	Tiki::shutdown();
  return 0;
}

extern Player *player;
extern struct board_info_node *currentbrd;
bool gameFrozen;
extern bool playerInputActive;

void play_zzt(char *filename) {
	int start;
	char tmp[50];

	gameFrozen = false;
	playerInputActive = true;
	
	switchbrd=-1;
  if(load_zzt(filename,1)==-1) {
		TextWindow t(ct,"Error","Unable to load world\r\r!ok;Ok");
		t.doMenu();
		return;
	}
	start=world.start;
	switch_board(0);
	remove_from_board(currentbrd,player);
	//player->arg1=1;
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
		//Time::sleep(100000);
	} while(0);//poll_game_device(0)!=START_BTN);
	//while(poll_game_device(0)==START_BTN);
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
		Time::sleep(80000);
    
		if(switchbrd>-1) {
      switch_board(switchbrd);
			debug("\x1b[0;37mWarping to \x1b[1;37m%s\n",currentbrd->title);
      draw_board();
			redraw_status();
      if(player->getFlags()&F_SLEEPING) player->update();
      switchbrd=-1;
    } else if(switchbrd==-2) {
			break;
		}
  }
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	free_world();
}

#if 0

void status(char *text) {
  int x;
  ct->locate(0,23);
  ct->color(0,7);
  for(x=0;x<BOARD_X;x++) {
    ct->printf(" ");
  }
  ct->locate(0,23);
  ct->printf("%s",text);
  video_refresh();
  video_refresh();
}





#ifdef NDEBUG //win32 release
WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main() {
#endif
  char tmp[50];
	char input[5][2][50];
  int x,y;
#ifdef DREAMCAST
  fs_chdir("/rd");
#endif
#ifdef LINUX
	chdir(PACKAGE_DATA_DIR);
#endif
  c99_sys_init("DreamZZT 3.0");
  load_font("pc.fnt");
  init_screen();
#ifdef NET
  c99_net_init();
#endif
  objects_init();
  ct->color(15,1);
  clear_screen();
  dzzt_logo();
	menu_background();
	/*text_window("Login","Please enter your DZZT.NET username and\r\
password below:\r\r\
%username;c99koder:Username:\r\
%password;:Password:\r\r\
!login;Login\r",tmp,input);*/
	
	while(1) {
		menu_background();
    text_window("Main Menu",MAIN_MENU,tmp,NULL);

    if(!strcmp(tmp,"quit") || tmp[0]=='\0') {
      break;
    } else if(!strcmp(tmp,"new")) {
      play_zzt(select_file(getcwd(tmp,50),"zzt",0));
    } else if(!strcmp(tmp,"tutorial")) {
      play_zzt("tutorial.zzt");
    } else if(!strcmp(tmp,"restore")) {
      play_zzt(select_file(getcwd(tmp,50),"sav",0));
    } else if(!strcmp(tmp,"net")) {
      net_menu();
    } else if(!strcmp(tmp,"credits")) {
      text_window("Credits",CREDITS,tmp,NULL);
    }
  }
}

void net_menu() {
#ifdef NET
  char *buf=NULL;
  char tmp[50];
  char msg[100];
  char url[100];
  char tmp2[50];
  char title[50];
  char type[100];
  int len=0;
  int x,y,fd;
#ifdef DREAMCAST
	if(c99_net_check()==0) {
		text_window("Error","$Broadband Settings Missing\r\
\r\
This function requires an internet\r\
connection.  Please configure your\r\
Dreamcast with a static IP in Dream\r\
Passport or Broadband Passport and\r\
try again.\r\r\
!ok;Back to main menu\r",tmp,NULL);
		return;
	}		
	ct->color(15,1);
	clear_screen();
	dzzt_logo();
	fs_chdir("/ram");
#endif
#ifdef LINUX
	chdir("/tmp");
#endif
  unlink("online.tmp");
  set_status_callback(status);
  strcpy(tmp,"index.php");
  do {
    ct->color(1,0);
    for(y=0;y<BOARD_Y;y++) {
      for(x=0;x<BOARD_X;x++) {
        ct->locate(x,y);
        ct->printf("%c",177);
      }
    }
    video_refresh();
    ct->locate(0,0);
    ct->color(15,1);
    strcpy(url,"/dzztnet/");
    strcat(url,tmp);
    //ct->printf("%s\n",url);
    //video_refresh();
    http_get_file("online.tmp","www.c99.org",80,url,type,&len);
    if(!strcmp(type,"application/x-zzt-oop")) {
      if(buf!=NULL) free(buf);
      buf=malloc(len+1);
      fd=open("online.tmp",O_RDONLY);
      read(fd,buf,len);
      buf[len]='\0';
      close(fd);
      strcpy(title,"Online Content");
      x=0;
      if(buf[0]=='$') {
        do {
          title[x]=buf[x+1];
          x++;
        } while(buf[x+1]!='\r' && buf[x+1]!='\n');
        title[x]='\0';
        x+=2;
      }
      text_window(title,buf+x,tmp,NULL);
    } else if(!strcmp(type,"application/x-zzt-game")) {
      if(buf!=NULL) free(buf);
      buf=malloc(len+1);
      fd=open("online.tmp",O_RDONLY);
      read(fd,buf,len);
      buf[len]='\0';
      close(fd);
      sprintf(msg,"$Download Complete\r\r\
%s has been downloaded.\r\
Would you like to play it now?\r\r\
!yes;Yes\r\
!no;No\r\r",tmp);
      text_window("File Download",msg,tmp2,NULL);
      if(!strcmp(tmp2,"yes")) {
        play_zzt("online.tmp");
      }
      x=0;
      if(buf[0]=='$') {
        do {
          title[x]=buf[x+1];
          x++;
        } while(buf[x+1]!='\r' && buf[x+1]!='\n');
        title[x]='\0';
        x+=2;
      }
      text_window(title,buf+x,tmp,NULL);
    } else {
      text_window("Error","Invalid URL or invalid file type\r\rPress Start to Continue\r\r",tmp,NULL);
      x=0;
      if(buf[0]=='$') {
        do {
          title[x]=buf[x+1];
          x++;
        } while(buf[x+1]!='\r' && buf[x+1]!='\n');
        title[x]='\0';
        x+=2;
      }
      text_window(title,buf+x,tmp,NULL);
    }
    unlink("online.tmp");
  } while(tmp[0]!='\0');
  free(buf);
#ifdef DREAMCAST
	fs_chdir("/rd");
#endif
#ifdef LINUX
	chdir(PACKAGE_DATA_DIR);
#endif
#endif
}

#endif
