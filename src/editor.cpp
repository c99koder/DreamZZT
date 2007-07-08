/* editor.cpp - DreamZZT Editor
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

#include "console.h"

#include "editor.h"
#include "object.h"
#include "board.h"
#include "status.h"
#include "os.h"

#if TIKI_PLAT == TIKI_NDS
std::string os_select_file(std::string title, std::string filter);
std::string os_save_file(std::string title, std::string filename, std::string filter);
#endif

extern ConsoleText *ct;
extern ConsoleText *st;

extern struct world_header world;
extern struct board_info_node *board_list;
extern struct board_info_node *currentbrd;

void draw_main() {
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	st->setANSI(true);
	
	st->locate(1,5);
	*st << "\x1b[0;30;47m L \x1b[1;37;44m Load ";
	*st << "\x1b[0;30;47m H \x1b[1;37;44m Help ";
	st->locate(1,6);
	*st << "\x1b[0;30;46m S \x1b[1;37;44m Save ";
	*st << "\x1b[0;30;46m Q \x1b[1;37;44m Quit ";
	st->locate(1,8);
	*st << "\x1b[0;30;47m B \x1b[1;37;44m Switch Boards ";
	st->locate(1,9);
	*st << "\x1b[0;30;46m I \x1b[1;37;44m Board Info ";
	st->locate(1,11);
	*st << "\x1b[0;30;47m  F1  \x1b[1;37;44m Item ";
	st->locate(1,12);
	*st << "\x1b[0;30;46m  F2  \x1b[1;37;44m Creature ";
	st->locate(1,13);
	*st << "\x1b[0;30;47m  F3  \x1b[1;37;44m Terrain ";
	st->locate(1,14);
	//*st << "\x1b[0;30;46m  F4  \x1b[1;37;44m Text ";
	st->locate(1,16);
	*st << "\x1b[0;30;47m Space \x1b[1;37;44m Plot ";
	st->locate(1,17);
	//*st << "\x1b[0;30;46m  Tab  \x1b[1;37;44m Draw ";
	st->locate(1,19);
	*st << "\x1b[0;30;47m P \x1b[1;37;44m Pattern ";
	st->locate(1,20);
	*st << "\x1b[0;30;46m C \x1b[1;37;44m Color ";
	
	st->setANSI(false);
}

void draw_item() {
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	st->setANSI(true);
	
	st->locate(1,5);
	*st << "\x1b[1;33;44m    Items: ";
	st->locate(1,6);
	*st << "\x1b[0;30;47m Z \x1b[1;37;44m Player ";
	st->locate(18,6);
	st->color(WHITE|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_PLAYER_SHAPE;
	st->locate(1,7);
	*st << "\x1b[0;30;46m A \x1b[1;37;44m Ammo ";
	st->locate(18,7);
	st->color(CYAN,BLUE);
	*st << (char)ZZT_AMMO_SHAPE;
	st->locate(1,8);
	*st << "\x1b[0;30;47m T \x1b[1;37;44m Torch ";
	st->locate(18,8);
	st->color(YELLOW,BLUE);
	*st << (char)ZZT_TORCH_SHAPE;
	st->locate(1,9);
	*st << "\x1b[0;30;46m G \x1b[1;37;44m Gem ";
	st->locate(18,9);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_GEM_SHAPE;
	st->locate(1,10);
	*st << "\x1b[0;30;47m K \x1b[1;37;44m Key ";
	st->locate(18,10);
	st->color(GREEN|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_KEY_SHAPE;
	st->locate(1,11);
	*st << "\x1b[0;30;46m D \x1b[1;37;44m Door ";
	st->locate(18,11);
	st->color(WHITE|HIGH_INTENSITY,GREEN);
	*st << (char)ZZT_DOOR_SHAPE;
	st->locate(1,12);
	*st << "\x1b[0;30;47m S \x1b[1;37;44m Scroll ";
	st->locate(18,12);
	st->color(WHITE|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_SCROLL_SHAPE;
	st->locate(1,13);
	*st << "\x1b[0;30;46m P \x1b[1;37;44m Passage ";
	st->locate(18,13);
	st->color(WHITE|HIGH_INTENSITY,RED);
	*st << (char)ZZT_PASSAGE_SHAPE;
	st->locate(1,14);
	*st << "\x1b[0;30;47m U \x1b[1;37;44m Duplicator ";
	st->locate(18,14);
	st->color(WHITE|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_DUPLICATOR_SHAPE;
	st->locate(1,15);
	*st << "\x1b[0;30;46m B \x1b[1;37;44m Bomb ";
	st->locate(18,15);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_BOMB_SHAPE;
	st->locate(1,16);
	*st << "\x1b[0;30;47m E \x1b[1;37;44m Energizer ";
	st->locate(18,16);
	st->color(MAGENTA,BLUE);
	*st << (char)ZZT_ENERGIZER_SHAPE;
	st->locate(1,18);
	*st << "\x1b[1;33;44m    Conveyers: ";
	st->locate(1,19);
	*st << "\x1b[0;30;46m 1 \x1b[1;37;44m Clockwise ";
	st->locate(1,20);
	*st << "\x1b[0;30;47m 2 \x1b[1;37;44m Counter ";
	st->setANSI(false);
}

void draw_creature() {
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	st->setANSI(true);
	
	st->locate(1,5);
	*st << "\x1b[1;33;44m    Creatures: ";
	st->locate(1,6);
	*st << "\x1b[0;30;47m B \x1b[1;37;44m Bear ";
	st->locate(18,6);
	st->color(YELLOW,BLUE);
	*st << (char)ZZT_BEAR_SHAPE;
	st->locate(1,7);
	*st << "\x1b[0;30;46m R \x1b[1;37;44m Ruffian ";
	st->locate(18,7);
	st->color(MAGENTA|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_RUFFIAN_SHAPE;
	st->locate(1,8);
	*st << "\x1b[0;30;47m O \x1b[1;37;44m Object ";
	st->locate(18,8);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_OBJECT_SHAPE;
	st->locate(1,9);
	*st << "\x1b[0;30;46m V \x1b[1;37;44m Slime ";
	st->locate(18,9);
	st->color(BLUE|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_SLIME_SHAPE;
	st->locate(1,10);
	*st << "\x1b[0;30;47m Y \x1b[1;37;44m Shark ";
	st->locate(18,10);
	st->color(WHITE,BLUE);
	*st << (char)ZZT_SHARK_SHAPE;
	st->locate(1,11);
	*st << "\x1b[0;30;46m G \x1b[1;37;44m Spinning gun ";
	st->locate(18,11);
	st->color(WHITE|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_SPINNING_GUN_SHAPE;
	st->locate(1,12);
	*st << "\x1b[0;30;47m P \x1b[1;37;44m Pusher ";
	st->locate(18,12);
	st->color(GREEN|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_PUSHER_SHAPE;
	
	st->locate(1,14);
	*st << "\x1b[1;33;44m    Beasts: ";
	st->locate(1,15);
	*st << "\x1b[0;30;46m L \x1b[1;37;44m Lion ";
	st->locate(18,15);
	st->color(RED|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_LION_SHAPE;
	st->locate(1,16);
	*st << "\x1b[0;30;47m T \x1b[1;37;44m Tiger ";
	st->locate(18,16);
	st->color(CYAN|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_TIGER_SHAPE;
	st->locate(1,18);
	*st << "\x1b[1;33;44m    Centipedes: ";
	st->locate(1,19);
	*st << "\x1b[0;30;46m H \x1b[1;37;44m Head ";
	st->locate(1,20);
	*st << "\x1b[0;30;47m S \x1b[1;37;44m Segment ";
	st->setANSI(false);
}

void draw_terrain() {
	ct->color(15,1);
	ct->clear();
	dzzt_logo();
	st->setANSI(true);
	
	st->locate(1,5);
	*st << "\x1b[1;33;44m    Terrains: ";
	st->locate(1,6);
	*st << "\x1b[0;30;47m W \x1b[1;37;44m Water ";
	st->locate(18,6);
	st->color(BLUE,WHITE);
	*st << (char)ZZT_WATER_SHAPE;
	st->locate(1,7);
	*st << "\x1b[0;30;46m F \x1b[1;37;44m Forest ";
	st->locate(18,7);
	st->color(BLUE,GREEN);
	*st << (char)ZZT_FOREST_SHAPE;

	st->locate(1,9);
	*st << "\x1b[1;33;44m    Walls: ";
	st->locate(1,10);
	*st << "\x1b[0;30;47m S \x1b[1;37;44m Solid ";
	st->locate(18,10);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_SOLID_SHAPE;
	st->locate(1,11);
	*st << "\x1b[0;30;46m N \x1b[1;37;44m Normal ";
	st->locate(18,11);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_NORMAL_SHAPE;
	st->locate(1,12);
	*st << "\x1b[0;30;47m B \x1b[1;37;44m Breakable ";
	st->locate(18,12);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_BREAKABLE_SHAPE;
	st->locate(1,13);
	*st << "\x1b[0;30;46m O \x1b[1;37;44m Boulder ";
	st->locate(18,13);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_BOULDER_SHAPE;
	st->locate(1,14);
	*st << "\x1b[0;30;47m 1 \x1b[1;37;44m Slider (NS) ";
	st->locate(18,14);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_SLIDER_NS_SHAPE;
	st->locate(1,15);
	*st << "\x1b[0;30;46m 2 \x1b[1;37;44m Slider (EW) ";
	st->locate(18,15);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_SLIDER_EW_SHAPE;
	st->locate(1,16);
	*st << "\x1b[0;30;47m A \x1b[1;37;44m Fake ";
	st->locate(18,16);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_FAKE_SHAPE;
	st->locate(1,17);
	*st << "\x1b[0;30;46m I \x1b[1;37;44m Invisible ";
	st->locate(18,17);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_INVISIBLE_SHAPE;
	st->locate(1,18);
	*st << "\x1b[0;30;47m L \x1b[1;37;44m Blink wall ";
	st->locate(18,18);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_BLINK_SHAPE;
	st->locate(1,19);
	*st << "\x1b[0;30;46m T \x1b[1;37;44m Transporter ";
	st->locate(18,19);
	st->color(YELLOW|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_TRANSPORTER_SHAPE;
	st->locate(1,20);
	*st << "\x1b[0;30;47m R \x1b[1;37;44m Ricochet ";
	st->locate(18,20);
	st->color(GREEN|HIGH_INTENSITY,BLUE);
	*st << (char)ZZT_RICOCHET_SHAPE;
	
	st->setANSI(false);
}

void draw_color(int fg, int bg) {
	for(int i=0; i<8; i++) {
		st->locate(9+i,22);
		st->color(i|HIGH_INTENSITY,i);
		*st << '\xdf';
		st->color(WHITE|HIGH_INTENSITY,BLUE);
		st->locate(9+i,21);
		*st << ((i==fg-8)?'\x1f':' ');
		st->locate(9+i,23);
		*st << ((i==bg)?'\x1e':' ');		
	}
}

void draw_pattern(ZZTObject *pattern[5], int pat) {
	Vector p;
	for(int i=0; i<5; i++) {
		p = pattern[i]->position();
		pattern[i]->setPosition(Vector( (float)(BOARD_X+1+i), 22.0f, 0.0f));
		if(i<4) pattern[i]->setColor(15);
		pattern[i]->draw();
		pattern[i]->setPosition(p);
		st->color(WHITE|HIGH_INTENSITY,BLUE);
		st->locate(1+i,23);
		*st << ((i==pat)?'\x1e':' ');
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
	
	currentbrd->board[edit_x][edit_y].obj->setHighlighted(true);
	
	do {
		draw_board();
		render();
		Time::sleep(8000);

		while (ec.getEvent(evt)) {
			if(evt.type == Event::EvtQuit) {
				edit_loop = false;
			} else if(evt.type == Event::EvtKeypress) {
				switch(evt.key) {
					case Event::KeyUp:
						currentbrd->board[edit_x][edit_y].obj->setHighlighted(false);
						edit_y--;
						if(edit_y<0) edit_y=0;
						currentbrd->board[edit_x][edit_y].obj->setHighlighted(true);
						break;
					case Event::KeyDown:
						currentbrd->board[edit_x][edit_y].obj->setHighlighted(false);
						edit_y++;
						if(edit_y>=BOARD_Y) edit_y=BOARD_Y-1;
						currentbrd->board[edit_x][edit_y].obj->setHighlighted(true);
						break;
					case Event::KeyLeft:
						currentbrd->board[edit_x][edit_y].obj->setHighlighted(false);
						edit_x--;
						if(edit_x<0) edit_x=0;
						currentbrd->board[edit_x][edit_y].obj->setHighlighted(true);
						break;
					case Event::KeyRight:
						currentbrd->board[edit_x][edit_y].obj->setHighlighted(false);
						edit_x++;
						if(edit_x>=BOARD_X) edit_x=BOARD_X-1;
						currentbrd->board[edit_x][edit_y].obj->setHighlighted(true);
						break;
					case 'u':
						if(world.use_3d) {
							if(!(o = currentbrd->board[edit_x][edit_y].under)) {
								o = currentbrd->board[edit_x][edit_y].obj;
							}
							o->setHeight((unsigned char)(((int)(o->height()) + 1) % 256));
						}
						break;
					case 'y':
						if(world.use_3d) {
							if(!(o = currentbrd->board[edit_x][edit_y].under)) {
								o = currentbrd->board[edit_x][edit_y].obj;
							}
							o->setHeight((unsigned char)(((int)o->height() - 1) % 256));
						}
						break;
				}
			} else if(evt.type == Event::EvtKeyUp) {
				switch(evt.key) {
					case Event::KeyEsc:
						edit_menu_mode=NONE;
						draw_main();
						draw_color(edit_fg,edit_bg);
						draw_pattern(pattern,edit_pat);							
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
						o->setPosition(Vector((float)edit_x, (float)edit_y, 0.0f));
						put(o);
						break;
					case 13:
						if(currentbrd->board[edit_x][edit_y].obj->flags() & F_OBJECT) {
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
						case 's':
							s = os_save_file("Select a game",world.title + ".zzt","zzt");
							if(s!="") {
								save_game(s.c_str());
								draw_main();
							}
							break;
						case 'b':
						{
							struct board_info_node *current=board_list;
							std::string boardmenu = "!create;(Create new board)\r";
							char tmp[100];
							int x=0;
							
							while(current!=NULL) {
								sprintf(tmp,"!%i;%s\r",x,current->title);
								boardmenu += tmp;
								x++;
								current=current->next;
							}

							TUIWindow t("Select a board");
							t.buildFromString(boardmenu);
							t.doMenu();
							
							if(t.getLabel() != "") {
								if(t.getLabel() == "create") {
									switch_board(new_board("untitled"));
								} else {
									switch_board(atoi(t.getLabel().c_str()));
								}
							}
						}
						break;
						case 'w':
						{
							TUIWindow t("World Info");

							t.addWidget(new TUITextInput   ("         World Name: ", &world.title));
							t.addWidget(new TUINumericInput("               Ammo: ", &world.ammo, 0, 65535));
							t.addWidget(new TUINumericInput("               Gems: ", &world.gems, 0, 65535));
							t.addWidget(new TUINumericInput("             Health: ", &world.health, 0, 65535));
							t.addWidget(new TUINumericInput("            Torches: ", &world.torches, 0, 65535));
							t.addWidget(new TUINumericInput("              Score: ", &world.score, 0, 65535));
							t.addWidget(new TUILabel(""));
							t.addWidget(new TUICheckBox    ("Saved Game", &world.saved));
#ifndef DZZT_LITE
							t.addWidget(new TUICheckBox    ("Use 3D Models", &world.use_3d));
#endif
							t.addWidget(new TUILabel(""));
							t.addWidget(new TUILabel       ("                 Flags",true));
							t.addWidget(new TUITextInput   ("   1: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("   2: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("   3: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("   4: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("   5: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("   6: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("   7: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("   8: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("   9: ", &world.flags[0]));
							t.addWidget(new TUITextInput   ("  10: ", &world.flags[0]));
							
							ec.stop();
							t.doMenu();
							ec.start();
						}
						break;
						case 'i':
						{
							TUIWindow t("Board Info");
							std::string title = currentbrd->title;
							
							t.addWidget(new TUITextInput(	 "", &title, true));
							TUIRadioGroup *rg;
							
							rg=new TUIRadioGroup						("          Board is dark: ",&currentbrd->dark);
							rg->add("No");
							rg->add("Yes");
							t.addWidget(rg);

							rg=new TUIRadioGroup						("   Re-Enter When Zapped: ",&currentbrd->reenter);
							rg->add("No");
							rg->add("Yes");
							t.addWidget(rg);

							t.addWidget(new TUINumericInput("             Re-Enter X: ",&currentbrd->reenter_x,0,254));
							t.addWidget(new TUINumericInput("             Re-Enter Y: ",&currentbrd->reenter_y,0,254));
							t.addWidget(new TUINumericInput("             Time Limit: ",&currentbrd->time,0,32767,10));
							t.addWidget(new TUINumericInput("          Maximum Shots: ",&currentbrd->maxshots,0,254));

							rg=new TUIRadioGroup           ("         Animated Water: ",&currentbrd->animatedWater);
							rg->add("Disabled");
							rg->add("Enabled");
							t.addWidget(rg);

							t.addWidget(new TUIWidget());
							t.addWidget(new TUILabel			 ("              Adjacent Boards",true));
							t.addWidget(new TUIBoardList	 (" North: ", &currentbrd->board_up));
							t.addWidget(new TUIBoardList	 (" South: ", &currentbrd->board_down));
							t.addWidget(new TUIBoardList	 ("  East: ", &currentbrd->board_right));
							t.addWidget(new TUIBoardList	 ("  West: ", &currentbrd->board_left));
							ec.stop();
							t.doMenu();
							strcpy(currentbrd->title, title.c_str());
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
							o->setCycle(1);
							put(o);
							break;
						case 'a':
							o=pattern[4]=create_object(ZZT_AMMO,edit_x,edit_y);
							o->setColor(CYAN,BLACK);
							put(o);
							break;
						case 't':
							o=pattern[4]=create_object(ZZT_TORCH,edit_x,edit_y);
							o->setColor(YELLOW,BLACK);
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
							o->setCycle(3);
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
							o->setCycle(3);
							put(o);
							break;
						case 'b':
							o=pattern[4]=create_object(ZZT_BOMB,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(3);
							put(o);
							break;
						case 'e':
							o=pattern[4]=create_object(ZZT_ENERGIZER,edit_x,edit_y);
							o->setColor(MAGENTA,BLACK);
							put(o);
							break;
					}
				}
				
				if(edit_menu_mode==CREATURE) {
					switch(evt.key) {
						case 'b':
							o=pattern[4]=create_object(ZZT_BEAR,edit_x,edit_y);
							o->setCycle(3);
							put(o);
							break;
						case 'r':
							o=pattern[4]=create_object(ZZT_RUFFIAN,edit_x,edit_y);
							o->setCycle(3);
							put(o);
							break;
						case 'o':
							o=pattern[4]=create_object(ZZT_OBJECT,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(3);
							put(o);
							break;
						case 'v':
							o=pattern[4]=create_object(ZZT_SLIME,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(3);
							put(o);
							break;
						case 'y':
							o=pattern[4]=create_object(ZZT_SHARK,edit_x,edit_y);
							o->setCycle(3);
							put(o);
							break;
						case 'g':
							o=pattern[4]=create_object(ZZT_SPINNING_GUN,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(3);
							put(o);
							break;
						case 'p':
							o=pattern[4]=create_object(ZZT_PUSHER,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(3);
							put(o);
							break;
						case 'l':
							o=pattern[4]=create_object(ZZT_LION,edit_x,edit_y);
							o->setCycle(3);
							put(o);
							break;
						case 't':
							o=pattern[4]=create_object(ZZT_TIGER,edit_x,edit_y);
							o->setCycle(3);
							put(o);
							break;
						case 'h':
							o=pattern[4]=create_object(ZZT_CENTIPEDE_HEAD,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(3);
							put(o);
							break;
						case 's':
							o=pattern[4]=create_object(ZZT_CENTIPEDE_BODY,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(3);
							put(o);
							break;					
					}
				}
				
				if(edit_menu_mode==TERRAIN) {
					switch(evt.key) {
						case 'w':
							o=pattern[4]=create_object(ZZT_WATER,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(1);
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
							o->setCycle(3);
							put(o);
							break;
						case 't':
							o=pattern[4]=create_object(ZZT_TRANSPORTER,edit_x,edit_y);
							o->setColor(edit_fg, edit_bg);
							o->setCycle(3);
							put(o);
							break;
						case 'r':
							o=pattern[4]=create_object(ZZT_RICOCHET,edit_x,edit_y);
							o->setColor(GREEN|HIGH_INTENSITY, BLACK);
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
	currentbrd->board[edit_x][edit_y].obj->setHighlighted(false);
}
