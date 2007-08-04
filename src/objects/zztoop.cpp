/* zztoop.cpp - ZZT-OOP scriptable object
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
#include <Tiki/eventcollector.h>
#include <stdlib.h>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>

using namespace Tiki;
using namespace Tiki::Hid;
using namespace Tiki::Audio;
using namespace Tiki::GL;
using namespace std;

#include "object.h"
#include "sound.h"
#include "word.h"
#include "board.h"
#include "status.h"
#include "http.h"
#include "debug.h"
#include "window.h"
#include "task.h"

extern struct world_header world;
extern Player *player;
extern ConsoleText *ct;
extern ZZTMusicStream *zm;

void ZZTOOP::setParam(int arg, unsigned char val) {
	if(arg == 1) m_shape = val;
	if(arg == 3 && val < 5) m_walk = (direction)val;
}

unsigned char ZZTOOP::param(int arg) {
	if(arg == 1) return m_shape;
	if(arg == 3) return (int)m_walk;
	return 0;
}

void ZZTOOP::create() {
	m_heading=IDLE;
}

void ZZTOOP::message(ZZTObject *them, std::string message) {
	if(!(m_flags & F_SLEEPING)) {
		if(message=="shot") task_shoot(this);
		if((message == "touch" && (them->type() == ZZT_PLAYER || them->type() == ZZT_OBJECT)) || message != "touch") zzt_goto(message);
	}
}

extern struct board_info_node *currentbrd;

ZZTObject *find_zztobj_by_name(int &x, int &y, std::string target) {
	ZZTObject *myobj;
	int i,j;
	
	transform(target.begin(), target.end(), target.begin(), ::tolower);
	
	for(j=y;j>=0;j--) {
		for(i=BOARD_X-1;i>=0;i--) {
			if(i==BOARD_X-1&&j==y) i=x;
			if(i<0) { i=BOARD_X-1; j--; }
			if(j<0) break;
			myobj=currentbrd->board[i][j].obj;
			if(myobj->type()==ZZT_OBJECT || myobj->type()==ZZT_LUA) {
				//printf("Comparing '%s' and '%s'\n",((ZZTOOP *)myobj)->get_zztobj_name().c_str(), target.c_str());
				if(((ZZTOOP *)myobj)->get_zztobj_name() == target) {
					x=i;
					y=j;
					return myobj;
				}
			}
		}
	}
	//printf("No match\n");
	return NULL;
}

std::string ZZTOOP::get_zztobj_name() {
	int y;
	std::string name;

	if((m_type==ZZT_OBJECT || m_type==ZZT_SCROLL) && m_prog[0]=='@') {
		y=1;
		while(m_prog[y]!='\r' && y < m_proglen) {
			name += m_prog[y];
			y++;
		}
		
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		
		return name;
	}
	return m_name;
}

void ZZTOOP::send(std::string cmd) {
	int x,y;
	ZZTObject *obj=NULL;
	std::string target;
	std::string label;
	std::string name;
	
	for(x=0; x < (int)cmd.length(); x++) {
		if(cmd[x]==':') {
			break;
		}
		target += cmd[x];
	}
	
	if(x == (int)cmd.length()) {
		zzt_goto(target);
		return;
	}
	
	x++;
	for(y=0; y<(int)cmd.length()-x; y++) {
		label += cmd[x+y];
	}
	
	if(target == "all" || target == "others") {
		for(y=BOARD_Y-1;y>=0;y--) {
			for(x=BOARD_X-1;x>=0;x--) {
				obj=currentbrd->board[x][y].obj;
				if(obj->type()==ZZT_OBJECT || obj->type()==ZZT_LUA) {
					if(!(target == "others" && obj==this)) {
						obj->message(this,label);
					}
				}
			}
		}
	} else {
		x=BOARD_X-1; y=BOARD_Y-1;
		obj=find_zztobj_by_name(x,y,target);
		while(obj!=NULL) {
			obj->message(this,label);
			x--;
			obj=find_zztobj_by_name(x,y,target);
		}
	}
}

void ZZTOOP::zap(std::string label) {
	std::string text;
	int x,y,newline=1,goagain=1;

	if(m_watch) debug("%s zapping %s",get_zztobj_name().c_str(),label.c_str());
	//printf("%s zapping %s",get_zztobj_name().c_str(),label.c_str());
		
	for(x=0;x<m_proglen;x++) {
		switch(m_prog[x]) {
		case '\r':
			newline=1;
			break;
		case ':':
			if(newline==1) {
				y=1;
				text = "";
				
				while(m_prog[x+y]!='\r') {
					text += m_prog[x+y];
					y++;
				}
				
				transform(text.begin(), text.end(), text.begin(), ::tolower);
				
				if(label == text) {
					m_prog[x]='\'';
					return;
				}
				x+=y-1;
				newline=0;
			}
			break;
		default:
			newline=0;
			break;
		}
	}
}

void ZZTOOP::restore(std::string label) {
	std::string text;
	int x,y,newline=0,goagain=1;
	
	if(m_watch) debug("%s restoring %s\n",get_zztobj_name().c_str(),label.c_str());
	
	//printf("%s restoring %s\n",get_zztobj_name().c_str(),label.c_str());
	
	
	for(x=0;x<m_proglen;x++) {
		switch(m_prog[x]) {
		case '\r':
			newline=1;
			break;
		case '\'':
			if(newline==1) {
				y=1;
				text="";
				while(m_prog[x+y]!='\r') {
					text += m_prog[x+y];
					y++;
				}
				
				transform(text.begin(), text.end(), text.begin(), ::tolower);
				
				if(label == text) {
					m_prog[x]=':';
				}
				x+=y-1;
				newline=0;
			}
			break;
		default:
			newline=0;
			break;
		}
	}
}

void ZZTOOP::zzt_goto(std::string label) {
	std::string text;
	int x,y,newline=0;
	if(label=="") return;

	if(label[0]=='#') {
		label.erase(0,1);
	}

	transform(label.begin(), label.end(), label.begin(), ::tolower);
	
	if(m_watch) debug("\x1b[1;37m%s\x1b[0;37m recieved \x1b[1;37m%s\n",get_zztobj_name().c_str(), label.c_str());
	
	//printf("%s: recieved %s\n",get_zztobj_name().c_str(), label.c_str());
	
	for(x=0;x<m_proglen;x++) {
		switch(m_prog[x]) {
		case '\r':
			newline=1;
			break;
		case ':':
			x++;
			if(newline==1) {
				y=0;
				text = "";
				while(m_prog[x+y]!='\r') {
					text += m_prog[x+y];
					y++;
				}
				
				transform(text.begin(), text.end(), text.begin(), ::tolower);
				
				//printf("%s comparing labels: '%s' and '%s'\n",get_zztobj_name().c_str(),label.c_str(),text.c_str());
				if(label == text) {
					m_progpos=x+y;
					//printf("Next characters: '%c,%c,%c,%c\n'",m_prog[m_progpos],m_prog[m_progpos+1],m_prog[m_progpos+2],m_prog[m_progpos+3]);
					return;
				}
				x+=(y-1);
				newline=0;
			}
			break;
		default:
			newline=0;
			break;
		}
	}
	if(label == "restart") {
		m_progpos=0;
		goagain=0;
	} else {
		//printf("Label not found\n");
	}
}

void ZZTOOP::exec(std::string text) {
	std::vector<std::string> words;
	std::string tmp;
	std::string lbl;
	std::string args;
	ZZTObject *theirobj;
	struct board_data *b;
	ZZTObject *o;
	static int textfg=1,textbg=0;
	int i,j,went=0,color,color2,neg,res;
	
	transform(text.begin(), text.end(), text.begin(), ::tolower);
	
	words = wordify(text,' ');
	if(words.size() > 1) {
				args = text.substr(words[0].length() + 1);
	} else {
				args = "";
	}

	if(m_watch) debug("\x1b[1;37m%s: \x1b[0;37m%s\n",get_zztobj_name().c_str(), text.c_str());
	
	if(words[0] == "go") {
				res=str_to_direction(args);
				if(!move((direction)res,true)) {
					m_progpos-=((short)text.length()+2);
				}
				goagain=0;
	}
	else if(words[0] == "walk") {
		m_walk=str_to_direction(args);
		if(m_walk==IDLE) goagain=1;
	}
	else if(words[0] == "play") {
		if(zm != NULL) {
			if(playedLast==0 && zm!=NULL && !zm->isPlaying()) {
				if(zm!=NULL) zm->setTune(args);
			} else {
				if(zm!=NULL) zm->appendTune(args);
			}
			goagain = 1;
			playedLast = 1;
		}
	}
	else if(words[0] == "try") {
		res=str_to_direction(args);
		if(!move((direction)res,true)) {
			zzt_goto(words[words.size() - 1]);
		}
	}
	else if(words[0] == "cycle") {
		m_cycle=atoi(args.c_str());
		if(m_cycle==0) m_cycle=1;
		goagain=1;
	}
	else if(words[0] == "put" && words.size() > 2) {
		color=str_to_color(words[2]);
		if(color!=-1) {
			words.erase(words.begin() + 2);
			if(words.size() < 3) words.push_back(std::string("empty"));
		}
		if(str_to_obj(words[2])==-1) {
			set_msg("ERR: undefined item");
			return;
		} else {
			o=get(str_to_direction(words[1]));
			if(o!=NULL && o->flags() & F_PUSHABLE) o->move(str_to_direction(words[1]),true);
			o=create_object(str_to_obj(words[2]),str_to_direction(words[1]));
			o->inherit(get(str_to_direction(words[1])));
			if(color!=-1) o->setColor(color);
			if(o->bg()>7) o->setBg(o->bg() - 8);
			remove_from_board(currentbrd,get(str_to_direction(words[1])));
			put(o);
		}
		goagain=1;
	}
	else if(words[0] == "become") {
		color=str_to_color(words[1]);
		if(color!=-1) words.erase(words.begin() + 1);
		if(str_to_obj(words[1])==-1) {
			set_msg("ERR: undefined item");
		} else {
			o=::create_object(str_to_obj(words[1]),(int)m_position.x,(int)m_position.y);
			o->setFg(m_fg);
			o->setBg(m_bg);
			o->setCycle(m_cycle);								
			if(o->type()==ZZT_OBJECT || o->type()==ZZT_LUA) {
				o->setShape(m_shape);
				o->setProg(m_prog,m_proglen,m_progpos);
			}
			if(color!=-1) o->setColor(color);
			else o->setColor(*m_color);
			m_progpos = -1;
			task_kill(this);
			remove_from_board(currentbrd, this, true);
			put(o,true);
			goagain=0;
			return;
		}
	}
	else if(words[0] == "change" && words.size() > 2) {
		goagain=1;
		color=str_to_color(words[1]);
		if(color!=-1) words.erase(words.begin() + 1);
		color2=str_to_color(words[2]);
		if(color2!=-1) words.erase(words.begin() + 2);
		i=BOARD_X-1; j=BOARD_Y-1;
		do {
			res=0;
			b=get_block_by_type(str_to_obj(words[1]),i,j);
			if(b!=NULL) {
				//Debug::printf("Type: %s\nColor: (%i) (%i)\n",b->obj->name().c_str(),b->obj->color(),color);
				if(color==-1 || b->obj->color()==color || b->obj->color() == (color - 8)) {
					if(words[1] == words[2]) {
						b->obj->setColor(color2);
					} else {
						if(str_to_obj(words[2])==-1) {
							set_msg("ERR: undefined item");
							return;
						} else {
							o=::create_object(str_to_obj(words[2]),i,j);
							o->inherit(b->obj);
							remove_from_board(currentbrd, b->obj);
							if(color2 != -1) o->setColor(color2);
							put(o);
						}
					}
				}
			}
			i--;
		} while(b!=NULL);
	}
	else if(words[0] == "__dark") {
				currentbrd->dark=atoi(words[1].c_str());
				draw_board();
	}
	else if(words[0] == "set") {
		for(i=0;i<10;i++) {
			if(world.flags[i]=="") {
				debug("\x1b[0;37mFlag \x1b[1;37m%s\x1b[0;37m set\n",words[1].c_str());
				world.flags[i] = words[1];
				break;
			}
		}
		goagain=1;
	}
	else if(words[0] == "clear") {
		for(i=0;i<10;i++) {
			if(world.flags[i] == words[1]) {
				debug("\x1b[0;37mFlag \x1b[1;37m%s\x1b[0;37m cleared\n",words[1].c_str());
				world.flags[i] = "";
				break;
			}
		}
		goagain=1;
	}
	else if(words[0] == "if") {
		int oop=-1;
		res=0;
		if(words[1] == "not") {
			neg=1;
			words.erase(words.begin() + 1);
		} else {
			neg=0;
		}
				
		if(words[2] == "then") words.erase(words.begin() + 2);
		if(words.size()>3 && words[3] == "then") words.erase(words.begin() + 3);
		
		if(words[1] == "alligned" || words[1] == "aligned") {
			if(player != NULL && (player->position().x==m_position.x || player->position().y == m_position.y)) res=1;
			oop = 2;
		} else if(words[1] == "any") {
			color=str_to_color(words[2]);
			if(color!=-1) words.erase(words.begin() + 2);
			i=BOARD_X-1; j=BOARD_Y-1;
			do {
				b=get_block_by_type(str_to_obj(words[2]),i,j);
				if(b!=NULL) {
					if(color==-1) break;
					if(color<8 && b->obj->color()>=8) color+=8;
					if(color>=8 && b->obj->color()<8) color-=8;
				}
				i--;
			} while(b!=NULL && (color!=-1 && b->obj->color()!=color));
			if(b!=NULL) res=1;
			oop = 3;
		} else if(words[1] == "contact") {
			oop = 2;
			if(m_position.y > 0 && currentbrd->board[(int)m_position.x][(int)m_position.y-1].obj!=NULL) {
				if(currentbrd->board[(int)m_position.x][(int)m_position.y-1].obj->type()==ZZT_PLAYER) {
					res=1;
				}
			}
			if(m_position.y < BOARD_Y-1 && currentbrd->board[(int)m_position.x][(int)m_position.y+1].obj!=NULL) {
				if(currentbrd->board[(int)m_position.x][(int)m_position.y+1].obj->type()==ZZT_PLAYER) {
					res=1;
				}
			}
			if(m_position.x > 0 && currentbrd->board[(int)m_position.x-1][(int)m_position.y].obj!=NULL) {
				if(currentbrd->board[(int)m_position.x-1][(int)m_position.y].obj->type()==ZZT_PLAYER) {
					res=1;
				}
			}
			if(m_position.x < BOARD_X-1 && currentbrd->board[(int)m_position.x+1][(int)m_position.y].obj!=NULL) {
				if(currentbrd->board[(int)m_position.x+1][(int)m_position.y].obj->type()==ZZT_PLAYER) {
					res=1;
				}
			}
		} else if(words[1] == "blocked") {
			switch(str_to_direction(text.substr((words[0] + words[1]).length() + 2))) {
				case UP:
					if(::is_empty(currentbrd, (int)m_position.x, (int)m_position.y-1)==0) {
						res=1;
					}
					break;
				case DOWN:
					if(::is_empty(currentbrd, (int)m_position.x, (int)m_position.y+1)==0) {
						res=1;
					}
					break;
				case RIGHT:
					if(::is_empty(currentbrd, (int)m_position.x+1, (int)m_position.y)==0) {
						res=1;
					}
					break;
				case LEFT:
					if(::is_empty(currentbrd, (int)m_position.x-1, (int)m_position.y)==0) {
						res=1;
					}
					break;
			}
			oop = 3;
		} else {
			oop = 2;
			if(words[1] == "dreamcast") {
#ifdef DREAMCAST
				res=1;
#else
				res=0;
#endif
			} else if (words[1] == "pc") {
#ifndef DREAMCAST
				res=1;
#else
				res=0;
#endif
			} else if(words[1] == "energized") {
				res=(world.energizer_cycle>0);
			} else {
				for(i=0;i<10;i++) {
					if(world.flags[i] == words[1]) {
						res=1;
						break;
					}
				}
			}
		}
		if(neg==1) res=!res;
		goagain=1;
		if(res==1) {
			lbl = "";
			for(int i=oop; i < words.size(); i++) {
				lbl += words[i];
				if(i != words.size() -1) lbl += " ";
			}
			exec(lbl);
		}
	}
	else if(words[0] == "bind") {
		i=BOARD_X-1;
		j=BOARD_Y-1;
		theirobj=find_zztobj_by_name(i,j,words[1]);
		if(theirobj!=NULL)	{
			m_prog=theirobj->prog();
			m_progpos=0;
			m_proglen=theirobj->progLen();
			return;
		}
		return;
	}
	else if(words[0] == "zap") {
				tmp = words[1];
				lbl = "";
				for(i=0;i<(int)tmp.length();i++) {
					lbl += tmp[i];
					if(tmp[i]==':') break;
				}
				
				if(i<(int)tmp.length()) {
					int x=BOARD_X-1,y=BOARD_Y-1;
					theirobj=find_zztobj_by_name(x,y,lbl);
					while(theirobj!=NULL) {
						((ZZTOOP *)theirobj)->zap(tmp.substr(i+1));
						x--;
						theirobj=find_zztobj_by_name(x,y,lbl);
					}
				} else {
					zap(lbl);
				}
				goagain=1;
	}
	else if(words[0] == "restore") {
				tmp = words[1];
				lbl = "";
				
				for(i=0;i<(int)tmp.length();i++) {
					lbl += tmp[i];
					if(tmp[i]==':') break;
				}
				
				if(i<(int)tmp.length()) {
					int x=BOARD_X-1,y=BOARD_Y-1;
					((ZZTOOP *)find_zztobj_by_name(x,y,lbl))->restore(tmp.substr(i+1));
				} else {
					restore(lbl);
				}
				goagain=1;
	}
	else if(words[0] == "lock") {
		m_flags|=F_SLEEPING;
		goagain=1;
	}
	else if(words[0] == "unlock") {
		m_flags&=~F_SLEEPING;
		goagain=1;
	}
	else if(words[0] == "send") {
		send(words[1]);
		goagain=1;
	}
	/*else if(words[0] == "teleport") {
		if(::is_empty(currentbrd,atoi(words[1]),atoi(words[2]))==0) {
			//set_msg("Invalid teleport.");
		} else {
			m_position.x=atoi(get_word(1));
			m_position.y=atoi(get_word(2));
		}
	}*/
	else if(words[0] == "color") {
		m_fg=atoi(words[1].c_str());
	}
	else if(words[0] == "char") {
		m_shape=atoi(words[1].c_str());
		draw();
				goagain=1;
	}
	else if(words[0] == "give") {
		if(words[1] == "ammo") {
			give_ammo(atoi(words[2].c_str()));
		}
		if(words[1] == "gems") {
			give_gems(atoi(words[2].c_str()));
		}
		if(words[1] == "torch" || words[1] == "torches") {
			give_torch(atoi(words[2].c_str()));
		}
		if(words[1] == "score") {
			give_score(atoi(words[2].c_str()));
		}
		if(words[1] == "health") {
			give_health(atoi(words[2].c_str()));
		}
		if(words[1] == "time") {
			give_time(atoi(words[2].c_str()));
		}
		goagain=1;
	}
	else if(words[0] == "take") {
		if(words[1] == "gems") {
			if(words.size() > 3 && world.gems<atoi(words[2].c_str())) { zzt_goto(words[3]); }
			else take_gems(atoi(words[2].c_str()));
		}
		if(words[1] == "torch" || words[1] == "torches") {
			if(words.size() > 3 && world.torches<atoi(words[2].c_str())) { zzt_goto(words[3]); }
			else take_torch(atoi(words[2].c_str()));
		}
		if(words[1] == "ammo") {
			if(words.size() > 3 && world.ammo<atoi(words[2].c_str())) { zzt_goto(words[3]); }
			else take_ammo(atoi(words[2].c_str()));
		}
		if(words[1] == "score") {
			if(words.size() > 3 && world.score<atoi(words[2].c_str())) { zzt_goto(words[3]); }
			else take_score(atoi(words[2].c_str()));
		}
		if(words[1] == "health") {
			if(words.size() > 3 && world.health<atoi(words[2].c_str())) { zzt_goto(words[3]); }
			else take_health(atoi(words[2].c_str()));
		}
		if(words[1] == "time") {
			if(currentbrd->time<atoi(words[2].c_str())) { zzt_goto(words[3]); }
			else take_time(atoi(words[2].c_str()));
		}
		goagain=1;
	}
	else if(words[0] == "die") {
		task_kill(this);
		m_flags = F_DELETED;
		m_progpos=-1;
	}
	else if(words[0] == "end") {
		m_progpos=-1;
		return;
	}
	else if(words[0] == "endgame") {
		world.health=0;
		return;
	}
	else if(words[0] == "throwstar") {
	}
	else if(words[0] == "shoot") {
		shoot(str_to_direction(args));
	}
	else {
		send(words[0]);
		goagain=1;
	}	
}

void ZZTOOP::update() {
	std::string text;
	int x=0,y=0,newline=0,linecount=0,ty=0,went=0;
	std::string lbl;
	std::string msg;
	
	if(m_proglen<1 || world.health<=0) return;
	
	playedLast = 0;
	goagain = 1;
	
	while(goagain) {
		goagain=0;
		if(m_progpos>m_proglen || m_progpos==-1) { break; }
		text="";
		//if(m_watch) printf("%s: next characters: '%c,%c,%c,%c\n'",get_zztobj_name().c_str(),m_prog[m_progpos],m_prog[m_progpos+1],m_prog[m_progpos+2],m_prog[m_progpos+3]);
		switch(m_prog[m_progpos]) {
		case ':':
			while(m_prog[m_progpos]!='\r') {
				m_progpos++;
			}
			goagain=1;
			break;
		case '?':
			x=++m_progpos;
			y=0;
			while(m_prog[y+x]!='\r' && m_prog[y+x]!='#' && m_prog[y+x]!='?' && m_prog[y+x]!='/') {
				text += m_prog[y+x];
				y++;
			}
			m_progpos += (y-1);
				
			if(m_watch) debug("\x1b[1;37m%s: \x1b[0;37m%s\n",get_zztobj_name().c_str(), text.c_str());
#ifdef DEBUG
			//printf("%s: %s\n",get_zztobj_name().c_str(), text.c_str());
#endif
			move(str_to_direction(text), true);			
			goagain=0;
			break;
		case '/':
			x=++m_progpos;
			y=0;
			while(m_prog[y+x]!='\r' && m_prog[y+x]!='#' && m_prog[y+x]!='?' && m_prog[y+x]!='/') {
				text += m_prog[y+x];
				y++;
			}
			if(m_watch) debug("\x1b[1;37m%s: \x1b[0;37m%s\n",get_zztobj_name().c_str(), text.c_str());
#ifdef DEBUG
			//printf("%s: %s\n",get_zztobj_name().c_str(), text.c_str());
#endif
			
			if(move(str_to_direction(text), true)) {
				m_progpos+=(y-1);
			} else {
				m_progpos-=2;
			}
				
			goagain=0;
			break;
		case '\'':
		case '@':
			while(m_prog[m_progpos]!='\r') {
				m_progpos++;
			}
			goagain=1;
			break;
		case '^':
		case '#':
			x=++m_progpos;
			y=0;
			while(m_prog[y+x]!='\r') {
				text += m_prog[y+x];
				y++;
				m_progpos++;
			}
#ifdef DEBUG
		//Debug::printf("%s: %s\n",get_zztobj_name().c_str(), text.c_str());
#endif
			
			exec(text);	
			break;
		case '\r':
			if(msg.size() > 0) msg += "\r";
			goagain=1;
			break;
		default:
		if(msg.size() > 0) msg += "\r";
			while(m_prog[m_progpos]!='\r' && m_progpos < m_proglen) {
				msg += m_prog[m_progpos++];
			}
			went=0;
			goagain=1;
			break;
		}
		if(m_progpos>0) m_progpos++;
		if(went++>6) goagain=0;
	}

	if(msg.size() > 0 && msg!="\r") {
#ifdef DEBUG
		//Debug::printf("%s: %s\n",get_zztobj_name().c_str(), msg.c_str());
#endif
		if(msg.find("\r") != msg.npos && msg.find("\r") != (msg.length() - 1) && (world.magic==65535 || (msg.find("\r", msg.find("\r") + 1) != (msg.length() - 1)))) {
			if(zm!=NULL && zm->hasTune()) zm->start();
			TUIWindow *t= new TUIWindow(get_zztobj_name());
			t->buildFromString(msg);
			t->doMenu();
			draw_board();
			if(t->getLabel()!="\0") { zzt_goto(t->getLabel()); }
			delete t;
		} else {
			if(msg[msg.length() -1] == '\r') msg.resize(msg.length()-1);
			set_msg((char *)msg.c_str());
		}
	}

	if(m_walk!=IDLE && !(m_flags & F_DELETED)) {
		move(m_walk);
	}
	if(zm!=NULL && zm->hasTune()) zm->start();
}
