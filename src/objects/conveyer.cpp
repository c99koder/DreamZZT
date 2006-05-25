/*
 *  conveyer.cpp
 *  ZZT Conveyer
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

#include "object.h"
#include "board.h"

extern struct board_info_node *currentbrd;

extern char spin_anim[4];
char moved[3][3];

int Conveyer::cw(ZZTObject *them) {
  if(!(them->getFlags()&F_PUSHABLE) || them->getPushed()) return 0;

  if((int)them->getPosition().y==(int)m_position.y-1 && ((int)them->getPosition().x==(int)m_position.x-1||(int)them->getPosition().x==(int)m_position.x)) {
    if(::is_empty(currentbrd,(int)them->getPosition().x+1,(int)them->getPosition().y)) {
      them->move(RIGHT);
      moved[((int)them->getPosition().x-(int)m_position.x)+1][((int)them->getPosition().y-(int)m_position.y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[(int)them->getPosition().x+1][(int)them->getPosition().y].obj!=NULL) {
        return cw(currentbrd->board[(int)them->getPosition().x+1][(int)them->getPosition().y].obj);
      } else {
        return 0;
      }
    }
  } else if((int)them->getPosition().x==(int)m_position.x+1 && ((int)them->getPosition().y==(int)m_position.y-1 || (int)them->getPosition().y==(int)m_position.y)) {
    if(::is_empty(currentbrd,(int)them->getPosition().x,(int)them->getPosition().y+1)) {
      them->move(DOWN);
      moved[((int)them->getPosition().x-(int)m_position.x)+1][((int)them->getPosition().y-(int)m_position.y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[(int)them->getPosition().x][(int)them->getPosition().y+1].obj!=NULL) {
        return cw(currentbrd->board[(int)them->getPosition().x][(int)them->getPosition().y+1].obj);
      } else {
        return 0;
      }
    }
  } else if((int)them->getPosition().y==(int)m_position.y+1 && ((int)them->getPosition().x==(int)m_position.x+1 || (int)them->getPosition().x==(int)m_position.x)) {
    if(::is_empty(currentbrd,(int)them->getPosition().x-1,(int)them->getPosition().y)) {
      them->move(LEFT);
      moved[((int)them->getPosition().x-(int)m_position.x)+1][((int)them->getPosition().y-(int)m_position.y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[(int)them->getPosition().x-1][(int)them->getPosition().y].obj!=NULL) {
        return cw(currentbrd->board[(int)them->getPosition().x-1][(int)them->getPosition().y].obj);
      } else {
        return 0;
      }
    }
  } else if((int)them->getPosition().x==(int)m_position.x-1 && ((int)them->getPosition().y==(int)m_position.y+1 || (int)them->getPosition().y==(int)m_position.y)) {
    if(::is_empty(currentbrd,(int)them->getPosition().x,(int)them->getPosition().y-1)) {
      them->move(UP);
      moved[((int)them->getPosition().x-(int)m_position.x)+1][((int)them->getPosition().y-(int)m_position.y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[(int)them->getPosition().x][(int)them->getPosition().y-1].obj!=NULL) {
        return cw(currentbrd->board[(int)them->getPosition().x][(int)them->getPosition().y-1].obj);
      } else {
        return 0;
      }
    }
  }
  return 0;
}

int Conveyer::ccw(ZZTObject *them) {
  if(!(them->getFlags()&F_PUSHABLE) || them->getPushed()) return 0;

  if((int)them->getPosition().y==(int)m_position.y-1 && ((int)them->getPosition().x==(int)m_position.x+1||(int)them->getPosition().x==(int)m_position.x)) {
    if(::is_empty(currentbrd,(int)them->getPosition().x-1,(int)them->getPosition().y)) {
      them->move(LEFT);
      moved[((int)them->getPosition().x-(int)m_position.x)+1][((int)them->getPosition().y-(int)m_position.y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[(int)them->getPosition().x-1][(int)them->getPosition().y].obj!=NULL) {
        return ccw(currentbrd->board[(int)them->getPosition().x-1][(int)them->getPosition().y].obj);
      } else {
        return 0;
      }
    }
  } else if((int)them->getPosition().x==(int)m_position.x+1 && ((int)them->getPosition().y==(int)m_position.y+1 || (int)them->getPosition().y==(int)m_position.y)) {
    if(::is_empty(currentbrd,(int)them->getPosition().x,(int)them->getPosition().y-1)) {
      them->move(UP);
      moved[((int)them->getPosition().x-(int)m_position.x)+1][((int)them->getPosition().y-(int)m_position.y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[(int)them->getPosition().x][(int)them->getPosition().y-1].obj!=NULL) {
        return ccw(currentbrd->board[(int)them->getPosition().x][(int)them->getPosition().y-1].obj);
      } else {
        return 0;
      }
    }
  } else if((int)them->getPosition().y==(int)m_position.y+1 && ((int)them->getPosition().x==(int)m_position.x-1 || (int)them->getPosition().x==(int)m_position.x)) {
    if(::is_empty(currentbrd,(int)them->getPosition().x+1,(int)them->getPosition().y)) {
      them->move(RIGHT);
      moved[((int)them->getPosition().x-(int)m_position.x)+1][((int)them->getPosition().y-(int)m_position.y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[(int)them->getPosition().x+1][(int)them->getPosition().y].obj!=NULL) {
        return ccw(currentbrd->board[(int)them->getPosition().x+1][(int)them->getPosition().y].obj);
      } else {
        return 0;
      }
    }
  } else if((int)them->getPosition().x==(int)m_position.x-1 && ((int)them->getPosition().y==(int)m_position.y-1 || (int)them->getPosition().y==(int)m_position.y)) {
    if(::is_empty(currentbrd,(int)them->getPosition().x,(int)them->getPosition().y+1)) {
      them->move(DOWN);
      moved[((int)them->getPosition().x-(int)m_position.x)+1][((int)them->getPosition().y-(int)m_position.y)+1]=1;
      return 1;
    } else {
      if(currentbrd->board[(int)them->getPosition().x][(int)them->getPosition().y+1].obj!=NULL) {
        return ccw(currentbrd->board[(int)them->getPosition().x][(int)them->getPosition().y+1].obj);
      } else {
        return 0;
      }
    }
  }
  return 0;
}

void Conveyer::update() {
  int x,y;
  if(m_type==ZZT_CONVEYER_CW) {
    m_animIndex++;
    m_animIndex%=4;
  } else {
    m_animIndex--;
    if(m_animIndex<0) m_animIndex=3;
  }
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
          if(m_type==ZZT_CONVEYER_CW) {
						if(cw(currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj)) currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj->setPushed(1);
					}
          else if(m_type==ZZT_CONVEYER_CCW) {
						if(ccw(currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj)) currentbrd->board[(int)m_position.x+(x-1)][(int)m_position.y+(y-1)].obj->setPushed(1);
					}
        }
      }
    }
  }
}
