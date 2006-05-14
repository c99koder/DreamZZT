/*
 *  terrain.cpp
 *  ZZT Terrain types
 *
 *  Copyright 2000 - 2006 Sam Steele. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/hid.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Tiki;
using namespace Tiki::Hid;
using namespace Tiki::Audio;

#include "status.h"
#include "object.h"
#include "board.h"
#include "dirxn.h"
#include "sound.h"

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;
int forestmsg=0,invismsg=0,watermsg=0;

update_handler laser_update(struct object *me) {
	me->arg1--;
	if(me->arg1<=0) {
		remove_from_board(currentbrd,me);
	}
  return 0;
}

create_handler blink_create(struct object *me) {
  if(me->xstep==65535) me->xstep=-1;
  if(me->ystep==65535) me->ystep=-1;
	me->arg3=me->arg1;
	//me->cycle=1;
  return 0;
}

update_handler blink_update(struct object *me) {
	int x=me->x;
	int y=me->y;
	struct object *tmp;
	
	me->arg3--;
	if(me->arg3<0) {
		me->arg3=(me->arg2*4);
		do {
			x+=me->xstep;
			y+=me->ystep;
			if(is_empty(currentbrd,x,y) || currentbrd->board[x][y].obj->type==ZZT_BULLET) {
				if(currentbrd->board[x][y].obj->type==ZZT_BULLET) {
					remove_from_board(currentbrd,currentbrd->board[x][y].obj);
				}
				if(me->ystep!=0) {
					tmp=create_object(ZZT_VERTICAL_BLINK,x,y);
				} else {
					tmp=create_object(ZZT_HORIZONTAL_BLINK,x,y);
				}
				tmp->arg1=me->arg2*2;
				tmp->fg=me->fg;
				tmp->bg=me->bg;
				tmp->cycle=me->cycle;
				currentbrd->board[x][y].under=currentbrd->board[x][y].obj;
				currentbrd->board[x][y].obj=tmp;
				draw_block(x,y);
			} else {
				if(currentbrd->board[x][y].obj!=NULL && currentbrd->board[x][y].obj->message!=NULL) {
					currentbrd->board[x][y].obj->message(currentbrd->board[x][y].obj,me,"shot");
				}
				break;
			}
		} while(1);
	}
  return 0;
}

create_handler text_create(struct object *me) {
  me->shape=me->bg*16+me->fg;
  me->fg=15;
  if(me->type==ZZT_WHITE_TEXT) {
    me->bg=0;
    me->color=&me->fg;
  } else {
    me->bg=(me->type-ZZT_BLUE_TEXT)+1;
    me->color=&me->bg;
  }
  return 0;
}

create_handler terrain_create(struct object *me) {
  switch(me->type) {
  case ZZT_EMPTY:
    me->shape=32;
    me->fg=0;
    me->bg=0;
    break;
  case ZZT_WATER:
    me->bg=1;
    me->fg=15;
    me->cycle=1;
  }
  return 0;
}

update_handler water_update(struct object *me) {
  //me->shape=(rand()%2)+0xb1;
  if(rand()%8==1) {
    me->arg1=1;
    me->arg2=1;
  }
  switch(me->arg2) {
  case 0:
    me->arg1=0;
    me->fg=1;
    me->shape=0xb0;
    break;
  case 1:
    me->fg=9;
    me->shape=0xb0;
    break;
  case 2:
    me->fg=9;
    me->shape=0xb1;
    break;
  case 3:
    me->fg=9;
    me->shape=0xb2;
    break;
  case 4:
    me->fg=9;
    me->shape=0xdb;
    me->arg1=-1;
    break;
  }
  me->arg2+=me->arg1;
  draw_block(me->x,me->y);
  return 0;
}

msg_handler terrain_message(struct object *me, struct object *them, char *message) {
  if(!strcmp(message,"touch") && them->type==ZZT_PLAYER) {
    switch(me->type) {
    case ZZT_FOREST:
      if(forestmsg==0) {
        set_msg("A path is cleared through the forest.");
        forestmsg=1;
      }
			zm->setTune("ta");
			zm->start();
      remove_from_board(currentbrd,me);
      move(them,toward(them,me));      
      break;
    case ZZT_INVISIBLE:
      if(invismsg==0) {
        set_msg("You are blocked by an invisible wall.");
        invismsg=1;
      }
			zm->setTune("t--dc");
			zm->start();
			me->type=ZZT_NORMAL;
      me->shape=ZZT_NORMAL_SHAPE;
      draw_block(me->x,me->y);
      break;
    case ZZT_WATER:
      if(watermsg==0) {
        set_msg("Your path is blocked by water.");
        watermsg=1;
      }
			zm->setTune("t+c+c");
			zm->start();
    }
  }
  return 0;
}
