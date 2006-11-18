/* items.h - ZZT Items
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

#define ZZT_ENERGIZER_SHAPE 0x7F
#define ZZT_ENERGIZER_NAME "energizer"
#define ZZT_ENERGIZER_FLAGS F_ITEM
#define ZZT_ENERGIZER_CLASS Inventory

#define ZZT_KEY_SHAPE 0x0c
#define ZZT_KEY_NAME "key"
#define ZZT_KEY_FLAGS F_PUSHABLE | F_ITEM
#define ZZT_KEY_CLASS Inventory

#define ZZT_GEM_SHAPE 0x04
#define ZZT_GEM_NAME "gem"
#define ZZT_GEM_FLAGS F_PUSHABLE | F_ITEM
#define ZZT_GEM_CLASS Inventory

#define ZZT_TORCH_SHAPE 0x9d
#define ZZT_TORCH_NAME "torch"
#define ZZT_TORCH_FLAGS F_ITEM | F_GLOW
#define ZZT_TORCH_CLASS Inventory

#define ZZT_AMMO_SHAPE 0x84
#define ZZT_AMMO_NAME "ammo"
#define ZZT_AMMO_FLAGS F_ITEM
#define ZZT_AMMO_CLASS Inventory

#define ZZT_DOOR_SHAPE 0x08
#define ZZT_DOOR_NAME "door"
#define ZZT_DOOR_FLAGS F_NONE
#define ZZT_DOOR_CLASS Inventory

class Inventory : public ZZTObject {
public:
	Inventory(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { }
	void create();
	void message(ZZTObject *them, std::string msg);
};
