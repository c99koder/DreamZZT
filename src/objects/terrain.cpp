/* terrain.cpp - ZZT terrain
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

#include "status.h"
#include "object.h"
#include "board.h"
#include "sound.h"

extern ZZTMusicStream *zm;
extern ConsoleText *ct;

extern struct board_info_node *currentbrd;
int forestmsg=0,invismsg=0,watermsg=0;

void Laser::setParam(int arg, unsigned char val) {
	if(arg==1) m_time = val;
}

unsigned char Laser::param(int arg) {
	if(arg == 1) return m_time;
	return 0;
}

void Laser::update() {
	m_time--;
	if(m_time<=0) {
		remove_from_board(currentbrd,this);
	}
}

void Blink::setParam(int arg, unsigned char val) {
	if(arg==1) m_start = val;
	if(arg==2) m_fire = val;
}

unsigned char Blink::param(int arg) {
	if(arg == 1) return m_start;
	if(arg == 2) return m_fire;
	return 0;
}


void Blink::create() {
	m_counter = m_start;
}

void Blink::update() {
	int x = (int)m_position.x;
	int y = (int)m_position.y;
	ZZTObject *tmp;
	
	m_counter--;
	if(m_counter<0) {
		m_counter=(m_fire*4);
		do {
			x += (int)m_step.x;
			y += (int)m_step.y;
			if(y >= BOARD_Y || x >= BOARD_X) break;
			if(::is_empty(currentbrd,x,y) || currentbrd->board[x][y].obj->type()==ZZT_BULLET) {
				if(currentbrd->board[x][y].obj->type()==ZZT_BULLET) {
					remove_from_board(currentbrd,currentbrd->board[x][y].obj);
				}
				if(m_step.y!=0) {
					tmp=::create_object(ZZT_VERTICAL_BLINK,x,y);
				} else {
					tmp=::create_object(ZZT_HORIZONTAL_BLINK,x,y);
				}
				tmp->setParam(1,m_fire*2);
				tmp->setFg(m_fg);
				tmp->setBg(m_bg);
				tmp->setCycle(m_cycle);
				currentbrd->board[x][y].under=currentbrd->board[x][y].obj;
				currentbrd->board[x][y].obj=tmp;
				draw_block(x,y);
			} else {
				if(currentbrd->board[x][y].obj!=NULL) {
					currentbrd->board[x][y].obj->message(this,"shot");
				}
				break;
			}
		} while(1);
	}
}

void Text::create() {
	m_shape=m_bg*16+m_fg;
	m_fg=15;
	if(m_type==ZZT_WHITE_TEXT) {
		m_bg=0;
		m_color=&m_fg;
	} else {
		m_bg=(m_type-ZZT_BLUE_TEXT)+1;
		m_color=&m_bg;
	}
}

void Terrain::create() {
	/*if(!m_model) {
		m_model = new AMFModelInstance("block.amf", m_position, Vector(1.0f, 0.0f, 0.0f));
		gl->insert(m_model);
	}*/

	switch(m_type) {
	case ZZT_EMPTY:
		m_shape=32;
		m_fg=0;
		m_bg=0;
		break;
	case ZZT_WATER:
		//m_bg=1;
		//m_fg=15;
		m_cycle=1;
		break;
	}
}

void Water::update() {
	if(currentbrd->animatedWater) {
		if(rand()%8==1) {
			m_counter=1;
			m_cstep=1;
		}
		switch(m_counter) {
		case 0:
			m_cstep=0;
			m_fg=1;
			m_shape=0xb0;
			break;
		case 1:
			m_fg=9;
			m_shape=0xb0;
			break;
		case 2:
			m_fg=9;
			m_shape=0xb1;
			break;
		case 3:
			m_fg=9;
			m_shape=0xb2;
			break;
		case 4:
			m_fg=9;
			m_shape=0xdb;
			m_cstep=-1;
			break;
		}
		m_counter+=m_cstep;
		draw();
	}
}

void Terrain::message(ZZTObject *them, std::string message) {
	if(message == "touch" && them->type()==ZZT_PLAYER) {
		switch(m_type) {
		case ZZT_FOREST:
			if(forestmsg==0) {
				set_msg("A path is cleared through the forest.");
				forestmsg=1;
			}
			zm->setTune("ta");
			zm->start();
			remove_from_board(currentbrd,this);
			them->move(them->toward(this));			
			break;
		case ZZT_INVISIBLE:
			if(invismsg==0) {
				set_msg("You are blocked by an invisible wall.");
				invismsg=1;
			}
			zm->setTune("t--dc");
			zm->start();
			m_type=ZZT_NORMAL;
			m_shape=ZZT_NORMAL_SHAPE;
			draw();
			break;
		case ZZT_WATER:
			if(watermsg==0) {
				set_msg("Your path is blocked by water.");
				watermsg=1;
			}
			zm->setTune("t+c+c");
			zm->start();
		}
	}
}
