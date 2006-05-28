/* player.cpp - Player object
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
#include <Tiki/tikitime.h>
#include <Tiki/thread.h>
#include <string.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Audio;
using namespace Tiki::Thread;

#include <Tiki/drawables/console.h>
#include "object.h"
#include "board.h"
#include "http.h"
#include "status.h"
#include "sound.h"

extern ZZTMusicStream *zm;
extern ConsoleText *ct;
extern ConsoleText *dt;
extern Mutex zzt_screen_mutex;

extern int switchbrd;
extern struct world_header world;
Player *player=NULL;
extern struct board_info_node *currentbrd;
void status(char *text);

#define MENU "Select an option:\r\
!save;Save game\r\
!hints;Online hints\r\
!quit;Quit game"

int player_hidCookie=-1;

bool playerInputActive;

void player_hidCallback(const Event & evt, void * data) {
	//((Player *)data)->processEvent(evt);
	if(player!=NULL) player->processEvent(evt);
}

void Player::processEvent(const Event & evt) {
	struct board_info_node *brd;
	ZZTObject *obj;
	
	if(!playerInputActive || m_counter > 0) return;
	
	if (evt.type == Hid::Event::EvtQuit) {
		switchbrd = -2;
	}
	if (evt.type == Hid::Event::EvtKeyDown) {
		switch(evt.key) {
			case 's':
				switchbrd=-4;
				break;
			case 't':
				if(world.torches>0) {
					if(currentbrd->dark==1) {
						world.torch_cycle=200;
						take_torch(1);
					} else {
						set_msg("Don't need torch - room is not dark!");
					}
				} else {
					set_msg("You don't have any torches!");
				}
				break;
			case 13:
				if(m_flags&F_SLEEPING) {
					m_flags|=F_SLEEPING;
				} else {
					m_flags&=~F_SLEEPING;
				}
				break;
				/*case Event::KeyEsc:
				TextWindow t("Game Menu",MENU);
				t.doMenu();
				
				if(!strcmp(t.getLabel(),"quit")) {
					switchbrd=-2;
				} else if(!strcmp(tmp,"hints")) {
					hints_menu();
					draw_board();
				} else if(!strcmp(tmp,"save")) {
					filename=select_file(getcwd(NULL,50),"sav",1);
					if(filename!=NULL) {
						save_game(filename);
#ifdef DREAMCAST
						vmuify(filename);
#endif
					}
					draw_board();
				} else { 
					draw_board();
				}*/		
		}	
	} else if (evt.type == Hid::Event::EvtKeypress) {
		m_flags&=~F_SLEEPING;
		
		if(evt.mod & Hid::Event::KeyShift) {
			switch(evt.key) {
				case Event::KeyUp:
					shoot(UP);
					m_heading=UP;
					break;
				case Event::KeyDown:
					shoot(DOWN);
					m_heading=DOWN;
					break;
				case Event::KeyLeft:
					shoot(LEFT);
					m_heading=LEFT;
					break;
				case Event::KeyRight:
					shoot(RIGHT);
					m_heading=RIGHT;
					break;
			}					
		} else {
			switch(evt.key) {
				case Event::KeyUp:
					m_heading=UP;
					if(m_position.y==0&&board_up()>0) {
						brd=get_board(board_up());
						obj=get_obj_by_type(brd,ZZT_PLAYER);
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj=brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under;
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under = NULL;
						obj->setPosition(Vector(m_position.x, BOARD_Y-1,0));
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under=brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj;
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj=obj;
						switchbrd=board_up();
					} else {
						move(UP);
					}
					break;
				case Event::KeyDown:
					m_heading=DOWN;
					if(m_position.y>=BOARD_Y-1&&board_down()>=0) {
						brd=get_board(board_down());
						obj=get_obj_by_type(brd,ZZT_PLAYER);
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj=brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under;
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under = NULL;
						obj->setPosition(Vector(m_position.x,0,0));
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under=brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj;
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj=obj;
						switchbrd=board_down();
					} else {
						move(DOWN);
					}
					break;
				case Event::KeyLeft:
					m_heading=LEFT;
					if(m_position.x==0&&board_left()>=0) {
						brd=get_board(board_left());
						obj=get_obj_by_type(brd,ZZT_PLAYER);
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj=brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under;
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under = NULL;
						obj->setPosition(Vector(BOARD_X-1, m_position.y,0));
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under=brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj;
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj=obj;
						switchbrd=board_left();
					} else {
						move(LEFT);
					}
					break;
				case Event::KeyRight:
					m_heading=RIGHT;
					if(m_position.x>=BOARD_X-1&&board_right()>=0) {
						brd=get_board(board_right());
						obj=get_obj_by_type(brd,ZZT_PLAYER);
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj=brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under;
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under = NULL;
						obj->setPosition(Vector(0,m_position.y,0));
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].under=brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj;
						brd->board[(int)obj->getPosition().x][(int)obj->getPosition().y].obj=obj;
						switchbrd=board_right();
					} else {
						move(RIGHT);
					}
					break;			
				case 32:
					if(currentbrd->maxshots==0) {
						set_msg("Can't shoot in here");
					} else {
						shoot(m_heading);
					}
					break;
			}
		}

		m_counter = 1;
	}
}

void hints_menu() {
#ifdef NET
  char *buf=NULL;
  char tmp[50];
	char args[20];
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
!ok;Back to main menu\r",tmp);
		return;
	}		
        ct->color(15,1);
        clear_screen();
        dzzt_logo();
	draw_hud_ingame();
	fs_chdir("/ram");
#endif
#ifdef LINUX
	chdir("/tmp");
#endif
  set_status_callback(status);
  sprintf(args,"game=%s&board=%i",world.title,currentbrd->num);
	strcpy(tmp,"index.php?");
  do {
    draw_board();
    video_refresh();
    ct->locate(0,0);
    ct->color(15,1);
    strcpy(url,"/dzztnet/hints/");
    strcat(url,tmp);
		strcat(url,args);
    //ct->printf("%s\n",url);
    //video_refresh();
    http_get_file("online.tmp","68.38.128.211",80,url,type,&len);
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
      text_window(title,buf+x,tmp);
    }
  } while(tmp[0]!='\0');
  unlink("online.tmp");
  free(buf);
#ifdef DREAMCAST
	fs_chdir("/rd");
#endif
#ifdef LINUX
	chdir(PACKAGE_DATA_DIR);
#endif
#endif
}

void Player::create() {
  m_shape=2;
  m_fg=15;
  m_bg=1;
  m_counter = 0;
	
	if(player_hidCookie == -1) {
		printf("Registering player callback\n");
		player_hidCookie = Hid::callbackReg(player_hidCallback, this);
	}
}

void Player::message(ZZTObject *them, std::string message) {
  if(message == "shot" || message == "bombed" || message == "time") {
		if(world.energizer_cycle==0 || message == "time") {
			take_health(10);
			if(message != "time") {
				set_msg("Ouch!");
				zm->setTune("t--c+c---c+d#");
				zm->start();
			}
			if(currentbrd->reenter && world.health>0) {
				world.time=currentbrd->time;
				draw_time();
				currentbrd->board[(int)m_position.x][(int)m_position.y].obj=currentbrd->board[(int)m_position.x][(int)m_position.y].under;
				currentbrd->board[(int)m_position.x][(int)m_position.y].under=NULL;
				draw_block(m_position.x,m_position.y);
				m_position=m_step;
				currentbrd->board[(int)m_position.x][(int)m_position.y].under=currentbrd->board[(int)m_position.x][(int)m_position.y].obj;
				currentbrd->board[(int)m_position.x][(int)m_position.y].obj=this;
				draw_block(m_position.x,m_position.y);
				m_flags|=F_SLEEPING;
			}
		}
  }
}

void whip(struct object *me, int x, int y, char shape) {
/*  struct object *them=currentbrd->board[x][y].obj;
  ct->locate(x,y);
  ct->color(7,0); ct->printf("%c",shape);
  if(them!=NULL && them->message!=NULL) {
    them->message(them,me,"whipped");
  } else if(currentbrd->board[x][y].code==I_BREAKABLE) {
    currentbrd->board[x][y].code=I_EMPTY;
    currentbrd->board[x][y].color=0;
    currentbrd->board[x][y].solid=0;
  }
  video_refresh();
  draw_block(x,y);*/
}

void Player::update() {
	int s=0;
	
	if(m_counter > 0) m_counter--;
  
	if(m_flags&F_SLEEPING) {
    ct->locate(BOARD_X+4,6);
    ct->color(15,1);
    ct->printf("Pausing...");
    do {
      if(s==0) {
        draw();
        s++;
      } else {
        s=0;
        ct->locate(m_position.x,m_position.y);
        ct->color(currentbrd->board[(int)m_position.x][(int)m_position.y].under->getFg(),currentbrd->board[(int)m_position.x][(int)m_position.y].under->getBg());
        ct->printf("%c",currentbrd->board[(int)m_position.x][(int)m_position.y].under->getShape());
      }
			draw_msg();
			render();
			Time::sleep(100000);
    } while(m_flags&F_SLEEPING);
    ct->locate(BOARD_X+4,6);
    ct->color(15,1);
    ct->printf("          ");
    draw();
  }

	if(world.energizer_cycle%2==0) {
		m_shape=2;
		m_bg=1;
		m_fg=15;
	} else {
		m_shape=1;
		m_fg=15;
		m_bg=5;
	}
}
