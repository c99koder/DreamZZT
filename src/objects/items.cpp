/* items.cpp - ZZT items
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

#include "board.h"
#include "object.h"
#include "status.h"
#include "sound.h"
#include "debug.h"

extern ZZTMusicStream *zm;

extern struct world_header world;
extern struct board_info_node *currentbrd;
extern Player *player;

int gemmsg=0,torchmsg=0,ammomsg=0,enermsg=0;

void Transporter::create() {
	m_anim=12;
	m_counter=0;
	m_heading=UP;
	if(m_step.x==1) {
		m_shape='>';
		m_anim=0;
		m_heading=RIGHT;
	}
	if(m_step.x==-1) {
		m_shape='<';
		m_anim=4;
		m_heading=LEFT;
	}
	if(m_step.y==1) {
		m_shape='v';
		m_anim=8;
		m_heading=DOWN;
	}
	if(m_step.y==-1) {
		m_shape='^';
		m_anim=12;
		m_heading=UP;
	}
}

char teleport_anim[16]={ '|',')','>',')', '|','(','<','(','-','v','_','v','-','^','~','^' };

void Transporter::update() {
	m_shape=teleport_anim[m_anim+m_counter];
	m_counter++;
	m_counter%=4;
	draw();
}

void Transporter::message(ZZTObject *them, std::string message) {
	int x = (int)m_position.x, y = (int)m_position.y;
	if(them->type()==ZZT_PLAYER && them->heading()==m_heading && message == "touch") {
		do {
			x += (int)m_step.x;
			y += (int)m_step.y;

			if(x<=0 || y<=0 || x>=BOARD_X || y>=BOARD_Y || ::is_empty(currentbrd,(m_position + m_step).x,(m_position + m_step).y)) {
				remove_from_board(currentbrd, them);
				them->clearFlag(F_DELETED);
				currentbrd->board[(int)them->position().x][(int)them->position().y].under = NULL;
				them->setPosition(m_position + m_step);
				put(them, them->position().x, them->position().y);
				break;
			} else if(currentbrd->board[x][y].obj->type()==ZZT_TRANSPORTER &&
					  currentbrd->board[x][y].obj->step().x==m_step.x*-1 &&
					  currentbrd->board[x][y].obj->step().y==m_step.y*-1) {
				remove_from_board(currentbrd, them);
				them->clearFlag(F_DELETED);
				currentbrd->board[(int)them->position().x][(int)them->position().y].under = NULL;
				them->setPosition(currentbrd->board[x][y].obj->position() + m_step);
				put(them, them->position().x, them->position().y);
				break;
			}
		} while(x>0 && x<BOARD_X && y>0 && y<BOARD_Y);
		if(zm!=NULL)
			zm->setTune("tc+d-e+f#-g#+a#c+d");
		if(zm!=NULL)
			zm->start();
	}
}

void Scroll::message(ZZTObject *them, std::string message) {
	if(message == "get") {
		if(zm!=NULL)
			zm->setTune("tc-c+d-d+e-e+f-f+g-g");
		if(zm!=NULL)
			zm->start();
		m_progpos = 0;
		update();
	}
}

void Scroll::update() {
	m_shape=0xE8;
	m_fg++;
	if(m_fg>15)
		m_fg=9;
	if(m_progpos>=0) {
		ZZTOOP::update();
		remove_from_board(currentbrd,this);
		player->move(toward(this));
	}
	draw();
}

bool Energizer::get() {
	world.energizer_cycle=80;
	if(enermsg==0) {
		enermsg=1;
		set_msg("Energizer - You are invincible!");
	}
	if(zm!=NULL)
		zm->setTune("s.-cd#e@s.-f+f-fd#c+c-d#ef+f-fd#c+c-d#e@s.-f+f-fd#c+c-d#ef+f-fd#c+c-d#e@s.-f+f-fd#c+c-d#ef+f-fd#c+c-d#e@s.-f+f-fd#c+c-d#ef+f-fd#c+c-d#e@s.-f+f-fd#c+c-d#e");
	if(zm!=NULL)
		zm->lock();
	if(zm!=NULL)
		zm->start();
	player->send("all:energize");
	return true;
}

bool Gem::get() {
	give_gems(1);
	give_health((world.magic == MAGIC_SZT)? 10 : 1);
	give_score(10);
	if(gemmsg==0) {
		gemmsg=1;
		set_msg("Gems give you Health!");
	}
	if(zm!=NULL)
		zm->setTune("t+c-gec");
	if(zm!=NULL)
		zm->start();
	return true;
}

bool Ammo::get() {
	give_ammo((world.magic == 65534) ? 10 : 5);
	if(ammomsg==0) {
		ammomsg=1;
		set_msg((char *)((world.magic == 65534) ? "Ammunition:\r10 shots" : "Ammunition - 5 shots per container."));
	}
	if(zm!=NULL)
		zm->setTune("tcc#d");
	if(zm!=NULL)
		zm->start();
	return true;
}

bool Torch::get() {
	give_torch(1);
	if(torchmsg==0) {
		torchmsg=1;
		set_msg("Torch - used for lighting in the underground.");
	}
	if(zm!=NULL)
		zm->setTune("tcase");
	if(zm!=NULL)
		zm->start();
	return true;
}

bool Stone::get() {
	give_z(1);
	set_msg("You have found a\rStone of Power!");
	if(zm!=NULL)
		zm->setTune("tcase");
	if(zm!=NULL)
		zm->start();
	return true;
}

void Stone::update() {
	m_fg++;
	if(m_fg>15)
		m_fg=9;
	m_shape = 'A' + rand() % 26;
	draw();
}

bool Key::get() {
	char tmp[128];
	if(world.keys[(m_fg%8)-1]==1) {
		sprintf(tmp,"You already have a %s key!",int_to_color(*m_color).c_str());
		set_msg(tmp);
		if(zm!=NULL)
			zm->setTune("sc-c");
		if(zm!=NULL)
			zm->start();
		return false;
	} else {
		world.keys[(m_fg%8)-1]=1;
		sprintf(tmp,"You now have the %s key",int_to_color(*m_color).c_str());
		set_msg(tmp);
		draw_keys();
		if(zm!=NULL)
			zm->setTune("t+cegcegceg+sc");
		if(zm!=NULL)
			zm->start();
		return true;
	}
}

bool Door::get() {
	char tmp[128];
	if(world.keys[(m_bg%8)-1]==1) {
		sprintf(tmp,"The %s door is now open!",int_to_color(*m_color).c_str());
		set_msg(tmp);
		world.keys[(m_bg%8)-1]=0;
		draw_keys();
		if(zm!=NULL)
			zm->setTune("tcgbcgb+ic");
		if(zm!=NULL)
			zm->start();
		return true;
	} else {
		sprintf(tmp,"The %s door is locked.",int_to_color(*m_color).c_str());
		set_msg(tmp);
		if(zm!=NULL)
			zm->setTune("t--gc");
		if(zm!=NULL)
			zm->start();
		return false;
	}
}

void Inventory::message(ZZTObject *them, std::string message) {
	if(message == "get") {
		if(get()) {
			debug("\x1b[0;37mA \x1b[1;37m%s\x1b[0;37m was picked up.\n",m_name.c_str());
			remove();
		}
	} else if(message == "shot" && type() == ZZT_GEM && (them->name() == "player" || (them->name() == "bullet" && them->param(1) == 0))) {
		printf("Shot by %s\n",them->name().c_str());
		remove();
	}
}
