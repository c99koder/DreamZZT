/* object.h - ZZTObject base class
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

#ifndef _OBJECT_H
#define _OBJECT_H

#include <Tiki/vector.h>
#include <Tiki/file.h>
using namespace Tiki::Math;
#include "window.h"

enum direction { IDLE, LEFT, RIGHT, UP, DOWN, SHOOTING=128 };

class ZZTObject {
public:
	ZZTObject(int type, int x, int y, int shape, int flags, std::string name);
	virtual ~ZZTObject() { m_isValid = false; }
	
	
	direction opposite(enum direction dir);
	direction toward(ZZTObject *them);
	direction clockwise(direction dir);
	direction str_to_direction(std::string s);
	
	bool isValid() { return m_isValid; }
	
	int str_to_color(std::string color);
	std::string int_to_color(int color);
	
	int distance(ZZTObject *them);
	int dist_x(ZZTObject *them);
	int dist_y(ZZTObject *them);
	bool move(direction d, bool trying=false, bool origin=true);
	bool is_empty(direction d, bool ignorePlayer=false);
	ZZTObject *create_object(int type, direction d);
	
	Vector position() { return m_position; }
	void setPosition(Vector p) { m_position = p; }
	Vector step() { return m_step; }
	void setStep(Vector s) { m_step = s; }
	char shape() { return m_shape; }
	void setShape(int s) { m_shape = s; }
	int color() { return *m_color; }
	void setColor(int c) { 
		if(m_color == &m_bg && c > 7) { //Force background to low-intensity
			c -= 8;
		}
		*m_color = c; 
	}
	void setColor(int fg, int bg) {
		m_fg = fg;
		m_bg = bg;
	}
	int type() { return m_type; }
	int fg() { return m_fg; }
	void setFg(int f) { m_fg = f; }
	int bg() { return m_bg; }
	void setBg(int b) { m_bg = b; }
	std::string name() { return m_name; }
	void setName(std::string name) { m_name = name; }
	bool updated() { return m_updated; }
	void setUpdated(bool u) { m_updated = u; }
	bool pushed() { return m_pushed; }
	void setPushed(bool p) { m_pushed = p; }
	short cycle() { return m_cycle; }
	void setCycle(short c) { m_cycle = c; }
	int tick() { return m_tick; }
	void setTick(int t) { m_tick = t; }
	int flags() { return m_flags; }
	int flag(int f) { return m_flags & f; }
	void setFlag(int flag) { m_flags |= flag; }
	void setFlags(int flags) { m_flags = flags; }
	void setProg(std::string prog, int len, int pos) { m_prog=prog; m_proglen = len; m_progpos = pos; }
	void setHeading(direction h) { m_heading = h; }
	direction heading() { return m_heading; }
	std::string prog() { return m_prog; }
	int progLen() { return m_proglen; }
	int progPos() { return m_progpos; }
	
	void draw();
	void edit();
	virtual void addEditWidgets(TUIWindow *w) { };
	
	virtual void setParam(int arg, unsigned char val) { };
	virtual unsigned char param(int arg) { return 0; }
	virtual void update() { };
	virtual void message(ZZTObject *from, std::string msg) { };
	virtual void create() { };
	
	void shoot(direction d);
	
protected:
	Vector m_position;
	Vector m_step;
	unsigned short m_cycle;
	int m_tick;
	int m_fg;
	int m_bg;
	int *m_color;
	int m_board;
	direction m_heading;
	int m_type;
	unsigned char m_shape;
	std::string m_name; 
	int m_flags;
	std::string m_prog;
	short m_proglen;
	short m_progpos;
	bool m_updated;
	bool m_pushed;
	
private:
	bool m_isValid;
};

#define ZZT_TYPE_COUNT 0x3e
#define ZZT_EMPTY 0x00
#define ZZT_EDGE 0x01
#define ZZT_EXPLOSION 0x02
#define ZZT_PLAYER 0x04
#define ZZT_AMMO 0x05
#define ZZT_TORCH 0x06
#define ZZT_GEM 0x07
#define ZZT_KEY 0x08
#define ZZT_DOOR 0x09
#define ZZT_SCROLL 0x0A
#define ZZT_PASSAGE 0x0B
#define ZZT_DUPLICATOR 0x0C
#define ZZT_BOMB 0x0D
#define ZZT_ENERGIZER 0x0E
#define ZZT_STAR 0x0F
#define ZZT_CONVEYER_CW 0x10
#define ZZT_CONVEYER_CCW 0x11
#define ZZT_BULLET 0x12
#define ZZT_WATER 0x13
#define ZZT_FOREST 0x14
#define ZZT_SOLID 0x15
#define ZZT_NORMAL 0x16
#define ZZT_BREAKABLE 0x17
#define ZZT_BOULDER 0x18
#define ZZT_SLIDER_NS 0x19
#define ZZT_SLIDER_EW 0x1A
#define ZZT_FAKE 0x1B
#define ZZT_INVISIBLE 0x1C
#define ZZT_BLINK 0x1D
#define ZZT_TRANSPORTER 0x1E
#define ZZT_LINE 0x1F
#define ZZT_RICOCHET 0x20
#define ZZT_HORIZONTAL_BLINK 0x21
#define ZZT_BEAR 0x22
#define ZZT_RUFFIAN 0x23
#define ZZT_OBJECT 0x24
#define ZZT_SLIME 0x25
#define ZZT_SHARK 0x26
#define ZZT_SPINNING_GUN 0x27
#define ZZT_PUSHER 0x28
#define ZZT_LION 0x29
#define ZZT_TIGER 0x2A
#define ZZT_VERTICAL_BLINK 0x2B
#define ZZT_CENTIPEDE_HEAD 0x2C
#define ZZT_CENTIPEDE_BODY 0x2D
#define ZZT_BLUE_TEXT 0x2F
#define ZZT_GREEN_TEXT 0x30
#define ZZT_CYAN_TEXT 0x31
#define ZZT_RED_TEXT 0x32
#define ZZT_PURPLE_TEXT 0x33
#define ZZT_YELLOW_TEXT 0x34
#define ZZT_WHITE_TEXT 0x35
/*#define ZZT_EMPTY 0x36	 | (set in colour byte) | White blinking text
#define ZZT_EMPTY 0x37	 | (set in colour byte) | Blue blinking text
#define ZZT_EMPTY 0x38	 | (set in colour byte) | Green blinking text
#define ZZT_EMPTY 0x39	 | (set in colour byte) | Cyan blinking text
#define ZZT_EMPTY 0x3A	 | (set in colour byte) | Red blinking text
#define ZZT_EMPTY 0x3B	 | (set in colour byte) | Purple blinking text
#define ZZT_EMPTY 0x3C	 | (set in colour byte) | Yellow blinking text
#define ZZT_EMPTY 0x3D	 | (set in colour byte) | Grey blinking text*/
#define ZZT_LUA 0x40

#define F_NONE 0
#define F_EMPTY 1
#define F_PUSHABLE 2
#define F_DELETED 4
#define F_SLEEPING 8
#define F_ITEM 16
#define F_ENEMY 32
#define F_GLOW 64
#define F_OBJECT 128
#define F_PUSHER 256

#include "objects/objects.h"
#include "objects/terrain.h"
#include "objects/items.h"
#include "objects/enemy.h"

ZZTObject *create_object(int type, int x, int y);
ZZTObject *create_copy(ZZTObject *source);
int str_to_obj(std::string str);
int str_to_color(char *color);
std::string int_to_color(int col);
#endif

