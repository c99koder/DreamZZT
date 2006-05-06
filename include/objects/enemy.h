#define ZZT_LION_UPDATE (update_handler)lion_update
#define ZZT_LION_MESSAGE (msg_handler)enemy_message
#define ZZT_LION_CREATE NULL
#define ZZT_LION_SHAPE 0xea
#define ZZT_LION_NAME "lion"
#define ZZT_LION_FLAGS F_PUSHABLE|F_ENEMY|F_OBJECT

#define ZZT_TIGER_UPDATE (update_handler)tiger_update
#define ZZT_TIGER_MESSAGE (msg_handler)enemy_message
#define ZZT_TIGER_CREATE NULL
#define ZZT_TIGER_SHAPE 0xe3
#define ZZT_TIGER_NAME "tiger"
#define ZZT_TIGER_FLAGS F_PUSHABLE|F_ENEMY|F_OBJECT

#define ZZT_SPINNING_GUN_UPDATE (update_handler)gun_update
#define ZZT_SPINNING_GUN_MESSAGE NULL
#define ZZT_SPINNING_GUN_CREATE NULL
#define ZZT_SPINNING_GUN_SHAPE 0xe3
#define ZZT_SPINNING_GUN_NAME "spinning-gun"
#define ZZT_SPINNING_GUN_FLAGS F_OBJECT

update_handler gun_update(struct object *me);
update_handler lion_update(struct object *me);
update_handler tiger_update(struct object *me);
msg_handler enemy_message(struct object *me, struct object *them, char *message);
