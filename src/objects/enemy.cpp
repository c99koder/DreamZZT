/* enemy.cpp - ZZT Enemys
 * Copyright (C) 2000 - 2007 Sam Steele
 *
 * This file is part of DreamZZT.
 *
 * DreamZZT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * DreamZZT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */ 

#include <Tiki/tiki.h>
#include <Tiki/hid.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Audio;

#include "object.h"
#include "board.h"
#include "status.h"
#include "sound.h"
#include "debug.h"
#include "task.h"

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;
extern Player *player;

void Lion::update() {
  if(player != NULL && rand()%10<m_intel) {
    move(toward(player));
  } else {
    move((direction)((rand()%4)+1));
  }
}

void Bear::update() {
  if(player != NULL && (dist_x(player) < (9-m_intel) || dist_y(player) < (9-m_intel))) {
    move(toward(player));
  }
}

void Ruffian::setParam(int arg, unsigned char val) {
	if(arg == 2) m_rest = val;

	Enemy::setParam(arg,val);
}

unsigned char Ruffian::getParam(int arg) {
	if(arg == 2) return m_rest;
	
	return Enemy::getParam(arg);
}

void Ruffian::update() {
	if(m_moveCounter-- > 0) {
		move(m_heading);
	} else if(m_restCounter-- <= 0) {
		if(rand()%9 < m_intel) {
			m_heading = toward(player);
		} else {
			m_heading = (direction)(((rand()%4)+1));
		}
		m_moveCounter = rand() % 10;
	} else if(m_moveCounter <= 0) {
		m_restCounter = 6 + (rand() % m_rest);
	}
}

char gun[4] = {27,24,26,25};

void SpinningGun::setParam(int arg, unsigned char val) {
	if(arg == 2) m_rate = val;
	
	Enemy::setParam(arg,val);	
}

unsigned char SpinningGun::getParam(int arg) {
	if(arg == 2) return m_rate;
	
	return Enemy::getParam(arg);
}

void SpinningGun::update() {
  m_animIndex++;
  m_animIndex%=4;
  m_shape=gun[m_animIndex];
  draw();
	
  if(player != NULL && rand()%10<m_rate) {
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

void Tiger::setParam(int arg, unsigned char val) {
	if(arg == 2) m_rate = val;
	
	Enemy::setParam(arg,val);
}

unsigned char Tiger::getParam(int arg) {
	if(arg == 2) return m_rate;
	
	return Enemy::getParam(arg);
}

void Tiger::update() {
	Lion::update();

  if(rand()%10<m_rate) {
		if(player != NULL && (((m_position - player->getPosition()).x >= -1 && (m_position - player->getPosition()).x <= 1) ||
			 ((m_position - player->getPosition()).y >= -1 && (m_position - player->getPosition()).y <= 1))) {
			shoot(toward(player));
		}
  }
}

void Enemy::message(ZZTObject *them, std::string message) {
  if((
			(message == "shot" && them->getType() == ZZT_BULLET && them->getParam(1) == 0)
			 || them->getType()==ZZT_PLAYER || message == "bombed") && m_type != ZZT_SPINNING_GUN) {
    give_score(2);
    draw_score();
    if(message == "touch" || message == "thud") {
      them->message(this,"shot");
    } 
		zm->setTune("t+c---c++++c--c");
		zm->start();
		debug("\x1b[0;37mA \x1b[1;37m%s\x1b[0;37m was killed.\n",m_name.c_str());
		task_kill(this);
    remove_from_board(currentbrd,this);
  }
	if(m_type == ZZT_BEAR && them->getType() == ZZT_BREAKABLE) {
		remove_from_board(currentbrd,them);
		remove_from_board(currentbrd,this);
		zm->setTune("t+c-c-c");
		zm->start();
	}
}
