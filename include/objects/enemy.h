/* enemy.h - ZZT Enemys
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

class Enemy : public ZZTObject {
public:
	Enemy() {
		m_intel = 4;
		m_bg = 0;
	}

	virtual ~Enemy() { }
	void create();

	void setParam(int arg, unsigned char val) {
		if(arg==1)
			m_intel = val;
	}
	unsigned char param(int arg) {
		if(arg==1)
			return m_intel;
		else
			return 0;
	}
	void message(ZZTObject *them, std::string msg);

	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUISlider("Intelligence         ",&m_intel));
	}

protected:
	int m_intel;
};

class Lion : public Enemy {
public:
	Lion() {
		m_shape = 0xEA;
		m_name = "lion";
		m_flags = F_PUSHABLE|F_OBJECT;
	}
	void update();
	int type() { return ZZT_LION; }
	ZZTObject *alloc() {
		return new Lion();
	}
};

class Shark : public Lion {
public:
	Shark() {
		m_shape = '^';
		m_name = "shark";
		m_flags = F_PUSHABLE|F_OBJECT;
	}
	int type() { return ZZT_SHARK; }
	ZZTObject *alloc() {
		return new Shark();
	}
};

class Bear : public Enemy {
public:
	Bear();
	void update();
	int type() { return ZZT_BEAR; }
	ZZTObject *alloc() {
		return new Bear();
	}
	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUISlider("Sensitivity          ",&m_intel));
	}
};

class Ruffian : public Enemy {
public:
	Ruffian() {
		m_shape = 0x05;
		m_name = "ruffian";
		m_flags = F_PUSHABLE|F_OBJECT;
		m_rest=4;
		m_restCounter=0;
		m_moveCounter=0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void update();
	int type() { return ZZT_RUFFIAN; }
	ZZTObject *alloc() {
		return new Ruffian();
	}
	void addEditWidgets(TUIWindow *w) {
		Enemy::addEditWidgets(w);
		w->addWidget(new TUISlider("Rest Time            ",&m_rest));
	}
private:
	int m_rest, m_restCounter, m_moveCounter;
};

class Tiger : public Lion {
public:
	Tiger() {
		m_shape = 0xE3;
		m_name = "tiger";
		m_flags = F_PUSHABLE|F_OBJECT;
		m_rate = 4;
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void update();
	int type() { return ZZT_TIGER; }
	ZZTObject *alloc() {
		return new Tiger();
	}
	void addEditWidgets(TUIWindow *w) {
		Enemy::addEditWidgets(w);
		w->addWidget(new TUISlider("Firing Rate          ",&m_rate));
	}
private:
	int m_rate;
};

class SpinningGun : public Enemy {
public:
	SpinningGun() {
		m_shape = 0x18;
		m_name = "spinning-gun";
		m_flags = F_OBJECT;
		m_rate = 4;
		m_animIndex = 0;
	}
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);
	void update();
	int type() { return ZZT_SPINNING_GUN; }
	ZZTObject *alloc() {
		return new SpinningGun();
	}
	void addEditWidgets(TUIWindow *w) {
		Enemy::addEditWidgets(w);
		w->addWidget(new TUISlider("Firing Rate          ",&m_rate));
	}
private:
	int m_rate, m_animIndex;
};

class Centipede : public Enemy {
public:
	Centipede() {
		m_flags = F_OBJECT;
		m_nextHeading = IDLE;
		m_discovery = false;
		m_next = m_prev = NULL;
		m_deviance = 0;
	}

	~Centipede() {
		if(m_prev != NULL)
			m_prev->unlinkNext();
		if(m_next != NULL)
			m_next->unlinkPrev();
	}
	
	virtual ZZTObject *alloc() {
		return NULL;
	}
	int type() { return (m_shape==0xE9)?ZZT_CENTIPEDE_HEAD:ZZT_CENTIPEDE_BODY; }
	bool isDiscovered() {
		return !(m_next==NULL && m_prev==NULL);
	}
	void discover(Centipede *prev);
	void doMove(direction d);
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);
	void setParam(int arg, unsigned char val);
	unsigned char param(int arg);

	void reverse() {
		Centipede *tmp = m_next;
		m_next = m_prev;
		m_prev = tmp;

		if(m_prev != NULL) {
			m_heading=m_nextHeading=toward(m_prev);
			m_prev->reverse();
		}
	}

	void unlinkPrev() {
		m_prev = NULL;
	}
	void unlinkNext() {
		m_next = NULL;
	}
private:
	direction m_nextHeading;
	Centipede *m_next,*m_prev;
	bool m_discovery;
	int m_deviance;
};

class CentipedeHead : public Centipede {
public:
	CentipedeHead() {
		m_shape = 0xE9;
		m_name = "centipede-head";
	}
	ZZTObject *alloc() {
		return new CentipedeHead(); 
	}	
};

class CentipedeBody : public Centipede {
public:
	CentipedeBody() {
		m_shape = 'O';
		m_name = "centipede-body";
	}
	ZZTObject *alloc() {
		return new CentipedeBody(); 
	}
};

class Slime : public ZZTObject {
public:
	Slime() {
		m_shape = '*';
		m_name = "slime";
		m_flags = F_OBJECT;
		m_rate = m_counter = 4;
		m_cycle = 3;
	}

	~Slime() { }
	int type() { return ZZT_SLIME; }
	ZZTObject *alloc() {
		return new Slime();
	}
	void create();
	void update();
	void message(ZZTObject *them, std::string msg);
	void setParam(int arg, unsigned char val) {
		if(arg==2)
			m_rate = val;
	}
	unsigned char param(int arg) {
		if(arg==1)
			return m_rate;
		else
			return 0;
	}

	void addEditWidgets(TUIWindow *w) {
		w->addWidget(new TUISlider("Rate                 ",&m_rate));
	}

protected:
	int m_rate, m_counter;
};
