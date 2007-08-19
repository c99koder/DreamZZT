/* bullet.cpp - Bullet object
 * Copyright (c) 2000 - 2007 Sam Steele
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

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;
extern struct world_header world;

extern char ammo_name[100];

void bullet_sound_init() {
#ifdef DREAMCAST
	if(sound_id==-1) {
		sound_id=snd_sfx_load("/cd/fire.wav");
	}
#endif
}

void Bullet::setParam(int arg, unsigned char val) {
	if(arg == 1)
		m_owner = val;
	if(arg == 2)
		m_heading = (direction)val;
}

unsigned char Bullet::param(int arg) {
	if(arg == 1)
		return (unsigned char)m_owner;
	if(arg == 2)
		return (unsigned char)m_heading;
	return 0;
}

void Bullet::update() {
	move(m_heading);
}

void Bullet::message(ZZTObject *them, std::string message) {
	if(message == "thud") {
		them->message(this,"shot");
		if(them->type()==ZZT_BREAKABLE) {
			remove_from_board(currentbrd,them);
			if(zm!=NULL)
				zm->setTune("t-c");
			if(zm!=NULL)
				zm->start();
		}
		if(them->type()==ZZT_RICOCHET) {
			m_heading = opposite(m_heading);
			if(zm!=NULL && !zm->isPlaying()) {
				zm->setTune("t9");
				if(zm!=NULL)
					zm->start();
			}
			return;
		}
		for(int d = (int)LEFT; d <= (int)DOWN; d++) {
			ZZTObject *o=get
			             ((direction)d);
			if(o!=NULL && o->type() == ZZT_RICOCHET) {
				if(zm!=NULL && !zm->isPlaying()) {
					zm->setTune("t9");
					if(zm!=NULL)
						zm->start();
				}
				m_heading = opposite(toward(o));
				if(!is_empty(m_heading))
					remove_from_board(currentbrd, this);
				else
					(move(m_heading));
				return;
			}
		}
		if(them->type()==ZZT_BULLET && them != this)
			remove_from_board(currentbrd, them);
		remove_from_board(currentbrd,this);
	}
}

void ZZTObject::shoot(enum direction dir) {
	ZZTObject *bullet=NULL;
	int dx=0,dy=0;

	switch(dir) {
	case UP:
		dx=0;
		dy=-1;
		break;
	case DOWN:
		dx=0;
		dy=1;
		break;
	case LEFT:
		dx=-1;
		dy=0;
		break;
	case RIGHT:
		dx=1;
		dy=0;
		break;
	case IDLE:
	case SHOOTING:
		return;
	}

	if(m_type==ZZT_PLAYER) {
		if(world.ammo<1) {
			//sprintf(buf,"You don't have any %s",ammo_name);
			set_msg("You don't have any ammo");
			return;
		}
	}
	if(is_empty(dir) || currentbrd->board[(int)m_position.x+dx][(int)m_position.y+dy].obj->type() == ZZT_WATER) {
		if(m_type==ZZT_PLAYER) {
			world.ammo--;
			draw_ammo();
		}
		bullet=create_object(ZZT_BULLET,dir);
		bullet->setParam(1,m_type==ZZT_PLAYER?0:1);
		bullet->setHeading(dir);
		bullet->setCycle(1);
		put(bullet, bullet->position().x, bullet->position().y);
		if(zm != NULL && !zm->isPlaying()) {
			if(m_type==ZZT_PLAYER) {
				if(zm!=NULL)
					zm->setTune("t+c-c-c");
			} else {
				if(zm!=NULL)
					zm->setTune("c-f#");
			}
			if(zm!=NULL)
				zm->start();
		}
	} else {
		ZZTObject *o = get
			               (dir);
		if(o!=NULL) {
			if(o->type()==ZZT_BREAKABLE) {
				remove_from_board(currentbrd,o);
			} else if(o->type()!=ZZT_BULLET) {
				o->message(this,"shot");
			}
			if(is_empty(dir)) {
				if(m_type==ZZT_PLAYER) {
					world.ammo--;
					draw_ammo();
				}
			}
		}
	}
}

int Bullet::type() { return ZZT_BULLET; }
