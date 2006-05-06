/*
 *  dup.cpp
 *  Duplicater object
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

#include <stdlib.h>
#include "object.h"
#include "board.h"
#include "dirxn.h"

extern struct board_info_node *currentbrd;

char dupanim[5]={'.',7,'o','O','.' };

update_handler duplicator_update(struct object *me) {
  struct object *obj;
  struct object *obj2;
  int x=me->x+(me->xstep*-1),y=me->y+(me->ystep*-1);

  me->shape=dupanim[me->arg3++];
  draw_block(me->x,me->y);
  if(me->arg3==5) {
    obj=currentbrd->board[me->x+me->xstep][me->y+me->ystep].obj;
    if(currentbrd->board[x][y].obj->flags&F_PUSHABLE) {
      if(me->xstep==-1) move(currentbrd->board[x][y].obj,RIGHT);
      if(me->xstep==1) move(currentbrd->board[x][y].obj,LEFT);
      if(me->ystep==-1) move(currentbrd->board[x][y].obj,DOWN);
      if(me->ystep==1) move(currentbrd->board[x][y].obj,UP);
    }
    if(is_empty(currentbrd,x,y)) {
      currentbrd->board[x][y].under=currentbrd->board[x][y].obj;
      currentbrd->board[x][y].obj=create_object(obj->type,x,y);
      obj2=currentbrd->board[x][y].obj;
      obj2->arg1=obj->arg1;
      obj2->arg2=obj->arg2;
      obj2->arg3=obj->arg3;
      obj2->arg4=obj->arg4;
      obj2->arg5=obj->arg5;
      obj2->shape=obj->shape;
      obj2->fg=obj->fg;
      obj2->bg=obj->bg;
      obj2->cycle=obj->cycle;
      if(obj2->create!=NULL) obj2->create(obj2);
      draw_block(x,y);
    }
  }
  me->arg3%=5;
  return 0;
}

create_handler duplicator_create(struct object *me) {
  me->cycle=(10-me->arg2)+2;
  if(me->xstep==65535) me->xstep=-1;
  if(me->ystep==65535) me->ystep=-1;
  return 0;
}
