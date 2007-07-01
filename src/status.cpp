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
#include <Tiki/plxcompat.h>
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
#include "GraphicsLayer.h"

ConsoleText *st;
extern ConsoleText *ct;
#ifndef DZZT_LITE
extern GraphicsLayer *gl;
#endif

extern struct board_info_node *currentbrd;
extern unsigned char zztascii[55];
extern struct world_header world;

int msgcount=0;
void redraw_status();

void set_msg(char *text) {
	redraw_status();
	strcpy(currentbrd->message,text);
	currentbrd->msgcount=24;
}

void redraw_status() {
	int x;

	for(x=0;x<BOARD_X;x++) {
		draw_block(x,BOARD_Y-1);
	}
}

void draw_msg() {
	if(currentbrd->msgcount>0) {
		int length = (int)strlen(currentbrd->message);
		int left = (BOARD_X/2)-((length+2)/2);
		ct->color((currentbrd->msgcount%6)+9,0);
		ct->locate(left,BOARD_Y-1);
		ct->printf(" %s ",currentbrd->message);
#ifndef DZZT_LITE
		for(int x=left; x<=left+length; x++) {
			gl->clear(x,BOARD_Y-1);
		}
#endif
		
		currentbrd->msgcount--;
		if(currentbrd->msgcount==0) redraw_status();
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
	else world.time-=count;
	draw_time();
}

void give_time(int count) {
	world.time+=count;
	draw_time();
}

void draw_ammo() {
	st->putColor(2,9, HIGH_INTENSITY | CYAN | (BLUE << 8));
	st->putChar(2,9,0x84);

	st->locate(3, 9);
	st->color(14,1);
	st->printf("    Ammo:%i   ",world.ammo);
}

void take_ammo(int count) {
	if(count > world.ammo)
		world.ammo=0;
	else world.ammo-=count;
	draw_ammo();
}

void give_ammo(int count) {
	world.ammo+=count;
	draw_ammo();
}

void draw_health() {
	st->putColor(3,8, HIGH_INTENSITY | WHITE | (BLUE << 8));
	st->putChar(3,8,0x02);

	st->locate(3,8);
	st->color(14,1);
	st->printf("  Health:%i  ",world.health);
}

void take_health(int count) {
	if(world.health<count) 
		world.health=0;
	else world.health-=count;
	draw_health();
}

void give_health(int count) {
	world.health+=count;
	draw_health();
}

void draw_torch() {
	int x;
	
	st->putColor(2,10, YELLOW | (BLUE << 8));
	st->putChar(2,10,0x9d);

	st->locate(3,10);
	st->color(14,1);
	st->printf(" Torches:%i  ",world.torches);
	st->locate(15,10);

	if(world.torch_cycle>0) {
		st->color(6,6);
		for(x=0;x<world.torch_cycle/50;x++) {
			st->printf("%c",177);
		}
		st->color(0,0);		
		for(x=0;x<4-(world.torch_cycle/50);x++) {
			st->printf("%c",176);
		}
	} else {
		st->printf("    ");
	}
}

void take_torch(int count) {
	if(world.health<=0) return;
	if(world.torches < count) 
		world.torches=0;
	else world.torches-=count;
	draw_torch();
}

void give_torch(int count) {
	if(world.health<=0) return;
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
	if(world.health<=0) return;
	if(world.gems < count) 
		world.gems = 0;
	else world.gems-=count;
	draw_gems();
}

void give_gems(int count) {
	if(world.health<=0) return;
	world.gems+=count;
	draw_gems();
}

void draw_score() {
	st->locate(3,12);
	st->color(14,1);
	st->printf("   Score:%i  ",world.score + world.task_points);
}

void take_score(int count) {
	if(world.health<=0) return;
	if(world.score < count)
		world.score = 0;
	else world.score-=count;
	draw_score();
}

void give_score(int count) {
	if(world.health<=0) return;
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
	draw_gems();
	draw_score();
	draw_keys();
#if TIKI_PLAT == TIKI_DC || TIKI_PLAT == TIKI_NDS
	st->locate(9,15);
	st->color(0,7);
	st->printf(" X ");
	st->color(15,1);
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
	st->printf(" T ");
	st->color(15,1);
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
}
