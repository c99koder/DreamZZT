/* passage.cpp - Passage object
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
#include "sound.h"

extern int switchbrd;

extern ZZTMusicStream *zm;

void Passage::setParam(int arg, unsigned char val) {
	if(arg==3) m_dest = val;
}

unsigned char Passage::param(int arg) {
	if(arg == 3) return m_dest;
	return 0;
}

void Passage::message(ZZTObject *them, std::string message) {
	ZZTObject *obj;
	ZZTObject *obj2;
	board_info_node *brd=get_board(m_dest);
	Vector pos;
	
	if(message == "touch" && them->type()==ZZT_PLAYER && brd!=NULL) {
		decompress(brd);
		obj=get_obj_by_type(brd,ZZT_PLAYER);
		
		obj2=get_obj_by_color(brd,ZZT_PASSAGE,m_fg,m_bg);
		pos = obj->position();
		
		if(obj2!=NULL) {
			brd->board[(int)pos.x][(int)pos.y].obj=brd->board[(int)pos.x][(int)pos.y].under;
			brd->board[(int)pos.x][(int)pos.y].under=NULL;
			pos = obj2->position();
			obj->setPosition(pos);
			obj->setHeading(IDLE);
			them->setHeading(IDLE);
			brd->board[(int)pos.x][(int)pos.y].under=obj2;
			brd->board[(int)pos.x][(int)pos.y].obj=obj;
		}

		if(brd!=NULL) switchbrd=m_dest;
		obj->setFlag(F_SLEEPING);
		if(zm!=NULL) zm->setTune("tceg tc#fg# tdf#a td#ga# teg#+c");
		if(zm!=NULL) zm->start();
	}
}

void Passage::create() {
	m_color=&m_bg;
}