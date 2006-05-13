/*
 *  player.cpp
 *  ZZT Player
 *
 *  Copyright 2000 - 2006 Sam Steele. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/tikitime.h>
#include <string.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;

#include <Tiki/drawables/console.h>
#include "object.h"
#include "board.h"
#include "http.h"
#include "status.h"

extern ConsoleText *ct;

extern int switchbrd;
extern struct world_header world;
struct object *player=NULL;
extern struct board_info_node *currentbrd;
void status(char *text);

#define MENU "Select an option:\r\
!save;Save game\r\
!hints;Online hints\r\
!quit;Quit game"

int player_hidCookie=-1;

void player_hidCallback(const Event & evt, void * data) {
	struct object *me=player;
	struct board_info_node *brd;
	struct object *obj;
	
	if (evt.type == Hid::Event::EvtQuit) {
		switchbrd = -2;
	}
	if (evt.type == Hid::Event::EvtKeyDown) {
		switch(evt.key) {
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
				if(me->flags&F_SLEEPING) {
					me->flags|=F_SLEEPING;
				} else {
					me->flags&=~F_SLEEPING;
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
		me->flags&=~F_SLEEPING;
		
		if(evt.mod & Hid::Event::KeyShift) {
			switch(evt.key) {
				case Event::KeyUp:
					shoot(me,UP);
					me->heading=UP;
					break;
				case Event::KeyDown:
					shoot(me,DOWN);
					me->heading=DOWN;
					break;
				case Event::KeyLeft:
					shoot(me,LEFT);
					me->heading=LEFT;
					break;
				case Event::KeyRight:
					shoot(me,RIGHT);
					me->heading=RIGHT;
					break;
			}					
		} else {
			switch(evt.key) {
				case Event::KeyUp:
					if(me->y==0&&board_up()>0) {
						brd=get_board(board_up());
						obj=get_obj_by_type(brd,ZZT_PLAYER);
						brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
						obj->x=me->x;
						obj->y=BOARD_Y-1;
						brd->board[obj->x][obj->y].under=brd->board[obj->x][obj->y].obj;
						brd->board[obj->x][obj->y].obj=obj;
						switchbrd=board_up();
					} else {
						me->heading=UP;
						move(me,UP);
					}
					break;
				case Event::KeyDown:
					if(me->y>=BOARD_Y-1&&board_down()>=0) {
						brd=get_board(board_down());
						obj=get_obj_by_type(brd,ZZT_PLAYER);
						brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
						obj->x=me->x;
						obj->y=0;
						brd->board[obj->x][obj->y].under=brd->board[obj->x][obj->y].obj;
						brd->board[obj->x][obj->y].obj=obj;
						switchbrd=board_down();
					} else {
						me->heading=DOWN;
						move(me,DOWN);
					}
					break;
				case Event::KeyLeft:
					if(me->x==0&&board_left()>=0) {
						brd=get_board(board_left());
						obj=get_obj_by_type(brd,ZZT_PLAYER);
						brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
						obj->x=BOARD_X-1;
						obj->y=me->y;
						brd->board[obj->x][obj->y].under=brd->board[obj->x][obj->y].obj;
						brd->board[obj->x][obj->y].obj=obj;
						switchbrd=board_left();
					} else {
						me->heading=LEFT;
						move(me,LEFT);
					}
					break;
				case Event::KeyRight:
					if(me->x>=BOARD_X-1&&board_right()>=0) {
						brd=get_board(board_right());
						obj=get_obj_by_type(brd,ZZT_PLAYER);
						brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
						obj->x=0;
						obj->y=me->y;
						brd->board[obj->x][obj->y].under=brd->board[obj->x][obj->y].obj;
						brd->board[obj->x][obj->y].obj=obj;
						switchbrd=board_right();
					} else {
						me->heading=RIGHT;
						move(me,RIGHT);
					}
					break;			
				case 32:
					if(currentbrd->maxshots==0) {
						set_msg("Can't shoot in here");
					} else {
						shoot(me,(direction)me->heading);
					}
					break;
			}
		}
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

create_handler player_create(struct object *me) {
  me->shape=2;
  me->fg=15;
  me->bg=1;
	
	if(player_hidCookie == -1) {
		printf("Registering player callback\n");
		player_hidCookie = Hid::callbackReg(player_hidCallback, NULL);
	}
  return 0;
}

msg_handler player_message(struct object *me, struct object *them, char *message) {
  if(!strcmp(message,"shot") || !strcmp(message,"bombed") || !strcmp(message,"time")) {
		if(world.energizer_cycle==0) {
			take_health(10);
			if(strcmp(message,"time")) {
				set_msg("Ouch!");
			}
			if(currentbrd->reenter && world.health>0) {
				world.time=currentbrd->time;
				draw_time();
				currentbrd->board[me->x][me->y].obj=currentbrd->board[me->x][me->y].under;
				draw_block(me->x,me->y);
				me->x=me->xstep;
				me->y=me->ystep;
				currentbrd->board[me->x][me->y].under=currentbrd->board[me->x][me->y].obj;
				currentbrd->board[me->x][me->y].obj=me;
				draw_block(me->x,me->y);
				me->flags|=F_SLEEPING;
				me->update(me);
			}
		}
  }
  return 0;
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

update_handler player_update(struct object *me) {
  struct object *obj;
  struct object *under;
  struct board_info_node *brd;
  int oldx=me->x,oldy=me->y;
  char tmp[50];
  int x;
  int s=0;
	char *filename;
	if(me->arg1==1) return 0;
  
	if(me->flags&F_SLEEPING) {
    ct->locate(BOARD_X+4,6);
    ct->color(15,1);
    ct->printf("Pausing...");
    do {
      if(s==0) {
        draw_block(me->x,me->y);
        s++;
      } else {
        s=0;
        ct->locate(me->x,me->y);
        ct->color(currentbrd->board[me->x][me->y].under->fg,currentbrd->board[me->x][me->y].under->bg);
        ct->printf("%c",currentbrd->board[me->x][me->y].under->shape);
      }
			draw_msg();
			Frame::begin();
			ct->draw(Drawable::Opaque);
			Frame::transEnable();
			ct->draw(Drawable::Trans);
			Frame::finish();
			Time::sleep(100000);
    } while(me->flags&F_SLEEPING);
    ct->locate(BOARD_X+4,6);
    ct->color(15,1);
    ct->printf("          ");
    draw_block(me->x,me->y);
  }

	if(world.energizer_cycle%2==0) {
		me->shape=2;
		me->bg=1;
		me->fg=15;
	} else {
		me->shape=1;
		me->fg=15;
		me->bg=5;
	}
	/*x=poll_game_device(0);	
	if(world.health>0) {
		switch(x) {
		case FIRE_UP:
			shoot(me,UP);
			me->heading=UP;
			break;
		case FIRE_DOWN:
			shoot(me,DOWN);
			me->heading=DOWN;
			break;
		case FIRE_LEFT:
			shoot(me,LEFT);
			me->heading=LEFT;
			break;
		case FIRE_RIGHT:
			shoot(me,RIGHT);
			me->heading=RIGHT;
			break;
		case MOVE_UP:
			if(me->y==0&&board_up()>0) {
				brd=get_board(board_up());
				obj=get_obj_by_type(brd,ZZT_PLAYER);
				brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
				obj->x=me->x;
				obj->y=BOARD_Y-1;
				brd->board[obj->x][obj->y].under=brd->board[obj->x][obj->y].obj;
				brd->board[obj->x][obj->y].obj=obj;
				switchbrd=board_up();
			} else {
				me->heading=UP;
				move(me,UP);
			}
			break;
		case MOVE_DOWN:
			if(me->y>=BOARD_Y-1&&board_down()>=0) {
				brd=get_board(board_down());
				obj=get_obj_by_type(brd,ZZT_PLAYER);
				brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
				obj->x=me->x;
				obj->y=0;
				brd->board[obj->x][obj->y].under=brd->board[obj->x][obj->y].obj;
				brd->board[obj->x][obj->y].obj=obj;
				switchbrd=board_down();
			} else {
				me->heading=DOWN;
				move(me,DOWN);
			}
			break;
		case MOVE_LEFT:
			if(me->x==0&&board_left()>=0) {
				brd=get_board(board_left());
				obj=get_obj_by_type(brd,ZZT_PLAYER);
				brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
				obj->x=BOARD_X-1;
				obj->y=me->y;
				brd->board[obj->x][obj->y].under=brd->board[obj->x][obj->y].obj;
				brd->board[obj->x][obj->y].obj=obj;
				switchbrd=board_left();
			} else {
				me->heading=LEFT;
				move(me,LEFT);
			}
			break;
		case MOVE_RIGHT:
			if(me->x>=BOARD_X-1&&board_right()>=0) {
				brd=get_board(board_right());
				obj=get_obj_by_type(brd,ZZT_PLAYER);
				brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
				obj->x=0;
				obj->y=me->y;
				brd->board[obj->x][obj->y].under=brd->board[obj->x][obj->y].obj;
				brd->board[obj->x][obj->y].obj=obj;
				switchbrd=board_right();
			} else {
				me->heading=RIGHT;
				move(me,RIGHT);
			}
			break;
		case BUTTON_X:
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
		case FIRE_BTN:
			//printf("Breakpoint!\n");
			if(currentbrd->maxshots==0) {
				set_msg("Can't shoot in here");
			} else {
				shoot(me,me->heading);
			}
			break;
		case START_BTN:
			me->flags|=F_SLEEPING;
			while(poll_game_device(0)==START_BTN);
			break;
		}
	}		
	if(x==QUIT_BTN) {
		while(poll_game_device(0)==QUIT_BTN);
		text_window("Game Menu",MENU,tmp);
		if(!strcmp(tmp,"quit")) {
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
		}
	}*/

	draw_block(me->x,me->y);
  return 0;
}
