/* object.cpp - ZZTObject base class
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
#include "console.h"
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

using namespace Tiki;
using namespace Tiki::Hid;
using namespace Tiki::Audio;
using namespace Tiki::GL;
using namespace std;

#include "object.h"
#include "board.h"
#include "word.h"
#include "sound.h"
#include "task.h"

#ifndef DZZT_LITE
#include "GraphicsLayer.h"
#endif

extern int zztascii[];
extern Player *player;
extern ZZTMusicStream *zm;
extern struct world_header world;
extern ConsoleText *ct;
extern ConsoleText *st;
#ifndef DZZT_LITE
extern GraphicsLayer *gl;
#endif
extern struct board_info_node *currentbrd;

int dir_to_delta[5] = { 0,-1,1,-1,1 };
extern struct board_info_node *currentbrd;

int id_pool=0;

direction ZZTObject::opposite(direction dir) {
	if(dir==LEFT) { return RIGHT; }
	if(dir==RIGHT) { return LEFT; }
	if(dir==UP) { return DOWN; }
	if(dir==DOWN) { return UP; }
	return IDLE;
}

direction ZZTObject::clockwise(direction dir) {
	if(dir==LEFT) { return UP; }
	if(dir==RIGHT) { return DOWN; }
	if(dir==UP) { return RIGHT; }
	if(dir==DOWN) { return LEFT; }
	return IDLE;
}

direction ZZTObject::str_to_direction(string s) {
	int neg=0;
	direction res=IDLE;
		
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	
	vector<string> words = wordify(s, ' ');
	vector<string>::iterator words_iter;
	
	for(words_iter = words.begin(); words_iter != words.end(); words_iter++) {
		if((*words_iter) == "opp") {
			neg=1;
		} else if((*words_iter) == "cw") {
			neg=2;
		} else if((*words_iter) == "ccw") {
			neg=3;
		} else if((*words_iter) == "rndp") {
			if(rand()%2==0) {
				neg=2;
			} else {
				neg=3;
			}
		} else if((*words_iter) == "flow") {
			res=m_heading;
		} else if((*words_iter) == "seek") {
			res=toward(player);
		} else if((*words_iter) == "rndns") {
			if(rand()%2==1) {
				res=UP;
			} else {
				res=DOWN;
			}
		} else if((*words_iter) == "rndne") {
			if(rand()%2==1) {
				res=UP;
			} else {
				res=RIGHT;
			}
			break;
		} else if((*words_iter) == "n" || (*words_iter) == "north") {
			res = UP;
			break;
		} else if((*words_iter) == "s" || (*words_iter) == "south") {
			res = DOWN;
			break;
		} else if((*words_iter) == "e" || (*words_iter) == "east") {
			res = RIGHT;
			break;
		} else if((*words_iter) == "w" || (*words_iter) == "west") {
			res = LEFT;
			break;
		}
	}
	if(neg==1) {
		res=opposite(res);
	}
	if(neg==2) {
		res=clockwise(res);
	}
	if(neg==3) {
		res=opposite(clockwise(res));
	}
	return res;
}

direction ZZTObject::toward(ZZTObject *them) {
	//relative direction between me and them
	direction dirx=IDLE,diry=IDLE;
	Vector dist = m_position - them->position();

	if(dist.x < 0) { dirx=RIGHT; }
	if(dist.x > 0) { dirx=LEFT; }
	if(dist.y < 0) { diry=DOWN; }
	if(dist.y > 0) { diry=UP; }
	if(dist.x<0) { dist.x*=-1; }
	if(dist.y<0) { dist.y*=-1; }
	if(abs((int)dist.x)>abs((int)dist.y)) {
		if(!::is_empty(currentbrd, (int)(m_position.x + dir_to_delta[dirx]), (int)m_position.y) && 
			 (diry!=IDLE && ::is_empty(currentbrd, (int)m_position.x, (int)(m_position.y + dir_to_delta[diry]))))
			return diry;
		else 
			return dirx;
	} else { 
		if(!::is_empty(currentbrd, (int)m_position.x, (int)(m_position.y + dir_to_delta[diry])) && 
			 (dirx!=IDLE && ::is_empty(currentbrd, (int)(m_position.x + dir_to_delta[diry]), (int)m_position.y)))
			return dirx;
		else 
			return diry;
	}
}

int ZZTObject::distance(ZZTObject *them) {
	//distance between me and them
	Vector dist = m_position - them->position();

	if(dist.x<0) { dist.x*=-1; }
	if(dist.y<0) { dist.y*=-1; }
	return int(dist.x + dist.y);
}

int ZZTObject::dist_x(ZZTObject *them) {
	//distance between me and them (x axis only)
	Vector dist = m_position - them->position();
	
	if(dist.x<0) { dist.x*=-1; }
	if(dist.y<0) { dist.y*=-1; }
	return (int)dist.x;
}

int ZZTObject::dist_y(ZZTObject *them) {
	//distance between me and them (x axis only)
	Vector dist = m_position - them->position();
	
	if(dist.x<0) { dist.x*=-1; }
	if(dist.y<0) { dist.y*=-1; }
	return (int)dist.y;
}

int dist(int x1, int y1, int x2, int y2) {
	int a,b;
	a=(x1-x2)*(x1-x2);
	b=(y1-y2)*(y1-y2);
	return (int)sqrt((float)a+b);
}

bool ZZTObject::is_empty(direction d, bool ignorePlayer) {
	switch(d) {
		case UP:
			return !!::is_empty(currentbrd, (int)m_position.x, (int)m_position.y - 1, ignorePlayer);
		case DOWN:
			return !!::is_empty(currentbrd, (int)m_position.x, (int)m_position.y + 1, ignorePlayer);
		case LEFT:
			return !!::is_empty(currentbrd, (int)m_position.x - 1, (int)m_position.y, ignorePlayer);
		case RIGHT:
			return !!::is_empty(currentbrd, (int)m_position.x + 1, (int)m_position.y, ignorePlayer);
	}
	return false;
}

ZZTObject *ZZTObject::create_object(int type, direction d) {
	switch(d) {
		case UP:
			return ::create_object(type, (int)m_position.x, (int)m_position.y - 1);
		case DOWN:
			return ::create_object(type, (int)m_position.x, (int)m_position.y + 1);
		case LEFT:
			return ::create_object(type, (int)m_position.x - 1, (int)m_position.y);
		case RIGHT:
			return ::create_object(type, (int)m_position.x + 1, (int)m_position.y);
	}
	return false;
}

ZZTObject *ZZTObject::get(direction d) {
	int dx=0,dy=0;

	switch(d) {
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
		case SHOOTING:
			return NULL;
	}
	
	if(m_position.x + dx >= 0 && m_position.x + dx < BOARD_X && m_position.y + dy >= 0 && m_position.y + dy < BOARD_Y) {
		return currentbrd->board[(int)m_position.x + dx][(int)m_position.y + dy].obj;
	}
	
	return NULL;
}

void ZZTObject::remove() {
	remove_from_board(m_board, this);
}

bool ZZTObject::move(enum direction dir, bool trying, bool origin) {
	//move in the specified direction
	ZZTObject *them=NULL;
	int x,y,oldx,oldy,suc=0;

	if(m_type==ZZT_SLIDER_NS&&(dir==LEFT||dir==RIGHT)) {
		return 0;
	}
	if(m_type==ZZT_SLIDER_EW&&(dir==UP||dir==DOWN)) {
		return 0;
	}
	oldx = (int)m_position.x;
	oldy = (int)m_position.y;
	x = (int)m_position.x;
	y = (int)m_position.y;
	switch(dir) {
		case LEFT:
			x = (int)m_position.x - 1;
			break;
		case RIGHT:
			x = (int)m_position.x + 1;
			break;
		case UP:
			y = (int)m_position.y - 1;
			break;
		case DOWN:
			y = (int)m_position.y + 1;
			break;
		case IDLE: 
			return true;
		default:
			printf("Unknown direction %i\n",dir);
			break;
	}
	if(x<0) { x=0; }
	if(y<0) { y=0; }
	if(x>=BOARD_X) { x=BOARD_X-1; }
	if(y>=BOARD_Y) { y=BOARD_Y-1; }
	if((m_type != ZZT_SHARK && is_empty(dir)) || ((m_type==ZZT_BULLET || m_type==ZZT_SHARK) && m_board->board[x][y].obj->type()==ZZT_WATER)) {
		m_position.x = (float)x;
		m_position.y = (float)y;
		m_heading = dir;
		if(!m_board->board[x][y].obj->isValid()) printf("Warning: putting invalid object under %i,%i\n",x,y);
		m_board->board[x][y].under=m_board->board[x][y].obj;
		m_board->board[x][y].obj=this;
		m_bg=m_board->board[x][y].under->bg();
		m_board->board[oldx][oldy].obj=m_board->board[oldx][oldy].under;
		if(m_board->board[oldx][oldy].obj != NULL && !m_board->board[oldx][oldy].obj->isValid()) {
			printf("Warning: putting invalid object at %i,%i\n",oldx,oldy);
			m_board->board[oldx][oldy].obj = NULL;
		}
		m_board->board[oldx][oldy].under=NULL;
		if(m_board->board[oldx][oldy].obj==NULL || !m_board->board[oldx][oldy].obj->isValid()) {
			m_board->board[oldx][oldy].obj=::create_object(ZZT_EMPTY,oldx,oldy);
		}
		suc = 1;
	} else {
		them=m_board->board[x][y].obj;
		if(them!=NULL && them!=this && them->isValid() == 1) {
			if(them->flags()&F_ITEM && m_type==ZZT_PLAYER){
				them->message(this,"get");
				if(is_empty(dir)) {
					suc=1;
					task_get(them);
				}
			}
			else if(suc==0 && them->flags()&F_PUSHABLE && m_type!=ZZT_BULLET && (m_flags&F_PUSHER || !origin) ) {
				if(them->move(dir,trying,false)) {
					suc=1;
				} else {
					them->message(this,"crush");
					if(is_empty(dir)) {
						suc=1;
						task_get(them);	
					}
				}
			}
			if(suc==0) {
				if(!trying && !(m_flags & F_SLEEPING)) message(them,"thud");
				if(!trying && !(them->flags() & F_SLEEPING) && m_type != ZZT_BULLET) {
					them->message(this,"touch");
					task_touch(them);
				}
			} else {
				move(dir);
			}
		} else {
			if(!(m_flags & F_SLEEPING)) message(this,"thud");
		}
	}
	
	return !!suc;
}

int ZZTObject::str_to_color(std::string color) {
	if(color.find("black") == 0) {
		return 0;
	} else if(color.find("blue") == 0) {
		return 9;
	} else if(color.find("green") == 0) {
		return 10;
	} else if(color.find("cyan") == 0) {
		return 11;
	} else if(color.find("red") == 0) {
		return 12;
	} else if(color.find("purple") == 0) {
		return 13;
	} else if(color.find("yellow") == 0) {
		return 14;
	} else if(color.find("white") == 0) {
		return 15;
	} else {
		return -1;
	}
}

std::string ZZTObject::int_to_color(int color) {
	switch(color) {
	case 0:
	case 8:
		return "black";
		break;
	case 1:
	case 9:
		return "blue";
		break;
	case 2:
	case 10:
		return "green";
		break;
	case 3:
	case 11:
		return "cyan";
		break;
	case 4:
	case 12:
		return "red";
		break;
	case 5:
	case 13:
		return "purple";
		break;
	case 6:
	case 14:
		return "yellow";
		break;
	case 7:
	case 15:
		return "white";
		break;
	}
	return "unknown";
}

ZZTObject::ZZTObject(int type, int x, int y, int shape, int flags, std::string name) {
	m_position.x = (float)x;
	m_position.y = (float)y;
	m_step.x = 0.0f;
	m_step.y = 0.0f;
	m_heading = IDLE;
	m_type = type;
	m_cycle=0;
	m_tick=0;
	m_flags=flags;
	m_shape=shape;
	m_color=&m_fg;
	m_fg=15;
	m_bg=0;
	m_name=name;
	m_updated=0;
	m_pushed=0;
	m_progpos=0;
	m_prog="";
	m_proglen=0;
	m_highlighted = false;
	m_modelName="";
	m_height=0;
	m_board=currentbrd;
	m_isValid = true;
}

void ZZTObject::inherit(ZZTObject *o) {
	if(o->type() != ZZT_EMPTY) {
		setParam(1,o->param(1));
		setParam(2,o->param(2));
		setParam(3,o->param(3));
		setParam(4,o->param(4));
		setFg(o->fg());
		setBg(o->bg());
		setCycle(o->cycle());								
		setColor(o->color());
	}
	if(o->type()==ZZT_OBJECT) {
		setShape(o->shape());
		setProg(o->prog(),o->progLen(),o->progPos());
	}
}

extern bool debug_show_objects;
extern int disp_off_x;
extern int disp_off_y;

void ZZTObject::draw() {
	float a,b;
	Vector dist(0,0,0);
	
	if(m_position.x - disp_off_x < 0 || m_position.x - disp_off_x >= ct->getCols() || m_position.y - disp_off_y < 0 || m_position.y - disp_off_y >= ct->getRows()) return;
	
	if(m_type == ZZT_OBJECT && debug_show_objects) {
		ct->putColor((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, (HIGH_INTENSITY | ((rand() % 6)+1)));
		ct->putChar((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, (rand()%125) + 129);
		return;
	}
	
	if(m_highlighted && m_flash++ < 30) {
		ct->putColor((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, WHITE);
		ct->putChar((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, 0xDB);
		return;
	}
	
	if(player!=NULL) dist = position() - player->position();
	
	a=(dist.x)*(dist.x)/2.0f;
	b=(dist.y)*(dist.y);
	
	if(m_board->dark && world.editing == 0 && !(m_flags&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
		int color = (HIGH_INTENSITY | BLACK);
		ct->putColor((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, color);
		ct->putChar((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, 177);
#ifndef DZZT_LITE
		gl->clear((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y);
#endif
	} else if(m_board->dark && world.editing == 0 && !(m_flags&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
		int color = (HIGH_INTENSITY | BLACK);
#ifndef DZZT_LITE
		if(!gl->put((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, color, m_height, m_modelName, m_highlighted && (m_flash < 30))) {
#endif
			ct->putColor((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, color);
			ct->putChar((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, m_shape);
#ifndef DZZT_LITE
		}
#endif
	} else if(m_board->dark && world.editing == 0 && !(m_flags&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
		int color = m_fg%8;
#ifndef DZZT_LITE
		if(!gl->put((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, color, m_height, m_modelName, m_highlighted && (m_flash < 30))) {
#endif
			ct->putColor((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, color);
			ct->putChar((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, m_shape);
#ifndef DZZT_LITE
		}
#endif
	} else {
		if(m_position.x > BOARD_X) {
			int color = ((m_fg > 7) ? HIGH_INTENSITY : 0) | (m_fg%8) | (m_bg << 8);
			st->putColor((int)m_position.x - BOARD_X, (int)m_position.y, color);
			if(world.editing==1 && m_type == ZZT_INVISIBLE) {
#ifndef DZZT_LITE
				if(!gl->put((int)m_position.x - BOARD_X, (int)m_position.y, color, m_height, m_modelName, m_highlighted && (m_flash < 30)))
#endif
					st->putChar((int)m_position.x - BOARD_X, (int)m_position.y, 0xB0);
			} else {
#ifndef DZZT_LITE
				if(!gl->put((int)m_position.x - BOARD_X, (int)m_position.y, color, m_height, m_modelName, m_highlighted && (m_flash < 30)))
#endif
					st->putChar((int)m_position.x - BOARD_X, (int)m_position.y, m_shape);
			}
		} else {
			int color = ((m_fg > 7) ? HIGH_INTENSITY : 0) | (m_fg%8) | (m_bg << 8);
			ct->putColor((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, ((m_fg > 7) ? HIGH_INTENSITY : 0) | (m_fg%8) | (m_bg << 8));
			if(world.editing==1 && m_type == ZZT_INVISIBLE) {
				ct->putChar((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, 0xB0);
			} else {
#ifndef DZZT_LITE
				if(!gl->put((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, color, m_height, m_modelName, m_highlighted && (m_flash < 30)))
#endif
					ct->putChar((int)m_position.x - disp_off_x, (int)m_position.y - disp_off_y, m_shape);
			}
		}
	}
	
	if(m_flash > 40) m_flash = 0;
}

void ZZTObject::edit() {
	TUIWindow w(m_name);
	
	addEditWidgets(&w);
	w.addWidget(new TUINumericInput("Cycle                ",&m_cycle,0,255));
	w.addWidget(new TUILabel(""));
	w.addWidget(new TUILabel("             Advanced Tweaking",true));
	w.addWidget(new TUINumericInput("X Step  ",&m_step.x,-128,127));
	w.addWidget(new TUINumericInput("Y Step  ",&m_step.y,-128,127));
	
	w.doMenu();
	create();
}

#if TIKI_PLAT == TIKI_NDS
#include "objdb-nolua"
#else
#include "objdb" //autogenerated functions
#endif
