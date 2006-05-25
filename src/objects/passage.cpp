/*
 *  passage.cpp
 *  ZZT passage object
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
#include "sound.h"

extern int switchbrd;

extern ZZTMusicStream *zm;

void Passage::setParam(int arg, int val) {
	if(arg==3) m_dest = val;
}

void Passage::message(ZZTObject *them, std::string message) {
	ZZTObject *obj;
  ZZTObject *obj2;
  board_info_node *brd=get_board(m_dest);
  Vector pos;
	
  if(message == "touch" && them->getType()==ZZT_PLAYER) {
    obj=get_obj_by_type(brd,ZZT_PLAYER);
    obj2=get_obj_by_color(brd,ZZT_PASSAGE,*m_color);
		pos = obj->getPosition();
		
		if(obj2!=NULL) {
      brd->board[(int)pos.x][(int)pos.y].obj=brd->board[(int)pos.x][(int)pos.y].under;
			brd->board[(int)pos.x][(int)pos.y].under=NULL;
			pos = obj2->getPosition();
      obj->setPosition(pos);
      brd->board[(int)pos.x][(int)pos.y].under=obj2;
      brd->board[(int)pos.x][(int)pos.y].obj=obj;
    }

    switchbrd=m_dest;
    obj->setFlag(F_SLEEPING);
		zm->setTune("tceg tc#fg# tdf#a td#ga# teg#+c");
		zm->start();
  }
}

void Passage::create() {
  m_color=&m_bg;
}