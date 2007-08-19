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

extern "C" {
#include "lua.h"
}

class Transporter : public ZZTObject {
public:
	Transporter() {
		m_shape = '<';
		m_name = "transporter";
		m_flags = F_OBJECT;
		m_step.x=0;
		m_step.y=1;
	}
	int type() { return ZZT_TRANSPORTER; }
	ZZTObject *alloc() {
		return new Transporter();
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

class Bomb : public ZZTObject {
public:
	Bomb() {
		m_shape = 0x0B;
		m_name = "bomb";
		m_flags = F_OBJECT;
		m_counter = 0;
		m_start = 0;
	}
	int type() { return ZZT_BOMB; }
	ZZTObject *alloc() {
		return new Bomb();
	}
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);
	void setParam(int arg, unsigned char val) {
		if(arg==1)
			m_start = val;
	}
	unsigned char param(int arg) {
		if(arg==1)
			return (m_shape==0x0B)?0:(m_shape) - '0';
		else
			return 0;
	}
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

class Explosion : public ZZTObject {
public:
	Explosion() {
		m_shape = 177;
		m_name = "explosion";
		m_flags = F_NONE;
	}
	int type() { return ZZT_EXPLOSION; }
	ZZTObject *alloc() {
		return new Explosion();
	}
	void create();
	void update();
private:
	int m_counter;
};

#define BULLET_DAMAGE 10

class Bullet : public ZZTObject {
public:
	Bullet() {
		m_shape = 249;
		m_name = "bullet";
		m_flags = F_OBJECT;
		m_fg = 15;
		m_bg = 0;
		m_owner = 0;
	}
	int type();
	ZZTObject *alloc() {
		return new Bullet();
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
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

class Ricochet : public ZZTObject {
public:
	Ricochet() {
		m_shape = '*';
		m_name = "ricochet";
		m_flags = F_NONE;
	}
	int type() { return ZZT_RICOCHET; }
	ZZTObject *alloc() {
		return new Ricochet();
	}
};

class Pusher : public ZZTObject {
public:
	Pusher() {
		m_shape = 0x10;
		m_name = "pusher";
		m_flags = F_OBJECT | F_PUSHER;
		m_step.x=0;
		m_step.y=-1;
	}
	int type() { return ZZT_PUSHER; }
	ZZTObject *alloc() {
		return new Pusher();
	}
	void create();
	void update();
	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUIDirection("Direction            ",&m_step));
	}
};

class SliderNS : public ZZTObject {
public:
	SliderNS() {
		m_shape = 0x12;
		m_name = "sliderns";
		m_flags = F_PUSHABLE;
	}
	int type() { return ZZT_SLIDER_NS; }
	ZZTObject *alloc() {
		return new SliderNS();
	}
};

class SliderEW : public ZZTObject {
public:
	SliderEW() {
		m_shape = 0x1D;
		m_name = "sliderew";
		m_flags = F_PUSHABLE;
	}
	int type() { return ZZT_SLIDER_EW; }
	ZZTObject *alloc() {
		return new SliderEW();
	}
};

class ConveyerCW : public ZZTObject {
public:
	ConveyerCW() {
		m_shape = '/';
		m_name = "conveyercw";
		m_flags = F_OBJECT;
		m_animIndex = 0;
	}
	int type() { return ZZT_CONVEYER_CW; }
	ZZTObject *alloc() {
		return new ConveyerCW();
	}
	void update();
private:
	int cw(ZZTObject *them);
	int m_animIndex;
};

class ConveyerCCW : public ZZTObject {
public:
	ConveyerCCW() {
		m_shape = '\\';
		m_name = "conveyerccw";
		m_flags = F_OBJECT;
		m_animIndex = 0;
	}
	int type() { return ZZT_CONVEYER_CCW; }
	ZZTObject *alloc() {
		return new ConveyerCCW();
	}
	void update();
private:
	int ccw(ZZTObject *them);
	int m_animIndex;
};

enum KeywordType { ktypeCommand, ktypeDirection, ktypeObject, ktypeColor, ktypeInventory, ktypeLogic, ktypeString, ktypeNumber };
struct Keyword {
	KeywordType type;
	std::string word;
	int value;
};

class ZZTOOP : public ZZTObject {
public:
	ZZTOOP() {
		m_shape = 0x01;
		m_name = "object";
		m_flags = F_OBJECT | F_PUSHER;
		m_walk = IDLE;
		m_watch = false;
	}
	int type() { return ZZT_OBJECT; }
	ZZTObject *alloc() {
		return new ZZTOOP();
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);

	void watch() {
		m_watch = !m_watch;
	}
	void zzt_goto(std::string label);
	void send(std::string cmd);
	void zap(std::string label);
	void restore(std::string label);
	std::string get_zztobj_name();
	void exec(std::string cmd);
	std::vector<Keyword> tokenize(std::string text, bool parseDirections = true);
protected:
	int playedLast,goagain;
	direction m_walk;
	bool m_watch;
};

class Scroll : public ZZTOOP {
public:
	Scroll() {
		m_shape = 0xE8;
		m_name = "scroll";
		m_flags = F_ITEM|F_OBJECT;
	}
	int type() { return ZZT_SCROLL; }
	ZZTObject *alloc() {
		return new Scroll();
	}
	void create() {
		m_fg = 15;
		m_shape = 0xE8;
		m_progpos = -1;
	}
	void message(ZZTObject *them, std::string msg);
	void update();
};

class Passage : public ZZTObject {
public:
	Passage() {
		m_shape = 0xF0;
		m_name = "passage";
		m_flags = F_GLOW|F_OBJECT;
		m_dest = 0;
	}
	int type() { return ZZT_PASSAGE; }
	ZZTObject *alloc() {
		return new Passage();
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void create();
	void message(ZZTObject *them, std::string msg);
	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUIBoardList("Destination          ",&m_dest));
	}
private:
	unsigned char m_dest;
};

class Duplicator : public ZZTObject {
public:
	Duplicator() {
		m_shape = '.';
		m_name = "duplicator";
		m_flags = F_OBJECT|F_PUSHER;
		m_animIndex = 0;
		m_rate = 0;
	}
	int type() { return ZZT_DUPLICATOR; }
	ZZTObject *alloc() {
		return new Duplicator();
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void create();
	void update();
private:
	int m_animIndex, m_rate;
};

class Player : public ZZTOOP {
public:
	Player() {
		m_shape = 0x02;
		m_flags = F_PUSHABLE | F_GLOW | F_OBJECT | F_PUSHER;
		m_name = "player";
		m_move = IDLE;
		m_shoot = IDLE;
		m_cycle = 1;
		m_height = 1;
		m_modelName = "player.amf";
		m_shot = false;
	}
	~Player();
	int type() {
		return ZZT_PLAYER;
	}
	ZZTObject *alloc() {
		return new Player();
	}
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);
	void processEvent(const Tiki::Hid::Event & evt);
	void setHeading(direction h) {
		m_heading = h;
		m_move = IDLE;
		m_shoot = IDLE;
	}
private:
	direction m_move, m_shoot;
	bool m_shot;
};

class ZZTLUA : public ZZTObject {
public:
	ZZTLUA();
	~ZZTLUA();

	int type() { return ZZT_LUA; }
	ZZTObject *alloc() {
		return new ZZTLUA();
	}

	void create();
	void update();
	void message(ZZTObject *from, std::string msg) {
		push_message(from, msg);
	};
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);

	struct message_queue_item {
		ZZTObject *them;
		std::string message;
	};

	int message_count() {
		return (int)message_queue.size();
	}

	message_queue_item pop_message() {
		message_queue_item i = *(message_queue.begin());
		message_queue.pop_front();
		return i;
	}
private:
	std::list<message_queue_item> message_queue;

	void push_message(ZZTObject *them, std::string message) {
		message_queue_item i;
		i.them = them;
		i.message = message;
		message_queue.push_back(i);
	}
	lua_State* m_luaVM;
	bool m_serialized;
};
