/* status.h - Manage and display the ZZT HUD
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

#ifndef _STATUS_H
#define _STATUS_H

#include <Tiki/drawables/console.h>

void set_msg(char *text);
void redraw_status();
void draw_msg();
void dzzt_logo();
void draw_ammo();
void take_ammo(int count);
void give_ammo(int count);
void draw_health();
void take_health(int count);
void give_health(int count);
void draw_torch();
void take_torch(int count);
void give_torch(int count);
void draw_gems();
void take_gems(int count);
void give_gems(int count);
void draw_score();
void take_score(int count);
void give_score(int count);
void draw_time();
void take_time(int count);
void give_time(int count);
void draw_keys();
void draw_hud_ingame();

class TextWindow {
public:
	TextWindow(Tiki::GL::ConsoleText *c,std::string title,std::string text);

	char *getLabel() { return retlbl; }
	void doMenu();
	void pc_box(int x, int y,int w,int h,int fg,int bg);
	void draw_shadow(int x, int y);

private:
	// HID input callback.
		static void hidCallback(const Tiki::Hid::Event & evt, void * data);
	virtual void processHidEvent(const Tiki::Hid::Event & evt);
	int m_hidCookie;
	
	int y,loop,maxy,dirty;
  char txt[150][50];
  char lbl[50][20];
  int lblidx[150];
  int q,lblcnt;	
	char retlbl[20];
	Tiki::GL::ConsoleText *console;
};
#endif