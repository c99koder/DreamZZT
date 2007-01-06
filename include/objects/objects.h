/* objects.h - ZZT objects
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

//TRANSPORTER
#define ZZT_TRANSPORTER_SHAPE '<'
#define ZZT_TRANSPORTER_NAME "transporter"
#define ZZT_TRANSPORTER_FLAGS F_OBJECT
#define ZZT_TRANSPORTER_CLASS Transporter

class Transporter : public ZZTObject {
public:
	Transporter(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) {
		m_step.x=0;
		m_step.y=1;
	}
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);
	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUIDirection("Direction            ",&m_step));
	}
private:
	int m_counter, m_anim;
};
	
//BOMB
#define ZZT_BOMB_SHAPE 0x0B
#define ZZT_BOMB_NAME "bomb"
#define ZZT_BOMB_FLAGS F_OBJECT
#define ZZT_BOMB_CLASS Bomb

class Bomb : public ZZTObject {
public:
	Bomb(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_counter = 0;
		m_start = 0;
	}
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);
	void setParam(int arg, unsigned char val) { if(arg==1) m_start = val; }
	unsigned char getParam(int arg) { if(arg==1) return (m_shape==ZZT_BOMB_SHAPE)?0:(m_shape) - '0'; else return 0; }
	void addEditWidgets(TUIWindow *w) {
		TUIRadioGroup *rg = new TUIRadioGroup("Direction            ",&m_start);
		rg->add("Inactive");
		rg->add("1");
		rg->add("2");
		rg->add("3");
		rg->add("4");
		rg->add("5");
		rg->add("6");
		rg->add("7");
		rg->add("8");
		rg->add("9");
		w->addWidget(rg);
	}
private:
	unsigned short int m_counter,m_start;
};

//EXPLOSION
#define ZZT_EXPLOSION_SHAPE 177
#define ZZT_EXPLOSION_NAME "explosion"
#define ZZT_EXPLOSION_FLAGS F_NONE
#define ZZT_EXPLOSION_CLASS Explosion

class Explosion : public ZZTObject {
public:
	Explosion(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { }
	void create();
	void update();
private:
	int m_counter;
};

//BULLET
#define ZZT_BULLET_SHAPE 249
#define ZZT_BULLET_NAME "bullet"
#define ZZT_BULLET_FLAGS F_OBJECT
#define ZZT_BULLET_CLASS Bullet
#define BULLET_DAMAGE 10

class Bullet : public ZZTObject {
public:
	Bullet(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_fg = 15;
		m_bg = 0;
		m_owner = 0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char getParam(int arg);
	void update();
	void message(ZZTObject *them, std::string msg);
	void addEditWidgets(TUIWindow *w) {
		TUIRadioGroup *rg = new TUIRadioGroup("Fired By             ",&m_owner);
		rg->add("Player");
		rg->add("Creature");
		w->addWidget(new TUIDirection("Direction            ",&m_step));
	}
private:
	unsigned short int m_owner;
};

//PUSHER
#define ZZT_PUSHER_SHAPE 0x10
#define ZZT_PUSHER_NAME "pusher"
#define ZZT_PUSHER_FLAGS F_OBJECT | F_PUSHER
#define ZZT_PUSHER_CLASS Pusher

class Pusher : public ZZTObject {
public:
	Pusher(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_step.x=0;
		m_step.y=-1;
	}
	void create();
	void update();
	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUIDirection("Direction            ",&m_step));
	}
};

//SLIDER NS
#define ZZT_SLIDER_NS_SHAPE 0x12
#define ZZT_SLIDER_NS_NAME "sliderns"
#define ZZT_SLIDER_NS_FLAGS F_PUSHABLE
#define ZZT_SLIDER_NS_CLASS ZZTObject

//SLIDER EW
#define ZZT_SLIDER_EW_SHAPE 0x1D
#define ZZT_SLIDER_EW_NAME "sliderew"
#define ZZT_SLIDER_EW_FLAGS F_PUSHABLE
#define ZZT_SLIDER_EW_CLASS ZZTObject

//CONVEYER CW
#define ZZT_CONVEYER_CW_SHAPE '/'
#define ZZT_CONVEYER_CW_NAME "conveyercw"
#define ZZT_CONVEYER_CW_FLAGS F_OBJECT
#define ZZT_CONVEYER_CW_CLASS Conveyer

//CONVEYER CCW
#define ZZT_CONVEYER_CCW_SHAPE '\\'
#define ZZT_CONVEYER_CCW_NAME "conveyerccw"
#define ZZT_CONVEYER_CCW_FLAGS F_OBJECT
#define ZZT_CONVEYER_CCW_CLASS Conveyer

class Conveyer : public ZZTObject {
public:
	Conveyer(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_animIndex = 0;
	}
	void update();
private:
	int cw(ZZTObject *them);
	int ccw(ZZTObject *them);
	
	int m_animIndex;
};

//OBJECT
#define ZZT_OBJECT_SHAPE 0x01
#define ZZT_OBJECT_NAME "object"
#define ZZT_OBJECT_FLAGS F_OBJECT | F_PUSHER
#define ZZT_OBJECT_CLASS ZZTOOP

class ZZTOOP : public ZZTObject {
public:
	ZZTOOP(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_walk = IDLE;
		m_watch = false;
	}
	void setParam(int arg, unsigned char val);
	unsigned char getParam(int arg);
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);
	
	void watch() { m_watch = !m_watch; }
	void zzt_goto(std::string label);
	void send(std::string cmd);
	void zap(std::string label);
	void restore(std::string label);
	std::string get_zztobj_name();
	void exec(std::string cmd);
protected:
	int playedLast,goagain;
	direction m_walk;
	bool m_watch;
};

//SCROLL
#define ZZT_SCROLL_SHAPE 0xE8
#define ZZT_SCROLL_NAME "scroll"
#define ZZT_SCROLL_FLAGS F_ITEM|F_OBJECT
#define ZZT_SCROLL_CLASS Scroll

class Scroll : public ZZTOOP {
public:
	Scroll(int type, int x, int y, int shape, int flags, std::string name) : ZZTOOP(type, x, y, shape, flags, name) { 
		m_shape=ZZT_SCROLL_SHAPE;
	}
	
	void create() { 
		m_fg = 15;
		m_shape = ZZT_SCROLL_SHAPE;
		m_progpos = -1;	
	}
	void message(ZZTObject *them, std::string msg);
	void update();
};

//PASSAGE
#define ZZT_PASSAGE_SHAPE 0xF0
#define ZZT_PASSAGE_NAME "passage"
#define ZZT_PASSAGE_FLAGS F_GLOW|F_OBJECT
#define ZZT_PASSAGE_CLASS Passage

class Passage : public ZZTObject {
public:
	Passage(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) {
		m_dest = 0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char getParam(int arg);
	void create();
	void message(ZZTObject *them, std::string msg);
	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUIBoardList("Destination          ",&m_dest));
	}	
private:
	unsigned char m_dest;
};

//DUPLICATOR
#define ZZT_DUPLICATOR_SHAPE '.'
#define ZZT_DUPLICATOR_NAME "duplicator"
#define ZZT_DUPLICATOR_FLAGS F_OBJECT|F_PUSHER
#define ZZT_DUPLICATOR_CLASS Duplicator

class Duplicator : public ZZTObject {
public:
	Duplicator(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_animIndex = 0;
		m_rate = 0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char getParam(int arg);
	void create();
	void update();
private:
	int m_animIndex, m_rate;
};

//PLAYER
#define ZZT_PLAYER_SHAPE 2
#define ZZT_PLAYER_NAME "player"
#define ZZT_PLAYER_FLAGS F_PUSHABLE | F_GLOW | F_OBJECT | F_PUSHER
#define ZZT_PLAYER_CLASS Player

class Player : public ZZTOOP {
public:
	Player(int type, int x, int y, int shape, int flags, std::string name) : ZZTOOP(type, x, y, shape, flags, name) {
	}
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);
	void processEvent(const Tiki::Hid::Event & evt);
private:
	direction m_move, m_shoot;
};
