/*
 *  status.cpp
 *  Displays and manages the HUD
 *
 *  Copyright 2000 - 2006 Sam Steele. All rights reserved.
 *
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

#include "drawables/ConsoleText.h"
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
  ct->color(15,1);
  ct->printf("- - - - -");
  ct->locate(BOARD_X+2,2);
  ct->color(0,7);
  ct->printf("    DreamZZT    ");
  ct->locate(BOARD_X+5,3);
  ct->color(15,1);
  ct->printf("- - - - -");
}

void draw_time() {
  ct->locate(BOARD_X+2,7);
  ct->color(14,1);
	if(world.time>0) {
  	ct->printf("     Time:%i  ",world.time);
	} else {
  	ct->printf("               ");
	}
}

void take_time(int count) {
  world.time-=count;
  if(world.time<0) world.time=0;
  draw_time();
}

void give_time(int count) {
  world.time+=count;
  draw_time();
}

void draw_ammo() {
  ct->locate(BOARD_X+2,9);
  ct->color(11,1);
  ct->printf("%c",0x84);
  ct->color(14,1);
  ct->printf("    Ammo:%i   ",world.ammo);
}

void take_ammo(int count) {
  //printf("Taking %i ammo from %i\n",count,world.ammo);
  world.ammo-=count;
  if(world.ammo<0) world.ammo=0;
  draw_ammo();
}

void give_ammo(int count) {
  //printf("Giving %i ammo to %i\n",count,world.ammo);
  world.ammo+=count;
  draw_ammo();
}

void draw_health() {
  ct->locate(BOARD_X+2,8);
  ct->color(15,1);
  ct->printf("%c",2);
  ct->color(14,1);
  ct->printf("  Health:%i  ",world.health);
}

void take_health(int count) {
  if(world.health-count<0) world.health=0;
		else world.health-=count;
  draw_health();
}

void give_health(int count) {
  world.health+=count;
  draw_health();
}

void draw_torch() {
	int x;
  ct->locate(BOARD_X+2,10);
  ct->color(6,1);
  ct->printf("%c",0x9d);
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
  world.torches-=count;
  if(world.torches<0) world.torches=0;
  draw_torch();
}

void give_torch(int count) {
  //printf("Giving %i torches to %i\n",count,world.torches);
  world.torches+=count;
  draw_torch();
}

void draw_gems() {
  ct->locate(BOARD_X+2,11);
  ct->color(12,1);
  ct->printf("%c",0x04);
  ct->color(14,1);
  ct->printf("    Gems:%i  ",world.gems);
  //printf("Drawing gems: %i\n",world.gems);
}

void take_gems(int count) {
  //printf("Taking %i gems from %i\n",count,world.gems);
  world.gems-=count;
  if(world.gems<0) world.gems=0;
  draw_gems();
}

void give_gems(int count) {
  //printf("Giving %i gems to %i\n",count,world.gems);
  world.gems+=count;
  draw_gems();
}

void draw_score() {
  ct->locate(BOARD_X+2,12);
  ct->color(15,1);
  ct->printf(" ");
  ct->color(14,1);
  ct->printf("   Score:%i  ",world.score);
}

void take_score(int count) {
  world.score-=count;
  if(world.score<0) world.score=0;
  draw_score();
}

void give_score(int count) {
  world.score+=count;
  draw_score();
}

void draw_keys() {
  int x;
  ct->locate(BOARD_X+2,13);
  ct->color(15,1);
  ct->printf("%c",0x0c);
  ct->color(14,1);
  ct->printf("    Keys:");
  for(x=0;x<7;x++) {
    if(world.keys[x]==1) {
      ct->color(9+x,1);
      ct->printf("%c",0x0c);
    } else {
      ct->printf(" ");
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
	ct->locate(BOARD_X+7,17);
	ct->color(0,3);
	ct->printf(" ESC ");
	ct->color(15,1);
	ct->printf(" Menu");
	ct->locate(BOARD_X+5,18);
	ct->color(0,7);
	ct->printf(" ENTER ");
	ct->color(15,1);
	ct->printf(" Pause");
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
