/* board.cpp - World load / save and utility functions
 * Copyright (c) 2000-2006 Sam Steele
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
#include <Tiki/thread.h>
#include <Tiki/file.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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

struct world_header world;
struct board_info_node *board_list=NULL;
struct board_info_node *currentbrd=NULL;
extern Player *player;
void delay(long ms);

struct board_info_node *get_current_board() { return currentbrd; }
int world_sec=10;

char spin_anim[4]={'|','/','-','\\'};

void free_world() {
	struct board_info_node *current=board_list;
	struct board_info_node *prev=NULL;
	int x,y;
	
	while(current!=NULL) {
		for(y=0;y<BOARD_Y;y++) {
			for(x=0;x<BOARD_X;x++) {
				if(current->board[x][y].obj!=NULL && current->board[x][y].obj->isValid()) {
					delete current->board[x][y].obj;
				} else {
					printf("Invalid object on %s at %i, %i (%s)\n",current->title,x,y,current->board[x][y].obj->getName().c_str());
				}
				if(current->board[x][y].under!=NULL) {
					if(current->board[x][y].under->isValid()) {
						delete current->board[x][y].under;
					} else {
						printf("Invalid object on %s under %i, %i (%s)\n",current->title,x,y,current->board[x][y].under->getName().c_str());
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

Thread::Thread *spinner_thread;
bool spinner_active;

void *spinner_thd(void *text) {
  int x=0;
	spinner_active=true;
	
	while(spinner_active) {
		ct->locate(BOARD_X+3,5);
		ct->color(15,1);
		ct->printf("%s %c...",(char *)text,spin_anim[x]);
		x++;
		x%=4;
		render();
		Time::sleep(80000);
	}

	return 0; //Your *MOM* must return a value, Microsoft!
}
	
void spinner(char *text) {
	spinner_thread = new Thread::Thread(spinner_thd,text);
}

void spinner_clear() {
	spinner_active = false;
	
  ct->locate(BOARD_X+3,5);
  ct->color(15,1);
  ct->printf("            ");
}

/*void write_object(int fd, struct object *obj, struct object *under) {
	char ut,uc,x,y;
	//printf("%s\n",obj->name);
	ut=under->type;
	uc=under->fg+(16*under->bg);
	x=obj->x+1;
	y=obj->y+1;
	write(fd,&x,1);
	write(fd,&y,1);
	write(fd,&obj->xstep,2);
	write(fd,&obj->ystep,2);
	write(fd,&obj->cycle,2);
	write(fd,&obj->arg1,1);
	write(fd,&obj->arg2,1);
	write(fd,&obj->arg3,1);
	write(fd,&obj->arg4,4);
	write(fd,&ut,1);
	write(fd,&uc,1);
	write(fd,"\0\0\0\0",4);
	write(fd,&obj->progpos,2);
	write(fd,&obj->proglen,2);
	//printf("Proglen: %i\n",obj->proglen);
	write(fd,"\0\0\0\0\0\0\0\0",8);
	write(fd,obj->prog,obj->proglen);
}

void save_game(char *filename) {
	int fd,i;
	short int x,y,z;
	unsigned char c,code,color,code2,color2;
	struct board_info_node *curbrd=board_list;
	struct object *curplayer;
		
#ifdef DREAMCAST
  fd=fs_open(filename,O_WRONLY|O_TRUNC);
#endif
#ifdef WIN32
  fd=open(filename,O_WRONLY|O_BINARY,S_IREAD|S_IWRITE);
  if(fd==-1) fd=open(filename,O_WRONLY|O_BINARY|O_CREAT,S_IREAD|S_IWRITE);
#endif
#ifdef LINUX
  fd=open(filename,O_WRONLY,S_IRWXU);
  if(fd==-1) fd=open(filename,O_WRONLY|O_CREAT,S_IRWXU);
#endif
#ifdef MSDOS
  fd=open(filename,O_WRONLY|O_BINARY);
  if(fd==-1) fd=open(filename,O_WRONLY|O_BINARY|O_CREAT);
#endif
	
	if(fd==-1) {
		ct->printf("Save failed\n");
		return;
	}
  spinner("Saving");
	world.saved=1;	
	write(fd,&world,sizeof(world));
	lseek(fd,0x200,SEEK_SET);

	while(curbrd!=NULL) {
		spinner("Saving");
		write(fd,"\0\0",2); //size (int bytes) of the board
		c=strlen(curbrd->title);
		write(fd,&c,1);
		write(fd,curbrd->title,34);	
		for(x=0;x<16;x++) {
			write(fd,"\0",1);
		}
		c=0;
		code=curbrd->board[0][0].obj->getType();
		if(code>=ZZT_BLUE_TEXT && code <=ZZT_WHITE_TEXT) {
			color=curbrd->board[0][0].obj->shape;
		} else {
			color=curbrd->board[0][0].obj->fg+(16*curbrd->board[0][0].obj->bg);
		}
		z=0;
		for(y=0;y<BOARD_Y;y++) {
			for(x=0;x<BOARD_X;x++) {
				code2=curbrd->board[x][y].obj->getType();
				if(code2>=ZZT_BLUE_TEXT && code2<=ZZT_WHITE_TEXT) {
					color2=curbrd->board[x][y].obj->shape;
				} else {
					color2=curbrd->board[x][y].obj->fg+(16*curbrd->board[x][y].obj->bg);
				}
				if(c>=255 || code!=code2 || color!= color2) {
					write(fd,&c,1);
					write(fd,&code,1);
					write(fd,&color,1);
					//printf("Writing %i of %i (%i)\n",c,code,color);
					z+=c; c=0;
					code=curbrd->board[x][y].obj->getType();
					if(code>=ZZT_BLUE_TEXT && code <=ZZT_WHITE_TEXT) {
						color=curbrd->board[x][y].obj->shape;
					} else {
						color=curbrd->board[x][y].obj->fg+(16*curbrd->board[x][y].obj->bg);
					}
				}
				c++;
			}
		}
		if(c>0) {
			z+=c;
			write(fd,&c,1);
			write(fd,&code,1);
			write(fd,&color,1);
		}
		//printf("z: %i\n",z);
		write(fd,&curbrd->maxshots,1);
		write(fd,&curbrd->dark,1);
		write(fd,&curbrd->board_up,1);
		write(fd,&curbrd->board_down,1);
		write(fd,&curbrd->board_left,1);
		write(fd,&curbrd->board_right,1);
		write(fd,&curbrd->reenter,1);
		x=strlen(curbrd->message);
		write(fd,&x,1);
		write(fd,curbrd->message,60);
		write(fd,&curbrd->time,2);
		for(x=0;x<16;x++) write(fd,"\0",1);
		z=0;
		for(y=0;y<BOARD_Y;y++) {
			for(x=0;x<BOARD_X;x++) {
				if(curbrd->board[x][y].obj->flags&F_OBJECT) {
					//printf("Object: %s\n",curbrd->board[x][y].obj->name);
					z++;
				}
				if(curbrd->board[x][y].obj->getType()==ZZT_PLAYER) curplayer=curbrd->board[x][y].obj;
			}
		}
		z--;
		printf("Writing %i objects\n",z);
		write(fd,&z,2);
		write_object(fd,curplayer,curbrd->board[curplayer->x][curplayer->y].under);
		for(y=0;y<BOARD_Y;y++) {
			for(x=0;x<BOARD_X;x++) {
				if(curbrd->board[x][y].obj->getType()!=ZZT_PLAYER && curbrd->board[x][y].obj->flags&F_OBJECT) write_object(fd,curbrd->board[x][y].obj,curbrd->board[x][y].under);
			}
		}		
		curbrd=curbrd->next;
	}
	close(fd);
  spinner_clear();
}*/

int is_empty(struct board_info_node *curbrd, int x, int y) {
	if(x<0 || y<0 || x>=BOARD_X || y >= BOARD_Y) return 0;
  if(curbrd->board[x][y].obj!=NULL && !(curbrd->board[x][y].obj->getFlags()&F_EMPTY)) {
    return 0;
  }
  return 1;
}

struct board_data *get_block_by_type(int type, int &x, int &y) {
  int i,j;
  for(j=y;j<BOARD_Y;j++) {
    for(i=0;i<BOARD_X;i++) {
      if(i==0&&j==y) i=x;
      if(i>=BOARD_X) { i=0; j++; }
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
  for(j=y;j<BOARD_Y;j++) {
    for(i=0;i<BOARD_X;i++) {
      if(i==0&&j==y) i=x;
      if(i>=BOARD_X) { i=0; j++; }
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

  for(y=0;y<BOARD_Y;y++) {
    for(x=0;x<BOARD_X;x++) {
      if(board->board[x][y].obj->getType()==type && board->board[x][y].obj->getColor()==color) return board->board[x][y].obj;
    }
  }
  return NULL;
}

ZZTObject *get_obj_by_type(struct board_info_node *board, int type) {
  int x,y;

  for(y=0;y<BOARD_Y;y++) {
    for(x=0;x<BOARD_X;x++) {
      if(board->board[x][y].obj!=NULL && board->board[x][y].obj->getType()==type) return board->board[x][y].obj;
    }
  }
  return NULL;
}

void remove_from_board(struct board_info_node *brd, ZZTObject *me) {  
	Vector pos = me->getPosition();
  brd->board[(int)pos.x][(int)pos.y].obj=brd->board[(int)pos.x][(int)pos.y].under;
  if(brd->board[(int)pos.x][(int)pos.y].obj==NULL) {
    brd->board[(int)pos.x][(int)pos.y].obj=create_object(ZZT_EMPTY,pos.x,pos.y);
  }
  brd->board[(int)pos.x][(int)pos.y].under=NULL;
  me->setFlags(F_DELETED);
  draw_block((int)pos.x,(int)pos.y);
}

struct board_info_node *get_board_list() {
  return board_list;
}

struct board_info_node *get_board(int num) {
  int x;
  struct board_info_node *current=board_list;
  while(current!=NULL && current->num<num) current=current->next;
  if(current!=NULL && current->num==num) return current;
  return NULL;
}

void boardTransition(direction d, board_info_node *newbrd) {
	int i,x,y;
	ZZTObject *o;
	float a,b;
	Vector dist;
	
	switch(d) {
		case UP:
			for(i=1; i< BOARD_Y; i+=2) {
				for(y=0; y<BOARD_Y; y++) {
					for(x=0; x<BOARD_X; x++) {
						if(y < i) {
							o=newbrd->board[x][BOARD_Y+y-i].obj;
						} else {
							o=currentbrd->board[x][y-i].obj;
							if(o->getType() == ZZT_PLAYER) o=currentbrd->board[x][y-i].under;
						}
						
						dist = o->getPosition() - player->getPosition();
						
						a=(dist.x)*(dist.x)/2.0f;
						b=(dist.y)*(dist.y);
						
						if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,177);
						} else if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,o->getShape());
						} else if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
							ct->putColor(x,y,o->getFg()%8);
							ct->putChar(x,y,o->getShape());
						} else {
							ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
							ct->putChar(x,y,o->getShape());
						}
					}
				}
				render();
				Time::sleep(1000);
			}
			break;
		case DOWN:
			for(i=BOARD_Y-1; i > 0; i-=2) {
				for(y=0; y<BOARD_Y; y++) {
					for(x=0; x<BOARD_X; x++) {
						if(y < i) {
							o=currentbrd->board[x][BOARD_Y+y-i].obj;
							if(o->getType() == ZZT_PLAYER) o=currentbrd->board[x][BOARD_Y+y-i].under;
						} else {
							o=newbrd->board[x][y-i].obj;
						}
						
						dist = o->getPosition() - player->getPosition();
						
						a=(dist.x)*(dist.x)/2.0f;
						b=(dist.y)*(dist.y);
						
						if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,177);
						} else if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,o->getShape());
						} else if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
							ct->putColor(x,y,o->getFg()%8);
							ct->putChar(x,y,o->getShape());
						} else {
							ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
							ct->putChar(x,y,o->getShape());
						}
					}
				}
				render();
				Time::sleep(1000);
			}
		break;
		case LEFT:
			for(i=1; i< BOARD_X; i+=4) {
				for(y=0; y<BOARD_Y; y++) {
					for(x=0; x<BOARD_X; x++) {
						if(x < i) {
							o=newbrd->board[BOARD_X+x-i][y].obj;
						} else {
							o=currentbrd->board[x-i][y].obj;
							if(o->getType() == ZZT_PLAYER) o=currentbrd->board[x-i][y].under;
						}
						
						dist = o->getPosition() - player->getPosition();
						
						a=(dist.x)*(dist.x)/2.0f;
						b=(dist.y)*(dist.y);
						
						if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,177);
						} else if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,o->getShape());
						} else if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
							ct->putColor(x,y,o->getFg()%8);
							ct->putChar(x,y,o->getShape());
						} else {
							ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
							ct->putChar(x,y,o->getShape());
						}
					}
				}
				render();
				Time::sleep(1000);
			}
			break;
		case RIGHT:
			for(i=BOARD_X-1; i > 0; i-=4) {
				for(y=0; y<BOARD_Y; y++) {
					for(x=0; x<BOARD_X; x++) {
						if(x < i) {
							o=currentbrd->board[BOARD_X+x-i][y].obj;
							if(o->getType() == ZZT_PLAYER) o=currentbrd->board[BOARD_X+x-i][y].under;
						} else {
							o=newbrd->board[x-i][y].obj;
						}
						
						dist = o->getPosition() - player->getPosition();
						
						a=(dist.x)*(dist.x)/2.0f;
						b=(dist.y)*(dist.y);
						
						if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,177);
						} else if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
							ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
							ct->putChar(x,y,o->getShape());
						} else if(currentbrd->dark && !(o->getFlags()&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
							ct->putColor(x,y,o->getFg()%8);
							ct->putChar(x,y,o->getShape());
						} else {
							ct->putColor(x,y,((o->getFg() > 7) ? HIGH_INTENSITY : 0) | (o->getFg()%8) | (o->getBg() << 8));
							ct->putChar(x,y,o->getShape());
						}
					}
				}
				render();
				Time::sleep(1000);
			}
			break;
	}
}

void switch_board(int num) {
	direction h = (player==NULL)?IDLE:player->getHeading();
	
  world.start=num;
	player=(Player *)get_obj_by_type(get_board(num),ZZT_PLAYER);
	player->setStep(player->getPosition());

	if(player!=NULL && currentbrd!=NULL) boardTransition(h,get_board(num));
	
  currentbrd=get_board(num);
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

int read_zzt_int(File &fd) {
	unsigned char a,b;
	fd.read(&a,1);
	fd.read(&b,1);
	
	return (b*256) + a;
}

void load_objects(File &fd, struct board_info_node *board) {
  short int len,proglen;
  unsigned char x,y,z,cnt, p1, p2, p3, ut, uc;
  short int xstep,ystep,cycle,dumb,progpos,leader,follower;
  char *tmp;
	char pad[20];
	ZZTObject *curobj=NULL;
	
	cnt = read_zzt_int(fd);
  //fd.readle16(&cnt,1); //number of objects
#ifdef DEBUG
  printf("Loading %i objects...\n",cnt);
#endif
  for(z=0;z<=cnt;z++) {
    fd.read(&x,1); x--;
		fd.read(&y,1); y--;
#ifdef DEBUG
    //if((x>=0 && y>=0) && (x<BOARD_X && y<BOARD_Y)) printf("Storing params at: %i,%i (%s)\n",x,y,board->board[x][y].obj->getName().c_str());
#endif
    fd.readle16(&xstep,1); //xstep 
    fd.readle16(&ystep,1); //ystep
    fd.readle16(&cycle,1); //cycle
    fd.read(&p1,1); //p1
    fd.read(&p2,1); //p2
    fd.read(&p3,1); //p3
    fd.readle16(&leader,1); //leaderindex
    fd.readle16(&follower,1); //followerindex
    fd.read(&ut,1); //ut
    fd.read(&uc,1); //uc
		fd.read(&pad,4); //another 4-byte int. this time a pointer! ignore it.
    fd.readle16(&progpos,1); //ZZT-OOP offset
    fd.readle16(&proglen,1); //length of ZZT-OOP code
		fd.read(&pad,8);
		tmp=NULL;
		if(x<0 || y<0 || x > BOARD_X || y > BOARD_Y) {
			printf("Ignoring bad object at %i,%i\n",x,y);
			continue;
		}
    curobj=board->board[x][y].obj;
		if(curobj==NULL) printf("Null object at %i,%i\n",x,y);
    if(proglen>0/* && (curobj->getType()==ZZT_OBJECT || curobj->getType()==ZZT_SCROLL)*/) {
	    tmp=(char *)malloc(proglen+1);
      /*for(len=0;len<proglen;len++) {
      	tmp[len]=file_read_byte(fd);
				//if(tmp[len]=='\r') tmp[len]='\n';
      }*/
			fd.read(tmp,proglen);
      tmp[proglen]='\0';
			//printf("Program (%i/%i): %s\n",len,proglen,tmp);
    }
    if(curobj!=NULL && x>=0 && y>=0) {
			curobj->setStep(Vector(xstep,ystep,0));
			curobj->setCycle(cycle);
      curobj->setParam(1,p1);
      curobj->setParam(2,p2);
      curobj->setParam(3,p3);
			curobj->setProg((tmp==NULL)?"":tmp,proglen,progpos);
      curobj->create();
      board->board[x][y].under=create_object(ut,x,y);
      board->board[x][y].under->setFg(uc%16);
      board->board[x][y].under->setBg(uc/16);
      //board->board[x][y].under->create();
    } else {
      printf("Invalid object at: (%i,%i)\n",x,y);
    }
  }
}

int load_zzt(char *filename, int titleonly) {
  unsigned int c,x,y,z,sum=0,q;
	unsigned char len,col,cod;
  struct board_info_node *current=NULL;
  ZZTObject *curobj=NULL;
  ZZTObject *prev=NULL; 

	File fd(filename,"rb");

	spinner("Loading");
	fd.readle16(&world.magic,1);
	if(world.magic!=65535) return -1;
	
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
  current=(struct board_info_node *)malloc(sizeof(struct board_info_node));
  board_list=current;
	if(titleonly==1) world.board_count=1;
  for(q=0;q<=world.board_count;q++) {
    //current->objlist=NULL;
    curobj=NULL;
		//fd.readle16(&c, 1);
    read_zzt_int(fd);//size (in bytes) of the board
		fd.read(&len,1);
    fd.read(current->title,34);
    current->title[len]='\0';
#ifdef DEBUG
    printf("Board title: %s\n",current->title);
#endif
    for(x=0;x<16;x++) { fd.read(&cod,1); } //padding
    //here comes the RLE data!
    x=0;y=0;z=0; prev=NULL; curobj=NULL;
    while(z<1500) {
      fd.read(&len,1);
      fd.read(&cod,1);
      fd.read(&col,1);
			//printf("%i %i %i %i\n",len,cod,col, z);
      for(c=0;c<len;c++) {
	      z++;
	      if(x>BOARD_X-1) { x=0; y++; }
        current->board[x][y].obj=NULL;
        current->board[x][y].under=NULL;
        current->board[x][y].obj=create_object(cod,x,y);
        curobj=current->board[x][y].obj;
        if(curobj!=NULL) {
          curobj->setFg(col%16);
          curobj->setBg(col/16);
					curobj->create();
        } else {
					printf("Unknown type encountered at (%i, %i): %i\n",x,y,cod);
					current->board[x][y].obj=create_object(ZZT_EMPTY,x,y);
				}
      	x++;
      }
    }
    if(z!=1500) { 
      printf("RLE mismatch!\n"); 
      return -1; 
    }
    //intelligent line drawing!
    for(x=0;x<BOARD_X;x++) {
      for(y=0;y<BOARD_Y;y++) {
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
    current->num=q;
    fd.read(&current->maxshots,1);
    fd.read(&current->dark,1);
    fd.read(&current->board_up,1);
    fd.read(&current->board_down,1);
    fd.read(&current->board_left,1);
    fd.read(&current->board_right,1);
    fd.read(&current->reenter,1);
		current->msgcount=0;
		fd.read(&cod,1);
		fd.read(&current->message,60);
		//printf("Message: %s\n",current->message);
		fd.readle16(&current->time,1);
    for(x=0;x<16;x++) { fd.read(&cod,1); } //more padding
    load_objects(fd,current);
    //printf("q: %i world.board_count: %i\n",q,world.board_count);
    if(q<world.board_count) {
      current->next=(struct board_info_node *)malloc(sizeof(struct board_info_node));
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

//void delay(float seconds);

void update_brd() {
  ZZTObject *o=NULL;
  ZZTObject *p=NULL;
  ZZTObject *t=NULL;
  struct board_info_node *current=currentbrd;
  int rx,ry,x,y,i=0,j;
  char buf[30];

	if(current->num>0) {
		if(world.torch_cycle>0) {
			world.torch_cycle--;
			draw_torch();
		}
		
		if(world.time>0) {
			draw_time();
		}
	
		world_sec--;
		if(world.health==0 && world_sec==0) {
			set_msg("Game Over - Press ESC");
		}
		
		if(world.energizer_cycle>0) {
			world.energizer_cycle--;
			if(world.energizer_cycle==0) {
				zm->setTune("s.-c-a#gf#fd#c");
				zm->start();
			}
		}
		
		if(world_sec==0 && currentbrd->time>0) {
			take_time(1);
			if(world.time==10) { set_msg("Time is running out!"); }
			if(world.time==0) player->message(player,"time");
		}
		if(world_sec==0) world_sec=10;
	}
	
	for(j=0; j<2; j++) {
		i=0;
		for(y=0;y<BOARD_Y;y++) {
			for(x=0;x<BOARD_X;x++) {
				o=current->board[x][y].obj;
				if(o!=NULL && o->getUpdated()==0) { 
					o->setTick(o->getTick()-1);
					if(o->getTick()<=0 && i%2==j) {
						//printf("Updating: %s\n",o->name);
						o->update();
						if(o->getFlags()&F_DELETED) {
							delete o;
						} else {
							o->setUpdated(true);
							o->setTick(o->getCycle());
						}
					}
				}
			}
			if(o!=NULL) {
				i++; i%=2;
			}							
		}
	}
	
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

int flicker = 0;

void draw_block(int x, int y) {
  if(x>=BOARD_X||y>=BOARD_Y||x<0||y<0) return;
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
