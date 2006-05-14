/*
 *  bullet.cpp
 *  ZZT bullet
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

#include "object.h"
#include "board.h"
#include "status.h"
#include "sound.h"

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;
extern struct world_header world;

extern char ammo_name[100];

void bullet_sound_init() {
#ifdef DREAMCAST
  if(sound_id==-1) { sound_id=snd_sfx_load("/cd/fire.wav"); }
#endif
}

update_handler bullet_update(struct object *myobj) {
  move(myobj,(direction)myobj->arg2);
  return 0;
}

msg_handler bullet_message(struct object *me, struct object *them, char *message) {

  if(!strcmp(message,"thud")) {
    if(me->arg1==0 && them->type!=ZZT_PLAYER || me->arg1==1 && them->type==ZZT_PLAYER) {
      if(them->message!=NULL) them->message(them,me,"shot");
    } 
		if(them->type==ZZT_BREAKABLE) {
			remove_from_board(currentbrd,them);
			zm->setTune("t-c");
			zm->start();
		}
    remove_from_board(currentbrd,me);
  }
  return 0;
}

void shoot(struct object *owner, enum direction dir) {
	struct object *bullet=NULL;
	int dx,dy;
	char buf[100];

	switch(dir) {
	  case UP:
	   dx=0; dy=-1;
	   break;
    case DOWN:
     dx=0; dy=1;
     break;
    case LEFT:
     dx=-1; dy=0;
     break;
    case RIGHT:
     dx=1; dy=0;
     break;
	}
  if(owner->type==ZZT_PLAYER) {
    if(world.ammo<1) {
			//sprintf(buf,"You don't have any %s",ammo_name);
      set_msg("You don't have any ammo");
      return;
    }
  }
  if(is_empty(currentbrd,owner->x+dx,owner->y+dy)) {
    if(owner->type==ZZT_PLAYER) world.ammo--;
    draw_ammo();
    bullet=create_object(ZZT_BULLET,owner->x+dx,owner->y+dy);
    bullet->arg1=owner->type==ZZT_PLAYER?0:1;
    bullet->arg2=dir;
	  bullet->cycle=1;
	  bullet->flags=F_NONE;
    currentbrd->board[bullet->x][bullet->y].obj=currentbrd->board[bullet->x][bullet->y].under;
    currentbrd->board[bullet->x][bullet->y].obj=bullet;
		zm->setTune("t+c-c-c");
		zm->start();
  } else {
    if(currentbrd->board[owner->x+dx][owner->y+dy].obj!=NULL) {
      if(currentbrd->board[owner->x+dx][owner->y+dy].obj->message!=NULL) {
				currentbrd->board[owner->x+dx][owner->y+dy].obj->message(currentbrd->board[owner->x+dx][owner->y+dy].obj,owner,"shot");
			} else if(currentbrd->board[owner->x+dx][owner->y+dy].obj->type==ZZT_BREAKABLE) {
				remove_from_board(currentbrd,currentbrd->board[owner->x+dx][owner->y+dy].obj);
			}
			if(is_empty(currentbrd,owner->x+dx,owner->y+dy)) {
				if(owner->type==ZZT_PLAYER) world.ammo--;
				draw_ammo();
			}
    }
  }
}
