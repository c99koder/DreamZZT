/*
 *  bullet.cpp
 *  ZZT bullet
 *
 *  Copyright 2000 - 2006 Sam Steele. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/hid.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Tiki;
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
  if(sound_id==-1) { sound_id=snd_sfx_load("/cd/fire.wav"); }
#endif
}

void Bullet::setParam(int arg,int value) {
	if(arg == 1) m_owner = value;
}

void Bullet::update() {
  move(m_heading);
}

void Bullet::message(ZZTObject *them, std::string message) {
  if(message == "thud") {
    if(m_owner == 0 && them->getType()!=ZZT_PLAYER || m_owner == 1 && them->getType()==ZZT_PLAYER) {
      them->message(this,"shot");
    } 
		if(them->getType()==ZZT_BREAKABLE) {
			remove_from_board(currentbrd,them);
			zm->setTune("t-c");
			zm->start();
		}
    remove_from_board(currentbrd,this);
  }
}

void ZZTObject::shoot(enum direction dir) {
	ZZTObject *bullet=NULL;
	int dx,dy;
	char buf[100];

	switch(dir) {
	  case UP:
	   dx=0; dy=-1;
	   break;
    case DOWN:
     dx=0; dy=1;
     break;
    case LEFT:
     dx=-1; dy=0;
     break;
    case RIGHT:
     dx=1; dy=0;
     break;
		case IDLE:
			return;
	}
	
  if(m_type==ZZT_PLAYER) {
    if(world.ammo<1) {
			//sprintf(buf,"You don't have any %s",ammo_name);
      set_msg("You don't have any ammo");
      return;
    }
  }
  if(is_empty(dir)) {
    if(m_type==ZZT_PLAYER) world.ammo--;
    draw_ammo();
    bullet=create_object(ZZT_BULLET,dir);
    bullet->setParam(1,m_type==ZZT_PLAYER?0:1);
    bullet->setHeading(dir);
	  bullet->setCycle(1);
    currentbrd->board[(int)m_position.x+dx][(int)m_position.y+dy].obj=currentbrd->board[(int)m_position.x+dx][(int)m_position.y+dy].under;
    currentbrd->board[(int)m_position.x+dx][(int)m_position.y+dy].obj=bullet;
		zm->setTune("t+c-c-c");
		zm->start();
  } else {
    if(currentbrd->board[(int)m_position.x+dx][(int)m_position.y+dy].obj!=NULL) {
			if(currentbrd->board[(int)m_position.x+dx][(int)m_position.y+dy].obj->getType()==ZZT_BREAKABLE) {
				remove_from_board(currentbrd,currentbrd->board[(int)m_position.x+dx][(int)m_position.y+dy].obj);
			} else {
				currentbrd->board[(int)m_position.x+dx][(int)m_position.y+dy].obj->message(this,"shot");
			} 
			if(is_empty(dir)) {
				if(m_type==ZZT_PLAYER) world.ammo--;
				draw_ammo();
			}
    }
  }
}
