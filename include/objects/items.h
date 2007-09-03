/* items.h - ZZT Items
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

class Inventory : public ZZTObject {
public:
	Inventory() {
	}
	virtual bool get() { return false; }
	virtual void update() { }
	void message(ZZTObject *them, std::string msg);
};

class Energizer : public Inventory {
public:
	Energizer() {
		m_shape = 0x07;
		m_flags = F_ITEM;
		m_name = "energizer";
	}
	ZZTObject *alloc() {
		return new Energizer();
	}
	bool get();
	int type() { return ZZT_ENERGIZER; }
};

class Key : public Inventory {
public:
	Key() {
		m_shape = 0x0C;
		m_flags = F_PUSHABLE | F_ITEM;
		m_name = "key";
	}
	ZZTObject *alloc() {
		return new Key();
	}
	bool get();
	int type() { return ZZT_KEY; }
};

class Gem : public Inventory {
public:
	Gem() {
		m_modelName = "gem.amf";
		m_shape = 0x04;
		m_flags = F_PUSHABLE | F_ITEM;
		m_name = "gem";
	}
	ZZTObject *alloc() {
		return new Gem();
	}
	bool get();
	int type() { return ZZT_GEM; }
};


class Torch : public Inventory {
public:
	Torch() {
		m_modelName = "torch.amf";
		m_fg = YELLOW;
		m_bg = BLACK;
		m_shape = 0x9D;
		m_flags = F_PUSHABLE | F_ITEM;
		m_name = "torch";
	}
	ZZTObject *alloc() {
		return new Torch();
	}	
	bool get();
	int type() { return ZZT_TORCH; }
};

class Ammo : public Inventory {
public:
	Ammo() {
		m_fg = CYAN;
		m_bg = BLACK;
		m_shape = 0x84;
		m_flags = F_ITEM;
		m_name = "ammo";
	}
	ZZTObject *alloc() {
		return new Ammo();
	}
	bool get();
	int type() { return ZZT_AMMO; }
};

class Door : public Inventory {
public:
	Door() {
		m_fg = WHITE | HIGH_INTENSITY;
		m_color=&m_bg;
		m_shape = 0x08;
		m_flags = F_ITEM;
		m_name = "door";
	}
	ZZTObject *alloc() {
		return new Door();
	}
	bool get();
	int type() { return ZZT_DOOR; }
};

class Stone : public Inventory {
public:
	Stone() {
		m_fg = 9 + (rand() % 8);
		m_bg = BLACK;
		m_shape = 'A' + (rand() % 26);
		m_flags = F_ITEM;
		m_name = "stone";
	}
	ZZTObject *alloc() {
		return new Stone();
	}
	bool get();
	void update();
	int type() { return SZT_STONE; }
};
