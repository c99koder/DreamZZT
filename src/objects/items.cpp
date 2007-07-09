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
	if(m_step.x==1) { m_shape='>'; m_anim=0; m_heading=RIGHT; }
	if(m_step.x==-1) { m_shape='<'; m_anim=4; m_heading=LEFT; }
	if(m_step.y==1) { m_shape='v'; m_anim=8; m_heading=DOWN; }
	if(m_step.y==-1) { m_shape='^'; m_anim=12; m_heading=UP; }
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

			if(x<=0 || y<=0 || x>=BOARD_X || y>=BOARD_Y) {
				remove_from_board(currentbrd, them);
				them->clearFlag(F_DELETED);
				currentbrd->board[(int)them->position().x][(int)them->position().y].under = NULL;
				them->setPosition(m_position + m_step);
				put(them);
				break;
			} else if(currentbrd->board[x][y].obj->type()==ZZT_TRANSPORTER &&
				(currentbrd->board[x][y].obj->step().x==m_step.x*-1 ||
				currentbrd->board[x][y].obj->step().y==m_step.y*-1)) {
				remove_from_board(currentbrd, them);
				them->clearFlag(F_DELETED);
				currentbrd->board[(int)them->position().x][(int)them->position().y].under = NULL;
				them->setPosition(currentbrd->board[x][y].obj->position() + m_step);
				put(them);
				break;
			}
		} while(x>0 && x<BOARD_X && y>0 && y<BOARD_Y);
		if(zm!=NULL) zm->setTune("tc+d-e+f#-g#+a#c+d");
		if(zm!=NULL) zm->start();
	}
}
	
void Scroll::message(ZZTObject *them, std::string message) {
	if(message == "get") {
		if(zm!=NULL) zm->setTune("tc-c+d-d+e-e+f-f+g-g");
		if(zm!=NULL) zm->start();
		m_progpos = 0;
	}
}

void Scroll::update() {
	m_shape=ZZT_SCROLL_SHAPE;
	m_fg++;
	if(m_fg>15) m_fg=9;
	if(m_progpos>=0) {
		ZZTOOP::update();
		remove_from_board(currentbrd,this);
		player->move(toward(this));
	}
	draw();
}

void Inventory::message(ZZTObject *them, std::string message) {
	char tmp[100];
	int ok=0;
	
	if(message == "get") {
		ok=1;
		switch(m_type) {
		case ZZT_ENERGIZER:
			world.energizer_cycle=80;
			if(enermsg==0) {
				enermsg=1;
				set_msg("Energizer - You are invincible!");
			}
			if(zm!=NULL) zm->setTune("s.-cd#e@s.-f+f-fd#c+c-d#ef+f-fd#c+c-d#e@s.-f+f-fd#c+c-d#ef+f-fd#c+c-d#e@s.-f+f-fd#c+c-d#ef+f-fd#c+c-d#e@s.-f+f-fd#c+c-d#ef+f-fd#c+c-d#e@s.-f+f-fd#c+c-d#e");
			if(zm!=NULL) zm->lock();
			if(zm!=NULL) zm->start();
			break;
		case ZZT_GEM:
			give_gems(1);
			give_health(1);
			give_score(10);
			if(gemmsg==0) {
				gemmsg=1;
				set_msg("Gems give you Health!");
			}
			if(zm!=NULL) zm->setTune("t+c-gec");
			if(zm!=NULL) zm->start();
			break;
		case ZZT_AMMO:
			give_ammo((world.magic == 65534) ? 10 : 5);
			if(ammomsg==0) {
				ammomsg=1;
				set_msg((char *)((world.magic == 65534) ? "Ammunition:\r10 shots" : "Ammunition - 5 shots per container."));
			}
			if(zm!=NULL) zm->setTune("tcc#d");
			if(zm!=NULL) zm->start();
			break;
		case ZZT_TORCH:
			give_torch(1);
			if(torchmsg==0) {
				torchmsg=1;
				set_msg("Torch - used for lighting in the underground.");
			}
			if(zm!=NULL) zm->setTune("tcase");
			if(zm!=NULL) zm->start();
			break;
		case ZZT_KEY:
			if(world.keys[(m_fg%8)-1]==1) {
				sprintf(tmp,"You already have a %s key!",int_to_color(*m_color).c_str());
				if(zm!=NULL) zm->setTune("sc-c");
				if(zm!=NULL) zm->start();
				ok=0;
			} else {
				world.keys[(m_fg%8)-1]=1;
				sprintf(tmp,"You now have the %s key",int_to_color(*m_color).c_str());
				draw_keys();
				if(zm!=NULL) zm->setTune("t+cegcegceg+sc");
				if(zm!=NULL) zm->start();
			}
			set_msg(tmp);
			break;
		case ZZT_DOOR:
			if(them->type()==ZZT_PLAYER) {
				if(world.keys[(m_bg%8)-1]==1) {
					//ok=0;
					sprintf(tmp,"The %s door is now open!",int_to_color(*m_color).c_str());
					set_msg(tmp);
					world.keys[(m_bg%8)-1]=0;
					draw_keys();
					//remove_from_board(currentbrd, this);
					//them->move(toward(this));
					if(zm!=NULL) zm->setTune("tcgbcgb+ic");
					if(zm!=NULL) zm->start();					
				} else {
					sprintf(tmp,"The %s door is locked.",int_to_color(*m_color).c_str());
					set_msg(tmp);
					ok=0;
					if(zm!=NULL) zm->setTune("t--gc");
					if(zm!=NULL) zm->start();
				}
			}
			break;
		}
		if(ok) debug("\x1b[0;37mA \x1b[1;37m%s\x1b[0;37m was picked up.\n",m_name.c_str());
	} else if(message == "shot") {
		switch(m_type) {
		case ZZT_GEM:
			ok=1;
			//remove_from_board(currentbrd,me);
			break;
		}
	} else if(message == "touch") {
		//switch(m_type) {
		//}
	}
	if(ok) {
		remove_from_board(currentbrd,this);
	}
}

void Inventory::create() {

}
