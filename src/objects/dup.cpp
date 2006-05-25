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
using namespace Tiki::Audio;

#include <stdlib.h>
#include "object.h"
#include "board.h"
#include "sound.h"

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;

char dupanim[5]={'.',7,'o','O','.' };

void Duplicator::setParam(int arg, int val) {
	if(arg==2) m_rate = val;
}

void Duplicator::update() {
  ZZTObject *obj;
  ZZTObject *obj2;
  int x=m_position.x - m_step.x ,y=m_position.y - m_step.y;

  m_shape=dupanim[m_animIndex++];
  draw();
	
  if(m_animIndex==5) {
    obj=currentbrd->board[(int)(m_position.x + m_step.x)][(int)(m_position.x + m_step.x)].obj;
    if(currentbrd->board[x][y].obj->getFlags()&F_PUSHABLE) {
      if(m_step.x==-1) currentbrd->board[x][y].obj->move(RIGHT);
      if(m_step.x==1) currentbrd->board[x][y].obj->move(LEFT);
      if(m_step.y==-1) currentbrd->board[x][y].obj->move(DOWN);
      if(m_step.y==1) currentbrd->board[x][y].obj->move(UP);
    }
    if(::is_empty(currentbrd,x,y)) {
      currentbrd->board[x][y].under=currentbrd->board[x][y].obj;
      currentbrd->board[x][y].obj=create_copy(obj);
      obj2=currentbrd->board[x][y].obj;
			obj2->setPosition(Vector(x,y,0));
      obj2->create();
      draw_block(x,y);
			zm->setTune("scdefg");
			zm->start();			
    }
  }
  m_animIndex%=5;
}

void Duplicator::create() {
  m_cycle=(10-m_rate)+2;
}
