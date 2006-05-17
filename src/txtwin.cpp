/*
 *  txtwin.cpp
 *  Displays and manages a text popup window with ZZT formatting
 *
 *  Copyright 2000 - 2006 Sam Steele. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/tikitime.h>
#include <string.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;

#include <Tiki/drawables/console.h>
#include "status.h"

extern ConsoleText *ct;
extern ConsoleText *dt;

void draw_shadow(int x, int y) {
  int fg=(ct->getColor(x,y)/16)-8;
  if(fg<0) fg=8;
	ct->putColor(x,y, fg);
}

void pc_box(int x, int y,int w,int h,int fg,int bg) {
  int i,j,f;
  //draw a box using IBM extended ASCII
  ct->putColor(x,y,fg | (bg << 8));
	ct->putChar(x,y,218);

  for(i=0;i<w;i++) {
		ct->putColor(x+i+1,y,fg | (bg << 8));
		ct->putChar(x+i+1,y,196);
  }

	ct->putColor(x+w+1,y,fg | (bg << 8));
	ct->putChar(x+w+1,y,191);
  
	for(i=0;i<h;i++) {
		ct->putColor(x,y+i+1,fg | (bg << 8));
		ct->putChar(x,y+i+1,179);

    for(j=0;j<w;j++) {
			ct->putColor(x+j+1,y+i+1,fg | (bg << 8));
			ct->putChar(x+j+1,y+i+1,' ');
    }
    
		ct->putColor(x+j+1,y+i+1,fg | (bg << 8));
		ct->putChar(x+j+1,y+i+1,179);

    draw_shadow(x+w+2,y+i+1);
    draw_shadow(x+w+3,y+i+1);
    ct->color(fg,bg);
  }
  
	ct->locate(x,y+h+1);
  ct->printf("%c",192);
  for(i=0;i<w;i++)
    ct->printf("%c",196);
  ct->printf("%c",217);
  draw_shadow(x+w+2,y+h+1);
  draw_shadow(x+w+3,y+h+1);
	
  for(i=0;i<w+2;i++)
    draw_shadow(x+2+i,y+h+2);
}

//draw a window with text in it, wait for "start", delete the window
TextWindow::TextWindow(char *title,char *text) {
	int x=0,u=0,z=0,i=0;
	
	y=0;
	loop=1;
	maxy=0;
	dirty=1;
	lblcnt=0;
	q=0;
 	
	for(y=0;y<150;y++) txt[y][44]=' ';
	y=0;
  pc_box(6,3,45,17,YELLOW | HIGH_INTENSITY, BLUE);
  ct->locate(7+22-strlen(title)/2,4);
  ct->color(GREY | HIGH_INTENSITY, BLUE);
  ct->printf("%s",title);
  ct->locate(6,5);
  ct->color(14,1);
  ct->printf("%c",195);
  for(x=0;x<45;x++)
    ct->printf("%c",196);
  ct->printf("%c",180); 
  for(x=0;x<50;x++) {
    lbl[x][0]='\0';
  }
  for(x=0;x<150;x++) {
    lblidx[x]=-1;
  }
  x=0;
  while(text[z]!='\0') {
    if(x==0) {
      if(text[z]=='$') { 
        z++; 
        txt[y][44]='$';
        u=0;
        while(text[z+u]!='\r' && text[z+u]!='\n' && text[z+u]!='\0') {
          u++; 
        }
        for(i=0;i<20-(u/2);i++) {
          txt[y][x++]=' ';
        }
      } else if(text[z]=='!') { 
        z++;
        txt[y][44]='!';
        q=0;
        lblidx[y]=lblcnt;
        while(text[z]!=';') {
          lbl[lblcnt][q++]=text[z++];
        }
				lbl[lblcnt][q]='\0';
#ifdef DEBUG
				printf("Label %i: %s\n",lblcnt,lbl[lblcnt]);
#endif
        lblcnt++;
        z++;
      } else { txt[y][44]=' '; }
	  }
		if(text[z]=='\t') {
			q=5-(x%5);
			for(u=0;u<q;u++) {
				txt[y][x]=' ';
				x++;
			}
			z++;
		}
    txt[y][x]=text[z];
    z++; x++;
    if(text[z-1]=='\r'||text[z-1]=='\n'/* || x>43*/) {
      if(x<=42) {
        txt[y][x-1]=' ';
      }
      for(u=x;u<42;u++) {
        txt[y][u]=' ';
      }
      txt[y][42]='\0';
      if(txt[y][44]=='!') txt[y][37]='\0';
      y++;
      x=0;
    }
  }
  for(u=x;u<42;u++) {
    txt[y][u]=' ';
  }
  txt[y][42]='\0';  
  if(txt[y][44]=='!') txt[y][37]='\0';
  maxy=y;
  dirty=1;
  y=-7;
}

extern int player_hidCookie;
extern struct object *player;
void player_hidCallback(const Event & evt, void * data);

void TextWindow::doMenu() {
	int x,u;
	
	m_hidCookie = Hid::callbackReg(hidCallback, this);
	if(player) {
		Hid::callbackUnreg(player_hidCookie);
	}
	
  while(loop) {
		Frame::begin();
		ct->draw(Drawable::Opaque);
		dt->draw(Drawable::Opaque);
		Frame::transEnable();
		ct->draw(Drawable::Trans);
		dt->draw(Drawable::Trans);
		Frame::finish();
		
    if(dirty) {
      dirty=0;
      for(x=0;x<15;x++) {
        if(txt[x+y][44]=='$') { ct->color(15,1); } else { ct->color(14,1); }
        ct->locate(9,6+x);
        if(x+y==-5 || x+y==-2 || x+y==maxy+2) {
          for(u=0;u<42;u++) ct->printf(" ");
        }
        if(x+y==-4) {
					ct->setANSI(true);
					*ct << "\x1b[1;36mUse \x1b[37m\x18 \x1b[36mand \x1b[37m\x19 \x1b[36mto scroll and press " << ((TIKI_PLAT == TIKI_DC) ? "start " : "enter ") << "\x1b[36mor";
					ct->setANSI(false);
        }
        if(x+y==-3) {
					ct->setANSI(true);
          *ct << "\x1b[1;37m" << ((TIKI_PLAT == TIKI_DC) ? "fire " : "space ") << "\x1b[36mto select.  Press \x1b[37m" << ((TIKI_PLAT == TIKI_DC) ? " B  " : "ESC ") << "\x1b[36mto close.    ";
					ct->setANSI(false);
        }
        if(x+y==-1 || x+y==maxy+1) {
          for(u=0;u<42;u++) {
            ct->printf("%c",u%5==0?7:' ');
          }
        }
        if(x+y<=maxy && x+y>=0) {
					if(txt[x+y][44]=='!') {
						ct->color(13,1);
						ct->printf("   %c  ",16);
						ct->color(15,1);
						/*if(6+x==13) {
							color(13,1);
							c99_printf("  %c  ",16);   
							color(15,1);
						} else {
							c99_printf("     ");
							color(7,1);
						}*/
					}
					for(u=0; u<42 && txt[x+y][u] != '\0'; u++) {
						ct->putColor(((txt[x+y][44]=='!')?15:9)+u, 6+x, HIGH_INTENSITY | ((txt[x+y][44]=='$' || txt[x+y][44]=='!') ? WHITE : YELLOW) | (BLUE << 8));
						ct->putChar(((txt[x+y][44]=='!')?15:9)+u,6+x,txt[x+y][u]);
					}
				}
      }
      ct->color(12,1);
      ct->locate(7,13);
      ct->printf("%c",175);
      ct->locate(7+44,13);
      ct->printf("%c",174);
    }
		Time::sleep(80000);
  }
	
	Hid::callbackUnreg(m_hidCookie);
	if(player) {
		printf("Registering player callback\n");
		player_hidCookie = Hid::callbackReg(player_hidCallback, NULL);
	}
}

void TextWindow::hidCallback(const Event & evt, void * data) {
	assert( data );
	((TextWindow *)data)->processHidEvent(evt);
}

void TextWindow::processHidEvent(const Hid::Event & evt) { //Key handling for text popup window
	if (evt.type == Hid::Event::EvtQuit) {
		retlbl[0]='\0';
		loop=0;
	}
	if (evt.type == Hid::Event::EvtKeypress) {
		switch(evt.key) {
			case Event::KeyUp:
				y--;
				dirty=1;
				if(y<-7) {y=-7; dirty=0;}
				break;
			case Event::KeyDown:
				y++;
				dirty=1;
				if(y>maxy-7) { y=maxy-7; dirty=0; }
				break;
			case 13:
				if(lblidx[y+7]!=-1&&txt[y+7][44]=='!') {
					printf("Copying label %i %i: %s\n",y+7,lblidx[y+7],lbl[lblidx[y+7]]);
	      	strcpy(retlbl,lbl[lblidx[y+7]]);
          loop=0;
				} else {
					retlbl[0]='\0';
					loop=0;
				}
				break;
			case Event::KeyEsc:
				retlbl[0]='\0';
				loop=0;
				break;
		}
	}
	
	
	/*switch(poll_game_device(0)) {
	case MOVE_UP:

			break;
	case MOVE_DOWN:

			break;
	case FIRE_BTN:
	case START_BTN:
		while(poll_game_device(0)==START_BTN || poll_game_device(0) == FIRE_BTN);
				if(lblidx[y+7]!=-1&&txt[y+7][44]=='!') {
					printf("Copying label %i %i: %s\n",y+7,lblidx[y+7],lbl[lblidx[y+7]]);
	      	strcpy(retlbl,lbl[lblidx[y+7]]);
          loop=0;
				} else if(lblidx[y+7]!=-1&&txt[y+7][44]=='%') {
					read_string(input[lblidx[y+7]][1],28,6+7);
					menu_background();
					dirty=1;
					pc_box(6,3,45,17,14,1);
					locate(7+22-strlen(title)/2,4);
					color(15,1);
					c99_printf("%s",title);
					locate(6,5);
					color(14,1);
					c99_printf("%c",195);
					for(i=0;i<45;i++)
						c99_printf("%c",196);
					c99_printf("%c",180); 
          loop=1;
				} else {
					retlbl[0]='\0';
				}
					break;
	case QUIT_BTN:
				while(poll_game_device(0)==QUIT_BTN);
		retlbl[0]='\0';
		loop=0;
		break;
	}*/	
}
