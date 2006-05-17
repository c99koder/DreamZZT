/*
 *  debug.cpp
 *  DreamZZT
 *
 *  Created by Sam Steele on 5/16/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdarg.h>
#include <string.h>
#include <Tiki/tiki.h>
#include <Tiki/hid.h>

#include <Tiki/drawables/console.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;

#include "debug.h"
#include "word.h"
#include "board.h"
#include "status.h"

extern struct board_info_node *currentbrd;
extern struct board_info_node *board_list;
extern struct world_header world;
extern int switchbrd;

ConsoleText *dt=NULL;
extern ConsoleText *ct;

int debug_hidCookie=-1;
extern struct object *player;
int debug_visible = 0;

std::string debug_cmdline;

void process_debug() {
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
		if(player->x - 1 >= 0) remove_from_board(currentbrd,currentbrd->board[player->x-1][player->y].obj);
		if(player->y - 1 >= 0) remove_from_board(currentbrd,currentbrd->board[player->x][player->y-1].obj);
		if(player->x + 1 < BOARD_X) remove_from_board(currentbrd,currentbrd->board[player->x+1][player->y].obj);
		if(player->y + 1 < BOARD_Y) remove_from_board(currentbrd,currentbrd->board[player->x][player->y+1].obj);
		debug("Cleared area around player.\n");
	} else if(debug_cmdline.find("warp ") == 0) {
		switchbrd = atoi(debug_cmdline.c_str() + 5);
		player->flags |= F_SLEEPING;
	} else if(debug_cmdline == "boards") {
		int x=0;
		struct board_info_node *current=board_list;
		while(current!=NULL) {
			debug("\x1b[1;37m%i: \x1b[0m%s\n",x,current->title);
			x++;
			current=current->next;
		}
	} else if(debug_cmdline == "quit") {
		switchbrd = -2;
	} else {
		debug("Invalid command.\n");
	}
	debug_cmdline = "";
	debug("");
}

void debug_hidCallback(const Event & evt, void * data) {
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
			}
		} else if (evt.key >= 32 && evt.key <= 128) {
			debug_cmdline += evt.key;
			*dt << "\x1b[s"; // Save cursor position
			dt->locate(0,19);
			dt->color(GREY | HIGH_INTENSITY, BLACK);
			*dt << "> \x1b[1;32m" << debug_cmdline.c_str() << "\x1b[k"; //clear EOL
			*dt << "\x1b[u"; // Restore cursor position
		} else if (evt.key == 13) {
			process_debug();
		}
	}
}

void debug_init() {
	dt = new ConsoleText(80,20,new Texture("zzt-ascii.png", true));
	dt->setSize(640,240);
	dt->setTranslate(Vector(1024,360,0));
	dt->setANSI(true);
	dt->color(GREY, BLACK);
	dt->clear();
	debug("\n\nDreamZZT 3.0\n(C) 2006 Sam Steele\n\nREADY.\n");	
	debug_hidCookie = Hid::callbackReg(debug_hidCallback, NULL);
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
	dt->locate(0,19);
	dt->locate(0,19);
	dt->color(GREY | HIGH_INTENSITY, BLACK);
	*dt << "> \x1b[1;32m" << debug_cmdline.c_str() << "\x1b[k"; //clear EOL
	*dt << "\x1b[u"; // Restore cursor position
}