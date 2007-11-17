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

class Edge : public ZZTObject {
public:
	Edge() {
		m_shape = 0x00;
		m_name = "green";
		m_flags = F_NONE;
	}
	int type() { return ZZT_EDGE; }
	ZZTObject *alloc() {
		return new Edge();
	}
	void message(ZZTObject *them, std::string message);
};

class Boulder : public ZZTObject {
public:
	Boulder() {
		m_shape = 0xFE;
		m_name = "boulder";
		m_flags = F_PUSHABLE;
	}
	int type() { return ZZT_BOULDER; }
	ZZTObject *alloc() {
		return new Boulder();
	}
};

class Line : public ZZTObject {
public:
	Line() {
		m_shape = 0xFA;
		m_name = "line";
		m_flags = F_NONE;
	}
	int type() { return ZZT_LINE; }
	ZZTObject *alloc() {
		return new Line();
	}
};

class Normal : public ZZTObject {
public:
	Normal() {
		m_shape = 0xB2;
		m_name = "normal";
		m_flags = F_NONE;
	}
	int type() { return ZZT_NORMAL; }
	ZZTObject *alloc() {
		return new Normal();
	}
};

class Solid : public ZZTObject {
public:
	Solid() {
		m_shape = 0xDB;
		m_name = "solid";
		m_flags = F_NONE;
	}
	int type() { return ZZT_SOLID; }
	ZZTObject *alloc() {
		return new Solid();
	}
};

class Empty : public ZZTObject {
public:
	Empty() {
		m_shape = ' ';
		m_name = "empty";
		m_flags = F_EMPTY;
	}
	void create() {
		m_shape=32;
		m_fg=0;
		m_bg=0;
	}	
	int type() { return ZZT_EMPTY; }
	ZZTObject *alloc() {
		return new Empty();
	}
};

class Fake : public ZZTObject {
public:
	Fake() {
		m_shape = 0xB2;
		m_name = "fake";
		m_flags = F_EMPTY;
	}
	int type() { return ZZT_FAKE; }
	ZZTObject *alloc() {
		return new Fake();
	}
};

class Floor : public ZZTObject {
public:
	Floor() {
		m_shape = 0xB0;
		m_name = "floor";
		m_flags = F_EMPTY;
	}
	int type() { return SZT_FLOOR; }
	ZZTObject *alloc() {
		return new Floor();
	}
};

class Breakable : public ZZTObject {
public:
	Breakable() {
		m_shape = 0xB1;
		m_name = "breakable";
		m_flags = F_NONE;
	}
	int type() { return ZZT_BREAKABLE; }
	ZZTObject *alloc() {
		return new Breakable();
	}
};

class Forest : public ZZTObject {
public:
	Forest() {
		m_shape = 0xB0;
		m_name = "forest";
		m_flags = F_ITEM;
	}
	int type() { return ZZT_FOREST; }
	ZZTObject *alloc() {
		return new Forest();
	}
	void message(ZZTObject *them, std::string message);
};

class Water : public ZZTObject {
public:
	Water() {
		m_shape = 0xB0;
		m_flags = F_NONE;
		m_name = "water";
		m_counter = 0;
		m_cstep = 0;
	}
	int type() { return ZZT_WATER; }
	ZZTObject *alloc() {
		return new Water();
	}
	void update();
	void message(ZZTObject *them, std::string message);
private:
	int m_counter, m_cstep;
};

class RiverN : public ZZTObject {
public:
	RiverN()  {
		m_shape = 0x1E;
		m_name = "water-n";
		m_flags = F_EMPTY;
	}
	int type() {
		return SZT_WATER_N;
	}
	ZZTObject *alloc() {
		return new RiverN();
	}
};

class RiverS : public ZZTObject {
public:
	RiverS()  {
		m_shape = 0x1F;
		m_name = "water-s";
		m_flags = F_EMPTY;
	}
	int type() {
		return SZT_WATER_S;
	}
	ZZTObject *alloc() {
		return new RiverS();
	}
};

class RiverE : public ZZTObject {
public:
	RiverE()  {
		m_shape = 0x10;
		m_name = "water-e";
		m_flags = F_EMPTY;
	}
	int type() {
		return SZT_WATER_E;
	}
	ZZTObject *alloc() {
		return new RiverE();
	}
};

class RiverW : public ZZTObject {
public:
	RiverW()  {
		m_shape = 0x11;
		m_name = "water-w";
		m_flags = F_EMPTY;
	}
	int type() {
		return SZT_WATER_W;
	}
	ZZTObject *alloc() {
		return new RiverW();
	}
};

class Invisible : public ZZTObject {
public:
	Invisible() {
		m_shape = ' ';
		m_name = "invisible";
		m_flags = F_NONE;
	}
	int type() { return ((m_shape == ' ')?ZZT_INVISIBLE:ZZT_NORMAL); }
	ZZTObject *alloc() {
		return new Invisible();
	}
	void message(ZZTObject *them, std::string message);
};

class Text : public ZZTObject {
public:
	Text() {
		m_name = "text";
		m_textcolor = m_bg = BLACK;
	}
	void create();
protected:
	char m_textcolor;
};

class BlueText : public Text {
public:
	BlueText() {
		m_textcolor = m_bg = BLUE;
	}
	int type();
	ZZTObject *alloc() {
		return new BlueText();
	}
};

class GreenText : public Text {
public:
	GreenText() {
		m_textcolor = m_bg = GREEN;
	}
	int type();
	ZZTObject *alloc() {
		return new GreenText();
	}
};

class CyanText : public Text {
public:
	CyanText() {
		m_textcolor = m_bg = CYAN;
	}
	int type();
	ZZTObject *alloc() {
		return new CyanText();
	}
};

class RedText : public Text {
public:
	RedText() {
		m_textcolor = m_bg = RED;
	}
	int type();
	ZZTObject *alloc() {
		return new RedText();
	}
};

class PurpleText : public Text {
public:
	PurpleText() {
		m_textcolor = m_bg = MAGENTA;
	}
	int type();
	ZZTObject *alloc() {
		return new PurpleText();
	}
};

class YellowText : public Text {
public:
	YellowText() {
		m_textcolor = m_bg = YELLOW;
	}
	int type();
	ZZTObject *alloc() {
		return new YellowText();
	}
};

class WhiteText : public Text {
public:
	WhiteText() {
		m_textcolor = m_bg = BLACK;
	}
	int type();
	ZZTObject *alloc() {
		return new WhiteText();
	}
};

class Blink : public ZZTObject {
public:
	Blink() {
		m_shape = 206;
		m_name = "blink";
		m_flags = F_OBJECT;
		m_step.x=0;
		m_step.y=-1;
		m_start = 4;
		m_fire = 4;
	}
	int type() { return ZZT_BLINK; }
	ZZTObject *alloc() {
		return new Blink();
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
	Laser() {
		m_time = 0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void update();
private:
	int m_time;
};

class HorizontalLaser : public Laser {
public:
	HorizontalLaser() {
		m_shape = 0xCD;
		m_name = "horizontal blink";
		m_flags = F_OBJECT;
	}
	int type();
	ZZTObject *alloc() {
		return new HorizontalLaser();
	}
};

class VerticalLaser : public Laser {
public:
	VerticalLaser() {
		m_shape = 0xBA;
		m_name = "vertical blink";
		m_flags = F_OBJECT;
	}
	int type();
	ZZTObject *alloc() {
		return new VerticalLaser();
	}
};
