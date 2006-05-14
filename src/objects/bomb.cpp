/*
 *  bomb.cpp
 *  ZZT Bomb
 *
 *  Copyright 2000 - 2006 Sam Steele. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/hid.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Tiki;
using namespace Tiki::Hid;
using namespace Tiki::Audio;

#include "board.h"
#include "object.h"
#include "sound.h"

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;

msg_handler bomb_message(struct object *me, struct object *them, char *message) {
  if(them->type==ZZT_PLAYER && !strcmp(message,"touch") && me->shape==ZZT_BOMB_SHAPE) {
    me->shape='9';
    me->flags|=F_PUSHABLE;
    draw_block(me->x,me->y);
		zm->setTune("tcf+cf+c");
		zm->start();
  }
  return 0;
}

update_handler bomb_update(struct object *me) {
  int x,y;
  double a,b;
  struct object *them;
  if(me->shape <= '9' && me->shape >= '1') {
    me->shape--;
    me->cycle=4;
    me->arg1=4;
    draw_block(me->x,me->y);
  }
  if(me->shape=='0' && me->arg1>0) {
    me->arg1--;
    me->cycle=1;
    //remove_from_board(me);

    for(y=me->y-4;y<=me->y+4;y++) {
      for(x=me->x-7;x<=me->x+7;x++) {
        a=((x-me->x)*(x-me->x))/2.0f;
        b=(y-me->y)*(y-me->y);

        if((int)sqrt(a+b) == 4-me->arg1) {
          if(x>=0 && y >= 0 && x<BOARD_X && y<BOARD_Y) {
            them=currentbrd->board[x][y].obj;
            if(them->type==ZZT_BREAKABLE || is_empty(currentbrd,x,y)) {
              currentbrd->board[x][y].obj=create_object(ZZT_EXPLOSION,x,y);
              currentbrd->board[x][y].obj->create(currentbrd->board[x][y].obj);
              draw_block(x,y);
            } else if(them!=NULL && them->message!=NULL) {
              them->message(them,me,"bombed");
            }
          }
        }
      }
    }
  }
  if(me->arg1<=0) {
    remove_from_board(currentbrd,me);
  }
  return 0;
}

create_handler bomb_create(struct object *me) {
  me->arg1=4;
  return 0;
}

create_handler explosion_create(struct object *me) {
  me->arg1=4;
  me->shape=176;
  me->fg=7;
  me->bg=0;
  me->cycle=1;
  /*currentbrd->board[me->x][me->y].code=I_EMPTY;
  currentbrd->board[me->x][me->y].color=0;
  currentbrd->board[me->x][me->y].solid=0;  */
  return 0;
}

update_handler explosion_update(struct object *me) {
  switch(me->arg1) {
  case 4:
    me->fg=7;
    me->bg=0;
    break;
  case 3:
    me->fg=15;
    me->bg=0;
    break;
  case 2:
    me->fg=14;
    me->bg=0;
    break;
  case 1:
    me->fg=12;
    me->bg=0;
    break;
  case 0:
    me->fg=0;
    me->bg=0;
    remove_from_board(currentbrd,me);
    break;
  }
  if(me->arg1>0) me->arg1--;
  draw_block(me->x,me->y);
  return 0;
}
