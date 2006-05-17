/*
 *  items.cpp
 *  ZZT Items
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

#include "board.h"
#include "object.h"
#include "status.h"
#include "sound.h"
#include "debug.h"

extern ZZTMusicStream *zm;

extern struct world_header world;
extern struct board_info_node *currentbrd;
extern struct object *player;

int gemmsg=0,torchmsg=0,ammomsg=0,enermsg=0;

create_handler teleport_create(struct object *me) {
	me->arg1=12;
	me->heading=UP;
	if(me->xstep==1) { me->shape='>'; me->arg1=0; me->heading=RIGHT; }
	if(me->xstep==-1) { me->shape='<'; me->arg1=4; me->heading=LEFT; }
	if(me->ystep==1) { me->shape='v'; me->arg1=8; me->heading=DOWN; }
	if(me->ystep==-1) { me->shape='^'; me->arg1=12; me->heading=UP; }
  return 0;
}

char teleport_anim[16]={ '|',')','>',')', '|','(','<','(','-','v','_','v','-','^','~','^' };
	
update_handler teleport_update(struct object *me) {
	me->shape=teleport_anim[me->arg1+me->arg2];
	me->arg2++;
	me->arg2%=4;
	draw_block(me->x,me->y);
  return 0;
}

msg_handler teleport_message(struct object *me, struct object *them, char *message) {
	int x=me->x,y=me->y;
	if(them->type==ZZT_PLAYER && them->heading==me->heading && !strcmp(message,"touch")) {
		do {
			x+=me->xstep;
			y+=me->ystep;
			printf("Searching (%i,%i) found %i looking for %i\n",x,y,currentbrd->board[x][y].obj->type,ZZT_TRANSPORTER);
			if(x<=0 || y<=0 || x>=BOARD_X || y>=BOARD_Y) {
				currentbrd->board[player->x][player->y].obj=currentbrd->board[player->x][player->y].under;
				currentbrd->board[me->x+me->xstep][me->y+me->ystep].under=currentbrd->board[me->x+me->xstep][me->y+me->ystep].obj;
				currentbrd->board[me->x+me->xstep][me->y+me->ystep].obj=player;
				draw_block(player->x,player->y);
				player->x=me->x+me->xstep;
				player->y=me->y+me->ystep;
				draw_block(player->x,player->y);
				break;
			} else if(currentbrd->board[x][y].obj->type==ZZT_TRANSPORTER &&
				(currentbrd->board[x][y].obj->xstep==me->xstep*-1 ||
				currentbrd->board[x][y].obj->ystep==me->ystep*-1)) {
				currentbrd->board[player->x][player->y].obj=currentbrd->board[player->x][player->y].under;
				currentbrd->board[x+me->xstep][y+me->ystep].under=currentbrd->board[x+me->xstep][y+me->ystep].obj;
				currentbrd->board[x+me->xstep][y+me->ystep].obj=player;
				draw_block(player->x,player->y);
				player->x=x+me->xstep;
				player->y=y+me->ystep;
				draw_block(player->x,player->y);
				break;
			}
		} while(x>0 && x<BOARD_X && y>0 && y<BOARD_Y);
	}
  return 0;
}
	
msg_handler scroll_message(struct object *me, struct object *them, char *message) {
	if(!strcmp(message,"get")) {
		zm->setTune("tc-c+d-d+e-e+f-f+g-g");
		zm->start();
		zztoop_update(me);
		remove_from_board(currentbrd,me);
		move(them,toward(them,me));
	}
  return 0;
}

update_handler scroll_update(struct object *me) {
	me->fg++;
	if(me->fg>15) me->fg=9;
	draw_block(me->x,me->y);
  return 0;
}

msg_handler inv_message(struct object *me, struct object *them, char *message) {
  char tmp[100];
  int ok=0;
  if(!strcmp(message,"get")) {
    ok=1;
    switch(me->type) {
    case ZZT_ENERGIZER:
			world.energizer_cycle=80;
      if(enermsg==0) {
        enermsg=1;
        set_msg("Energizer - You are invincible!");
      }
			zm->setTune("s.-cd#e");
			zm->start();
      break;
    case ZZT_GEM:
      give_gems(1);
      give_health(1);
      give_score(10);
      if(gemmsg==0) {
        gemmsg=1;
        set_msg("Gems give you Health!");
      }
			zm->setTune("t+c-gec");
			zm->start();
      break;
    case ZZT_AMMO:
      give_ammo(5);
      if(ammomsg==0) {
        ammomsg=1;
        set_msg("Ammunition - 5 shots per container.");
      }
			zm->setTune("tcc#d");
			zm->start();
      break;
    case ZZT_TORCH:
      give_torch(1);
      if(torchmsg==0) {
        torchmsg=1;
        set_msg("Torch - used for lighting in the underground.");
      }
			zm->setTune("tcase");
			zm->start();
      break;
    case ZZT_KEY:
      if(world.keys[me->fg-9]==1) {
        sprintf(tmp,"You already have a %s key!",int_to_color(*me->color));
        ok=0;
      } else {
        world.keys[me->fg-9]=1;
				printf("world.keys[%i]=%i\n",me->fg-9,world.keys[me->fg-9]);
        sprintf(tmp,"You now have the %s key",int_to_color(*me->color));
        draw_keys();
				zm->setTune("t+cegcegceg+sc");
				zm->start();
      }
      set_msg(tmp);
      break;
    }
		if(ok) debug("\x1b[0;37mA \x1b[1;37m%s\x1b[0;37m was picked up.\n",me->name);
  } else if(!strcmp(message,"shot")) {
    switch(me->type) {
    case ZZT_GEM:
      ok=1;
      //remove_from_board(currentbrd,me);
      break;
    }
  } else if(!strcmp(message,"touch")) {
    switch(me->type) {
    case ZZT_DOOR:
      if(them->type==ZZT_PLAYER) {
        if(world.keys[(me->bg%8)-1]==1) {
          ok=0;
          sprintf(tmp,"The %s door is now open!",int_to_color(*me->color));
          set_msg(tmp);
          world.keys[(me->bg%8)-1]=0;
          draw_keys();
          remove_from_board(currentbrd, me);
          move(them,toward(them,me));
					zm->setTune("tcgbcgb+ic");
					zm->start();					
        } else {
          sprintf(tmp,"The %s door is locked.",int_to_color(*me->color));
          set_msg(tmp);
          ok=0;
					zm->setTune("t--gc");
					zm->start();
        }
      }
      break;
    }
  }
  if(ok) {
    remove_from_board(currentbrd,me);
    //move(them,toward(them,me));
  }
  return 0;
}

create_handler inv_create(struct object *me) {
  switch(me->type) {
  case ZZT_DOOR:
    me->fg=15;
    me->shape=8;
    me->color=&me->bg;
    break;
	case ZZT_KEY:
		if(me->fg<9) me->fg+=9;
		break;
	}
  return 0;
}
