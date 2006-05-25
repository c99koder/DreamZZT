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
using namespace Tiki::Audio;

#include "object.h"
#include "board.h"
#include "status.h"
#include "sound.h"
#include "debug.h"

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;
extern Player *player;

void Lion::update() {
  if(rand()%10<m_intel) {
    move(toward(player));
  } else {
    move((direction)(rand()%4));
  }
}

void Bear::update() {
  if(dist_x(player) < (9-m_intel) || dist_y(player) < (9-m_intel)) {
    move(toward(player));
  }
}

void Ruffian::setParam(int arg, int val) {
	if(arg == 2) m_rest = val;

	Enemy::setParam(arg,val);
}

void Ruffian::update() {
	if(m_moveCounter-- > 0) {
		move(m_heading);
	} else if(m_restCounter-- <= 0) {
		if(rand()%9 < m_intel) {
			m_heading = toward(player);
		} else {
			m_heading = (direction)(rand() % 4);
		}
		m_moveCounter = rand() % 10;
	} else if(m_moveCounter <= 0) {
		m_restCounter = 6 + (rand() % m_rest);
	}
}

char gun[4] = {27,24,26,25};

void SpinningGun::setParam(int arg, int val) {
	if(arg == 2) m_rate = val;
	
	Enemy::setParam(arg,val);	
}

void SpinningGun::update() {
  m_animIndex++;
  m_animIndex%=4;
  m_shape=gun[m_animIndex];
  draw();
	
  if(rand()%10<m_rate) {
    if(rand()%10<m_intel) {
      if(((m_position - player->getPosition()).x >= -2 && (m_position - player->getPosition()).x <= 2) ||
        ((m_position - player->getPosition()).y >= -2 && (m_position - player->getPosition()).y <= 2)) {
        shoot(toward(player));
      }
    } else {
      if(rand()%(m_intel+2)==0) {
        shoot(toward(player));
      }
    }
  }
}

void Tiger::setParam(int arg, int val) {
	if(arg == 2) m_rate = val;
	
	Enemy::setParam(arg,val);
}

void Tiger::update() {
	Lion::update();

  if(rand()%10<m_rate) {
		if(((m_position - player->getPosition()).x >= -1 && (m_position - player->getPosition()).x <= 1) ||
			 ((m_position - player->getPosition()).y >= -1 && (m_position - player->getPosition()).y <= 1)) {
			shoot(toward(player));
		}
  }
}

void Enemy::message(ZZTObject *them, std::string message) {
  if(message == "shot" || them->getType()==ZZT_PLAYER || message == "bombed") {
    give_score(2);
    draw_score();
    if(message == "touch" || message == "thud") {
      them->message(this,"shot");
    } 
		debug("\x1b[0;37mA \x1b[1;37m%s\x1b[0;37m was killed.\n",m_name.c_str());
    remove_from_board(currentbrd,this);
  }
	if(m_type == ZZT_BEAR && them->getType() == ZZT_BREAKABLE) {
		remove_from_board(currentbrd,them);
		remove_from_board(currentbrd,this);
		zm->setTune("t+c-c-c");
		zm->start();
	}
}
