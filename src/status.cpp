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

#include <Tiki/drawables/console.h>
#include "board.h"
#include "object.h"

extern ConsoleText *ct;

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
    ct->color((currentbrd->msgcount%6)+9,0);
    ct->locate((BOARD_X/2)-((strlen(currentbrd->message)+2)/2),BOARD_Y-1);
    ct->printf(" %s ",currentbrd->message);
    currentbrd->msgcount--;
    if(currentbrd->msgcount==0) redraw_status();
  }
}

void dzzt_logo() {
  ct->locate(BOARD_X+5,1);
  ct->color(GREY | HIGH_INTENSITY, BLUE);
  ct->printf("- - - - -");
  ct->locate(BOARD_X+2,2);
  ct->color(BLACK, GREY);
  ct->printf("    DreamZZT    ");
  ct->locate(BOARD_X+5,3);
  ct->color(GREY | HIGH_INTENSITY, BLUE);
  ct->printf("- - - - -");
}

void draw_time() {
  ct->locate(BOARD_X+2,7);
  ct->color(YELLOW | HIGH_INTENSITY , BLUE);
	if(world.time>0) {
  	ct->printf("     Time:%i  ",world.time);
	} else {
  	ct->printf("               ");
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
	ct->putColor(BOARD_X+2,9, HIGH_INTENSITY | CYAN | (BLUE << 8));
	ct->putChar(BOARD_X+2,9,0x84);

	ct->locate(BOARD_X+3, 9);
  ct->color(14,1);
  ct->printf("    Ammo:%i   ",world.ammo);
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
	ct->putColor(BOARD_X+3,8, HIGH_INTENSITY | WHITE | (BLUE << 8));
	ct->putChar(BOARD_X+3,8,0x02);

  ct->locate(BOARD_X+3,8);
  ct->color(14,1);
  ct->printf("  Health:%i  ",world.health);
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
	
	ct->putColor(BOARD_X+2,10, YELLOW | (BLUE << 8));
	ct->putChar(BOARD_X+2,10,0x9d);

  ct->locate(BOARD_X+3,10);
  ct->color(14,1);
  ct->printf(" Torches:%i  ",world.torches);
	ct->locate(BOARD_X+15,10);

	if(world.torch_cycle>0) {
		ct->color(6,6);
		for(x=0;x<world.torch_cycle/50;x++) {
			ct->printf("%c",177);
		}
		ct->color(0,0);		
		for(x=0;x<4-(world.torch_cycle/50);x++) {
			ct->printf("%c",176);
		}
	} else {
		ct->printf("    ");
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
	ct->putColor(BOARD_X+2,11, HIGH_INTENSITY | RED | (BLUE << 8));
	ct->putChar(BOARD_X+2,11,0x04);

  ct->locate(BOARD_X+3,11);
  ct->color(14,1);
  ct->printf("    Gems:%i  ",world.gems);
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
  ct->locate(BOARD_X+3,12);
  ct->color(14,1);
  ct->printf("   Score:%i  ",world.score + world.task_points);
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
	ct->putColor(BOARD_X+2,13, HIGH_INTENSITY | GREY | (BLUE << 8));
	ct->putChar(BOARD_X+2,13,0x0c);

  ct->locate(BOARD_X+3,13);
  ct->color(14,1);
  ct->printf("    Keys:");

  for(i=0;i<7;i++) {
		ct->putColor(BOARD_X+12+i,13, HIGH_INTENSITY | (i+1) | (BLUE << 8));

    if(world.keys[i]==1) {
			ct->putChar(BOARD_X+12+i,13,0x0c);
    } else {
			ct->putChar(BOARD_X+12+i,13,' ');
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
#ifdef DREAMCAST
	ct->locate(BOARD_X+9,15);
	ct->color(0,7);
	ct->printf(" X ");
	ct->color(15,1);
	ct->printf(" Torch");
	ct->locate(BOARD_X+9,17);
	ct->color(0,3);
	ct->printf(" B ");
	ct->color(15,1);
	ct->printf(" Menu");
	ct->locate(BOARD_X+5,18);
	ct->color(0,7);
	ct->printf(" Start ");
	ct->color(15,1);
	ct->printf(" Pause");
	ct->locate(BOARD_X+7,20);
	ct->color(0,3);
	ct->printf(" %c%c%c%c",24,25,26,27);
	ct->color(15,1);
	ct->printf(" Move");
	ct->locate(BOARD_X+3,21);
	ct->color(0,7);
	ct->printf(" A + %c%c%c%c",24,25,26,27);
	ct->color(15,1);
	ct->printf(" Shoot");
#else
	ct->locate(BOARD_X+9,15);
	ct->color(0,7);
	ct->printf(" T ");
	ct->color(15,1);
	ct->printf(" Torch");
	ct->locate(BOARD_X+9,16);
	ct->color(0,3);
	ct->printf(" S ");
	ct->color(15,1);
	ct->printf(" Save");
	ct->locate(BOARD_X+9,17);
	ct->color(0,7);
	ct->printf(" P ");
	ct->color(15,1);
	ct->printf(" Pause");
	ct->locate(BOARD_X+7,18);
	ct->color(0,3);
	ct->printf(" ESC ");
	ct->color(15,1);
	ct->printf(" Quit");
	ct->locate(BOARD_X+7,20);
	ct->color(0,3);
	ct->printf(" %c%c%c%c",24,25,26,27);
	ct->color(15,1);
	ct->printf(" Move");
	ct->locate(BOARD_X+1,21);
	ct->color(0,7);
	ct->printf(" Shift %c%c%c%c",24,25,26,27);
	ct->color(15,1);
	ct->printf(" Shoot");
#endif
}
