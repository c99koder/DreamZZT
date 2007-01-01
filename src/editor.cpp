/* window.cpp - ZZT-formatted text popup window
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
#include <string.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Thread;

#include <Tiki/drawables/console.h>

#include "editor.h"
#include "object.h"
#include "board.h"
#include "status.h"
#include "os.h"

extern ConsoleText *ct;

extern struct world_header world;
extern struct board_info_node *board_list;
extern struct board_info_node *currentbrd;

void draw_main() {
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	ct->setANSI(true);
	
	ct->locate(BOARD_X+1,5);
	*ct << "\x1b[0;30;47m L \x1b[1;37;44m Load ";
	*ct << "\x1b[0;30;47m H \x1b[1;37;44m Help ";
	ct->locate(BOARD_X+1,6);
	*ct << "\x1b[0;30;46m S \x1b[1;37;44m Save ";
	*ct << "\x1b[0;30;46m Q \x1b[1;37;44m Quit ";
	ct->locate(BOARD_X+1,8);
	*ct << "\x1b[0;30;47m B \x1b[1;37;44m Switch Boards ";
	ct->locate(BOARD_X+1,9);
	*ct << "\x1b[0;30;46m I \x1b[1;37;44m Board Info ";
	ct->locate(BOARD_X+1,11);
	*ct << "\x1b[0;30;47m  F1  \x1b[1;37;44m Item ";
	ct->locate(BOARD_X+1,12);
	*ct << "\x1b[0;30;46m  F2  \x1b[1;37;44m Creature ";
	ct->locate(BOARD_X+1,13);
	*ct << "\x1b[0;30;47m  F3  \x1b[1;37;44m Terrain ";
	ct->locate(BOARD_X+1,14);
	*ct << "\x1b[0;30;46m  F4  \x1b[1;37;44m Text ";
	ct->locate(BOARD_X+1,16);
	*ct << "\x1b[0;30;47m Space \x1b[1;37;44m Plot ";
	ct->locate(BOARD_X+1,17);
	*ct << "\x1b[0;30;46m  Tab  \x1b[1;37;44m Draw ";
	ct->locate(BOARD_X+1,19);
	*ct << "\x1b[0;30;47m P \x1b[1;37;44m Pattern ";
	ct->locate(BOARD_X+1,20);
	*ct << "\x1b[0;30;46m C \x1b[1;37;44m Color ";
	
	ct->setANSI(false);
}

void draw_item() {
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	ct->setANSI(true);
	
	ct->locate(BOARD_X+1,5);
	*ct << "\x1b[1;33;44m    Items: ";
	ct->locate(BOARD_X+1,6);
	*ct << "\x1b[0;30;47m Z \x1b[1;37;44m Player ";
	ct->locate(78,6);
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_PLAYER_SHAPE;
	ct->locate(BOARD_X+1,7);
	*ct << "\x1b[0;30;46m A \x1b[1;37;44m Ammo ";
	ct->locate(78,7);
	ct->color(CYAN,BLUE);
	*ct << (char)ZZT_AMMO_SHAPE;
	ct->locate(BOARD_X+1,8);
	*ct << "\x1b[0;30;47m T \x1b[1;37;44m Torch ";
	ct->locate(78,8);
	ct->color(YELLOW,BLUE);
	*ct << (char)ZZT_TORCH_SHAPE;
	ct->locate(BOARD_X+1,9);
	*ct << "\x1b[0;30;46m G \x1b[1;37;44m Gem ";
	ct->locate(78,9);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_GEM_SHAPE;
	ct->locate(BOARD_X+1,10);
	*ct << "\x1b[0;30;47m K \x1b[1;37;44m Key ";
	ct->locate(78,10);
	ct->color(GREEN|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_KEY_SHAPE;
	ct->locate(BOARD_X+1,11);
	*ct << "\x1b[0;30;46m D \x1b[1;37;44m Door ";
	ct->locate(78,11);
	ct->color(WHITE|HIGH_INTENSITY,GREEN);
	*ct << (char)ZZT_DOOR_SHAPE;
	ct->locate(BOARD_X+1,12);
	*ct << "\x1b[0;30;47m S \x1b[1;37;44m Scroll ";
	ct->locate(78,12);
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_SCROLL_SHAPE;
	ct->locate(BOARD_X+1,13);
	*ct << "\x1b[0;30;46m P \x1b[1;37;44m Passage ";
	ct->locate(78,13);
	ct->color(WHITE|HIGH_INTENSITY,RED);
	*ct << (char)ZZT_PASSAGE_SHAPE;
	ct->locate(BOARD_X+1,14);
	*ct << "\x1b[0;30;47m U \x1b[1;37;44m Duplicator ";
	ct->locate(78,14);
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_DUPLICATOR_SHAPE;
	ct->locate(BOARD_X+1,15);
	*ct << "\x1b[0;30;46m B \x1b[1;37;44m Bomb ";
	ct->locate(78,15);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_BOMB_SHAPE;
	ct->locate(BOARD_X+1,16);
	*ct << "\x1b[0;30;47m E \x1b[1;37;44m Energizer ";
	ct->locate(78,16);
	ct->color(MAGENTA,BLUE);
	*ct << (char)ZZT_ENERGIZER_SHAPE;
	ct->locate(BOARD_X+1,18);
	*ct << "\x1b[1;33;44m    Conveyers: ";
	ct->locate(BOARD_X+1,19);
	*ct << "\x1b[0;30;46m 1 \x1b[1;37;44m Clockwise ";
	ct->locate(BOARD_X+1,20);
	*ct << "\x1b[0;30;47m 2 \x1b[1;37;44m Counter ";
	ct->setANSI(false);
}

void draw_creature() {
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	ct->setANSI(true);
	
	ct->locate(BOARD_X+1,5);
	*ct << "\x1b[1;33;44m    Creatures: ";
	ct->locate(BOARD_X+1,6);
	*ct << "\x1b[0;30;47m B \x1b[1;37;44m Bear ";
	ct->locate(78,6);
	ct->color(YELLOW,BLUE);
	*ct << (char)ZZT_BEAR_SHAPE;
	ct->locate(BOARD_X+1,7);
	*ct << "\x1b[0;30;46m R \x1b[1;37;44m Ruffian ";
	ct->locate(78,7);
	ct->color(MAGENTA|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_RUFFIAN_SHAPE;
	ct->locate(BOARD_X+1,8);
	*ct << "\x1b[0;30;47m O \x1b[1;37;44m Object ";
	ct->locate(78,8);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_OBJECT_SHAPE;
	ct->locate(BOARD_X+1,9);
	*ct << "\x1b[0;30;46m V \x1b[1;37;44m Slime ";
	ct->locate(78,9);
	ct->color(BLUE|HIGH_INTENSITY,BLUE);
	*ct << '*';//(char)ZZT_GEM_SHAPE;
	ct->locate(BOARD_X+1,10);
	*ct << "\x1b[0;30;47m Y \x1b[1;37;44m Shark ";
	ct->locate(78,10);
	ct->color(WHITE,BLUE);
	*ct << (char)ZZT_SHARK_SHAPE;
	ct->locate(BOARD_X+1,11);
	*ct << "\x1b[0;30;46m G \x1b[1;37;44m Spinning gun ";
	ct->locate(78,11);
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_SPINNING_GUN_SHAPE;
	ct->locate(BOARD_X+1,12);
	*ct << "\x1b[0;30;47m P \x1b[1;37;44m Pusher ";
	ct->locate(78,12);
	ct->color(GREEN|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_PUSHER_SHAPE;
	
	ct->locate(BOARD_X+1,14);
	*ct << "\x1b[1;33;44m    Beasts: ";
	ct->locate(BOARD_X+1,15);
	*ct << "\x1b[0;30;46m L \x1b[1;37;44m Lion ";
	ct->locate(78,15);
	ct->color(RED|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_LION_SHAPE;
	ct->locate(BOARD_X+1,16);
	*ct << "\x1b[0;30;47m T \x1b[1;37;44m Tiger ";
	ct->locate(78,16);
	ct->color(CYAN|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_TIGER_SHAPE;
	ct->locate(BOARD_X+1,18);
	*ct << "\x1b[1;33;44m    Centipedes: ";
	ct->locate(BOARD_X+1,19);
	*ct << "\x1b[0;30;46m H \x1b[1;37;44m Head ";
	ct->locate(BOARD_X+1,20);
	*ct << "\x1b[0;30;47m S \x1b[1;37;44m Segment ";
	ct->setANSI(false);
}

void draw_terrain() {
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	ct->setANSI(true);
	
	ct->locate(BOARD_X+1,5);
	*ct << "\x1b[1;33;44m    Terrains: ";
	ct->locate(BOARD_X+1,6);
	*ct << "\x1b[0;30;47m W \x1b[1;37;44m Water ";
	ct->locate(78,6);
	ct->color(BLUE,WHITE);
	*ct << (char)ZZT_WATER_SHAPE;
	ct->locate(BOARD_X+1,7);
	*ct << "\x1b[0;30;46m F \x1b[1;37;44m Forest ";
	ct->locate(78,7);
	ct->color(BLUE,GREEN);
	*ct << (char)ZZT_FOREST_SHAPE;

	ct->locate(BOARD_X+1,9);
	*ct << "\x1b[1;33;44m    Walls: ";
	ct->locate(BOARD_X+1,10);
	*ct << "\x1b[0;30;47m S \x1b[1;37;44m Solid ";
	ct->locate(78,10);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_SOLID_SHAPE;
	ct->locate(BOARD_X+1,11);
	*ct << "\x1b[0;30;46m N \x1b[1;37;44m Normal ";
	ct->locate(78,11);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_NORMAL_SHAPE;
	ct->locate(BOARD_X+1,12);
	*ct << "\x1b[0;30;47m B \x1b[1;37;44m Breakable ";
	ct->locate(78,12);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_BREAKABLE_SHAPE;
	ct->locate(BOARD_X+1,13);
	*ct << "\x1b[0;30;46m O \x1b[1;37;44m Boulder ";
	ct->locate(78,13);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_BOULDER_SHAPE;
	ct->locate(BOARD_X+1,14);
	*ct << "\x1b[0;30;47m 1 \x1b[1;37;44m Slider (NS) ";
	ct->locate(78,14);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_SLIDER_NS_SHAPE;
	ct->locate(BOARD_X+1,15);
	*ct << "\x1b[0;30;46m 2 \x1b[1;37;44m Slider (EW) ";
	ct->locate(78,15);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_SLIDER_EW_SHAPE;
	ct->locate(BOARD_X+1,16);
	*ct << "\x1b[0;30;47m A \x1b[1;37;44m Fake ";
	ct->locate(78,16);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_FAKE_SHAPE;
	ct->locate(BOARD_X+1,17);
	*ct << "\x1b[0;30;46m I \x1b[1;37;44m Invisible ";
	ct->locate(78,17);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_INVISIBLE_SHAPE;
	ct->locate(BOARD_X+1,18);
	*ct << "\x1b[0;30;47m L \x1b[1;37;44m Blink wall ";
	ct->locate(78,18);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_BLINK_SHAPE;
	ct->locate(BOARD_X+1,19);
	*ct << "\x1b[0;30;46m T \x1b[1;37;44m Transporter ";
	ct->locate(78,19);
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << (char)ZZT_TRANSPORTER_SHAPE;
	ct->locate(BOARD_X+1,20);
	*ct << "\x1b[0;30;47m R \x1b[1;37;44m Ricochet ";
	ct->locate(78,20);
	ct->color(GREEN|HIGH_INTENSITY,BLUE);
	//*ct << (char)ZZT_RICOCHET_SHAPE;
	
	ct->setANSI(false);
}

void draw_color(int fg, int bg) {
	for(int i=0; i<8; i++) {
		ct->locate(BOARD_X+9+i,22);
		ct->color(i|HIGH_INTENSITY,i);
		*ct << '\xdf';
		ct->color(WHITE|HIGH_INTENSITY,BLUE);
		ct->locate(BOARD_X+9+i,21);
		*ct << ((i==fg-8)?'\x1f':' ');
		ct->locate(BOARD_X+9+i,23);
		*ct << ((i==bg)?'\x1e':' ');		
	}
}

void draw_pattern(ZZTObject *pattern[5], int pat) {
	Vector p;
	for(int i=0; i<5; i++) {
		p = pattern[i]->getPosition();
		pattern[i]->setPosition(Vector(BOARD_X+1+i, 22, 0));
		if(i<4) pattern[i]->setColor(15);
		pattern[i]->draw();
		pattern[i]->setPosition(p);
		ct->color(WHITE|HIGH_INTENSITY,BLUE);
		ct->locate(BOARD_X+1+i,23);
		*ct << ((i==pat)?'\x1e':' ');
	}
}

void edit_zzt() {
	EventCollector ec;
	Event evt;
	int edit_x=BOARD_X/2;
	int edit_y=BOARD_Y/2;
	int edit_fg=14;
	int edit_bg=0;
	int edit_pat=0;
	bool edit_loop=true;
	ZZTObject *pattern[5];
	ZZTObject *o=NULL;
	std::string s;
	int flash=0;
	enum { NONE, ITEM, CREATURE, TERRAIN, TEXT } edit_menu_mode = NONE;

	pattern[0] = create_object(ZZT_SOLID,0,0);
	pattern[1] = create_object(ZZT_NORMAL,0,0);
	pattern[2] = create_object(ZZT_EMPTY,0,0);
	pattern[3] = create_object(ZZT_LINE,0,0);
	pattern[4] = create_object(ZZT_EMPTY,0,0);
	
	world.editing=1;
	
	draw_main();
	draw_color(edit_fg,edit_bg);
	draw_pattern(pattern,edit_pat);
	
	do {
		draw_board();
		if(flash++ < 4) {
			ct->locate(edit_x,edit_y);
			ct->color(BLACK,WHITE);
			*ct << " ";
		} else if(flash>8) {
			flash=0;
		}
		render();
		Time::sleep(8000);

		while (ec.getEvent(evt)) {
			if(evt.type == Event::EvtQuit) {
				edit_loop = false;
			} else if(evt.type == Event::EvtKeypress) {
				flash=0;
				switch(evt.key) {
					case Event::KeyUp:
						edit_y--;
						if(edit_y<0) edit_y=0;
							break;
					case Event::KeyDown:
						edit_y++;
						if(edit_y>=BOARD_Y) edit_y=BOARD_Y-1;
							break;
					case Event::KeyLeft:
						edit_x--;
						if(edit_x<0) edit_x=0;
							break;
					case Event::KeyRight:
						edit_x++;
						if(edit_x>=BOARD_X) edit_x=BOARD_X-1;
							break;
				}
			} else if(evt.type == Event::EvtKeyUp) {
				switch(evt.key) {
					case Event::KeyEsc:
						if(edit_menu_mode==NONE) {
							edit_loop=false;
						} else {
							edit_menu_mode=NONE;
							draw_main();
						}
						break;
				}
			} else if(evt.type == Event::EvtKeyDown) {
				switch(evt.key) {		
					case Event::KeyF1:
						edit_menu_mode=ITEM;
						draw_item();
						break;
					case Event::KeyF2:
						edit_menu_mode=CREATURE;
						draw_creature();
						break;
					case Event::KeyF3:
						edit_menu_mode=TERRAIN;
						draw_terrain();
						break;
					case 32:
						if(edit_pat<4) pattern[edit_pat]->setColor(edit_fg, edit_bg);
						o=create_copy(pattern[edit_pat]);
						o->setPosition(Vector(edit_x,edit_y,0));
						put(o);
						break;
					case 13:
						if(currentbrd->board[edit_x][edit_y].obj->getFlags() & F_OBJECT) {
							ec.stop();
							currentbrd->board[edit_x][edit_y].obj->edit();
							ec.start();
						}
						pattern[4]=currentbrd->board[edit_x][edit_y].obj;
						break;
				}
				
				if(edit_menu_mode==NONE) {
					switch(evt.key) {					
						case 'q':
							edit_loop = false;
							break;
						case 'l':
						s = os_select_file("Select a game","zzt");
						if(s!="") {
							free_world();
							edit_menu_mode=NONE;
							ct->color(15,1);
							ct->clear();
							dzzt_logo();
							load_zzt(s.c_str(),0);
							draw_main();
							switch_board(world.start);
						}
						break;
					case 'i':
					{
						TUIWindow t("Board Info");
						TUIRadioGroup *rg;
						
						rg=new TUIRadioGroup            ("          Board is dark: ",&currentbrd->dark);
						rg->add("No");
						rg->add("Yes");
						t.addWidget(rg);

						rg=new TUIRadioGroup            ("   Re-Enter When Zapped: ",&currentbrd->reenter);
						rg->add("No");
						rg->add("Yes");
						t.addWidget(rg);

						t.addWidget(new TUINumericInput("             Re-Enter X: ",&currentbrd->reenter_x,0,254));
						t.addWidget(new TUINumericInput("             Re-Enter Y: ",&currentbrd->reenter_y,0,254));
						t.addWidget(new TUINumericInput("             Time Limit: ",&currentbrd->time,0,32767,10));						
						t.addWidget(new TUINumericInput("          Maximum Shots: ",&currentbrd->maxshots,0,254));
						t.addWidget(new TUIWidget());
						t.addWidget(new TUILabel       ("              Adjacent Boards",true));
						t.addWidget(new TUIBoardList   (" North: ", &currentbrd->board_up));
						t.addWidget(new TUIBoardList   (" South: ", &currentbrd->board_down));
						t.addWidget(new TUIBoardList   ("  East: ", &currentbrd->board_right));
						t.addWidget(new TUIBoardList   ("  West: ", &currentbrd->board_left));
						ec.stop();
						t.doMenu(ct);
						ec.start();
					}
						break;
					case 'c':
						if(evt.mod & Event::KeyShift) {
							edit_bg++;
							if(edit_bg>7) edit_bg=0;
						} else {
							edit_fg++;
							if(edit_fg>15) edit_fg=8;
						}
						break;
					case 'p':
						edit_pat++;
						if(edit_pat>4) edit_pat=0;
						break;
					}
				}

				if(edit_menu_mode==ITEM) {
					switch(evt.key) {
						case 'z':
							o=pattern[4]=create_object(ZZT_PLAYER,edit_x,edit_y);
							put(o);
							break;
						case 'a':
							o=pattern[4]=create_object(ZZT_AMMO,edit_x,edit_y);
							put(o);
							break;
						case 't':
							o=pattern[4]=create_object(ZZT_TORCH,edit_x,edit_y);
							put(o);
							break;
						case 'g':
							o=pattern[4]=create_object(ZZT_GEM,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'k':
							o=pattern[4]=create_object(ZZT_KEY,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'd':
							o=pattern[4]=create_object(ZZT_DOOR,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 's':
							o=pattern[4]=create_object(ZZT_SCROLL,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'p':
							o=pattern[4]=create_object(ZZT_PASSAGE,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'u':
							o=pattern[4]=create_object(ZZT_DUPLICATOR,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'b':
							o=pattern[4]=create_object(ZZT_BOMB,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'e':
							o=pattern[4]=create_object(ZZT_ENERGIZER,edit_x,edit_y);
							put(o);
							break;
					}
				}
				
				if(edit_menu_mode==CREATURE) {
					switch(evt.key) {
						case 'b':
							o=pattern[4]=create_object(ZZT_BEAR,edit_x,edit_y);
							put(o);
							break;
						case 'r':
							o=pattern[4]=create_object(ZZT_RUFFIAN,edit_x,edit_y);
							put(o);
							break;
						case 'o':
							o=pattern[4]=create_object(ZZT_OBJECT,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'v':
							//o=pattern[4]=create_object(ZZT_SLIME,edit_x,edit_y);
							//o->setColor(edit_fg, edit_bg);
							//put(o);
							break;
						case 'y':
							o=pattern[4]=create_object(ZZT_SHARK,edit_x,edit_y);
							put(o);
							break;
						case 'g':
							o=pattern[4]=create_object(ZZT_SPINNING_GUN,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'p':
							o=pattern[4]=create_object(ZZT_PUSHER,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'l':
							o=pattern[4]=create_object(ZZT_LION,edit_x,edit_y);
							put(o);
							break;
						case 't':
							o=pattern[4]=create_object(ZZT_TIGER,edit_x,edit_y);
							put(o);
							break;
						case 'h':
							o=pattern[4]=create_object(ZZT_CENTIPEDE_HEAD,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 's':
							o=pattern[4]=create_object(ZZT_CENTIPEDE_BODY,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;					
					}
				}
				
				if(edit_menu_mode==TERRAIN) {
					switch(evt.key) {
						case 'w':
							o=pattern[4]=create_object(ZZT_WATER,edit_x,edit_y);
							put(o);
							break;
						case 'f':
							o=pattern[4]=create_object(ZZT_FOREST,edit_x,edit_y);
							put(o);
							break;
						case 's':
							o=pattern[4]=create_object(ZZT_SOLID,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'n':
							o=pattern[4]=create_object(ZZT_NORMAL,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'b':
							o=pattern[4]=create_object(ZZT_BREAKABLE,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'o':
							o=pattern[4]=create_object(ZZT_BOULDER,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case '1':
							o=pattern[4]=create_object(ZZT_SLIDER_NS,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case '2':
							o=pattern[4]=create_object(ZZT_SLIDER_EW,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'a':
							o=pattern[4]=create_object(ZZT_FAKE,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'i':
							o=pattern[4]=create_object(ZZT_INVISIBLE,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'l':
							o=pattern[4]=create_object(ZZT_BLINK,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 't':
							o=pattern[4]=create_object(ZZT_TRANSPORTER,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							put(o);
							break;
						case 'r':
							//o=pattern[4]=create_object(ZZT_RICOCHET,edit_x,edit_y);
							put(o);
							break;
					}
				}
				
				draw_color(edit_fg,edit_bg);
				draw_pattern(pattern,edit_pat);
			}			
		}
	} while(edit_loop);
	world.editing=0;
}
