/* conveyer.cpp - Conveyer object
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

#include "object.h"
#include "board.h"

extern struct board_info_node *currentbrd;

extern char spin_anim[4];
char moved[3][3];

int ConveyerCW::cw(ZZTObject *them) {
	if(!(them->flags()&F_PUSHABLE) || them->pushed())
		return 0;

	if((int)them->position().y==(int)m_position.y-1 && ((int)them->position().x==(int)m_position.x-1||(int)them->position().x==(int)m_position.x)) {
		if(currentbrd->board[(int)them->position().x+1][(int)them->position().y].obj->type() == ZZT_EMPTY) {
			them->move(RIGHT);
			moved[((int)them->position().x-(int)m_position.x)+1][((int)them->position().y-(int)m_position.y)+1]=1;
			return 1;
		} else {
			if(currentbrd->board[(int)them->position().x+1][(int)them->position().y].obj!=NULL) {
				return cw(currentbrd->board[(int)them->position().x+1][(int)them->position().y].obj);
			} else {
				return 0;
			}
		}
	} else if((int)them->position().x==(int)m_position.x+1 && ((int)them->position().y==(int)m_position.y-1 || (int)them->position().y==(int)m_position.y)) {
		if(currentbrd->board[(int)them->position().x][(int)them->position().y+1].obj->type() == ZZT_EMPTY) {
			them->move(DOWN);
			moved[((int)them->position().x-(int)m_position.x)+1][((int)them->position().y-(int)m_position.y)+1]=1;
			return 1;
		} else {
			if(currentbrd->board[(int)them->position().x][(int)them->position().y+1].obj!=NULL) {
				return cw(currentbrd->board[(int)them->position().x][(int)them->position().y+1].obj);
			} else {
				return 0;
			}
		}
	} else if((int)them->position().y==(int)m_position.y+1 && ((int)them->position().x==(int)m_position.x+1 || (int)them->position().x==(int)m_position.x)) {
		if(currentbrd->board[(int)them->position().x-1][(int)them->position().y].obj->type() == ZZT_EMPTY) {
			them->move(LEFT);
			moved[((int)them->position().x-(int)m_position.x)+1][((int)them->position().y-(int)m_position.y)+1]=1;
			return 1;
		} else {
			if(currentbrd->board[(int)them->position().x-1][(int)them->position().y].obj!=NULL) {
				return cw(currentbrd->board[(int)them->position().x-1][(int)them->position().y].obj);
			} else {
				return 0;
			}
		}
	} else if((int)them->position().x==(int)m_position.x-1 && ((int)them->position().y==(int)m_position.y+1 || (int)them->position().y==(int)m_position.y)) {
		if(currentbrd->board[(int)them->position().x][(int)them->position().y-1].obj->type() == ZZT_EMPTY) {
			them->move(UP);
			moved[((int)them->position().x-(int)m_position.x)+1][((int)them->position().y-(int)m_position.y)+1]=1;
			return 1;
		} else {
			if(currentbrd->board[(int)them->position().x][(int)them->position().y-1].obj!=NULL) {
				return cw(currentbrd->board[(int)them->position().x][(int)them->position().y-1].obj);
			} else {
				return 0;
			}
		}
	}
	return 0;
}

int ConveyerCCW::ccw(ZZTObject *them) {
	if(!(them->flags()&F_PUSHABLE) || them->pushed())
		return 0;

	if((int)them->position().y==(int)m_position.y-1 && ((int)them->position().x==(int)m_position.x+1||(int)them->position().x==(int)m_position.x)) {
		if(currentbrd->board[(int)them->position().x-1][(int)them->position().y].obj->type() == ZZT_EMPTY) {
			them->move(LEFT);
			moved[((int)them->position().x-(int)m_position.x)+1][((int)them->position().y-(int)m_position.y)+1]=1;
			return 1;
		} else {
			if(currentbrd->board[(int)them->position().x-1][(int)them->position().y].obj!=NULL) {
				return ccw(currentbrd->board[(int)them->position().x-1][(int)them->position().y].obj);
			} else {
				return 0;
			}
		}
	} else if((int)them->position().x==(int)m_position.x+1 && ((int)them->position().y==(int)m_position.y+1 || (int)them->position().y==(int)m_position.y)) {
		if(currentbrd->board[(int)them->position().x][(int)them->position().y-1].obj->type() == ZZT_EMPTY) {
			them->move(UP);
			moved[((int)them->position().x-(int)m_position.x)+1][((int)them->position().y-(int)m_position.y)+1]=1;
			return 1;
		} else {
			if(currentbrd->board[(int)them->position().x][(int)them->position().y-1].obj!=NULL) {
				return ccw(currentbrd->board[(int)them->position().x][(int)them->position().y-1].obj);
			} else {
				return 0;
			}
		}
	} else if((int)them->position().y==(int)m_position.y+1 && ((int)them->position().x==(int)m_position.x-1 || (int)them->position().x==(int)m_position.x)) {
		if(currentbrd->board[(int)them->position().x+1][(int)them->position().y].obj->type() == ZZT_EMPTY) {
			them->move(RIGHT);
			moved[((int)them->position().x-(int)m_position.x)+1][((int)them->position().y-(int)m_position.y)+1]=1;
			return 1;
		} else {
			if(currentbrd->board[(int)them->position().x+1][(int)them->position().y].obj!=NULL) {
				return ccw(currentbrd->board[(int)them->position().x+1][(int)them->position().y].obj);
			} else {
				return 0;
			}
		}
	} else if((int)them->position().x==(int)m_position.x-1 && ((int)them->position().y==(int)m_position.y-1 || (int)them->position().y==(int)m_position.y)) {
		if(currentbrd->board[(int)them->position().x][(int)them->position().y+1].obj->type() == ZZT_EMPTY) {
			them->move(DOWN);
			moved[((int)them->position().x-(int)m_position.x)+1][((int)them->position().y-(int)m_position.y)+1]=1;
			return 1;
		} else {
			if(currentbrd->board[(int)them->position().x][(int)them->position().y+1].obj!=NULL) {
				return ccw(currentbrd->board[(int)them->position().x][(int)them->position().y+1].obj);
			} else {
				return 0;
			}
		}
	}
	return 0;
}

void ConveyerCW::update() {
	int x,y;
	m_animIndex++;
	m_animIndex%=4;
	m_shape=spin_anim[m_animIndex];
	draw();

	for(y=0;y<3;y++) {
		for(x=0;x<3;x++) {
			moved[x][y]=0;
		}
	}

	for(y=0;y<3;y++) {
		for(x=0;x<3;x++) {
			if(moved[x][y]==0 && !(x==1 && y==1)) {
				if(currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj!=NULL) {
					if(cw(currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj))
						currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj->setPushed(1);
				}
			}
		}
	}
}

void ConveyerCCW::update() {
	int x,y;
	if(m_animIndex==0)
		m_animIndex=4;
	m_animIndex--;
	m_shape=spin_anim[m_animIndex];
	draw();

	for(y=0;y<3;y++) {
		for(x=0;x<3;x++) {
			moved[x][y]=0;
		}
	}

	for(y=0;y<3;y++) {
		for(x=0;x<3;x++) {
			if(moved[x][y]==0 && !(x==1 && y==1)) {
				if(currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj!=NULL) {
					if(ccw(currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj))
						currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj->setPushed(1);
				}
			}
		}
	}
}
