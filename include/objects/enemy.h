/* enemy.h - ZZT Enemys
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

#define ZZT_LION_SHAPE 0xea
#define ZZT_LION_NAME "lion"
#define ZZT_LION_FLAGS F_PUSHABLE|F_ENEMY|F_OBJECT
#define ZZT_LION_CLASS Lion

#define ZZT_SHARK_SHAPE '^'
#define ZZT_SHARK_NAME "shark"
#define ZZT_SHARK_FLAGS F_PUSHABLE|F_ENEMY|F_OBJECT
#define ZZT_SHARK_CLASS Lion

#define ZZT_BEAR_SHAPE 0x99
#define ZZT_BEAR_NAME "bear"
#define ZZT_BEAR_FLAGS F_PUSHABLE|F_ENEMY|F_OBJECT
#define ZZT_BEAR_CLASS Bear

#define ZZT_RUFFIAN_SHAPE 0x05
#define ZZT_RUFFIAN_NAME "ruffian"
#define ZZT_RUFFIAN_FLAGS F_PUSHABLE|F_ENEMY|F_OBJECT
#define ZZT_RUFFIAN_CLASS Ruffian

#define ZZT_TIGER_SHAPE 0xe3
#define ZZT_TIGER_NAME "tiger"
#define ZZT_TIGER_FLAGS F_PUSHABLE|F_ENEMY|F_OBJECT
#define ZZT_TIGER_CLASS Tiger

#define ZZT_SPINNING_GUN_SHAPE 0xe3
#define ZZT_SPINNING_GUN_NAME "spinning-gun"
#define ZZT_SPINNING_GUN_FLAGS F_OBJECT
#define ZZT_SPINNING_GUN_CLASS SpinningGun

class Enemy : public ZZTObject {
public:
	Enemy(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { 
		m_intel = 0;
		m_bg = 0;
		
		switch(type) {
			case ZZT_LION:
				m_fg = 12;
				break;
			case ZZT_TIGER:
				m_fg = 11;
				break;
			case ZZT_BEAR:
				m_fg = 6;
				break;
			case ZZT_RUFFIAN:
				m_fg = 13;
				break;
		}
	}
		
	void setParam(int arg, unsigned char val) { if(arg==1) m_intel = val; }
	unsigned char getParam(int arg) { if(arg==1) return m_intel; }
	void message(ZZTObject *them, std::string msg);
protected:
	int m_intel;
};

class Lion : public Enemy {
public:
	Lion(int type, int x, int y, int shape, int flags, std::string name) : Enemy(type, x, y, shape, flags, name) { }
	void update();
};

class Bear : public Enemy {
public:
	Bear(int type, int x, int y, int shape, int flags, std::string name) : Enemy(type, x, y, shape, flags, name) { }
	void update();
};

class Ruffian : public Enemy {
public:
	Ruffian(int type, int x, int y, int shape, int flags, std::string name) : Enemy(type, x, y, shape, flags, name) { 
		m_rest=0;
		m_restCounter=0;
		m_moveCounter=0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char getParam(int arg);
	void update();
private:
	int m_rest, m_restCounter, m_moveCounter;
};

class Tiger : public Lion {
public:
	Tiger(int type, int x, int y, int shape, int flags, std::string name) : Lion(type, x, y, shape, flags, name) { 
		m_rate = 0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char getParam(int arg);
	void update();
private:
	int m_rate;
};

class SpinningGun : public Enemy {
public:
	SpinningGun(int type, int x, int y, int shape, int flags, std::string name) : Enemy(type, x, y, shape, flags, name) { 
		m_rate = 0;
		m_animIndex = 0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char getParam(int arg);
	void update();
private:
	int m_rate, m_animIndex;
};

