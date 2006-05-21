/*
 *  spinner.cpp
 *  ZZT Spinner
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

extern struct board_info_node *currentbrd;

extern char spin_anim[4];
char moved[3][3];

int spin_cw(struct object *me, struct object *them) {
  if(!(them->flags&F_PUSHABLE) || them->pushed) return 0;

  if(them->y==me->y-1 && (them->x==me->x-1||them->x==me->x)) {
    if(is_empty(currentbrd,them->x+1,them->y)) {
      move(them,RIGHT);
      moved[(them->x-me->x)+1][(them->y-me->y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[them->x+1][them->y].obj!=NULL) {
        return spin_cw(me,currentbrd->board[them->x+1][them->y].obj);
      } else {
        return 0;
      }
    }
  } else if(them->x==me->x+1 && (them->y==me->y-1 || them->y==me->y)) {
    if(is_empty(currentbrd,them->x,them->y+1)) {
      move(them,DOWN);
      moved[(them->x-me->x)+1][(them->y-me->y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[them->x][them->y+1].obj!=NULL) {
        return spin_cw(me,currentbrd->board[them->x][them->y+1].obj);
      } else {
        return 0;
      }
    }
  } else if(them->y==me->y+1 && (them->x==me->x+1 || them->x==me->x)) {
    if(is_empty(currentbrd,them->x-1,them->y)) {
      move(them,LEFT);
      moved[(them->x-me->x)+1][(them->y-me->y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[them->x-1][them->y].obj!=NULL) {
        return spin_cw(me,currentbrd->board[them->x-1][them->y].obj);
      } else {
        return 0;
      }
    }
  } else if(them->x==me->x-1 && (them->y==me->y+1 || them->y==me->y)) {
    if(is_empty(currentbrd,them->x,them->y-1)) {
      move(them,UP);
      moved[(them->x-me->x)+1][(them->y-me->y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[them->x][them->y-1].obj!=NULL) {
        return spin_cw(me,currentbrd->board[them->x][them->y-1].obj);
      } else {
        return 0;
      }
    }
  }
  return 0;
}

int spin_ccw(struct object *me, struct object *them) {
  if(!(them->flags&F_PUSHABLE) || them->pushed) return 0;

  if(them->y==me->y-1 && (them->x==me->x+1||them->x==me->x)) {
    if(is_empty(currentbrd,them->x-1,them->y)) {
      move(them,LEFT);
      moved[(them->x-me->x)+1][(them->y-me->y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[them->x-1][them->y].obj!=NULL) {
        return spin_cw(me,currentbrd->board[them->x-1][them->y].obj);
      } else {
        return 0;
      }
    }
  } else if(them->x==me->x+1 && (them->y==me->y+1 || them->y==me->y)) {
    if(is_empty(currentbrd,them->x,them->y-1)) {
      move(them,UP);
      moved[(them->x-me->x)+1][(them->y-me->y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[them->x][them->y-1].obj!=NULL) {
        return spin_cw(me,currentbrd->board[them->x][them->y-1].obj);
      } else {
        return 0;
      }
    }
  } else if(them->y==me->y+1 && (them->x==me->x-1 || them->x==me->x)) {
    if(is_empty(currentbrd,them->x+1,them->y)) {
      move(them,RIGHT);
      moved[(them->x-me->x)+1][(them->y-me->y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[them->x+1][them->y].obj!=NULL) {
        return spin_cw(me,currentbrd->board[them->x+1][them->y].obj);
      } else {
        return 0;
      }
    }
  } else if(them->x==me->x-1 && (them->y==me->y-1 || them->y==me->y)) {
    if(is_empty(currentbrd,them->x,them->y+1)) {
      move(them,DOWN);
      moved[(them->x-me->x)+1][(them->y-me->y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[them->x][them->y+1].obj!=NULL) {
        return spin_cw(me,currentbrd->board[them->x][them->y+1].obj);
      } else {
        return 0;
      }
    }
  }
  return 0;
}

update_handler spinner_update(struct object *me) {
  int x,y;
  if(me->type==ZZT_SPINNER_CW) {
    me->arg1++;
    me->arg1%=4;
  } else {
    me->arg1--;
    if(me->arg1<0) me->arg1=3;
  }
  me->shape=spin_anim[me->arg1];
  draw_block(me->x,me->y);
	
  for(y=0;y<3;y++) {
    for(x=0;x<3;x++) {
      moved[x][y]=0;
    }
  }
	
  for(y=0;y<3;y++) {
    for(x=0;x<3;x++) {
      if(moved[x][y]==0 && !(x==1 && y==1)) {
        if(currentbrd->board[me->x+(x-1)][me->y+(y-1)].obj!=NULL) {
          if(me->type==ZZT_SPINNER_CW) {
						if(spin_cw(me,currentbrd->board[me->x+(x-1)][me->y+(y-1)].obj)) currentbrd->board[me->x+(x-1)][me->y+(y-1)].obj->pushed=1;
					}
          else if(me->type==ZZT_SPINNER_CCW) {
						if(spin_ccw(me,currentbrd->board[me->x+(x-1)][me->y+(y-1)].obj)) currentbrd->board[me->x+(x-1)][me->y+(y-1)].obj->pushed=1;
					}
        }
      }
    }
  }
  return 0;
}
