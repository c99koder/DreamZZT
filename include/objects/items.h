#include "object.h"

#define ZZT_ENERGIZER_SHAPE 0x7F
#define ZZT_ENERGIZER_NAME "energizer"
#define ZZT_ENERGIZER_FG 6
#define ZZT_ENERGIZER_BG 0
#define ZZT_ENERGIZER_FLAGS F_ITEM
#define ZZT_ENERGIZER_CLASS Inventory

#define ZZT_KEY_SHAPE 0x0c
#define ZZT_KEY_NAME "key"
#define ZZT_KEY_FG 4
#define ZZT_KEY_BG 0
#define ZZT_KEY_FLAGS F_PUSHABLE | F_ITEM
#define ZZT_KEY_CLASS Inventory

#define ZZT_GEM_SHAPE 0x04
#define ZZT_GEM_NAME "gem"
#define ZZT_GEM_FG 4
#define ZZT_GEM_BG 0
#define ZZT_GEM_FLAGS F_PUSHABLE | F_ITEM
#define ZZT_GEM_CLASS Inventory

#define ZZT_TORCH_SHAPE 0x9d
#define ZZT_TORCH_NAME "torch"
#define ZZT_TORCH_FG 4
#define ZZT_TORCH_BG 0
#define ZZT_TORCH_FLAGS F_ITEM | F_GLOW
#define ZZT_TORCH_CLASS Inventory

#define ZZT_AMMO_SHAPE 0x84
#define ZZT_AMMO_NAME "ammo"
#define ZZT_AMMO_FG 4
#define ZZT_AMMO_BG 0
#define ZZT_AMMO_FLAGS F_ITEM
#define ZZT_AMMO_CLASS Inventory

#define ZZT_DOOR_SHAPE 0x0c
#define ZZT_DOOR_NAME "door"
#define ZZT_DOOR_FG 4
#define ZZT_DOOR_BG 0
#define ZZT_DOOR_FLAGS F_NONE
#define ZZT_DOOR_CLASS Inventory

class Inventory : public ZZTObject {
public:
	Inventory(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) { }
	void create();
	void message(ZZTObject *them, std::string msg);
};
