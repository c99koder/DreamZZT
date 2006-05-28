/* debug.cpp - Debug console
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

#include <stdarg.h>
#include <string.h>
#include <Tiki/tiki.h>
#include <Tiki/hid.h>
#include <Tiki/thread.h>
#include <Tiki/tikitime.h>
#include <Tiki/drawables/console.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Thread;

#include "debug.h"
#include "word.h"
#include "board.h"
#include "status.h"
#include "sound.h"

extern struct board_info_node *currentbrd;
extern struct board_info_node *board_list;
extern struct world_header world;
extern int switchbrd;
extern ZZTMusicStream *zm;

ConsoleText *dt=NULL;
extern ConsoleText *ct;

int debug_hidCookie=-1;
extern Player *player;
extern bool playerInputActive;
extern bool gameFrozen;
int debug_visible = 0;
std::string debug_cmdline;

Tiki::Thread::Thread *debug_thread;

void debug_hidCallback(const Event & evt, void * data);

void *process_debug(void *) {
	while(1) {
		debug_cmdline = "";
		debug("");
		
		debug_hidCookie = Hid::callbackReg(debug_hidCallback, NULL);
		
		while(debug_cmdline.length() == 0 || (debug_cmdline[debug_cmdline.length() - 1] != '\r')) {
			Time::sleep(10000);
		}; //Wait for enter

		Hid::callbackUnreg(debug_hidCookie);
		
		debug_cmdline.resize(debug_cmdline.length() - 1);

		if(player==NULL) continue;
		
		*dt << ">>> " << debug_cmdline.c_str() << endl;
		if(debug_cmdline == "+dark") {
			currentbrd->dark = 1;
			debug("Board is now dark.\n");
		} else if(debug_cmdline == "-dark") {
			currentbrd->dark = 0;
			debug("Board is no longer dark.\n");
		} else if(debug_cmdline == "+ammo") {
			give_ammo(10);
			debug("Increased ammo.\n");
		} else if(debug_cmdline == "-ammo") {
			take_ammo(10);
			debug("Decreased ammo.\n");
		} else if(debug_cmdline == "+gems") {
			give_gems(10);
			debug("Increased gems.\n");
		} else if(debug_cmdline == "-gems") {
			take_gems(10);
			debug("Decreased gems.\n");
		} else if(debug_cmdline == "+score") {
			give_score(10);
			debug("Increased score.\n");
		} else if(debug_cmdline == "-score") {
			take_score(10);
			debug("Decreased score.\n");
		} else if(debug_cmdline == "+health") {
			give_health(10);
			debug("Increased health.\n");
		} else if(debug_cmdline == "-health") {
			take_health(10);
			debug("Decreased health.\n");
		} else if(debug_cmdline == "+torch") {
			give_torch(10);
			debug("Increased torches.\n");
		} else if(debug_cmdline == "-torch") {
			take_torch(10);
			debug("Decreased torches\n");
		} else if(debug_cmdline == "zap") {
			if(player->getPosition().x - 1 >= 0) remove_from_board(currentbrd,currentbrd->board[(int)player->getPosition().x-1][(int)player->getPosition().y].obj);
			if(player->getPosition().y - 1 >= 0) remove_from_board(currentbrd,currentbrd->board[(int)player->getPosition().x][(int)player->getPosition().y-1].obj);
			if(player->getPosition().x + 1 < BOARD_X) remove_from_board(currentbrd,currentbrd->board[(int)player->getPosition().x+1][(int)player->getPosition().y].obj);
			if(player->getPosition().y + 1 < BOARD_Y) remove_from_board(currentbrd,currentbrd->board[(int)player->getPosition().x][(int)player->getPosition().y+1].obj);
			debug("Cleared area around player.\n");
		} else if(debug_cmdline.find("keys") == 0) {
			for(int i=0; i<7; i++) {
				world.keys[i] = 1;
			}
			draw_keys();
			debug("Player now has all keys.\n");
		} else if(debug_cmdline.find("warp ") == 0) {
			switchbrd = atoi(debug_cmdline.c_str() + 5);
			player->setFlag(F_SLEEPING);
		} else if(debug_cmdline == "freeze") {
			gameFrozen = true;
			debug("Game has been frozen\n");
		} else if(debug_cmdline == "unfreeze") {
			gameFrozen = false;
			debug("Game has been thawed\n");
		} else if(debug_cmdline == "warp") {
			int x=0;
			char tmp[100];
			struct board_info_node *current=board_list;
			std::string boardmenu = "";
			while(current!=NULL) {
				sprintf(tmp,"!%i;%s\r",x,current->title);
				boardmenu += tmp;
				x++;
				current=current->next;
			}
			dt->setANSI(false);
			TextWindow t(dt,"Select a board",(char *)boardmenu.c_str());
			t.doMenu();
			dt->setANSI(true);
			dt->color(GREY, BLACK);
			dt->clear();
			if(atoi(t.getLabel()) > 0) {
				debug("Warping to board %i\n",atoi(t.getLabel()));
				switchbrd = atoi(t.getLabel());
				player->setFlag(F_SLEEPING);
			} else {
				debug("");
			}
		} else if(debug_cmdline == "quit") {
			switchbrd = -2;
		} else {
			player->exec(debug_cmdline);
		}
		
		zm->setTune("i-g");
		zm->start();
	}
}

void debug_hidCallback(const Event & evt, void * data) {
	ct->color(WHITE|HIGH_INTENSITY, BLUE);
	ct->locate(65,23);

	if (evt.type == Hid::Event::EvtKeyDown) {
		if(evt.key == '`') {
			if(debug_visible==0) {
				debug_visible = 1;
				ct->setSize(640,240);
				dt->setTranslate(Vector(320,120,0));
				ct->setTranslate(Vector(320,360,0));
				debug_cmdline = "";
			} else {
				debug_visible = 0;
				ct->setSize(640,480);
				ct->setTranslate(Vector(320,240,0));
				dt->setTranslate(Vector(1024,360,0));
				playerInputActive = true;
				ct->setANSI(true);
				*ct << "\x1b[k"; // clear EOL
				ct->setANSI(false);
			}
		} else if (evt.key >= 32 && evt.key <= 128 && debug_visible) {
			debug_cmdline += evt.key;
			playerInputActive = false;
			*dt << "\x1b[s"; // Save cursor position
			dt->locate(0,24);
			dt->color(GREY | HIGH_INTENSITY, BLACK);
			*dt << "> \x1b[1;32m" << debug_cmdline.c_str() << "\x1b[k"; //clear EOL
			*dt << "\x1b[u"; // Restore cursor position
		} else if (evt.key == 13 && debug_visible) {
			playerInputActive = true;
			debug_cmdline += '\r';
		}
	}
	
	if(debug_visible) {
		*ct << "X: " << (int)player->getPosition().x << " Y: " << (int)player->getPosition().y << "    ";
	}
}

void debug_init() {
	dt = new ConsoleText(80,25,new Texture("zzt-ascii.png", true));
	dt->setSize(640,240);
	dt->setTranslate(Vector(1024,360,0));
	dt->setANSI(true);
	dt->color(GREY, BLACK);
	dt->clear();
	debug("\n\nDreamZZT 3.0\n(C) 2006 Sam Steele\n\nREADY.\n");	
	
	debug_thread = new Tiki::Thread::Thread(process_debug,NULL);
}

void debug(const char *fmt, ...) {
	char txt[1024];
	va_list args;
	
	va_start(args,fmt);
	vsprintf(txt,fmt,args);
	va_end(args);
	
	dt->color(YELLOW | HIGH_INTENSITY, BLACK);
	*dt << txt;
	*dt << "\x1b[s"; // Save cursor position
	dt->color(WHITE | HIGH_INTENSITY, BLUE);
	dt->locate(0,0);
	*dt << "                            DreamZZT Debug Console                              ";
	dt->locate(0,24);
	dt->locate(0,24);
	dt->color(GREY | HIGH_INTENSITY, BLACK);
	*dt << "> \x1b[1;32m" << debug_cmdline.c_str() << "\x1b[k"; //clear EOL
	*dt << "\x1b[u"; // Restore cursor position
}