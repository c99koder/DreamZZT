/* dup.cpp - Duplicator object
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

#include <stdlib.h>
#include "object.h"
#include "board.h"
#include "sound.h"

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;

char dupanim[6]={'.',(char)7,(char)248,'o','O','.' };

void Duplicator::setParam(int arg, unsigned char val) {
	if(arg==2) m_rate = val;
}

unsigned char Duplicator::param(int arg) {
	if(arg==2) return m_rate;
	return 0;
}

void Duplicator::update() {
	ZZTObject *obj;
	ZZTObject *obj2;
	int x = (int)(m_position.x - m_step.x), y = (int)(m_position.y - m_step.y);

	m_shape=dupanim[m_animIndex++];
	draw();
	
	if(m_animIndex==6) {
		obj=currentbrd->board[(int)(m_position.x + m_step.x)][(int)(m_position.y + m_step.y)].obj;
		if(currentbrd->board[x][y].obj->flags()&F_PUSHABLE || currentbrd->board[x][y].obj->type() == obj->type()) {
			if(m_step.x==-1) currentbrd->board[x][y].obj->move(RIGHT);
			if(m_step.x==1) currentbrd->board[x][y].obj->move(LEFT);
			if(m_step.y==-1) currentbrd->board[x][y].obj->move(DOWN);
			if(m_step.y==1) currentbrd->board[x][y].obj->move(UP);
		}
		if(::is_empty(currentbrd,x,y)) {
			currentbrd->board[x][y].under=currentbrd->board[x][y].obj;
			currentbrd->board[x][y].obj=create_copy(obj);
			obj2=currentbrd->board[x][y].obj;
			obj2->setPosition(Vector((float)x,(float)y,0));
			obj2->create();
			draw_block(x,y);
			zm->setTune("scdefg");
			zm->start();			
		} else {
			zm->setTune("--g#f#");
			zm->start();
		}
	}
	m_animIndex%=6;
}

void Duplicator::create() {
	if(m_cycle > 0) m_cycle=(9-m_rate)*2;
}
