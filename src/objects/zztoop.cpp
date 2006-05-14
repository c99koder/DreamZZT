/*
 *  zztoop.cpp
 *  ZZT-OOP scriptable NPC
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

#include "sound.h"
#include "object.h"
#include "word.h"
#include "board.h"
#include "status.h"
#include "http.h"

extern struct world_header world;
extern struct object *player;

extern ZZTMusicStream *zm;

void zzt_goto(struct object *me, char *label);

create_handler zztoop_create(struct object *me) {
  me->shape=me->arg1;
  me->arg3=5;
  return 0;
}

msg_handler zztoop_message(struct object *me, struct object *them, char *message) {
  //me->arg2=them->id;
  //printf("ZZT-OOP: %s\n",me->prog);
  zzt_goto(me,message);
  //me->update(me);
  return 0;
}

extern struct board_info_node *currentbrd;

struct object *find_zztobj_by_name(int *x, int *y, char *target) {
  struct object *myobj;
  char name[20];
  int z;
  int i,j;
  for(j=*y;j<BOARD_Y;j++) {
    for(i=0;i<BOARD_X;i++) {
      if(i==0&&j==*y) i=*x;
      if(i>=BOARD_X) { i=0; j++; }
      myobj=currentbrd->board[i][j].obj;
      //if(myobj==NULL) { break; }
			//printf("%i,%i: %s\n",i,j,myobj->name);
			fflush(NULL);
      if(myobj->type==ZZT_OBJECT && myobj->prog!=NULL && myobj->prog[0]=='@') {
        z=1;
        while(myobj->prog[z]!='\r' && z<myobj->proglen) {
	        name[z-1]=myobj->prog[z];
					if(name[z-1]>='A' && name[z-1]<='Z') //convert to lowercase 
						name[z-1]+=('a'-'A');
	        z++;
        }
        name[z-1]='\0';
        //strtolower(name);
        printf("Compare: %s and %s\n",target,name);
        if(!strcmp(name,strtolower(target))) {
          *x=i;
          *y=j;
	        return myobj;
        }
      }
    }
  }
	//printf("No match\n");
  return NULL;
}

char *get_zztobj_name(struct object *myobj) {
  int y;
  static char name[20];

  if((myobj->type==ZZT_OBJECT || myobj->type==ZZT_SCROLL) && myobj->prog[0]=='@') {
    y=1;
    while(myobj->prog[y]!='\r') {
	    name[y-1]=myobj->prog[y];
	    y++;
    }
    name[y-1]='\0';
    return strtolower(name);
  }
  return myobj->name;
}

void zzt_send(struct object *me, char *cmd) {
  int x,y;
  struct object *myobj=NULL;
	struct object *oldobj=NULL;
  struct board_info_node *brd;
  char target[100];
  char label[100];
  char name[100];
  for(x=0;x<strlen(cmd);x++) {
    if(cmd[x]==':') {
      break;
    }
    target[x]=cmd[x];
  }
  target[x]='\0';
  if(x==strlen(cmd)) {
    zzt_goto(me,target);
    return;
  }
  x++;
  for(y=0;y<strlen(cmd)-x;y++) {
    label[y]=cmd[x+y];
  }
  label[y]='\0';
  x=0;
  if(!strcmp(target,"all") || !strcmp(target,"others")) {
    for(y=0;y<BOARD_Y;y++) {
      for(x=0;x<BOARD_X;x++) {
        myobj=currentbrd->board[x][y].obj;
        if(myobj->type==ZZT_OBJECT) {
          if(myobj==me && !strcmp(target,"others")) {
            //printf("Not sending to self\n");
          } else {
            zzt_goto(myobj,label);
          }
        }
      }
    }
  } else {
    x=0; y=0;
    myobj=find_zztobj_by_name(&x,&y,target);
    while(myobj!=NULL) {
      zzt_goto(myobj,label);
      x++;
			oldobj=myobj;
      myobj=find_zztobj_by_name(&x,&y,target);
    };
  }
}

void zzt_zap(struct object *myobj, char *label) {
  char text[256];
  int x,y,newline=0,goagain=1;
	printf("%s zapping %s\n",get_zztobj_name(myobj),label);
  for(x=0;x<myobj->proglen;x++) {
    switch(myobj->prog[x]) {
    case '\r':
      newline=1;
      break;
    case ':':
      if(newline==1) {
				y=1;
				while(myobj->prog[x+y]!='\r') {
					text[y-1]=myobj->prog[x+y];
					y++;
				}
				text[y-1]='\0';
				if(!strcmp(label,text)) {
					myobj->prog[x]='\'';
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

void zzt_restore(struct object *myobj, char *label) {
  char text[256];
  int x,y,newline=0,goagain=1;
	printf("%s restoring %s\n",get_zztobj_name(myobj),label);
  for(x=0;x<myobj->proglen;x++) {
    switch(myobj->prog[x]) {
    case '\r':
      newline=1;
      break;
    case '\'':
      if(newline==1) {
				y=1;
				while(myobj->prog[x+y]!='\r') {
					text[y-1]=myobj->prog[x+y];
					y++;
				}
				text[y-1]='\0';
				if(!strcmp(label,text)) {
					myobj->prog[x]=':';
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

void zzt_goto(struct object *myobj, char *label) {
  char text[256];
  int x,y,newline=0,goagain=1;
  if(label==NULL) return;
  if(myobj==NULL) return;
  if(label[0]=='#') {
    for(x=0;x<strlen(label);x++) {
      label[x]=label[x+1];
    }
  }
  for(x=0;x<myobj->proglen;x++) {
    switch(myobj->prog[x]) {
    case '\r':
      newline=1;
      break;
    case ':':
      x++;
      if(newline==1) {
				y=0;
				while(myobj->prog[x+y]!='\r') {
					text[y]=myobj->prog[x+y];
					y++;
				}
				text[y]='\0';
				printf("Comparing labels: '%s' and '%s'\n",label,text);
				if(!strcmp(label,text)) {
					myobj->progpos=x+y;
					printf("Position set\n");
					return;
				}
				x+=y;
				newline=0;
      }
      break;
    default:
      newline=0;
      break;
    }
  }
  if(!strcmp(label,"restart")) {
    myobj->progpos=0;
  } else {
    //printf("Label not found\n");
  }
}

update_handler zztoop_update(struct object *myobj) {
  struct object *theirobj;
  struct board_data *b;
  static int textfg=1,textbg=0;
  int in,went=0,color,color2;
  char *text;
  char tmp[100];
  char lbl[40];
  int x=0,y=0,z,newline=0,linecount=0,goagain=1,tx=0,ty=0,i,j;
  int neg,res;
	if(myobj->proglen<1) return 0;
  //printf("%s: --CYCLE--\n",myobj->name);
  while(goagain) {
    goagain=0;
    if(myobj->progpos>myobj->proglen || myobj->progpos==-1) { break; }
    text=(char *)malloc(myobj->proglen+5);
    switch(myobj->prog[myobj->progpos]) {
    case ':':
      //printf("searching for EOL after :...");
      while(myobj->prog[myobj->progpos]!='\r') {
	      myobj->progpos++;
				//printf("%c",myobj->prog[myobj->progpos]);
      }
			//printf("\n");
      goagain=1;
      break;
		case '?':
      x=++myobj->progpos;
      y=0;
      while(myobj->prog[y+x]!='\r' && myobj->prog[y+x]!='#' && myobj->prog[y+x]!='?' && myobj->prog[y+x]!='/') {
	      text[y]=myobj->prog[y+x];
	      y++;
	      myobj->progpos++;
      }
      text[y]='\0';
			myobj->progpos--;
      wordify(strtolower(text),' ');
#ifdef DEBUG
      printf("%s executing: ?%s\n",myobj->name,text);
#endif
			move(myobj,word_to_direction(myobj,0));
			goagain=0;
			break;
    case '/':
      x=++myobj->progpos;
      y=0;
      while(myobj->prog[y+x]!='\r' && myobj->prog[y+x]!='#' && myobj->prog[y+x]!='?' && myobj->prog[y+x]!='/') {
	      text[y]=myobj->prog[y+x];
	      y++;
      }
      text[y]='\0';
      wordify(strtolower(text),' ');
#ifdef DEBUG
      printf("%s executing: /%s\n",myobj->name,text);
#endif
			x=word_to_direction(myobj,0);
			if(try_move(myobj,(direction)x)||x==5) {
				move(myobj,(direction)x);
				myobj->progpos+=(y-1);
			} else {
				printf("Move failed.\n");
				myobj->progpos-=2;
			}
			break;
    case '\'':
      //printf("searching for EOL after '...");
      while(myobj->prog[myobj->progpos]!='\r') {
	      myobj->progpos++;
				//printf("%c",myobj->prog[myobj->progpos]);
      }
			//printf("\n");
      goagain=1;
      break;
    case '@':
      x=++myobj->progpos;
      y=0;
      while(myobj->prog[y+x]!='\r') {
	      //printf("searching for EOL after @...");
	      text[y]=myobj->prog[y+x];
	      y++;
	      myobj->progpos++;
      }
      text[y]='\0';
      //set_name(myobj,text);
      goagain=1;
      break;
    case '^':
    case '#':
      x=++myobj->progpos;
      y=0;
      while(myobj->prog[y+x]!='\r') {
	      text[y]=myobj->prog[y+x];
	      y++;
	      myobj->progpos++;
      }
      text[y]='\0';
      wordify(strtolower(text),' ');
#ifdef DEBUG
      printf("%s executing: %s\n",myobj->name,text);
#endif
      strcpy(tmp,get_word(0));
			if(!strcmp("go",get_word(0))) {
				res=word_to_direction(myobj,1);
				if(try_move(myobj,(direction)res)) {
					move(myobj,(direction)res);
				} else {
					myobj->progpos-=(strlen(text)+2);
				}
				goagain=0;
			}
      else if(!strcmp("walk",get_word(0))) {
        myobj->arg3=word_to_direction(myobj,1);
        if(myobj->arg3==5) goagain=1;
      }
			else if(!strcmp("play",get_word(0))) {
				if(zm != NULL) {
					printf("Playing: %s\n", text + 5);
					zm->setTune(text + 5);
					if(!zm->isPlaying()) zm->start();
				}
				goagain = 1;
			}
      else if(!strcmp("try",get_word(0))) {
        res=word_to_direction(myobj,1);
        if(try_move(myobj,(direction)res)) {
          move(myobj,(direction)res);
        } else {
          if(get_word(2)!=NULL) zzt_goto(myobj,get_word(2));
        }
      }
      else if(!strcmp("cycle",get_word(0))) {
        myobj->cycle=atoi(get_word(1));
        if(myobj->cycle==0) myobj->cycle=1;
        goagain=1;
      }
      else if(!strcmp("put",get_word(0))) {
        color=str_to_color(get_word(2));
        if(color!=-1) delete_word(2);
        if(!strcmp("w",get_word(1)) || !strcmp("west",get_word(1))) {
          i=myobj->x-1; j=myobj->y;
        } else if(!strcmp("e",get_word(1)) || !strcmp("east",get_word(1))) {
          i=myobj->x+1; j=myobj->y;
        } else if(!strcmp("n",get_word(1)) || !strcmp("north",get_word(1))) {
          i=myobj->x; j=myobj->y-1;
        } else if(!strcmp("s",get_word(1)) || !strcmp("south",get_word(1))) {
          i=myobj->x; j=myobj->y+1;
        }
				if(i>=0 && i<BOARD_X && j>=0 && j<BOARD_Y) {
					b=&currentbrd->board[i][j];
					if(str_to_obj(get_word(2))==-1) {
						set_msg("ERR: undefined item");
					} else {
						b->under=b->obj;
						b->obj=create_object(str_to_obj(get_word(2)),i,j);
						if(b->obj->create!=NULL) b->obj->create(b->obj);
						if(color!=-1) *b->obj->color=color;
						if(b->obj->bg>7) b->obj->bg-=8;
						draw_block(i,j);
					}
				}
        goagain=1;
      }
      else if(!strcmp("become",get_word(0))) {
        color=str_to_color(get_word(1));
        if(color!=-1) delete_word(1);
        b=&currentbrd->board[myobj->x][myobj->y];
        if(str_to_obj(get_word(1))==-1) {
          set_msg("ERR: undefined item");
        } else {
          b->under=b->obj;
          b->obj=create_object(str_to_obj(get_word(1)),i,j);
          if(color!=-1) *b->obj->color=color;
          draw_block(myobj->x,myobj->y);
        }
        goagain=1;
      }
      else if(!strcmp("change",get_word(0))) {
        goagain=1;
        color=str_to_color(get_word(1));
        if(color!=-1) delete_word(1);
				color2=str_to_color(get_word(2));
				if(color2!=-1) delete_word(2);
        i=0; j=0;
        do {
          res=0;
          b=get_block_by_type(get_word(1),&i,&j);
          if(b!=NULL) {
						//printf("Type: %s\nColor: (%i) (%i)\n",b->obj->name,*b->obj->color,color);
            if(color==-1 || *b->obj->color==color || (color>=8 && *b->obj->color==color-8)) {
							if(!strcmp(get_word(1),get_word(2))) {
								*b->obj->color=color2;
							} else {
								b->obj->flags&=F_DELETED;
								if(str_to_obj(get_word(2))==-1) {
									set_msg("ERR: undefined item");
								} else {
									b->under=b->obj;
									//printf("Creating a %s...\n",get_word(2));
									b->obj=create_object(str_to_obj(get_word(2)),i,j);
									b->obj->arg1=b->under->arg1;
									b->obj->arg2=b->under->arg2;
									b->obj->arg3=b->under->arg3;
									b->obj->arg4=b->under->arg4;
									b->obj->prog=b->under->prog;
									b->obj->progpos=b->under->progpos;
									b->obj->proglen=b->under->proglen;
									if(b->obj->type==ZZT_OBJECT) b->obj->shape=b->under->shape;
									b->obj->fg=b->under->fg;
									b->obj->bg=b->under->bg;
									if(b->obj->create!=NULL) b->obj->create(b->obj);
									if(color!=-1) *b->obj->color=color; 
									else if(color2!=-1) *b->obj->color=color2;
									else *b->obj->color=*b->under->color;
									if(b->obj->bg>=8) b->obj->bg-=8;
									draw_block(i,j);
									free(b->under);
									b->under=NULL;
								}
							}
            }
          }
          i++;
        } while(b!=NULL);
      }
			else if(!strcmp("__dark",get_word(0))) {
				currentbrd->dark=atoi(get_word(1));
				draw_board();
			}
      else if(!strcmp("set",get_word(0))) {
        for(i=0;i<10;i++) {
					printf("Flag[%i].len=%i\n",i,world.flag[i].len);
          if(world.flag[i].len==0) {
					  printf("Storing flag in slot %i\n",i);
            strcpy((char *)world.flag[i].string,get_word(1));
						world.flag[i].len=strlen(get_word(1));
            break;
          }
        }
      }
      else if(!strcmp("clear",get_word(0))) {
        for(i=0;i<10;i++) {
          if(!strcmp((char *)world.flag[i].string,get_word(1))) {
            world.flag[i].string[0]='\0';
						world.flag[i].len=0;
            break;
          }
        }
      }
      else if(!strcmp("if",get_word(0))) {
        res=0;
        if(!strcmp("not",get_word(1))) {
          neg=1;
          delete_word(1);
        } else {
          neg=0;
        }
				if(!strcmp("alligned",get_word(1))) {
					if(player->x==myobj->x || player->y == myobj->y) res=1;
  				if(!strcmp("then",get_word(2))) delete_word(2);
          strcpy(lbl,get_word(2));
        } else if(!strcmp("any",get_word(1))) {
          color=str_to_color(get_word(2));
          if(color!=-1) delete_word(2);
          i=0; j=0;
          do {
            b=get_block_by_type(get_word(2),&i,&j);
            if(b!=NULL) {
							if(color==-1) break;
              if(color<8 && *b->obj->color>=8) color+=8;
              if(color>=8 && *b->obj->color<8) color-=8;
            }
            i++;
          } while(b!=NULL && (color!=-1 && *b->obj->color!=color));
          if(b!=NULL) res=1;
  				if(!strcmp("then",get_word(3))) delete_word(3);
          strcpy(lbl,get_word(3));
        } else if(!strcmp("contact",get_word(1))) {
					if(!strcmp("then",get_word(2))) delete_word(2);
          strcpy(lbl,get_word(2));
          if(currentbrd->board[myobj->x][myobj->y-1].obj!=NULL) {
            if(currentbrd->board[myobj->x][myobj->y-1].obj->type==ZZT_PLAYER) {
              res=1;
            }
          }
          if(currentbrd->board[myobj->x][myobj->y+1].obj!=NULL) {
            if(currentbrd->board[myobj->x][myobj->y+1].obj->type==ZZT_PLAYER) {
              res=1;
            }
          }
          if(currentbrd->board[myobj->x-1][myobj->y].obj!=NULL) {
            if(currentbrd->board[myobj->x-1][myobj->y].obj->type==ZZT_PLAYER) {
              res=1;
            }
          }
          if(currentbrd->board[myobj->x+1][myobj->y].obj!=NULL) {
            if(currentbrd->board[myobj->x+1][myobj->y].obj->type==ZZT_PLAYER) {
              res=1;
            }
          }
        } else if(!strcmp("blocked",get_word(1))) {
          switch(word_to_direction(myobj,2)) {
          case UP:
            if(is_empty(currentbrd,myobj->x,myobj->y-1)==0) {
              res=1;
            }
            break;
          case DOWN:
            if(is_empty(currentbrd,myobj->x,myobj->y+1)==0) {
              res=1;
            }
            break;
          case RIGHT:
            if(is_empty(currentbrd,myobj->x+1,myobj->y)==0) {
              res=1;
            }
						break;
          case LEFT:
            if(is_empty(currentbrd,myobj->x-1,myobj->y)==0) {
              res=1;
            }
            break;
          }
					if(!strcmp("then",get_word(3))) delete_word(3);
          strcpy(lbl,get_word(3));
        } else {
					if(!strcmp("then",get_word(2))) delete_word(2);
          strcpy(lbl,get_word(2));
					if(!strcmp("dreamcast",get_word(1))) {
#ifdef DREAMCAST
						res=1;
#else
						res=0;
#endif
					} else if (!strcmp("pc",get_word(1))) {
#ifndef DREAMCAST
						res=1;
#else
						res=0;
#endif
					} else {
						for(i=0;i<10;i++) {
							//printf("Flag %i: %s\n",i,world.flag[i].string);
							if(!strcmp((char *)world.flag[i].string,get_word(1))) {
								printf("Matched!");
								res=1;
								break;
							}
						}
					}
        }
        if(neg==1) res=!res;
        if(res==1) zzt_send(myobj,lbl);
        goagain=1;
      }
      else if(!strcmp("bind",get_word(0))) {
        i=0;
        j=0;
        theirobj=find_zztobj_by_name(&i,&j,get_word(1));
        if(theirobj!=NULL)  {
          free(myobj->prog);
          myobj->prog=theirobj->prog;
  	      myobj->progpos=0;
          myobj->proglen=theirobj->proglen;
          return 0;
        }
	      break;
      }
      else if(!strcmp("zap",get_word(0))) {
				strcpy(tmp,get_word(1));
				for(i=0;i<strlen(tmp);i++) {
					lbl[i]=tmp[i];
					if(tmp[i]==':') break;
				}
				lbl[i]='\0';
				if(i<strlen(tmp)) {
					x=0;y=0;
					theirobj=find_zztobj_by_name(&x,&y,lbl);
					while(theirobj!=NULL) {
						zzt_zap(theirobj,tmp+i+1);
						x++;
						theirobj=find_zztobj_by_name(&x,&y,lbl);
					}
				} else {
      		zzt_zap(myobj,lbl);
				}
        goagain=1;
      }
      else if(!strcmp("restore",get_word(0))) {
				strcpy(tmp,get_word(1));
				for(i=0;i<strlen(tmp);i++) {
					lbl[i]=tmp[i];
					if(tmp[i]==':') break;
				}
				lbl[i]='\0';
				if(i<strlen(tmp)) {
					x=0;y=0;
					zzt_restore(find_zztobj_by_name(&x,&y,lbl),tmp+i+1);
				} else {
      		zzt_restore(myobj,lbl);
				}
        goagain=1;
      }
      else if(!strcmp("lock",get_word(0))) {
	      myobj->flags|=F_SLEEPING;
	      goagain=1;
      }
      else if(!strcmp("unlock",get_word(0))) {
	      myobj->flags&=~F_SLEEPING;
        goagain=1;
      }
      else if(!strcmp("send",get_word(0))) {
	      zzt_send(myobj,get_word(1));
	      goagain=1;
      }
      else if(!strcmp("teleport",get_word(0))) {
	      if(is_empty(currentbrd,atoi(get_word(1)),atoi(get_word(2)))==0) {
	        //set_msg("Invalid teleport.");
	      } else {
	        myobj->x=atoi(get_word(1));
	        myobj->y=atoi(get_word(2));
	      }
      }
      else if(!strcmp("color",get_word(0))) {
      	myobj->fg=atoi(get_word(1));
      }
      else if(!strcmp("char",get_word(0))) {
      	myobj->shape=atoi(get_word(1));
        draw_block(myobj->x,myobj->y);
				goagain=1;
      }
      else if(!strcmp("give",get_word(0))) {
	      if(!strcmp("ammo",get_word(1))) {
	        give_ammo(atoi(get_word(2)));
	      }
	      if(!strcmp("gems",get_word(1))) {
	        give_gems(atoi(get_word(2)));
	      }
	      if(!strcmp("torch",get_word(1))) {
	        give_torch(atoi(get_word(2)));
	      }
	      if(!strcmp("score",get_word(1))) {
	        give_score(atoi(get_word(2)));
	      }
	      if(!strcmp("health",get_word(1))) {
	        give_health(atoi(get_word(2)));
	      }
	      if(!strcmp("time",get_word(1))) {
	        give_time(atoi(get_word(2)));
	      }
	      goagain=1;
      }
      else if(!strcmp("take",get_word(0))) {
	      if(!strcmp("gems",get_word(1))) {
	        if(world.gems<atoi(get_word(2))) { zzt_goto(myobj,get_word(3)); }
	        else take_gems(atoi(get_word(2)));
	      }
	      if(!strcmp("torch",get_word(1))) {
	        if(world.torches<atoi(get_word(2))) { zzt_goto(myobj,get_word(3)); }
	        else take_torch(atoi(get_word(2)));
	      }
	      if(!strcmp("ammo",get_word(1))) {
	        if(world.ammo<atoi(get_word(2))) { zzt_goto(myobj,get_word(3)); }
	        else take_ammo(atoi(get_word(2)));
	      }
	      if(!strcmp("score",get_word(1))) {
	        if(world.score<atoi(get_word(2))) { zzt_goto(myobj,get_word(3)); }
	        else take_score(atoi(get_word(2)));
	      }
	      if(!strcmp("health",get_word(1))) {
	        if(world.health<atoi(get_word(2))) { zzt_goto(myobj,get_word(3)); }
	        else take_health(atoi(get_word(2)));
	      }
	      if(!strcmp("time",get_word(1))) {
	        if(currentbrd->time<atoi(get_word(2))) { zzt_goto(myobj,get_word(3)); }
	        else take_time(atoi(get_word(2)));
	      }
	      goagain=1;
      }
      else if(!strcmp("die",get_word(0))) {
        remove_from_board(currentbrd,myobj);
	      myobj->progpos=-1;
      }
      else if(!strcmp("end",get_word(0))) {
	      myobj->progpos=-1;
	      break;
      }
      else if(!strcmp("endgame",get_word(0))) {
	      world.health=0;
	      break;
      }
      else if(!strcmp("play",get_word(0))) {
        goagain=1;
      }
      else if(!strcmp("throwstar",get_word(0))) {
      }
      else if(!strcmp("shoot",get_word(0))) {
        shoot(myobj,word_to_direction(myobj,1));
      }
      else {
        zzt_send(myobj,get_word(0));
        goagain=1;
      }
      break;
    case '\r':
      goagain=1;
      break;
    default:
      x=myobj->progpos;
      y=0;
      while(myobj->prog[y+x]!='\0') {
	      if(myobj->prog[y+x]=='\r') {
	        newline=1;
	        linecount++;
	      }
	      if((newline==1 && myobj->progpos!=-1) && (myobj->prog[y+x]=='#'||myobj->prog[y+x]=='/'||myobj->prog[y+x]=='?' || myobj->prog[x+y]==':' || myobj->prog[x+y]=='\'' || myobj->proglen <= x+y+1)) {
	        if(myobj->proglen < x+y || text[y-1]=='\r') {
						text[y-1]='\0';
					} else {
						text[y]='\0';
					}
	        if(text[0]!=':') {
	          if(linecount>1) {
              printf("%s\n%s\n",myobj->name,text);
              lbl[0]='\0';
	            TextWindow t(get_zztobj_name(myobj),text);
							t.doMenu();
              draw_board();
              if(t.getLabel()!='\0') { zzt_goto(myobj,t.getLabel()); }
	          } else {
							printf("%s\n",text);
	            set_msg(text);
	            goagain=1;
	          }
	        }
	        myobj->progpos--;
	        //myobj->update(me);
	        break;
	      }
	      text[y]=myobj->prog[y+x];
				//printf("%c",myobj->prog[x+y]);
        if(text[y]!='\r') newline=0;
	      y++;
	      myobj->progpos++;
      }
    }
    if(myobj->progpos>0) myobj->progpos++;
    if(went++>6) goagain=0;
    free(text);
  }
  if(myobj->arg3!=5) {
    move(myobj,(direction)myobj->arg3);
    //printf("Moving: %i\n",myobj->arg3);
  }
  //printf("returning..\n");
  return 0;
}
