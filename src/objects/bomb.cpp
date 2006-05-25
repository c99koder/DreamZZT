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

void Bomb::message(ZZTObject *them, std::string message) {
  if(them->getType()==ZZT_PLAYER && message == "touch" && m_shape==ZZT_BOMB_SHAPE) {
    m_shape='9';
    m_flags|=F_PUSHABLE;
    draw();
		zm->setTune("tcf+cf+c");
		zm->start();
  }
}

void Bomb::update() {
  int x,y;
  double a,b;
  ZZTObject *them;

  if(m_shape <= '9' && m_shape >= '1') {
    m_shape--;
    m_cycle=4;
    m_counter=4;
    draw();
  }
	
  if(m_shape=='0' && m_counter>0) {
    m_counter--;
    m_cycle=1;

    for(y=m_position.y-4;y<=m_position.y+4;y++) {
      for(x=m_position.x-7;x<=m_position.x+7;x++) {
        a=((x-m_position.x)*(x-m_position.x))/2.0f;
        b=(y-m_position.y)*(y-m_position.y);

        if((int)sqrt(a+b) == 4-m_counter) {
          if(x>=0 && y >= 0 && x<BOARD_X && y<BOARD_Y) {
            them=currentbrd->board[x][y].obj;
            if(them->getType()==ZZT_BREAKABLE || ::is_empty(currentbrd,x,y)) {
              currentbrd->board[x][y].obj=::create_object(ZZT_EXPLOSION,x,y);
              currentbrd->board[x][y].obj->create();
              draw_block(x,y);
            } else if(them!=NULL) {
              them->message(this,"bombed");
            }
          }
        }
      }
    }
  }
  if(m_counter<=0) {
    remove_from_board(currentbrd,this);
  }
}

void Bomb::create() {
  m_counter=4;
}

void Explosion::create() {
  m_counter=4;
  m_shape=176;
  m_fg=7;
  m_bg=0;
  m_cycle=1;
}

void Explosion::update() {
  switch(m_counter) {
  case 4:
    m_fg=7;
    m_bg=0;
    break;
  case 3:
    m_fg=15;
    m_bg=0;
    break;
  case 2:
    m_fg=14;
    m_bg=0;
    break;
  case 1:
    m_fg=12;
    m_bg=0;
    break;
  case 0:
    m_fg=0;
    m_bg=0;
    remove_from_board(currentbrd,this);
    break;
  }
  m_counter--;
  draw();
}
