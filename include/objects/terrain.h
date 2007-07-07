/* terrain.h - ZZT Terrain
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

//BOULDER
#define ZZT_BOULDER_SHAPE 0xFE
#define ZZT_BOULDER_NAME "boulder"
#define ZZT_BOULDER_FLAGS F_PUSHABLE
#define ZZT_BOULDER_CLASS Terrain

//LINE
#define ZZT_LINE_SHAPE 0xFA
#define ZZT_LINE_NAME "line"
#define ZZT_LINE_FLAGS F_NONE
#define ZZT_LINE_CLASS Terrain

//NORMAL
#define ZZT_NORMAL_SHAPE 0xB2
#define ZZT_NORMAL_NAME "normal"
#define ZZT_NORMAL_FLAGS F_NONE
#define ZZT_NORMAL_CLASS Terrain

//SOLID
#define ZZT_SOLID_SHAPE 0xDB
#define ZZT_SOLID_NAME "solid"
#define ZZT_SOLID_FLAGS F_NONE
#define ZZT_SOLID_CLASS Terrain

//EMPTY
#define ZZT_EMPTY_SHAPE 32
#define ZZT_EMPTY_NAME "empty"
#define ZZT_EMPTY_FLAGS F_EMPTY
#define ZZT_EMPTY_CLASS Terrain

//FAKE
#define ZZT_FAKE_SHAPE 0xB2
#define ZZT_FAKE_NAME "fake"
#define ZZT_FAKE_FLAGS F_EMPTY
#define ZZT_FAKE_CLASS Terrain

//FLOOR
#define ZZT_FLOOR_SHAPE 0xB0
#define ZZT_FLOOR_NAME "floor"
#define ZZT_FLOOR_FLAGS F_EMPTY
#define ZZT_FLOOR_CLASS Terrain

//BREAKABLE
#define ZZT_BREAKABLE_SHAPE 0xB1
#define ZZT_BREAKABLE_NAME "breakable"
#define ZZT_BREAKABLE_FLAGS F_NONE
#define ZZT_BREAKABLE_CLASS Terrain

//FOREST
#define ZZT_FOREST_SHAPE 0xb0
#define ZZT_FOREST_NAME "forest"
#define ZZT_FOREST_FLAGS F_NONE
#define ZZT_FOREST_CLASS Terrain

//WATER
#define ZZT_WATER_SHAPE 0xb0
#define ZZT_WATER_NAME "water"
#define ZZT_WATER_FLAGS F_NONE
#define ZZT_WATER_CLASS Water

//WATER (N)
#define ZZT_WATER_N_SHAPE 0x1e
#define ZZT_WATER_N_NAME "water-n"
#define ZZT_WATER_N_FLAGS F_EMPTY
#define ZZT_WATER_N_CLASS Water

//WATER (S)
#define ZZT_WATER_S_SHAPE 0x1f
#define ZZT_WATER_S_NAME "water-s"
#define ZZT_WATER_S_FLAGS F_EMPTY
#define ZZT_WATER_S_CLASS Water

//WATER (E)
#define ZZT_WATER_E_SHAPE 0x10
#define ZZT_WATER_E_NAME "water-e"
#define ZZT_WATER_E_FLAGS F_EMPTY
#define ZZT_WATER_E_CLASS Water

//WATER (W)
#define ZZT_WATER_W_SHAPE 0x11
#define ZZT_WATER_W_NAME "water-w"
#define ZZT_WATER_W_FLAGS F_EMPTY
#define ZZT_WATER_W_CLASS Water

//INVISIBLE
#define ZZT_INVISIBLE_SHAPE ' '
#define ZZT_INVISIBLE_NAME "invisible"
#define ZZT_INVISIBLE_FLAGS F_NONE
#define ZZT_INVISIBLE_CLASS Terrain

//BLINK
#define ZZT_BLINK_SHAPE 206
#define ZZT_BLINK_NAME "blink"
#define ZZT_BLINK_FLAGS F_OBJECT
#define ZZT_BLINK_CLASS Blink

//VERTICAL LASER
#define ZZT_VERTICAL_BLINK_SHAPE 0xBA
#define ZZT_VERTICAL_BLINK_NAME "vertical blink"
#define ZZT_VERTICAL_BLINK_FLAGS F_OBJECT
#define ZZT_VERTICAL_BLINK_CLASS Laser

//HORIZONTAL LASER
#define ZZT_HORIZONTAL_BLINK_SHAPE 0xCD
#define ZZT_HORIZONTAL_BLINK_NAME "horizontal blink"
#define ZZT_HORIZONTAL_BLINK_FLAGS F_OBJECT
#define ZZT_HORIZONTAL_BLINK_CLASS Laser

//BLUE
#define ZZT_BLUE_TEXT_SHAPE 0xB2
#define ZZT_BLUE_TEXT_NAME "text"
#define ZZT_BLUE_TEXT_FLAGS F_NONE
#define ZZT_BLUE_TEXT_CLASS Text

//GREEN
#define ZZT_GREEN_TEXT_SHAPE 0xB2
#define ZZT_GREEN_TEXT_NAME "text"
#define ZZT_GREEN_TEXT_FLAGS F_NONE
#define ZZT_GREEN_TEXT_CLASS Text

//CYAN
#define ZZT_CYAN_TEXT_SHAPE 0xB2
#define ZZT_CYAN_TEXT_NAME "text"
#define ZZT_CYAN_TEXT_FLAGS F_NONE
#define ZZT_CYAN_TEXT_CLASS Text

//RED
#define ZZT_RED_TEXT_SHAPE 0xB2
#define ZZT_RED_TEXT_NAME "text"
#define ZZT_RED_TEXT_FLAGS F_NONE
#define ZZT_RED_TEXT_CLASS Text

//PURPLE
#define ZZT_PURPLE_TEXT_SHAPE 0xB2
#define ZZT_PURPLE_TEXT_NAME "text"
#define ZZT_PURPLE_TEXT_FLAGS F_NONE
#define ZZT_PURPLE_TEXT_CLASS Text

//YELLOW
#define ZZT_YELLOW_TEXT_SHAPE 0xB2
#define ZZT_YELLOW_TEXT_NAME "text"
#define ZZT_YELLOW_TEXT_FLAGS F_NONE
#define ZZT_YELLOW_TEXT_CLASS Text

//WHITE
#define ZZT_WHITE_TEXT_SHAPE 0xB2
#define ZZT_WHITE_TEXT_NAME "text"
#define ZZT_WHITE_TEXT_FLAGS F_NONE
#define ZZT_WHITE_TEXT_CLASS Text

class Terrain : public ZZTObject {
public:
	Terrain(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) {
		m_modelName = "block.amf";
	}
	void create();
	void message(ZZTObject *them, std::string msg);
};

class Water : public Terrain {
public:
	Water(int type, int x, int y, int shape, int flags, std::string name) : Terrain(type, x, y, shape, flags, name) { 
		m_counter = 0;
		m_cstep = 0;
	}
	void update();
private:
	int m_counter, m_cstep;
};

class Blink : public ZZTObject {
public:
	Blink(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_step.x=0;
		m_step.y=-1;
		m_start = 4;
		m_fire = 4;
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void create();
	void update();
	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUIDirection("Direction            ",&m_step));
		w->addWidget(new TUISlider	 ("Start Time           ",&m_start));
		w->addWidget(new TUISlider	 ("Period               ",&m_fire));
	}	
private:
	int m_start, m_fire, m_counter;
};

class Laser : public ZZTObject {
public:
	Laser(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_time = 0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void update();
private:
	int m_time;
};

class Text : public ZZTObject {
public:
	Text(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { }
	void create();
};

