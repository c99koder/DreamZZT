/* status.cpp - Manage and display the ZZT HUD
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
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/tikitime.h>
#include <Tiki/file.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;

#include "console.h"
#include "board.h"
#include "object.h"
#include "status.h"
#ifdef USE_3DMODEL
#include "GraphicsLayer.h"

extern GraphicsLayer *gl;
#endif
Console *st;
extern Console *ct;
extern struct board_info_node *currentbrd;
extern unsigned char zztascii[55];
extern struct world_header world;
extern Player *player;

int msgcount=0;
int msgoff=0;

void set_msg(char *text) {
	strcpy(currentbrd->message,text);
	currentbrd->msgcount=24;
	msgoff=0;
}

extern int disp_off_x, disp_off_y;

void draw_msg() {
	char message[60];
	int i, length, left, bot;

	if(currentbrd->msgcount>0) {

		strcpy(message,currentbrd->message);
		for(i=0; i<strlen(message); i++) {
			if(message[i] == '\r' || ((i > ct->getCols() - 4) && world.magic != 65534)) {
				if(message[i] != '\r') {
					//Find a space to break on
					while(message[i] != ' ' && i > 0) {
						i--;
					}
					if(i==0)
						i = ct->getCols() - 4;
				}
				message[i] = '\0';
				left = (ct->getCols() / 2) - ((i+2)/2);
				ct->color((currentbrd->msgcount%6)+9,0);
				ct->locate(left,ct->getRows() - 2);
				ct->printf(" %s ",message);
				break;
			}
		}
		if(i < strlen(currentbrd->message)) {
			strcpy(message,currentbrd->message + i + 1);
		}
		length = (int)strlen(message);
		if(length > ct->getCols() - 2)
			length = ct->getCols() - 2;
		left = (ct->getCols() / 2) - ((length+2)/2);
		if(left < 0)
			left=0;
		strcpy(message,message+msgoff);
		message[(ct->getCols() - 2)] = '\0';
		if(strlen(message) > (ct->getCols() - 2) && msgoff + (ct->getCols() - 2) < strlen(message) && currentbrd->msgcount < 21)
			msgoff+=4;
		if(msgoff > strlen(message) - (ct->getCols() - 2))
			msgoff = strlen(message) - (ct->getCols() - 2);
		ct->color((currentbrd->msgcount%6)+9,0);
		ct->locate(left,ct->getRows() - 1);
		ct->printf(" %s ",message);
#ifdef USE_3DMODEL

		for(int x=left; x<=left+length; x++) {
			gl->clear(x,BOARD_Y-1);
		}
#endif

		currentbrd->msgcount--;
	}
}

void dzzt_logo() {
	st->color(15,1);
	st->clear();
	st->locate(5,1);
	st->color(GREY | HIGH_INTENSITY, BLUE);
	st->printf("- - - - -");
	st->locate(2,2);
	st->color(BLACK, GREY);
	st->printf("    DreamZZT    ");
	st->locate(5,3);
	st->color(GREY | HIGH_INTENSITY, BLUE);
	st->printf("- - - - -");
}

void draw_time() {
	st->locate(2,7);
	st->color(YELLOW | HIGH_INTENSITY , BLUE);
	if(world.time>0) {
		st->printf("     Time:%i  ",world.time);
	} else {
		st->printf("               ");
	}
}

void take_time(int count) {
	if(count > world.time)
		world.time=0;
	else
		world.time-=count;
	draw_time();
}

void give_time(int count) {
	world.time+=count;
	draw_time();
}

void draw_ammo() {
	st->putColor(2,(world.magic == MAGIC_SZT)?10:9, HIGH_INTENSITY | CYAN | (BLUE << 8));
	st->putChar(2,(world.magic == MAGIC_SZT)?10:9,0x84);

	st->locate(3, (world.magic == MAGIC_SZT)?10:9);
	st->color(14,1);
	st->printf("    Ammo:%i   ",world.ammo);
}

void take_ammo(int count) {
	if(count > world.ammo)
		world.ammo=0;
	else
		world.ammo-=count;
	draw_ammo();
}

void give_ammo(int count) {
	world.ammo+=count;
	draw_ammo();
}

void draw_health() {
	unsigned int t = world.health;
	TUIMeter m(&t, 100, 7);

	st->putColor(3,8, HIGH_INTENSITY | WHITE | (BLUE << 8));
	st->putChar(3,8,0x02);

	st->locate(3,8);
	st->color(14,1);
	if(world.magic == MAGIC_SZT) {
		*st << "  Health:";
		m.draw(st);
	} else {
		*st << "  Health:" << world.health << "   ";
	}
}

void take_health(int count) {
	if(world.health<count)
		world.health=0;
	else
		world.health-=count;
	draw_health();
}

void give_health(int count) {
	world.health+=count;
	draw_health();
}

void draw_z() {
	if(world.magic == MAGIC_SZT && world.z != -1) {
		if(world.z < 0) world.z = 0;
		for(int i=0; i < 16; i++) {
			if(world.flags[i][0] == 'Z') {
				st->locate(3,9);
				st->color(14,1);
				*st << (char *)(world.flags[i].c_str() + 1);
				st->locate(3+8,9);
				*st << ":" << world.z << "   ";
			}
		}
	}
}

void take_z(int count) {
	if(world.z<count)
		world.z=0;
	else
		world.z-=count;
	draw_z();
}

void give_z(int count) {
	world.z+=count;
	draw_z();
}

void draw_torch() {
	if(world.magic == MAGIC_SZT)
		return;
	unsigned int t = world.torch_cycle;
	TUIMeter m(&t, 200, 4);

	st->putColor(2,10, YELLOW | (BLUE << 8));
	st->putChar(2,10,0x9d);

	st->locate(3,10);
	st->color(14,1);
	st->printf(" Torches:%i  ",world.torches);
	st->locate(15,10);

	if(world.torch_cycle>0) {
		st->color(6,0);
		m.draw(st);
	} else {
		st->printf("    ");
	}
}

void take_torch(int count) {
	if(world.health<=0)
		return;
	if(world.torches < count)
		world.torches=0;
	else
		world.torches-=count;
	draw_torch();
}

void give_torch(int count) {
	if(world.health<=0)
		return;
	world.torches+=count;
	draw_torch();
}

void draw_gems() {
	st->putColor(2,11, HIGH_INTENSITY | RED | (BLUE << 8));
	st->putChar(2,11,0x04);

	st->locate(3,11);
	st->color(14,1);
	st->printf("    Gems:%i  ",world.gems);
}

void take_gems(int count) {
	if(world.health<=0)
		return;
	if(world.gems < count)
		world.gems = 0;
	else
		world.gems-=count;
	draw_gems();
}

void give_gems(int count) {
	if(world.health<=0)
		return;
	world.gems+=count;
	draw_gems();
}

void draw_score() {
	st->locate(3,12);
	st->color(14,1);
	st->printf("   Score:%i  ",world.score + world.task_points);
}

void take_score(int count) {
	if(world.health<=0)
		return;
	if(world.score < count)
		world.score = 0;
	else
		world.score-=count;
	draw_score();
}

void give_score(int count) {
	if(world.health<=0)
		return;
	world.score+=count;
	draw_score();
}

void draw_keys() {
	int i;
	st->putColor(2,13, HIGH_INTENSITY | GREY | (BLUE << 8));
	st->putChar(2,13,0x0c);

	st->locate(3,13);
	st->color(14,1);
	st->printf("    Keys:");

	for(i=0;i<7;i++) {
		st->putColor(12+i,13, HIGH_INTENSITY | (i+1) | (BLUE << 8));

		if(world.keys[i]==1) {
			st->putChar(12+i,13,0x0c);
		} else {
			st->putChar(12+i,13,' ');
		}
	}
}

void draw_hud_ingame() {
	dzzt_logo();
	draw_time();
	draw_health();
	draw_ammo();
	draw_torch();
	draw_z();
	draw_gems();
	draw_score();
	draw_keys();
	st->setANSI(false);
#if TIKI_PLAT == TIKI_DC || TIKI_PLAT == TIKI_NDS

	st->locate(9,15);
	st->color(0,7);
	st->printf(" X ");
	st->color(15,1);
	if(world.magic == MAGIC_SZT)
		st->printf(" Hint");
	else
		st->printf(" Torch");
	st->locate(9,16);
	st->color(0,3);
	st->printf(" Y ");
	st->color(15,1);
	st->printf(" Save");
	st->locate(9,17);
	st->color(0,7);
	st->printf(" B ");
	st->color(15,1);
	st->printf(" Quit");
	st->locate(5,18);
	st->color(0,3);
	st->printf(" Start ");
	st->color(15,1);
	st->printf(" Pause");
	st->locate(7,20);
	st->color(0,7);
	st->printf(" %c%c%c%c",24,25,26,27);
	st->color(15,1);
	st->printf(" Move");
	st->locate(3,21);
	st->color(0,3);
	st->printf(" A + %c%c%c%c",24,25,26,27);
	st->color(15,1);
	st->printf(" Shoot");
#else

	st->locate(9,15);
	st->color(0,7);
	if(world.magic == MAGIC_SZT)
		st->printf(" H ");
	else
		st->printf(" T ");
	st->color(15,1);
	if(world.magic == MAGIC_SZT)
		st->printf(" Hint");
	else
		st->printf(" Torch");
	st->locate(9,16);
	st->color(0,3);
	st->printf(" S ");
	st->color(15,1);
	st->printf(" Save");
	st->locate(9,17);
	st->color(0,7);
	st->printf(" P ");
	st->color(15,1);
	st->printf(" Pause");
	st->locate(7,18);
	st->color(0,3);
	st->printf(" ESC ");
	st->color(15,1);
	st->printf(" Menu");
	st->locate(7,20);
	st->color(0,3);
	st->printf(" %c%c%c%c",24,25,26,27);
	st->color(15,1);
	st->printf(" Move");
	st->locate(1,21);
	st->color(0,7);
	st->printf(" Space %c%c%c%c",24,25,26,27);
	st->color(15,1);
	st->printf(" Shoot");
#endif

	st->setANSI(true);
}
