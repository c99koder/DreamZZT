/* board.cpp - World load / save and utility functions
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
#include <Tiki/eventcollector.h>
#include <Tiki/tikitime.h>
#include <Tiki/thread.h>
#include <Tiki/file.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <machine/endian.h>
#include <algorithm>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Audio;

#include <Tiki/drawables/console.h>
#include "object.h"
#include "board.h"
#include "status.h"
#include "sound.h"

extern ZZTMusicStream *zm;

extern ConsoleText *ct;
extern ConsoleText *dt;
extern ConsoleText *st;

struct world_header world;
struct board_info_node *board_list=NULL;
struct board_info_node *currentbrd=NULL;
extern Player *player;
extern EventCollector *playerEventCollector;

struct board_info_node *get_current_board() { return currentbrd; }
int world_sec=10;
extern float zoom;

char spin_anim[4]={'|','/','-','\\'};

void free_world() {
	struct board_info_node *current=board_list;
	struct board_info_node *prev=NULL;
	std::vector<zzt_param>::iterator param_iter;
	int x,y;
	
	while(current!=NULL) {
		if(!current->compressed) {
			for(y=0;y<BOARD_Y;y++) {
				for(x=0;x<BOARD_X;x++) {
					if(current->board[x][y].obj!=NULL && current->board[x][y].obj->isValid()) {
						delete current->board[x][y].obj;
					} else {
						printf("Invalid object on %s at %i, %i (%s)\n",current->title,x,y,current->board[x][y].obj->getName().c_str());
					}
					if(current->board[x][y].under!=NULL) {
						if(current->board[x][y].under->isValid()==true) {
							delete current->board[x][y].under;
						} else {
							Debug::printf("Invalid object on %s under %i, %i\n",current->title,x,y);
						}
					}
				}
			}
		}
		prev=current;
		current=current->next;
		delete prev;		
	}
	board_list=NULL;
	currentbrd=NULL;
}

void put(ZZTObject *o) {
	if(currentbrd->board[(int)o->getPosition().x][(int)o->getPosition().y].under != NULL) delete currentbrd->board[(int)o->getPosition().x][(int)o->getPosition().y].under;
	
	if(o->getFlags() & F_OBJECT) 
		currentbrd->board[(int)o->getPosition().x][(int)o->getPosition().y].under = currentbrd->board[(int)o->getPosition().x][(int)o->getPosition().y].obj;
	else
		currentbrd->board[(int)o->getPosition().x][(int)o->getPosition().y].under = NULL;
	
	currentbrd->board[(int)o->getPosition().x][(int)o->getPosition().y].obj = o;
	o->create();
}

int new_board(char *title) {
	struct board_info_node *current=board_list;
	struct board_info_node *prev=NULL;
	int num=0;
	
	while(current!=NULL) {
		prev=current;
		current=current->next;
		num++;
	}
	
	current=new board_info_node;
	
	if(prev!=NULL) prev->next=current;
	else board_list=current;
	
	strncpy(current->title,title,50);
	current->num=num;
	current->maxshots=0;
	current->dark=0;
	current->board_up=0;
	current->board_down=0;
	current->board_left=0;
	current->board_right=0;
	current->reenter=0;
	current->reenter_x=254;
	current->reenter_y=254;
	current->time=0;
	current->message[0]='\0';
	current->msgcount=0;
	current->next=NULL;
	current->compressed=false;
	
	for(int y=0; y<BOARD_Y; y++) {
		for(int x=0; x<BOARD_X; x++) {
			if(x == BOARD_X/2 && y == BOARD_Y/2) {
				current->board[x][y].obj = create_object(ZZT_PLAYER,x,y);
				current->board[x][y].under = create_object(ZZT_EMPTY,x,y);
			} else {
				current->board[x][y].obj = create_object(ZZT_EMPTY,x,y);
				current->board[x][y].under = NULL;
			}
		}
	}
	
	world.board_count=num;
	
	return current->num;
}

void new_world() {
	free_world();
	
	world.magic=65535;
	world.board_count=0;
	world.ammo=0;
	world.gems=0;
	memset(world.keys,0,7);
	world.health=100;
	world.start=0;
	world.torches=0;
	world.torch_cycle=0;
	world.energizer_cycle=0;
	world.pad1=0;
	world.score=0;
	strcpy((char *)world.title.string,"UNTITLED");
	world.title.len=7;
	for(int i=0; i<10; i++) world.flag[i].len = 0;
	world.time=0;
	world.saved=0;
	world.editing=0;
	world.task_points=0;
	
	switch_board(new_board("Title Screen"));
}

Thread::Thread *spinner_thread;
bool spinner_active = false;

void *spinner_thd(void *text) {
	int x=0;
	
	while(spinner_active == true) {
		st->locate(3,4);
		st->color(15,1);
		st->printf("%s %c...",(char *)text,spin_anim[x]);
		x++;
		x%=4;
		render();
#if TIKI_PLAT == TIKI_DC
		Time::sleep(10000);
#else
		Time::sleep(80000);
#endif
	}

	return 0; //Your *MOM* must return a value, Microsoft!
}
	
void spinner(char *text) {
	if(spinner_active) spinner_clear();
	spinner_active = true;
	spinner_thread = new Thread::Thread(spinner_thd,text);
}

void spinner_clear() {
	if(spinner_active) {
		spinner_active = false;
		spinner_thread->join();
		delete spinner_thread;
	}
	st->locate(3,4);
	st->color(15,1);
	st->printf("            ");
}

int is_empty(struct board_info_node *curbrd, int x, int y, bool ignorePlayer) {
	if(x<0 || y<0 || x>=BOARD_X || y >= BOARD_Y) return 0;
	if(curbrd->board[x][y].obj!=NULL && !(curbrd->board[x][y].obj->getFlags()&F_EMPTY)) {
		if(ignorePlayer && curbrd->board[x][y].obj->getType() == ZZT_PLAYER) return 1;
		return 0;
	}
	return 1;
}

struct board_data *get_block_by_type(int type, int &x, int &y) {
	int i,j;
	for(j=y;j>=0;j--) {
		for(i=BOARD_X-1;i>=0;i--) {
			if(i==BOARD_X-1&&j==y) i=x;
			if(i<0) { i=BOARD_X-1; j--; }
			if(j<0) break;
			if(currentbrd->board[i][j].obj!=NULL) {
				if(currentbrd->board[i][j].obj->getType() == type) {
					x=i; y=j;
					return &currentbrd->board[i][j];
				}
			}
		}
	}
	return NULL;
}

ZZTObject *get_obj_by_type(int type, int &x, int &y) {
	int i,j;
	for(j=y;j>=0;j--) {
		for(i=BOARD_X-1;i>=0;i--) {
			if(i==BOARD_X-1&&j==y) i=x;
			if(i<0) { i=BOARD_X-1; j--; }
			if(j<0) break;
			if(currentbrd->board[i][j].obj!=NULL) {
				if(currentbrd->board[i][j].obj->getType() == type) {
					x=i; y=j;
					return currentbrd->board[i][j].obj;
				}
			}
		}
	}
	return NULL;
}

ZZTObject *get_obj_by_color(struct board_info_node *board, int type, int color) {
	int x,y;

	for(y=BOARD_Y-1;y>=0;y--) {
		for(x=BOARD_X-1;x>=0;x--) {
			if(board->board[x][y].obj->getType()==type && board->board[x][y].obj->getColor()==color) return board->board[x][y].obj;
		}
	}
	return NULL;
}

ZZTObject *get_obj_by_color(struct board_info_node *board, int type, int fg, int bg) {
	int x,y;
	
	for(y=BOARD_Y-1;y>=0;y--) {
		for(x=BOARD_X-1;x>=0;x--) {
			if(board->board[x][y].obj->getType()==type && board->board[x][y].obj->getFg()==fg &&
				 board->board[x][y].obj->getType()==type && board->board[x][y].obj->getBg()==bg) return board->board[x][y].obj;
		}
	}
	return NULL;
}

ZZTObject *get_obj_by_type(struct board_info_node *board, int type) {
	int x,y;

	for(y=BOARD_Y-1;y>=0;y--) {
		for(x=BOARD_X-1;x>=0;x--) {
			if(board->board[x][y].obj!=NULL && board->board[x][y].obj->getType()==type) return board->board[x][y].obj;
		}
	}
	return NULL;
}

void remove_from_board(struct board_info_node *brd, ZZTObject *me) {	
	Vector pos = me->getPosition();
	brd->board[(int)pos.x][(int)pos.y].obj=brd->board[(int)pos.x][(int)pos.y].under;
	if(brd->board[(int)pos.x][(int)pos.y].obj==NULL || !brd->board[(int)pos.x][(int)pos.y].obj->isValid()) {
		brd->board[(int)pos.x][(int)pos.y].obj=create_object(ZZT_EMPTY, (int)pos.x, (int)pos.y);
	}
	brd->board[(int)pos.x][(int)pos.y].under=NULL;
	me->setFlag(F_DELETED);
}

struct board_info_node *get_board_list() {
	return board_list;
}

struct board_info_node *get_board(int num) {
	struct board_info_node *current=board_list;
	while(current!=NULL && current->num<num) current=current->next;
	if(current!=NULL && current->num==num) return current;
	return NULL;
}

void boardTransition(direction d, board_info_node *newbrd) {
	int i,j,x,y;
	ZZTObject *o;
	float a,b;
	Vector dist;
	struct board_info_node *board=newbrd;
	bool changed[BOARD_X][BOARD_Y] = {0};
	
	if(playerEventCollector != NULL && playerEventCollector->listening()) playerEventCollector->stop();
	
	zoom = 1;
	
	switch(d) {
		case IDLE:
			for(i=0; i<(BOARD_X * BOARD_Y)/100; i++) {
				for(j=0; j<100; j++) {
					do {
						x=rand()%BOARD_X;
						y=rand()%BOARD_Y;
					} while(changed[x][y]);
					changed[x][y]=true;
					o = newbrd->board[x][y].obj;
					dist = o->getPosition() - player->getPosition();
					
					a=(dist.x)*(dist.x)/2.0f;
					b=(dist.y)*(dist.y);
										
					if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
						ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
						ct->putChar(x,y,177);
					} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
						ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
						ct->putChar(x,y,o->getShape());
					} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
						ct->putColor(x,y,o->getFg()%8);
						ct->putChar(x,y,o->getShape());
					} else {
						ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
						ct->putChar(x,y,o->getShape());
					}
				}
				render();
#if TIKI_PLAT != TIKI_DC
				Time::sleep(8000);
#endif
			}
			break;
		case UP:
			for(i=1; i< BOARD_Y; i+=2) {
				for(y=0; y<BOARD_Y; y++) {
					for(x=0; x<BOARD_X; x++) {
						if(y < i) {
							o=newbrd->board[x][BOARD_Y+y-i].obj;
							board=newbrd;
						} else {
							o=currentbrd->board[x][y-i].obj;
							if(o->getType() == ZZT_PLAYER) o=currentbrd->board[x][y-i].under;
							board=currentbrd;
						}
						
						dist = o->getPosition() - player->getPosition();
						
						a=(dist.x)*(dist.x)/2.0f;
						b=(dist.y)*(dist.y);
						
						if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,177);
						} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,o->getShape());
						} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
							ct->putColor(x,y,o->getFg()%8);
							ct->putChar(x,y,o->getShape());
						} else {
							ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
							ct->putChar(x,y,o->getShape());
						}
					}
				}
				render();
#if TIKI_PLAT != TIKI_DC
				Time::sleep(6000);
#endif
			}
			break;
		case DOWN:
			for(i=BOARD_Y-1; i > 0; i-=2) {
				for(y=0; y<BOARD_Y; y++) {
					for(x=0; x<BOARD_X; x++) {
						if(y < i) {
							o=currentbrd->board[x][BOARD_Y+y-i].obj;
							if(o->getType() == ZZT_PLAYER) o=currentbrd->board[x][BOARD_Y+y-i].under;
							board=currentbrd;
						} else {
							o=newbrd->board[x][y-i].obj;
							board=newbrd;
						}
						
						dist = o->getPosition() - player->getPosition();
						
						a=(dist.x)*(dist.x)/2.0f;
						b=(dist.y)*(dist.y);
						
						if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,177);
						} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,o->getShape());
						} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
							ct->putColor(x,y,o->getFg()%8);
							ct->putChar(x,y,o->getShape());
						} else {
							ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
							ct->putChar(x,y,o->getShape());
						}
					}
				}
				render();
#if TIKI_PLAT != TIKI_DC
				Time::sleep(6000);
#endif
			}
		break;
		case LEFT:
			for(i=1; i< BOARD_X; i+=4) {
				for(y=0; y<BOARD_Y; y++) {
					for(x=0; x<BOARD_X; x++) {
						if(x < i) {
							o=newbrd->board[BOARD_X+x-i][y].obj;
							board=newbrd;
						} else {
							o=currentbrd->board[x-i][y].obj;
							if(o->getType() == ZZT_PLAYER) o=currentbrd->board[x-i][y].under;
							board=currentbrd;
						}
						
						dist = o->getPosition() - player->getPosition();
						
						a=(dist.x)*(dist.x)/2.0f;
						b=(dist.y)*(dist.y);
						
						if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,177);
						} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,o->getShape());
						} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
							ct->putColor(x,y,o->getFg()%8);
							ct->putChar(x,y,o->getShape());
						} else {
							ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
							ct->putChar(x,y,o->getShape());
						}
					}
				}
				render();
#if TIKI_PLAT != TIKI_DC
				Time::sleep(6000);
#endif
			}
			break;
		case RIGHT:
			for(i=BOARD_X-1; i > 0; i-=4) {
				for(y=0; y<BOARD_Y; y++) {
					for(x=0; x<BOARD_X; x++) {
						if(x < i) {
							o=currentbrd->board[BOARD_X+x-i][y].obj;
							if(o->getType() == ZZT_PLAYER) o=currentbrd->board[BOARD_X+x-i][y].under;
							board=currentbrd;
						} else {
							o=newbrd->board[x-i][y].obj;
							board=newbrd;
						}
						
						dist = o->getPosition() - player->getPosition();
						
						a=(dist.x)*(dist.x)/2.0f;
						b=(dist.y)*(dist.y);
						
						if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,177);
						} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,o->getShape());
						} else if(board->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
							ct->putColor(x,y,o->getFg()%8);
							ct->putChar(x,y,o->getShape());
						} else {
							ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
							ct->putChar(x,y,o->getShape());
						}
					}
				}
				render();
#if TIKI_PLAT != TIKI_DC
				Time::sleep(6000);
#endif
			}
			break;
	}
	
	if(playerEventCollector != NULL && !playerEventCollector->listening()) playerEventCollector->start();
}

void switch_board(int num) {
	int oldbrd = world.start;
	direction h = (player==NULL)?IDLE:player->getHeading();
	decompress(get_board(num));
	world.start=num;
	player=(Player *)get_obj_by_type(get_board(num),ZZT_PLAYER);

	connect_lines(get_board(num));
	if(player!=NULL && currentbrd!=NULL && !world.editing) boardTransition(h,get_board(num));
	
	if(currentbrd != NULL && oldbrd != num) compress(currentbrd);
	
	currentbrd=get_board(num);
	if(player!=NULL) {
		currentbrd->reenter_x = (unsigned char)player->getPosition().x;
		currentbrd->reenter_y = (unsigned char)player->getPosition().y;
	}
	world.time=currentbrd->time;
	world_sec=10;
	draw_time();
	
	if(currentbrd->dark && world.torch_cycle<1) {
		set_msg("Room is dark - you need to light a torch!");
	}
}

struct world_header *get_world() {
	return &world;
}

int board_up() {
	return currentbrd->board_up;
}

int board_down() {
	return currentbrd->board_down;
}

int board_left() {
	return currentbrd->board_left;
}

int board_right() {
	return currentbrd->board_right;
}

void compress(board_info_node *board, bool silent) {
	rle_block rle;
	zzt_param param;
	int code,color;
	ZZTObject *obj,*under;
	bool foundPlayer=false;
	
	if(board->compressed) return;
	
	if(!silent) spinner("Working");
#ifdef DEBUG
	Debug::printf("Compressing board...\n");
#endif
	board->rle_data.clear();
	board->params.clear();
	
	rle.len=0;
	rle.cod=board->board[0][0].obj->getType();
	if(rle.cod>=ZZT_BLUE_TEXT && rle.cod <=ZZT_WHITE_TEXT) {
		rle.col=board->board[0][0].obj->getShape();
	} else {
		rle.col=board->board[0][0].obj->getFg()+(16*board->board[0][0].obj->getBg());
	}
	
	board->size=139;
	
	for(int y=0;y<BOARD_Y;y++) {
		for(int x=0;x<BOARD_X;x++) {
			obj=board->board[x][y].obj;
			under=board->board[x][y].under;
			
			if((foundPlayer && obj->getType() != ZZT_PLAYER && obj->getFlags() & F_OBJECT) || (!foundPlayer && obj->getType() == ZZT_PLAYER)) {
				param.x = (unsigned char)obj->getPosition().x;
				param.y = (unsigned char)obj->getPosition().y;
				param.xstep = (short)obj->getStep().x;
				param.ystep = (short)obj->getStep().y;
				param.cycle = obj->getCycle();
				for(int i=0; i<3; i++) {
					param.data[i] = obj->getParam(i+1);
				}
				param.leader = 0;//obj->getLeader();
				param.follower = 0;//obj->getFollower();
				if(under!=NULL) {
					param.ut=under->getType();
					param.uc=under->getFg()+(16*under->getBg());
				} else {
					param.ut=ZZT_EMPTY;
					param.uc=0;
				}
				param.progpos=obj->getProgPos();
				param.proglen=obj->getProgLen();
				param.prog=obj->getProg();
				
				board->params.push_back(param);
				board->size += param.proglen + 33;
				if(!foundPlayer) {
					foundPlayer = true;
					x=BOARD_Y; y=-1;
					break;
				}
			}
			if(foundPlayer) {
				code=obj->getType();
				if(code>=ZZT_BLUE_TEXT && code<=ZZT_WHITE_TEXT) {
					color=obj->getShape();
				} else {
					color=obj->getFg()+(16*obj->getBg());
				}
				if(rle.len>=255 || rle.cod!=code || rle.col!=color) {
					board->rle_data.push_back(rle);
					board->size+=3;
					rle.len=0;
					rle.cod=obj->getType();
					if(rle.cod>=ZZT_BLUE_TEXT && rle.cod <=ZZT_WHITE_TEXT) {
						rle.col=obj->getShape();
					} else {
						rle.col=obj->getFg()+(16*obj->getBg());
					}
				}
				delete obj;
				delete under;
				rle.len++;
			}
		}
	}
	if(rle.len>0) {
		board->rle_data.push_back(rle);
		board->size+=3;
	}		
	board->compressed=true;
	if(!silent) spinner_clear();
#ifdef DEBUG
	Debug::printf("Done!\n");
#endif
}

void decompress(board_info_node *board, bool silent) {
	int x=0,y=0,z=0;
	ZZTObject *curobj=NULL;
	ZZTObject *prev=NULL; 
	std::list<rle_block>::iterator rle_iter;
	std::list<zzt_param>::iterator param_iter;
	
	if(!board->compressed) return;
	if(!silent) spinner("Working");
#ifdef DEBUG
	Debug::printf("Decompressing board...\n");
#endif
	
	for(rle_iter=board->rle_data.begin(); rle_iter != board->rle_data.end(); rle_iter++) {
		for(z=0; z<(*rle_iter).len; z++) {
			if(x>BOARD_X-1) { x=0; y++; }
			board->board[x][y].obj=NULL;
			board->board[x][y].under=NULL;
			curobj=board->board[x][y].obj=create_object((*rle_iter).cod,x,y);
			if(curobj!=NULL) {
				curobj->setFg((*rle_iter).col%16);
				curobj->setBg((*rle_iter).col/16);
			} else {
				printf("Unknown type encountered at (%i, %i): %i\n",x,y,(*rle_iter).cod);
				board->board[x][y].obj=create_object(ZZT_EMPTY,x,y);
			}
			x++;
		}
	}
	
	for(param_iter=board->params.begin(); param_iter != board->params.end(); param_iter++) {
		if((*param_iter).x >= 0 && (*param_iter).x < BOARD_X && (*param_iter).y >= 0 && (*param_iter).y < BOARD_Y) {
			curobj = board->board[(*param_iter).x][(*param_iter).y].obj;
		} else {
			Debug::printf("Params out of bounds: %i, %i\n",(*param_iter).x,(*param_iter).y);
			curobj=NULL;
		}
		if(curobj!=NULL) {
			curobj->setStep(Vector((*param_iter).xstep,(*param_iter).ystep,0));
			curobj->setCycle((*param_iter).cycle);
			curobj->setParam(1,(*param_iter).data[0]);
			curobj->setParam(2,(*param_iter).data[1]);
			curobj->setParam(3,(*param_iter).data[2]);
			curobj->setProg((*param_iter).prog,(*param_iter).proglen,(*param_iter).progpos);
			board->board[(*param_iter).x][(*param_iter).y].under=create_object((*param_iter).ut,(*param_iter).x,(*param_iter).y);
			board->board[(*param_iter).x][(*param_iter).y].under->setFg((*param_iter).uc%16);
			board->board[(*param_iter).x][(*param_iter).y].under->setBg((*param_iter).uc/16);
		} else {
			Debug::printf("Invalid object at: (%i,%i)\n",x,y);
		}
	}
	
	for(y = 0; y < BOARD_Y; y++) {
		for(x = 0; x < BOARD_X; x++) {
			if(board->board[x][y].obj!=NULL) board->board[x][y].obj->create();
			if(board->board[x][y].under!=NULL) board->board[x][y].under->create();
		}
	}
	
	board->rle_data.clear();
	board->params.clear();
	
	board->compressed=false;
	if(!silent) spinner_clear();
#ifdef DEBUG
	Debug::printf("Done!\n");
#endif	
}	
	
void connect_lines(board_info_node *current) {
	for(int x=0;x<BOARD_X;x++) {
		for(int y=0;y<BOARD_Y;y++) {
			if(current->board[x][y].obj->getType()==ZZT_LINE) {
				if(x>0&&current->board[x-1][y].obj->getType()==ZZT_LINE && //left is a line
					 x<BOARD_X-1 && current->board[x+1][y].obj->getType()==ZZT_LINE) { //right is a line
					if(y>0&&current->board[x][y-1].obj->getType()==ZZT_LINE && //up is a line
						 y<BOARD_Y-1 &&current->board[x][y+1].obj->getType()==ZZT_LINE) { //down is a line
						current->board[x][y].obj->setShape(206);
					} else if(y<BOARD_Y-1 &&current->board[x][y+1].obj->getType()==ZZT_LINE) { //only down is a line
						current->board[x][y].obj->setShape(203);
					} else if(y>0&&current->board[x][y-1].obj->getType()==ZZT_LINE) { //only up is a line
						current->board[x][y].obj->setShape(202);
					} else { //only left and right
						current->board[x][y].obj->setShape(205);
					}
				} else if(x>0&&current->board[x-1][y].obj->getType()==ZZT_LINE) { //only left is a line
					if(y>0&&current->board[x][y-1].obj->getType()==ZZT_LINE && //up is a line
						 y<BOARD_Y-1 &&current->board[x][y+1].obj->getType()==ZZT_LINE) { //down is a line
						current->board[x][y].obj->setShape(185);
					} else if(y<BOARD_Y-1&&current->board[x][y+1].obj->getType()==ZZT_LINE) { //only down is a line
						current->board[x][y].obj->setShape(187);
					} else if(y>0&&current->board[x][y-1].obj->getType()==ZZT_LINE) { //only up is a line
						current->board[x][y].obj->setShape(188);
					} else { //only left
						current->board[x][y].obj->setShape(181);
					}
				} else if(x<BOARD_X-1 &&current->board[x+1][y].obj->getType()==ZZT_LINE) { //only right is a line
					if(y>0&&current->board[x][y-1].obj->getType()==ZZT_LINE && //up is a line
						 y<BOARD_Y-1 &&current->board[x][y+1].obj->getType()==ZZT_LINE) { //down is a line
						current->board[x][y].obj->setShape(204);
					} else if(y<BOARD_Y-1 &&current->board[x][y+1].obj->getType()==ZZT_LINE) { //only down is a line
						current->board[x][y].obj->setShape(201);
					} else if(y>1&&current->board[x][y-1].obj->getType()==ZZT_LINE) { //only up is a line
						current->board[x][y].obj->setShape(200);
					} else { //only right
						current->board[x][y].obj->setShape(198);
					}
				} else { //vertical
					if(y>0&&current->board[x][y-1].obj->getType()==ZZT_LINE && //up is a line
						 y<BOARD_Y-1&&current->board[x][y+1].obj->getType()==ZZT_LINE) { //down is a line
						current->board[x][y].obj->setShape(186);
					} else if(y<BOARD_Y-1&&current->board[x][y+1].obj->getType()==ZZT_LINE) { //only down is a line
						current->board[x][y].obj->setShape(210);
					} else if(y>1&&current->board[x][y-1].obj->getType()==ZZT_LINE){ //only up
						current->board[x][y].obj->setShape(208);
					} else { //only 1 block
						current->board[x][y].obj->setShape(249);
					}
				}
			}
		}
	}
}

int load_zzt(const char *filename, int titleonly) {
	unsigned short int c,x,y,z,sum=0,q;
	char pad[16];
	unsigned char len;
	rle_block rle;
	zzt_param param;
	struct board_info_node *current=NULL;
	char *prog;

	File fd(filename,"rb");

	spinner("Loading");
	fd.readle16(&world.magic,1);
	if(world.magic!=65535) {
		spinner_clear();
		return -1;
	}
	fd.readle16(&world.board_count,1);
	fd.readle16(&world.ammo,1);
	fd.readle16(&world.gems,1);
	fd.read(world.keys, 7);
	fd.readle16(&world.health,1);
	fd.readle16(&world.start,1);
	fd.readle16(&world.torches,1);
	fd.readle16(&world.torch_cycle,1);
	fd.readle16(&world.energizer_cycle,1);
	fd.readle16(&world.pad1,1);
	fd.readle16(&world.score,1);
	fd.read(&world.title, sizeof(zzt_string));
	fd.read(world.flag, sizeof(zzt_string) * 10);
	fd.readle16(&world.time,1);
	fd.read(&world.saved, 1);
	world.editing = 0;
	world.task_points = 0;
	
	world.title.string[world.title.len]='\0';
	for(x=0;x<10;x++) {
		world.flag[x].string[world.flag[x].len]='\0';
	}
#ifdef DEBUG
	printf("Magic: %i\n",world.magic);
	printf("Board count: %i\n",world.board_count);
	printf("Ammo: %i\n",world.ammo);
	printf("Gems: %i\n",world.gems);
	printf("Blue: %i\n",world.keys[0]);
	printf("Green: %i\n",world.keys[1]);
	printf("Cyan: %i\n",world.keys[2]);
	printf("Red: %i\n",world.keys[3]);
	printf("Purple: %i\n",world.keys[4]);
	printf("Yellow: %i\n",world.keys[5]);
	printf("White: %i\n",world.keys[6]);
	printf("Health: %i\n",world.health);
	printf("Start: %i\n",world.start);
	printf("Title: %s (%i)\n",world.title.string,world.title.len);
#endif
	fd.seek(0x200,SEEK_SET); //seek to the first board
	current=new board_info_node;
	board_list=current;
	if(titleonly==1) world.board_count=1;
	for(q=0;q<=world.board_count;q++) {
		fd.readle16(&current->size, 1);
		fd.read(&len,1);
		fd.read(current->title,50);
		current->title[len]='\0';
#ifdef DEBUG
		printf("Board title: %s\n",current->title);
#endif
		//here comes the RLE data!
		x=0;y=0;z=0;
		while(z<1500) {
			fd.read(&rle.len,1);
			fd.read(&rle.cod,1);
			fd.read(&rle.col,1);
			current->rle_data.push_back(rle);
			z += rle.len;
		}
		if(z!=1500) { 
			printf("RLE mismatch!\n"); 
			spinner_clear();
			return -1; 
		}

		current->num=q;
		current->compressed=true;
		fd.read(&current->maxshots,1);
		fd.read(&current->dark,1);
		fd.read(&current->board_up,1);
		fd.read(&current->board_down,1);
		fd.read(&current->board_left,1);
		fd.read(&current->board_right,1);
		fd.read(&current->reenter,1);
		current->msgcount=0;
		fd.read(&len,1);
		fd.read(&current->message,58);
		current->message[len]='\0';
		//printf("Message: %s\n",current->message);
		fd.read(&current->reenter_x,1); current->reenter_x--;
		fd.read(&current->reenter_y,1); current->reenter_y--;
		fd.readle16(&current->time,1);
		fd.read(&current->animatedWater,1);
		for(x=0;x<15;x++) { fd.read(&c,1); } //more padding
		
		//Load the object params
		fd.readle16(&c,1); //number of objects
#ifdef DEBUG
		printf("Loading %i objects...\n",c);
#endif
		for(z=0;z<=c;z++) {
			fd.read(&param.x,1); param.x--;
			fd.read(&param.y,1); param.y--;
			fd.readle16(&param.xstep,1); 
			fd.readle16(&param.ystep,1);
			fd.readle16(&param.cycle,1);
			fd.read(&param.data[0],1);
			fd.read(&param.data[1],1);
			fd.read(&param.data[2],1);
			fd.readle16(&param.leader,1);
			fd.readle16(&param.follower,1);
			fd.read(&param.ut,1);
			fd.read(&param.uc,1);
			fd.read(&pad,4);
			fd.readle16(&param.progpos,1);
			fd.readle16(&param.proglen,1);
			fd.read(&pad,8);
			param.prog="";
			if(param.proglen>0) {
				prog=(char *)malloc(param.proglen+1);
				fd.read(prog,param.proglen);
				prog[param.proglen]='\0';
				param.prog = prog;
				free(prog);
			}
			current->params.push_back(param);
		}
		
		if(q<world.board_count) {
			current->next=new board_info_node;
			current=current->next;
		} else {
			current->next=NULL;
		}
	}
	fd.close();
	spinner_clear();
	current=NULL;
	return 1;
}

void save_game(const char *filename) {
	struct board_info_node *curbrd=board_list;
	std::list<rle_block>::iterator rle_iter;
	std::list<zzt_param>::iterator params_iter;
	unsigned char x=0;
	unsigned short int i;
	
	File fd(filename,"wb");
	
	spinner("Saving");
	
	fd.writele16(&world.magic,1);
	fd.writele16(&world.board_count,1);
	fd.writele16(&world.ammo,1);
	fd.writele16(&world.gems,1);
	fd.write(world.keys,7);
	fd.writele16(&world.health,1);
	fd.writele16(&world.start,1);
	fd.writele16(&world.torches,1);
	fd.writele16(&world.torch_cycle,1);
	fd.writele16(&world.energizer_cycle,1);
	fd.writele16(&world.pad1,1);
	fd.writele16(&world.score,1);
	fd.write(&world.title,sizeof(zzt_string));
	fd.write(world.flag,sizeof(zzt_string) * 10);
	fd.writele16(&world.time,1); //FIXME: This should be total game time, not current board time remaining
	fd.write(&world.saved,1);
	
	//fd.seek(0x200,SEEK_SET);
	for(i=0; i<249; i++) {
		fd.write(&x,1);
	}
	
	while(curbrd!=NULL) {
		if(curbrd->size==0) decompress(curbrd,true); //Calculate board size for older DreamZZT files
		compress(curbrd,true);
#ifdef DEBUG		
		printf("Writing: %s\n",curbrd->title);
#endif
		fd.writele16(&curbrd->size,1);
		x = (unsigned char)strlen(curbrd->title);
		fd.write(&x,1);
		fd.write(curbrd->title,34);	
		for(x=0;x<16;x++) {
			fd.write("\0",1);
		}

		int rlecnt = 0;

		for(rle_iter = curbrd->rle_data.begin(); rle_iter != curbrd->rle_data.end(); rle_iter++) {
			fd.write(&(*rle_iter).len,1);
			fd.write(&(*rle_iter).cod,1);
			fd.write(&(*rle_iter).col,1);
			rlecnt += (*rle_iter).len;
		}
		
		fd.write(&curbrd->maxshots,1);
		fd.write(&curbrd->dark,1);
		fd.write(&curbrd->board_up,1);
		fd.write(&curbrd->board_down,1);
		fd.write(&curbrd->board_left,1);
		fd.write(&curbrd->board_right,1);
		fd.write(&curbrd->reenter,1);
		x = (unsigned char)strlen(curbrd->message);
		fd.write(&x,1);
		fd.write(curbrd->message,58);
		fd.write(&curbrd->reenter_x,1);
		fd.write(&curbrd->reenter_y,1);
		fd.writele16(&curbrd->time,1);
		fd.write(&curbrd->animatedWater,1);
		for(x=0;x<15;x++) fd.write("\0",1);

		i = (unsigned char)curbrd->params.size() - 1;
		fd.writele16(&i,1);
		printf("Writing %i objects...\n", i);

		i = 0;

		for(params_iter = curbrd->params.begin(); params_iter != curbrd->params.end(); params_iter++) {
			(*params_iter).x++; fd.write(&(*params_iter).x,1); (*params_iter).x--;
			(*params_iter).y++; fd.write(&(*params_iter).y,1); (*params_iter).y--;
			fd.writele16((uint16 *)&(*params_iter).xstep,1);
			fd.writele16((uint16 *)&(*params_iter).ystep,1);
			fd.writele16(&(*params_iter).cycle,1);
			for(int g=0; g<3; g++) {
				fd.write(&(*params_iter).data[g],1);
			}
			fd.write("\0\0\0\0",4);
			fd.write(&(*params_iter).ut,1);
			fd.write(&(*params_iter).uc,1);
			fd.write("\0\0\0\0",4);
			fd.writele16(&(*params_iter).progpos,1);
			fd.writele16((uint16 *)&(*params_iter).proglen,1);
			fd.write("\0\0\0\0\0\0\0\0",8);
			if((*params_iter).proglen>0) fd.write((*params_iter).prog.c_str(),(*params_iter).proglen);
			i++;
		}
		
		curbrd=curbrd->next;
	}
	fd.close();
	
	decompress(currentbrd, true);
	connect_lines(currentbrd);
	player=(Player *)get_obj_by_type(currentbrd,ZZT_PLAYER);
	spinner_clear();
}

void update_brd() {
	ZZTObject *o=NULL;
	ZZTObject *p=NULL;
	ZZTObject *t=NULL;
	struct board_info_node *current=currentbrd;
	int x,y,i=0;

	if(current->num>0) {
		if(world.torch_cycle>0) {
			world.torch_cycle--;
			draw_torch();
			if(world.torch_cycle==0) {
				zm->setTune("tc-c-c");
				zm->start();
			}
		}
		
		if(world.time>0) {
			draw_time();
		}
	
		world_sec--;
		if(world.health==0 && world_sec==0) {
			if(!zm->isLocked()) {
				zm->setTune("s.-cd#g+c-ga#+dgfg#+cf----q.c");
				zm->lock();
				zm->start();
			}
			set_msg("Game Over - Press ESC");
		}
		
		if(world.energizer_cycle>0) {
			world.energizer_cycle--;
			if(world.energizer_cycle==0) {
				zm->unlock();
				zm->setTune("s.-c-a#gf#fd#c");
				zm->start();
			}
		}
		
		if(world_sec==0 && currentbrd->time>0) {
			take_time(1);
			if(world.time==10) { 
				set_msg("Time is running out!"); 
				zm->setTune("i.+cfc-f+cfq.c");
				zm->start();
			}
			if(world.time==0) player->message(player,"time");
		}
		if(world_sec==0) world_sec=10;
	}
	
	//for(j=0; j<2; j++) {
		//i=0;
		for(y=0;y<BOARD_Y;y++) {
			for(x=BOARD_X-1;x>=0;x--) {
				o=current->board[x][y].obj;
				if(o!=NULL && o->isValid() && o->getUpdated()==0) { 
					o->setTick(o->getTick()-1);
					if((o->getTick()<=0 && o->getCycle() != 0) || world.health<=0/* && i%2==j*/) {
						o->update();
						if(o->getFlags()&F_DELETED) {
							if(current->board[x][y].obj == o) remove_from_board(current,o);
							if(current->board[x][y].under == o) current->board[x][y].under = NULL;
							delete o;
						} else {
							o->setUpdated(true);
							o->setTick(o->getCycle());
						}
						if(current->board[x][y].under != NULL && current->board[x][y].under->getFlags() & F_DELETED) {
							delete current->board[x][y].under;
							current->board[x][y].under = NULL;
						}
					}
				}
			}
			/*if(o!=NULL) {
				i++; i%=2;
			}*/							
		}
	//}
	
	for(y=0;y<BOARD_Y;y++) {
		for(x=0;x<BOARD_X;x++) {
			o=current->board[x][y].obj;
			if(o==NULL) {
				ct->locate(x,y);
				ct->color(15,4);
				ct->printf("X");
			} else {
				o->setUpdated(false);
				o->setPushed(false);
			}
		}
	}
}

void draw_block(int x, int y) {
	if(x>=BOARD_X||y>=BOARD_Y||x<0||y<0||currentbrd->board[x][y].obj==NULL) return;
	currentbrd->board[x][y].obj->draw();
}

void draw_board() {
	int x,y;
	
	for(y=0;y<BOARD_Y;y++) {
		for(x=0;x<BOARD_X;x++) {
			draw_block(x,y);
		}
	}
}
