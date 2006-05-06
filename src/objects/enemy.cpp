/*
 *  enemy.cpp
 *  ZZT Enemys
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

#include "object.h"
#include "board.h"
#include "status.h"

extern struct board_info_node *currentbrd;
extern struct object *player;

update_handler lion_update(struct object *me) {
  if(rand()%10<me->arg1) {
    move(me,toward(me,player));
  } else {
    move(me,(direction)(rand()%4));
  }
}

char gun[4] = {27,24,26,25};

update_handler gun_update(struct object *me) {
  me->arg3++;
  me->arg3%=4;
  me->shape=gun[me->arg3];
  draw_block(me->x,me->y);
  if(rand()%30<me->arg2) {
    if(rand()%10<me->arg1) {
      if((me->x>=player->x-1 && me->x<=player->x+1) ||
        (me->y>=player->y-1 && me->y<=player->y+1)) {
        shoot(me,toward(me,player));
      }
    } else {
      if(rand()%(me->arg1+1)==0) {
        shoot(me,toward(me,player));
      }
    }
  }
}

update_handler tiger_update(struct object *me) {
  lion_update(me);
  if(rand()%10<me->arg2) {
    if((me->x>=player->x-1 && me->x<=player->x+1) ||
      (me->y>=player->y-1 && me->y<=player->y+1)) {
      shoot(me,toward(me,player));
    }
  }
}

msg_handler enemy_message(struct object *me, struct object *them, char *message) {
  if(!strcmp(message,"shot") || them->type==ZZT_PLAYER || !strcmp(message,"bombed")) {
    give_score(2);
    draw_score();
    if(!strcmp(message,"touch") || !strcmp(message,"thud")) {
      them->message(them,me,"shot");
    }
    remove_from_board(currentbrd,me);
  }
}
