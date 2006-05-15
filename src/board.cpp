/*
 *  board.cpp
 *  Displays and manages a ZZT world
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
using namespace Tiki::Audio;

#include <Tiki/drawables/console.h>
#include "object.h"
#include "board.h"
#include "status.h"
#include "sound.h"

extern ZZTMusicStream *zm;

extern ConsoleText *ct;

struct world_header world;
struct board_info_node *board_list=NULL;
struct board_info_node *currentbrd=NULL;
extern struct object *player;
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
				if(current->board[x][y].obj!=NULL) {
					//printf("Freeing a %s at %i,%i\n",current->board[x][y].obj->name,x,y);
					//fflush(NULL);
					if(current->board[x][y].obj->prog!=NULL) {
						//printf("Freeing program: %s\n",current->board[x][y].obj->prog);
						//fflush(NULL);
						free(current->board[x][y].obj->prog);
					}
					free(current->board[x][y].obj);
				}
				if(current->board[x][y].under!=NULL) {
					//printf("Freeing a %s under %i,%i\n",current->board[x][y].under->name,x,y);
					if(current->board[x][y].under->prog!=NULL) {
						//printf("Freeing program\n");
						free(current->board[x][y].under->prog);
					}
					free(current->board[x][y].under);
				}
			}
	  }
		prev=current;
		current=current->next;
		//printf("Freeing board\n");
		free(prev);		
	}
	board_list=NULL;
	currentbrd=NULL;
}

void spinner(char *text) {
  static int x=0;
  ct->locate(BOARD_X+3,5);
  ct->color(15,1);
  ct->printf("%s %c...",text,spin_anim[x]);
  x++;
  x%=4;
	
	Frame::begin();
	ct->draw(Drawable::Opaque);
	Frame::transEnable();
	ct->draw(Drawable::Trans);
	Frame::finish();
}

void spinner_clear() {
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
		code=curbrd->board[0][0].obj->type;
		if(code>=ZZT_BLUE_TEXT && code <=ZZT_WHITE_TEXT) {
			color=curbrd->board[0][0].obj->shape;
		} else {
			color=curbrd->board[0][0].obj->fg+(16*curbrd->board[0][0].obj->bg);
		}
		z=0;
		for(y=0;y<BOARD_Y;y++) {
			for(x=0;x<BOARD_X;x++) {
				code2=curbrd->board[x][y].obj->type;
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
					code=curbrd->board[x][y].obj->type;
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
				if(curbrd->board[x][y].obj->type==ZZT_PLAYER) curplayer=curbrd->board[x][y].obj;
			}
		}
		z--;
		printf("Writing %i objects\n",z);
		write(fd,&z,2);
		write_object(fd,curplayer,curbrd->board[curplayer->x][curplayer->y].under);
		for(y=0;y<BOARD_Y;y++) {
			for(x=0;x<BOARD_X;x++) {
				if(curbrd->board[x][y].obj->type!=ZZT_PLAYER && curbrd->board[x][y].obj->flags&F_OBJECT) write_object(fd,curbrd->board[x][y].obj,curbrd->board[x][y].under);
			}
		}		
		curbrd=curbrd->next;
	}
	close(fd);
  spinner_clear();
}*/

int is_empty(struct board_info_node *curbrd, int x, int y) {
	if(x<0 || y<0 || x>=BOARD_X || y >= BOARD_Y) return 0;
  if(curbrd->board[x][y].obj!=NULL && !(curbrd->board[x][y].obj->flags&F_EMPTY)) {
    return 0;
  }
  return 1;
}

struct board_data *get_block_by_type(char *type, int *x, int *y) {
  int i,j;
  for(j=*y;j<BOARD_Y;j++) {
    for(i=0;i<BOARD_X;i++) {
      if(i==0&&j==*y) i=*x;
      if(i>=BOARD_X) { i=0; j++; }
			if(currentbrd->board[i][j].obj!=NULL) {
				if(currentbrd->board[i][j].obj->name==NULL) {
					//printf("Invalid object name: (%i,%i)=%i\n",i,j,currentbrd->board[i][j].obj->type);
				} else if (!strcmp(currentbrd->board[i][j].obj->name,type)) {
					*x=i; *y=j;
					return &currentbrd->board[i][j];
				}
			}
    }
  }
  return NULL;
}

struct object *get_obj_by_color(struct board_info_node *board, int type, int color) {
  int x,y;
  struct object *curobj;

  for(y=0;y<BOARD_Y;y++) {
    for(x=0;x<BOARD_X;x++) {
      curobj=board->board[x][y].obj;
      if(curobj->type==type && *curobj->color==color) return curobj;
    }
  }
  return NULL;
}

struct object *get_obj_by_type(struct board_info_node *board, int type) {
  int x,y;

  for(y=0;y<BOARD_Y;y++) {
    for(x=0;x<BOARD_X;x++) {
      if(board->board[x][y].obj!=NULL && board->board[x][y].obj->type==type) return board->board[x][y].obj;
    }
  }
  return NULL;
}

void remove_from_board(struct board_info_node *brd, struct object *me) {  
  brd->board[me->x][me->y].obj=brd->board[me->x][me->y].under;
  if(brd->board[me->x][me->y].obj==NULL) {
    brd->board[me->x][me->y].obj=create_object(ZZT_EMPTY,me->x,me->y);
  }
  brd->board[me->x][me->y].under=NULL;
  me->flags|=F_DELETED;
  draw_block(me->x,me->y);
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

void switch_board(int num) {
  world.start=num;
  currentbrd=get_board(num);
	world.time=currentbrd->time;
	world_sec=10;
	draw_time();
  player=get_obj_by_type(currentbrd,ZZT_PLAYER);
  player->xstep=player->x;
  player->ystep=player->y;
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
	struct object *curobj=NULL;
	
	cnt = read_zzt_int(fd);
  //fd.readle16(&cnt,1); //number of objects
#ifdef DEBUG
  printf("Loading %i objects...\n",cnt);
#endif
  for(z=0;z<=cnt;z++) {
    fd.read(&x,1); x--;
		fd.read(&y,1); y--;
#ifdef DEBUG
    if((x>=0 && y>=0) && (x<BOARD_X && y<BOARD_Y)) printf("Storing params at: %i,%i (%s)\n",x,y,board->board[x][y].obj->name);
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
    if(proglen>0/* && (curobj->type==ZZT_OBJECT || curobj->type==ZZT_SCROLL)*/) {
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
      curobj->xstep=xstep;
      curobj->ystep=ystep;
      curobj->cycle=cycle;
      curobj->arg1=p1;
      curobj->arg2=p2;
      curobj->arg3=p3;
      curobj->proglen=proglen;
      curobj->prog=tmp;
      curobj->progpos=progpos;
      if(curobj->create != NULL) curobj->create(curobj);
      board->board[x][y].under=create_object(ut,x,y);
      board->board[x][y].under->fg=uc%16;
      board->board[x][y].under->bg=uc/16;
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
  struct object *curobj=NULL;
  struct object *prev=NULL; 

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
    spinner("Loading");
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
          curobj->fg=col%16;
          curobj->bg=col/16;
					if(curobj->create != NULL) curobj->create(curobj);
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
        if(current->board[x][y].obj->type==ZZT_LINE) {
          if(x>0&&current->board[x-1][y].obj->type==ZZT_LINE && //left is a line
             x<BOARD_X-1 && current->board[x+1][y].obj->type==ZZT_LINE) { //right is a line
             if(y>0&&current->board[x][y-1].obj->type==ZZT_LINE && //up is a line
                y<BOARD_Y-1 &&current->board[x][y+1].obj->type==ZZT_LINE) { //down is a line
                current->board[x][y].obj->shape=206;
             } else if(y<BOARD_Y-1 &&current->board[x][y+1].obj->type==ZZT_LINE) { //only down is a line
                current->board[x][y].obj->shape=203;
             } else if(y>0&&current->board[x][y-1].obj->type==ZZT_LINE) { //only up is a line
                current->board[x][y].obj->shape=202;
             } else { //only left and right
                current->board[x][y].obj->shape=205;
             }
          } else if(x>0&&current->board[x-1][y].obj->type==ZZT_LINE) { //only left is a line
             if(y>0&&current->board[x][y-1].obj->type==ZZT_LINE && //up is a line
              y<BOARD_Y-1 &&current->board[x][y+1].obj->type==ZZT_LINE) { //down is a line
                current->board[x][y].obj->shape=185;
             } else if(y<BOARD_Y-1&&current->board[x][y+1].obj->type==ZZT_LINE) { //only down is a line
                  current->board[x][y].obj->shape=187;
             } else if(y>0&&current->board[x][y-1].obj->type==ZZT_LINE) { //only up is a line
                  current->board[x][y].obj->shape=188;
             } else { //only left
                  current->board[x][y].obj->shape=181;
             }
          } else if(x<BOARD_X-1 &&current->board[x+1][y].obj->type==ZZT_LINE) { //only right is a line
               if(y>0&&current->board[x][y-1].obj->type==ZZT_LINE && //up is a line
                  y<BOARD_Y-1 &&current->board[x][y+1].obj->type==ZZT_LINE) { //down is a line
                    current->board[x][y].obj->shape=204;
               } else if(y<BOARD_Y-1 &&current->board[x][y+1].obj->type==ZZT_LINE) { //only down is a line
                    current->board[x][y].obj->shape=201;
               } else if(y>1&&current->board[x][y-1].obj->type==ZZT_LINE) { //only up is a line
                    current->board[x][y].obj->shape=200;
               } else { //only right
                    current->board[x][y].obj->shape=198;
               }
          } else { //vertical
            if(y>0&&current->board[x][y-1].obj->type==ZZT_LINE && //up is a line
               y<BOARD_Y-1&&current->board[x][y+1].obj->type==ZZT_LINE) { //down is a line
               current->board[x][y].obj->shape=186;
            } else if(y<BOARD_Y-1&&current->board[x][y+1].obj->type==ZZT_LINE) { //only down is a line
              current->board[x][y].obj->shape=210;
            } else if(y>1&&current->board[x][y-1].obj->type==ZZT_LINE){ //only up
              current->board[x][y].obj->shape=208;
            } else { //only 1 block
							current->board[x][y].obj->shape=249;
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
  struct object *o=NULL;
  struct object *p=NULL;
  struct object *t=NULL;
  struct board_info_node *current=currentbrd;
  int rx,ry,x,y;
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
			if(world.time==0) player->message(player,player,"time");
		}
		if(world_sec==0) world_sec=10;
	}
	
  for(y=0;y<BOARD_Y;y++) {
    for(x=0;x<BOARD_X;x++) {
      o=current->board[x][y].obj;
      if(o!=NULL && o->updated==0) { 
        o->tick--;
        if(o->tick<=0 && o->update != NULL) {
					//printf("Updating: %s\n",o->name);
          o->update(o);
          if(o->flags&F_DELETED) {
            delete o;
          } else {
            o->updated=1;
            o->tick=o->cycle;
          }
        }
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
        o->updated=0;
        /*if(o!=NULL && o->flags&F_DELETED) {
          remove_from_board(current,o);
          free(o->name); free(o);
        }*/
      }
    }
  }
}

int flicker = 0;

void draw_block(int x, int y) {
  float a,b;
  struct object *them=currentbrd->board[x][y].obj;
  if(x>=BOARD_X||y>=BOARD_Y||x<0||y<0) return;
  a=((x-player->x)*(x-player->x))/2.0f;
  b=(y-player->y)*(y-player->y);
	
  if(currentbrd->dark && !(them->flags&F_GLOW) && (world.torch_cycle<1 || (b==(5*5) || sqrt(a+b) > 5))) {
    ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
		ct->putChar(x,y,177);
  } else if(currentbrd->dark && !(them->flags&F_GLOW) && (world.torch_cycle<1 || (b==(4*4) || sqrt(a+b) > 4))) {
    if(them!=NULL) {
      ct->putColor(x,y,(HIGH_INTENSITY | BLACK));
      ct->putChar(x,y,them->shape);
    }
  } else if(currentbrd->dark && !(them->flags&F_GLOW) && (world.torch_cycle<1 || (b==(3*3) || sqrt(a+b) > 3))) {
    if(them!=NULL) {
      ct->putColor(x,y,them->fg%8);
			ct->putChar(x,y,them->shape);
    }
  } else {
    if(them!=NULL) {
			ct->putColor(x,y,((them->fg > 7) ? HIGH_INTENSITY : 0) | (them->fg%8) | (them->bg << 8));
      ct->putChar(x,y,them->shape);
    }
  }
}

void draw_board() {
  int x,y;
	
  for(y=0;y<BOARD_Y;y++) {
    for(x=0;x<BOARD_X;x++) {
      draw_block(x,y);
    }
  }
}
