/*
 *  passage.cpp
 *  ZZT passage object
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
#include "sound.h"

extern int switchbrd;

extern ZZTMusicStream *zm;

msg_handler passage_message(struct object *me, struct object *them, char *message) {
	struct object *obj;
  struct object *obj2;
  struct board_info_node *brd=get_board(me->PASSAGE_DEST);
  //printf("Warping to %i\n",me->arg1);
  if(!strcmp(message,"touch")&&them->type==ZZT_PLAYER) {
    obj=get_obj_by_type(brd,ZZT_PLAYER);
    obj2=get_obj_by_color(brd,ZZT_PASSAGE,*me->color);
    if(obj2!=NULL) {
      brd->board[obj->x][obj->y].obj=brd->board[obj->x][obj->y].under;
      obj->x=obj2->x;
      obj->y=obj2->y;
      brd->board[obj->x][obj->y].under=obj2;
      brd->board[obj->x][obj->y].obj=obj;
    }
    /*obj->flags|=F_SLEEPING;*/
    switchbrd=me->PASSAGE_DEST;
    obj->flags|=F_SLEEPING;
		zm->setTune("tceg tc#fg# tdf#a td#ga# teg#+c");
		zm->start();
  }
  return 0;
}

create_handler passage_create(struct object *me) {
  me->color=&me->bg;
  return 0;
}