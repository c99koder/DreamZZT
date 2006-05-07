/*
 *  dirxn.cpp
 *  Direction and collision code
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
#include "word.h"

extern int zztascii[];
extern struct object *player;

enum direction opposite(enum direction dir) {
  if(dir==LEFT) { return RIGHT; }
  if(dir==RIGHT) { return LEFT; }
  if(dir==UP) { return DOWN; }
  if(dir==DOWN) { return UP; }
}

enum direction clockwise(enum direction dir) {
  if(dir==LEFT) { return UP; }
  if(dir==RIGHT) { return DOWN; }
  if(dir==UP) { return RIGHT; }
  if(dir==DOWN) { return LEFT; }
}

enum direction str_to_direction(char *dir) {
  if(!strcmp(dir,"n")||!strcmp(dir,"north")) {
    return UP;
  }
  if(!strcmp(dir,"s")||!strcmp(dir,"south")) {
    return DOWN;
  }
  if(!strcmp(dir,"e")||!strcmp(dir,"east")) {
    return RIGHT;
  }
  if(!strcmp(dir,"w")||!strcmp(dir,"west")) {
    return LEFT;
  }
  return (direction)5;
}

enum direction word_to_direction(struct object *me, int word) {
  int neg=0;
  direction res=(direction)5;
  if(!strcmp(get_word(word),"opp")||!strcmp(get_word(word),"opposite")) {
    neg=1;
		printf("Opposite!\n");
    delete_word(word);
  }
  if(!strcmp(get_word(word),"cw")||!strcmp(get_word(word),"opposite")) {
    neg=2;
    delete_word(word);
  }
  if(!strcmp(get_word(word),"ccw")||!strcmp(get_word(word),"opposite")) {
    neg=3;
    delete_word(word);
  }
  if(!strcmp(get_word(word),"rndp")||!strcmp(get_word(word),"opposite")) {
    if(rand()%2==0) {
      neg=2;
    } else {
      neg=3;
    }
    delete_word(word);
  }
  if(!strcmp(get_word(word),"flow")) {
    res=(direction)me->heading;
  } else if(!strcmp(get_word(word),"seek")) {
    res=toward(me,player);
  } else if(!strcmp(get_word(word),"rndns")) {
    if(rand()%2==1) {
      res=UP;
    } else {
      res=DOWN;
    }
  } else if(!strcmp(get_word(word),"rndne")) {
    if(rand()%2==1) {
      res=UP;
    } else {
      res=RIGHT;
    }
  } else {
    res=str_to_direction(get_word(word));
  }
  if(neg==1) {
		printf("Before: %i\n",res);
    res=opposite(res);
		printf("After: %i\n",res);
  }
  if(neg==2) {
    res=clockwise(res);
  }
  if(neg==3) {
    res=opposite(clockwise(res));
  }
  return res;
}

int dir_to_delta[4] = { -1,1,-1,1 };
extern struct board_info_node *currentbrd;

enum direction toward(struct object *me, struct object *them) {
  //relative direction between me and them
  int distx=0,disty=0;
  enum direction dirx=(direction)5,diry=(direction)5;
  distx=me->x - them->x;
  disty=me->y - them->y;
  if(distx < 0) { dirx=RIGHT; }
  if(distx > 0) { dirx=LEFT; }
  if(disty < 0) { diry=DOWN; }
  if(disty > 0) { diry=UP; }
  if(distx<0) { distx*=-1; }
  if(disty<0) { disty*=-1; }
  if(abs(distx)>abs(disty)) {
		if(!is_empty(currentbrd,me->x+dir_to_delta[dirx],me->y) && 
			(diry!=5 && is_empty(currentbrd,me->x,me->y+dir_to_delta[diry])))
			return diry;
		else 
			return dirx;
	} else { 
		if(!is_empty(currentbrd,me->x,me->y+dir_to_delta[diry]) && 
			(dirx!=5 && is_empty(currentbrd,me->x+dir_to_delta[diry],me->y)))
			return dirx;
		else 
			return diry;
	}
}

int distance(struct object *me, struct object *them) {
  //distance between me and them
  int distx,disty;
  distx=me->x - them->x;
  disty=me->y - them->y;
  if(distx<0) { distx*=-1; }
  if(disty<0) { disty*=-1; }
  return distx+disty;
}

int dist_x(struct object *me, struct object *them) {
  //distance between me and them (x axis only)
  int distx;
  distx=me->x - them->x;
  if(distx<0) { distx*=-1; }
  return distx;
}

int dist_y(struct object *me, struct object *them) {
  //distance between me and them (x axis only)
  int disty;
  disty=me->y - them->y;
  if(disty<0) { disty*=-1; }
  return disty;
}

int dist(int x1, int y1, int x2, int y2) {
  int a,b;
  a=(x1-x2)*(x1-x2);
  b=(y1-y2)*(y1-y2);
  return sqrt((float)a+b);
}

void move(struct object *me, enum direction dir) {
  //move an AOE object in the specified direction
  struct object *them=NULL;
  struct board_info_node *board=get_current_board();
  int d;
  int x,y,oldx,oldy,suc=0;
  char tmp[200];
  if(me==NULL) {
    printf("Warning: invalid object\n");
    return;
  }
  if(me->type==ZZT_SLIDER_NS&&(dir==LEFT||dir==RIGHT)) {
    return;
  }
  if(me->type==ZZT_SLIDER_EW&&(dir==UP||dir==DOWN)) {
    return;
  }
  oldx=me->x;
  oldy=me->y;
  x=me->x;
  y=me->y;
  switch(dir) {
  case LEFT:
    x=me->x-1;
    break;
  case RIGHT:
    x=me->x+1;
    break;
  case UP:
    y=me->y-1;
    break;
  case DOWN:
    y=me->y+1;
    break;
	case 5: //Idle
		break;
  default:
    printf("Unknown direction %i\n",dir);
    break;
  }
  if(x<0) { x=0; }
  if(y<0) { y=0; }
  if(x>=BOARD_X) { x=BOARD_X-1; }
  if(y>=BOARD_Y) { y=BOARD_Y-1; }
  //printf("dir: %i oldx: %i oldy: %i newx: %i newy: %i\n",dir,me->x,me->y,x,y);
  if(is_empty(board,x,y) || (me->type==ZZT_BULLET && board->board[x][y].obj->type==ZZT_WATER)) {
    me->x=x;
    me->y=y;
    me->heading=dir;
    board->board[x][y].under=board->board[x][y].obj;
    board->board[x][y].obj=me;
    me->bg=board->board[x][y].under->bg;
    board->board[oldx][oldy].obj=board->board[oldx][oldy].under;
		board->board[oldx][oldy].under=NULL;
    if(board->board[oldx][oldy].obj==NULL) {
      board->board[oldx][oldy].obj=create_object(ZZT_EMPTY,oldx,oldy);
    }
#ifdef MSDOS
    draw_block(x,y);
    draw_block(oldx,oldy);
#endif
  } else {
    them=board->board[x][y].obj;
    if(them!=NULL && them!=me) {
      //printf("Collision between %i and %i!\n",me->id,them->id);
      if(them->flags&F_ITEM && me->type==ZZT_PLAYER){
        //printf("Tryingto get...\n");
        them->message(them,me,"get");
        if(is_empty(board,x,y)) {
          //printf("Success!\n");
          suc=1;
        }
      }
      else if(suc==0 && them->flags&F_PUSHABLE && me->type!=ZZT_BULLET/*&& me->flags&F_PUSHER*/) {
        //printf("Trying to push...\n");
        if(!(me->flags&F_ENEMY) || (me->flags&F_ENEMY && them->type!=ZZT_PLAYER)) {
          move(them,dir);
          if(is_empty(board,x,y)) {
            //printf("Success!\n");
            //move(me,dir);
            suc=1;
          }
        }
      }
      if(suc==0) {
        if(me->message!=NULL&&!(them->flags&F_SLEEPING)) me->message(me,them,"thud");
        if(them->message!=NULL&&!(them->flags&F_SLEEPING)) them->message(them,me,"touch");
      } else {
        move(me,dir);
      }
    } else {
      //printf("Object %i hit a wall\n",me->id);
      if(me->message!=NULL) me->message(me,me,"thud");
    }
  }
}

int try_move(struct object *me, enum direction dir) {
  struct object *them=NULL;
  struct board_info_node *board=get_current_board();
  int d;
  int x,y,oldx,oldy,suc=0;
  char tmp[200];
  if(me==NULL) {
    printf("Warning: invalid object\n");
    return 0;
  }
  if(me->type==ZZT_SLIDER_NS&&(dir==LEFT||dir==RIGHT)) {
    return 0;
  }
  if(me->type==ZZT_SLIDER_EW&&(dir==UP||dir==DOWN)) {
    return 0;
  }
  oldx=me->x;
  oldy=me->y;
  x=me->x;
  y=me->y;
  switch(dir) {
  case LEFT:
    x=me->x-1;
    break;
  case RIGHT:
    x=me->x+1;
    break;
  case UP:
    y=me->y-1;
    break;
  case DOWN:
    y=me->y+1;
    break;
  default:
    printf("Unknown direction %i\n",dir);
    break;
  }
  if(x<0) { return 0; }
  if(y<0) { return 0; }
  if(x>=BOARD_X) { return 0; }
  if(y>=BOARD_Y) { return 0; }
  //printf("dir: %i oldx: %i oldy: %i newx: %i newy: %i\n",dir,me->x,me->y,x,y);
  if(is_empty(board,x,y) || (me->type==ZZT_BULLET && board->board[x][y].obj->type==ZZT_WATER)) {
		return 1;
	} else {
    them=board->board[x][y].obj;
    if(them!=NULL && them!=me) {
      //printf("Collision between %i and %i!\n",me->id,them->id);
      if(suc==0 && them->flags&F_PUSHABLE/* && me->flags&F_PUSHER*/) {
        return try_move(them,dir);
      } else {
				return 0;
			}
    } else {
      //printf("Object %i hit a wall\n",me->id);
      //if(me->message!=NULL) me->message(me,me,"thud");
			return 0;
    }
  }
	return 1;
}
