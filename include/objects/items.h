#include "object.h"

#define ZZT_ENERGIZER_UPDATE NULL
#define ZZT_ENERGIZER_MESSAGE (msg_handler)inv_message
#define ZZT_ENERGIZER_CREATE (create_handler)inv_create
#define ZZT_ENERGIZER_SHAPE 0x7F
#define ZZT_ENERGIZER_NAME "energizer"
#define ZZT_ENERGIZER_FG 6
#define ZZT_ENERGIZER_BG 0
#define ZZT_ENERGIZER_FLAGS F_ITEM

#define ZZT_KEY_UPDATE NULL
#define ZZT_KEY_MESSAGE (msg_handler)inv_message
#define ZZT_KEY_CREATE (create_handler)inv_create
#define ZZT_KEY_SHAPE 0x0c
#define ZZT_KEY_NAME "key"
#define ZZT_KEY_FG 4
#define ZZT_KEY_BG 0
#define ZZT_KEY_FLAGS F_PUSHABLE | F_ITEM

#define ZZT_GEM_UPDATE NULL
#define ZZT_GEM_MESSAGE (msg_handler)inv_message
#define ZZT_GEM_CREATE (create_handler)inv_create
#define ZZT_GEM_SHAPE 0x04
#define ZZT_GEM_NAME "gem"
#define ZZT_GEM_FG 4
#define ZZT_GEM_BG 0
#define ZZT_GEM_FLAGS F_PUSHABLE | F_ITEM

#define ZZT_TORCH_UPDATE NULL
#define ZZT_TORCH_MESSAGE (msg_handler)inv_message
#define ZZT_TORCH_CREATE (create_handler)inv_create
#define ZZT_TORCH_SHAPE 0x9d
#define ZZT_TORCH_NAME "torch"
#define ZZT_TORCH_FG 4
#define ZZT_TORCH_BG 0
#define ZZT_TORCH_FLAGS F_ITEM | F_GLOW

#define ZZT_AMMO_UPDATE NULL
#define ZZT_AMMO_MESSAGE (msg_handler)inv_message
#define ZZT_AMMO_CREATE (create_handler)inv_create
#define ZZT_AMMO_SHAPE 0x84
#define ZZT_AMMO_NAME "ammo"
#define ZZT_AMMO_FG 4
#define ZZT_AMMO_BG 0
#define ZZT_AMMO_FLAGS F_ITEM

#define ZZT_DOOR_UPDATE NULL
#define ZZT_DOOR_MESSAGE (msg_handler)inv_message
#define ZZT_DOOR_CREATE (create_handler)inv_create
#define ZZT_DOOR_SHAPE 0x0c
#define ZZT_DOOR_NAME "door"
#define ZZT_DOOR_FG 4
#define ZZT_DOOR_BG 0
#define ZZT_DOOR_FLAGS F_NONE

msg_handler inv_message(struct object *me, struct object *them, char *message);
create_handler inv_create(struct object *me);
