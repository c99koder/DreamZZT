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
#include "debug.h"
#include "window.h"
#include "task.h"

extern struct world_header world;
extern Player *player;
extern ConsoleText *ct;
extern ZZTMusicStream *zm;

extern std::map <int, ZZTObject *> ZZTObjectTypes;

enum kwds {
//inventory (excluding 'ammo' and 'torch' which are already in the ZZTObject map)
	kwdGems,
	kwdTorch,
	kwdHealth,
	kwdScore,
	kwdTime,
//logic flags
	kwdAny,
	kwdAligned,
	kwdContact,
	kwdEnergized,
	kwdBlocked,
	kwdNot,
//directional modifiers
	kwdCw,
	kwdCcw,
	kwdFlow,
	kwdOpp,
	kwdRndp,
	kwdRndns,
	kwdRndne,
	kwdSeek,
//commands
	kwd__Dark,
	kwdBecome,
	kwdBind,
	kwdChar,
	kwdChange,
	kwdClear,
	kwdColor,
	kwdCycle,
	kwdDie,
	kwdEnd,
	kwdEndgame,
	kwdGive,
	kwdGo,
	kwdIf,
	kwdLock,
	kwdPlay,
	kwdPut,
	kwdRestore,
	kwdSend,
	kwdSet,
	kwdShoot,
	kwdTake,
	kwdTry,
	kwdUnlock,
	kwdWalk,
	kwdZap
};

std::map<std::string, int> Commands;
std::map<std::string, int> Colors;
std::map<std::string, int> Directions;
std::map<std::string, int> Logics;
std::map<std::string, int> Inventorys;
std::map<std::string, int> ZZTObjects;

void buildSyntaxMap() {
	Commands["__dark"] = kwd__Dark;
	Commands["become"] = kwdBecome;
	Commands["bind"] = kwdBind;
	Commands["char"] = kwdChar;
	Commands["change"] = kwdChange;
	Commands["clear"] = kwdClear;
	Commands["color"] = kwdColor;
	Commands["cycle"] = kwdCycle;
	Commands["die"] = kwdDie;
	Commands["end"] = kwdEnd;
	Commands["endgame"] = kwdEndgame;
	Commands["give"] = kwdGive;
	Commands["go"] = kwdGo;
	Commands["if"] = kwdIf;
	Commands["lock"] = kwdLock;
	Commands["play"] = kwdPlay;
	Commands["put"] = kwdPut;
	Commands["restore"] = kwdRestore;
	Commands["send"] = kwdSend;
	Commands["set"] = kwdSet;
	Commands["shoot"] = kwdShoot;
	Commands["take"] = kwdTake;
	Commands["try"] = kwdTry;
	Commands["unlock"] = kwdUnlock;
	Commands["walk"] = kwdWalk;
	Commands["zap"] = kwdZap;
	
	Colors["black"] = BLACK;
	Colors["blue"] = BLUE+8;
	Colors["green"] = GREEN+8;
	Colors["cyan"] = CYAN+8;
	Colors["red"] = RED+8;
	Colors["purple"] = MAGENTA+8;
	Colors["magenta"] = MAGENTA+8;
	Colors["yellow"] = YELLOW+8;
	Colors["white"] = WHITE+8;
	
	Directions["n"] = UP;
	Directions["north"] = UP;
	Directions["s"] = DOWN;
	Directions["south"] = DOWN;
	Directions["e"] = RIGHT;
	Directions["east"] = RIGHT;
	Directions["w"] = LEFT;
	Directions["west"] = LEFT;
	Directions["i"] = IDLE;
	Directions["idle"] = IDLE;
	Directions["ccw"] = kwdCcw;
	Directions["cw"] = kwdCw;
	Directions["flow"] = kwdFlow;
	Directions["opp"] = kwdOpp;
	Directions["rndp"] = kwdRndp;
	Directions["rndns"] = kwdRndns;
	Directions["rndne"] = kwdRndne;
	Directions["seek"] = kwdSeek;
	
	Logics["any"] = kwdAny;
	Logics["aligned"] = kwdAligned;
	Logics["alligned"] = kwdAligned;
	Logics["blocked"] = kwdBlocked;
	Logics["contact"] = kwdContact;
	Logics["energized"] = kwdEnergized;
	Logics["not"] = kwdNot;
	
	Inventorys["gems"] = kwdGems;
	Inventorys["health"] = kwdHealth;
	Inventorys["score"] = kwdScore;
	Inventorys["torches"] = kwdTorch;
	Inventorys["time"] = kwdTime;
	
	for(std::map<int, ZZTObject *>::iterator o = ZZTObjectTypes.begin(); o != ZZTObjectTypes.end(); o++) {
		if(o->second != NULL) ZZTObjects[o->second->name()] = o->first;
	}
}

std::vector<Keyword> ZZTOOP::tokenize(std::string text, bool parseDirections) {
	Keyword k;
	std::vector<Keyword> keywords;
	std::vector<std::string> words;
	if(Commands.empty()) buildSyntaxMap();
	
	words = wordify(text,' ');
	
	for(std::vector<std::string>::iterator word = words.begin(); word != words.end(); word++) {
		if(Commands.find(*word) != Commands.end()) {
			k.type = ktypeCommand;
			k.value = (int)Commands[*word];
		} else if(Inventorys.find(*word) != Inventorys.end()) {
			k.type = ktypeInventory;
			k.value = (int)Inventorys[*word];
		} else if(Logics.find(*word) != Logics.end()) {
			k.type = ktypeLogic;
			k.value = (int)Logics[*word];
		} else if(Colors.find(*word) != Colors.end()) {
			k.type = ktypeColor;
			k.value = (int)Colors[*word];
		} else if(ZZTObjects.find(*word) != ZZTObjects.end()) {
			k.type = ktypeObject;
			k.value = (int)ZZTObjects[*word];
		} else if(Directions.find(*word) != Directions.end()) {
			k.type = ktypeDirection;
			if(parseDirections) {
				bool opp=false, cw=false, ccw=false;
				direction res=IDLE;
				for( ; word != words.end() && Directions.find(*word) != Directions.end(); word++) {
					if((*word) == "opp") {
						opp = !opp;
					} else if((*word) == "cw") {
						cw=true;
					} else if((*word) == "ccw") {
						ccw=true;
					} else if((*word) == "rndp") {
						if(rand()%2==0) {
							cw=true;
						} else {
							ccw=true;
						}
					} else if((*word) == "flow") {
						res=m_heading;
						break;
					} else if((*word) == "seek") {
						res=toward(player);
						break;
					} else if((*word) == "rndns") {
						if(rand()%2==1) {
							res=UP;
						} else {
							res=DOWN;
						}
						break;
					} else if((*word) == "rndne") {
						if(rand()%2==1) {
							res=UP;
						} else {
							res=RIGHT;
						}
						break;
					} else if((*word) == "n" || (*word) == "north") {
						res = UP;
						break;
					} else if((*word) == "s" || (*word) == "south") {
						res = DOWN;
						break;
					} else if((*word) == "e" || (*word) == "east") {
						res = RIGHT;
						break;
					} else if((*word) == "w" || (*word) == "west") {
						res = LEFT;
						break;
					} else if((*word) == "i" || (*word) == "idle") {
						res = IDLE;
						break;
					}
				}
				if(cw) {
					res=clockwise(res);
				}
				if(ccw) {
					res=opposite(clockwise(res));
				}
				if(opp) {
					res=opposite(res);
				}
				k.value = (int)res;
				opp = cw = ccw = false;
			} else {
				k.value = (int)Directions[*word];
			}
		} else if(atoi((*word).c_str()) > 0) {
			k.type = ktypeNumber;
			k.value = atoi((*word).c_str());
		} else {
			k.type = ktypeString;
			k.value = -1;
		}
		k.word = *word;
		keywords.push_back(k);
	}
	return keywords;
}

void ZZTOOP::setParam(int arg, unsigned char val) {
	if(arg == 1)
		m_shape = val;
	if(arg == 3 && val < 5)
		m_walk = (direction)val;
}

unsigned char ZZTOOP::param(int arg) {
	if(arg == 1)
		return m_shape;
	if(arg == 3)
		return (int)m_walk;
	return 0;
}

void ZZTOOP::create() {
	m_heading=IDLE;
}

void ZZTOOP::message(ZZTObject *them, std::string message) {
	if(!(m_flags & F_SLEEPING)) {
		if(message=="shot")
			task_shoot(this);
		if((message == "touch" && (them->type() == ZZT_PLAYER || them->type() == ZZT_OBJECT)) || message != "touch")
			zzt_goto(message);
	}
}

extern struct board_info_node *currentbrd;

ZZTObject *find_zztobj_by_name(int &x, int &y, std::string target) {
	ZZTObject *myobj;
	int i,j;

	transform(target.begin(), target.end(), target.begin(), ::tolower);

	for(j=y;j>=0;j--) {
		for(i=BOARD_X-1;i>=0;i--) {
			if(i==BOARD_X-1&&j==y)
				i=x;
			if(i<0) {
				i=BOARD_X-1;
				j--;
			}
			if(j<0)
				break;
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

	if((m_name=="object" || m_name=="scroll") && m_prog[0]=='@') {
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
		x=BOARD_X-1;
		y=BOARD_Y-1;
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

	if(m_watch)
		debug("%s zapping %s",get_zztobj_name().c_str(),label.c_str());
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

	if(m_watch)
		debug("%s restoring %s\n",get_zztobj_name().c_str(),label.c_str());

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
	if(label=="")
		return;

	if(label[0]=='#') {
		label.erase(0,1);
	}

	transform(label.begin(), label.end(), label.begin(), ::tolower);

	if(m_watch)
		debug("\x1b[1;37m%s\x1b[0;37m recieved \x1b[1;37m%s\n",get_zztobj_name().c_str(), label.c_str());

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

				if(label == text) {
					m_progpos=x+y;
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
	std::vector<Keyword> keywords;
	std::string tmp;
	std::string lbl;
	std::string args;
	ZZTObject *theirobj;
	struct board_data *b;
	ZZTObject *o;
	static int textfg=1,textbg=0;
	int i,j,went=0,color,color2,neg,res,oop;

	transform(text.begin(), text.end(), text.begin(), ::tolower);

	keywords = tokenize(text);
	
	if(keywords.size() > 1) {
		args = text.substr(keywords[0].word.length() + 1);
	} else {
		args = "";
	}

	if(m_watch)
		debug("\x1b[1;37m%s: \x1b[0;37m%s\n",get_zztobj_name().c_str(), text.c_str());

	printf("%s: %s\n",get_zztobj_name().c_str(), text.c_str());

	if(keywords[0].type == ktypeDirection) {
		move((direction)keywords[0].value, true);
	} else if(keywords[0].type == ktypeCommand) {
		switch(keywords[0].value) {
			case kwd__Dark:
				currentbrd->dark=keywords[1].value;
				draw_board();
				break;
			case kwdBecome:
				if(keywords[1].type == ktypeColor && keywords.size() > 2) {
					color = keywords[1].value;
					keywords.erase(keywords.begin() + 1);
					if(keywords.size() < 2) {
						Keyword k;
						k.type = ktypeObject;
						k.value = ZZT_EMPTY;
						k.word = "empty";
						keywords.push_back(k);
					}
				}
				if(keywords[1].value < 0) {
					set_msg("ERR: undefined item");
					return;
				} else {
					o=::create_object(keywords[1].value);
					o->setPosition(m_position);
					o->setFg(m_fg);
					o->setBg(m_bg);
					o->setCycle(m_cycle);
					if(o->type()==ZZT_OBJECT || o->type()==ZZT_LUA) {
						o->setShape(m_shape);
						o->setProg(m_prog,m_proglen,m_progpos);
					}
					if(color!=-1)
						o->setColor(color);
					else
						o->setColor(*m_color);
					m_progpos = -1;
					task_kill(this);
					remove_from_board(m_board, this, true);
					put(o,o->position().x,o->position().y,true);
					goagain=0;
					return;
				}
				break;
			case kwdBind:
				i=BOARD_X-1;
				j=BOARD_Y-1;
				theirobj=find_zztobj_by_name(i,j,keywords[1].word);
				if(theirobj!=NULL)	{
					m_prog=theirobj->prog();
					m_progpos=0;
					m_proglen=theirobj->progLen();
					return;
				}
				return;
				break;
			case kwdChange:
				if(keywords.size() > 2) {
					goagain=1;
					if(keywords[1].type == ktypeColor) {
						color=keywords[1].value;
						keywords.erase(keywords.begin() + 1);
					} else {
						color=-1;
					}
					if(keywords[2].type == ktypeColor) {
						color2=keywords[2].value;
						keywords.erase(keywords.begin() + 2);
					} else {
						color2=-1;
					}
					i=BOARD_X-1;
					j=BOARD_Y-1;
					//printf("Searching for: %s (%i)\n", keywords[1].word.c_str(), keywords[1].value);
					do {
						res=0;
						b=get_block_by_type(keywords[1].value,i,j);
						if(b!=NULL) {
							//Debug::printf("Type: %s\nColor: (%i) (%i)\n",b->obj->name().c_str(),b->obj->color(),color);
							if(color==-1 || b->obj->color()==color || b->obj->color() == (color - 8)) {
								if(keywords[1].value == keywords[2].value) {
									b->obj->setColor(color2);
								} else {
									if(keywords[2].value < 0) {
										set_msg("ERR: undefined item");
										return;
									} else {
										o=::create_object(keywords[2].value);
										o->inherit(b->obj);
										remove_from_board(currentbrd, b->obj);
										if(color2 != -1)
											o->setColor(color2);
										put(o,i,j);
									}
								}
							}
						}
						i--;
					} while(b!=NULL);
				}
				break;
			case kwdChar:
				m_shape=keywords[1].value;
				draw();
				goagain=1;
				break;
			case kwdClear:
				for(i=0;i<10;i++) {
					if(world.flags[i] == keywords[1].word) {
						debug("\x1b[0;37mFlag \x1b[1;37m%s\x1b[0;37m cleared\n",keywords[1].word.c_str());
						world.flags[i] = "";
						break;
					}
				}
				goagain=1;
				break;
			case kwdColor:
				m_fg=keywords[1].value;
				goagain=1;
				break;
			case kwdCycle:
				m_cycle=keywords[1].value;
				if(m_cycle==0)
					m_cycle=1;
				goagain=1;
				break;
			case kwdDie:
				task_kill(this);
				m_flags = F_DELETED;
				m_progpos=-1;
				break;
			case kwdEnd:
				m_progpos=-1;
				return;
			case kwdEndgame:
				world.health=0;
				return;
			case kwdGive:
				switch(keywords[1].value) {
					case ZZT_AMMO:
						give_ammo(keywords[2].value);
						break;
					case kwdGems:
						give_gems(keywords[2].value);
						break;
					case kwdTorch:
					case ZZT_TORCH:
						give_torch(keywords[2].value);
						break;
					case kwdScore:
						give_score(keywords[2].value);
						break;
					case kwdHealth:
						give_health(keywords[2].value);
						break;
					case kwdTime:
						give_time(keywords[2].value);
						break;
				}
				goagain=1;
				break;
			case kwdGo:
				if(!move((direction)keywords[1].value,true)) {
					m_progpos-=((short)text.length()+2);
				}
				goagain=0;
				break;
			case kwdIf:
				oop=-1;
				res=0;
				if(keywords[1].value == kwdNot) {
					neg=1;
					keywords.erase(keywords.begin() + 1);
				} else {
					neg=0;
				}

				if(keywords[2].word == "then")
					keywords.erase(keywords.begin() + 2);
				if(keywords.size()>3 && keywords[3].word == "then")
					keywords.erase(keywords.begin() + 3);

				switch(keywords[1].value) {
					case kwdAligned:
						if(player != NULL && (player->position().x==m_position.x || player->position().y == m_position.y))
							res=1;
						oop = 2;
						break;
					case kwdAny:
						color=keywords[2].value;
						if(color!=-1)
							keywords.erase(keywords.begin() + 2);
						i=BOARD_X-1;
						j=BOARD_Y-1;
						do {
							b=get_block_by_type(keywords[2].value,i,j);
							if(b!=NULL) {
								if(color==-1)
									break;
								if(color<8 && b->obj->color()>=8)
									color+=8;
								if(color>=8 && b->obj->color()<8)
									color-=8;
							}
							i--;
						} while(b!=NULL && (color!=-1 && b->obj->color()!=color));
						if(b!=NULL)
							res=1;
						oop = 3;
						break;
					case kwdContact:
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
						break;
					case kwdBlocked:
						switch((direction)keywords[2].value) {
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
						break;
					case kwdEnergized:
						res=(world.energizer_cycle>0);
						oop = 2;
						break;
					default:
						oop = 2;
						for(i=0;i<10;i++) {
							if(world.flags[i] == keywords[1].word) {
								res=1;
								break;
							}
						}
						break;
				}
				if(neg==1)
					res=!res;
				goagain=1;
				if(res==1) {
					lbl = "";
					for(int i=oop; i < keywords.size(); i++) {
						lbl += keywords[i].word;
						if(i != keywords.size() - 1)
							lbl += " ";
					}
					exec(lbl);
				}
				break;
			case kwdLock:
				m_flags|=F_SLEEPING;
				goagain=1;
				break;
			case kwdPlay:
				if(zm != NULL) {
					if(playedLast==0 && zm!=NULL && !zm->isPlaying()) {
						if(zm!=NULL)
							zm->setTune(args);
					} else {
						if(zm!=NULL)
							zm->appendTune(args);
					}
					goagain = 1;
					playedLast = 1;
				}
				break;
			case kwdPut:
				if(keywords[2].type == ktypeColor) {
					color = keywords[2].value;
					keywords.erase(keywords.begin() + 2);
					if(keywords.size() < 3) {
						Keyword k;
						k.type = ktypeObject;
						k.value = ZZT_EMPTY;
						k.word = "empty";
						keywords.push_back(k);
					}
				}
				if(keywords[2].value < 0) {
					set_msg("ERR: undefined item");
					return;
				} else {
					o=get((direction)keywords[1].value);
					if(o!=NULL && o->flags() & F_PUSHABLE)
						o->move((direction)keywords[1].value,true);
					o=create_object(keywords[2].value,(direction)keywords[1].value);
					o->inherit(get((direction)keywords[1].value));
					if(color!=-1)
						o->setColor(color);
					if(o->bg()>7)
						o->setBg(o->bg() - 8);
					remove_from_board(currentbrd,get((direction)keywords[1].value));
					put(o, o->position().x, o->position().y);
				}
				goagain=1;
				break;
			case kwdRestore:
				tmp = keywords[1].word;
				lbl = "";

				for(i=0;i<(int)tmp.length();i++) {
					lbl += tmp[i];
					if(tmp[i]==':')
						break;
				}

				if(i<(int)tmp.length()) {
					int x=BOARD_X-1,y=BOARD_Y-1;
					((ZZTOOP *)find_zztobj_by_name(x,y,lbl))->restore(tmp.substr(i+1));
				} else {
					restore(lbl);
				}
				goagain=1;
				break;
			case kwdSend:
				send(keywords[1].word);
				goagain=1;
				break;
			case kwdShoot:
				shoot((direction)keywords[1].value);
				break;
			case kwdSet:
				for(i=0;i<10;i++) {
					if(world.flags[i]=="") {
						debug("\x1b[0;37mFlag \x1b[1;37m%s\x1b[0;37m set\n",keywords[1].word.c_str());
						world.flags[i] = keywords[1].word;
						break;
					}
				}
				goagain=1;
				break;
			case kwdTake:
				switch(keywords[1].value) {
					case kwdGems:
						if(keywords.size() > 3 && world.gems<keywords[2].value) {
							zzt_goto(keywords[3].word);
						} else
							take_gems(keywords[2].value);
						break;
					case ZZT_AMMO:
						if(keywords.size() > 3 && world.ammo<keywords[2].value) {
							zzt_goto(keywords[3].word);
						} else
							take_ammo(keywords[2].value);
						break;
					case kwdTorch:
					case ZZT_TORCH:
						if(keywords.size() > 3 && world.torches<keywords[2].value) {
							zzt_goto(keywords[3].word);
						} else
							take_torch(keywords[2].value);
						break;
					case kwdScore:
						if(keywords.size() > 3 && world.score<keywords[2].value) {
							zzt_goto(keywords[3].word);
						} else
							take_score(keywords[2].value);
						break;
					case kwdHealth:
						if(keywords.size() > 3 && world.health<keywords[2].value) {
							zzt_goto(keywords[3].word);
						} else
							take_health(keywords[2].value);
						break;
					case kwdTime:
						if(keywords.size() > 3 && world.time<keywords[2].value) {
							zzt_goto(keywords[3].word);
						} else
							take_time(keywords[2].value);
						break;
				}
				goagain=1;
				break;
			/*
			 * kwdThrowstar
			 */
			case kwdTry:
				if(!move((direction)keywords[1].value,true)) {
					zzt_goto(keywords[keywords.size() - 1].word);
				}
				break;
			case kwdUnlock:
				m_flags&=~F_SLEEPING;
				goagain=1;
				break;
			case kwdWalk:
				m_walk=(direction)keywords[1].value;
				if(m_walk==IDLE)
					goagain=1;
				break;
			case kwdZap:
				tmp = keywords[1].word;
				lbl = "";
				for(i=0;i<(int)tmp.length();i++) {
					lbl += tmp[i];
					if(tmp[i]==':')
						break;
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
				break;
			default:
				printf("Unmatched command: %s\n", keywords[0].word.c_str());
				break;
		}
	} else {
		send(keywords[0].word);
		goagain=1;
	}
}

void ZZTOOP::update() {
	std::string text;
	int x=0,y=0,newline=0,linecount=0,ty=0,went=0;
	std::string lbl;
	std::string msg;

	if(m_proglen<1 || world.health<=0)
		return;

	playedLast = 0;
	goagain = 1;

	while(goagain) {
		goagain=0;
		if(m_progpos>m_proglen || m_progpos==-1) {
			break;
		}
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

			transform(text.begin(), text.end(), text.begin(), ::tolower);

			if(m_watch)
				debug("\x1b[1;37m%s: \x1b[0;37m%s\n",get_zztobj_name().c_str(), text.c_str());

			move((direction)(tokenize(text)[0].value), true);
			goagain=0;
			break;
		case '/':
			x=++m_progpos;
			y=0;
			while(m_prog[y+x]!='\r' && m_prog[y+x]!='#' && m_prog[y+x]!='?' && m_prog[y+x]!='/') {
				text += m_prog[y+x];
				y++;
			}
			
			transform(text.begin(), text.end(), text.begin(), ::tolower);
			
			if(m_watch)
				debug("\x1b[1;37m%s: \x1b[0;37m%s\n",get_zztobj_name().c_str(), text.c_str());

			if(move((direction)(tokenize(text)[0].value), true)) {
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
			if(msg.size() > 0)
				msg += "\r";
			goagain=1;
			break;
		default:
			if(msg.size() > 0)
				msg += "\r";
			while(m_prog[m_progpos]!='\r' && m_progpos < m_proglen) {
				msg += m_prog[m_progpos++];
			}
			went=0;
			goagain=1;
			break;
		}
		if(m_progpos>0)
			m_progpos++;
		if(went++>6)
			goagain=0;
	}

	if(msg.size() > 0 && msg!="\r") {
#ifdef DEBUG
		//Debug::printf("%s: %s\n",get_zztobj_name().c_str(), msg.c_str());
#endif
		int num = 0;
		for(int q = 0; q < msg.length(); q++) {
			if(msg[q] == '\r' && q != msg.length()-1)
				num++;
		}
		if(num >= 1 && (world.magic==65535 || num > 2)) {
			if(zm!=NULL && zm->hasTune())
				zm->start();
			TUIWindow *t= new TUIWindow(get_zztobj_name());
			t->buildFromString(msg);
			t->doMenu();
			draw_board();
			if(t->getLabel()!="\0") {
				zzt_goto(t->getLabel());
			}
			delete t;
		} else {
			if(msg[msg.length() -1] == '\r')
				msg.resize(msg.length()-1);
			set_msg((char *)msg.c_str());
		}
	}

	if(m_walk!=IDLE && !(m_flags & F_DELETED)) {
		move(m_walk);
	}
	if(zm!=NULL && zm->hasTune())
		zm->start();
}
