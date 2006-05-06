//TELEPORTER
#define ZZT_TRANSPORTER_UPDATE (update_handler)teleport_update
#define ZZT_TRANSPORTER_MESSAGE (msg_handler)teleport_message
#define ZZT_TRANSPORTER_CREATE (create_handler)teleport_create
#define ZZT_TRANSPORTER_SHAPE 0xE8
#define ZZT_TRANSPORTER_NAME "transporter"
#define ZZT_TRANSPORTER_FLAGS F_OBJECT
msg_handler teleport_message(struct object *me, struct object *them, char *message);
update_handler teleport_update(struct object *me);
create_handler teleport_create(struct object *me);

//SCROLL
#define ZZT_SCROLL_UPDATE (update_handler)scroll_update
#define ZZT_SCROLL_MESSAGE (msg_handler)scroll_message
#define ZZT_SCROLL_CREATE NULL
#define ZZT_SCROLL_SHAPE 0xE8
#define ZZT_SCROLL_NAME "scroll"
#define ZZT_SCROLL_FLAGS F_ITEM|F_OBJECT
msg_handler scroll_message(struct object *me, struct object *them, char *message);
update_handler scroll_update(struct object *me);

//BOMB
#define ZZT_BOMB_UPDATE (update_handler)bomb_update
#define ZZT_BOMB_MESSAGE (msg_handler)bomb_message
#define ZZT_BOMB_CREATE (create_handler)bomb_create
#define ZZT_BOMB_SHAPE 0x0B
#define ZZT_BOMB_NAME "bomb"
#define ZZT_BOMB_FLAGS F_NONE
msg_handler bomb_message(struct object *me, struct object *them, char *message);
update_handler bomb_update(struct object *me);
create_handler bomb_create(struct object *me);

//EXPLOSION
#define ZZT_EXPLOSION_UPDATE (update_handler)explosion_update
#define ZZT_EXPLOSION_MESSAGE (msg_handler)NULL
#define ZZT_EXPLOSION_CREATE (create_handler)explosion_create
#define ZZT_EXPLOSION_SHAPE 177
#define ZZT_EXPLOSION_NAME "explosion"
#define ZZT_EXPLOSION_FLAGS F_NONE
update_handler explosion_update(struct object *me);
create_handler explosion_create(struct object *me);

//PLAYER
#define ZZT_PLAYER_UPDATE (update_handler)player_update
#define ZZT_PLAYER_MESSAGE (msg_handler)player_message
#define ZZT_PLAYER_CREATE (create_handler)player_create
#define ZZT_PLAYER_SHAPE 1
#define ZZT_PLAYER_NAME "player"
#define ZZT_PLAYER_FLAGS F_PUSHABLE | F_GLOW |F_OBJECT
update_handler player_update(struct object *me);
create_handler player_create(struct object *me);
msg_handler player_message(struct object *me, struct object *them, char *message);

//BULLET
#define ZZT_BULLET_UPDATE (update_handler)bullet_update
#define ZZT_BULLET_MESSAGE (msg_handler)bullet_message
#define ZZT_BULLET_CREATE NULL
#define ZZT_BULLET_SHAPE (unsigned char)249
#define ZZT_BULLET_NAME "bullet"
#define ZZT_BULLET_FLAGS F_OBJECT
#define BULLET_DAMAGE 10
update_handler bullet_update(struct object *me);
msg_handler bullet_message(struct object *me, struct object *them, char *message);
void shoot(struct object *owner, enum direction dir);

//PUSHER
#define ZZT_PUSHER_UPDATE (update_handler)pusher_update
#define ZZT_PUSHER_MESSAGE (msg_handler)pusher_message
#define ZZT_PUSHER_CREATE (create_handler)pusher_create
#define ZZT_PUSHER_SHAPE 227
#define ZZT_PUSHER_NAME "pusher"
#define ZZT_PUSHER_FLAGS F_OBJECT
msg_handler pusher_message(struct object *me, struct object *them, char *message);
update_handler pusher_update(struct object *me);
create_handler pusher_create(struct object *me);

//SLIDER NS
#define ZZT_SLIDER_NS_UPDATE NULL;
#define ZZT_SLIDER_NS_MESSAGE NULL
#define ZZT_SLIDER_NS_CREATE NULL;
#define ZZT_SLIDER_NS_SHAPE 0x12
#define ZZT_SLIDER_NS_NAME "sliderns"
#define ZZT_SLIDER_NS_FLAGS F_PUSHABLE

//SLIDER EW
#define ZZT_SLIDER_EW_UPDATE NULL;
#define ZZT_SLIDER_EW_MESSAGE NULL
#define ZZT_SLIDER_EW_CREATE NULL;
#define ZZT_SLIDER_EW_SHAPE 0x1D
#define ZZT_SLIDER_EW_NAME "sliderew"
#define ZZT_SLIDER_EW_FLAGS F_PUSHABLE

//CONVEYER CW
#define ZZT_SPINNER_CW_UPDATE (update_handler)spinner_update
#define ZZT_SPINNER_CW_MESSAGE (msg_handler)NULL
#define ZZT_SPINNER_CW_CREATE (create_handler)NULL
#define ZZT_SPINNER_CW_SHAPE '/'
#define ZZT_SPINNER_CW_NAME "conveyercw"
#define ZZT_SPINNER_CW_FLAGS F_NONE

//CONVEYER CCW
#define ZZT_SPINNER_CCW_UPDATE (update_handler)spinner_update
#define ZZT_SPINNER_CCW_MESSAGE (msg_handler)NULL
#define ZZT_SPINNER_CCW_CREATE (create_handler)NULL
#define ZZT_SPINNER_CCW_SHAPE '\\'
#define ZZT_SPINNER_CCW_NAME "conveyerccw"
#define ZZT_SPINNER_CCW_FLAGS F_NONE
update_handler spinner_update(struct object *me);

//OBJECT
#define ZZT_OBJECT_UPDATE (update_handler)zztoop_update
#define ZZT_OBJECT_MESSAGE (msg_handler)zztoop_message
#define ZZT_OBJECT_CREATE (create_handler)zztoop_create
#define ZZT_OBJECT_SHAPE 'O'
#define ZZT_OBJECT_NAME "object"
#define ZZT_OBJECT_FLAGS F_OBJECT
update_handler zztoop_update(struct object *me);
create_handler zztoop_create(struct object *me);
msg_handler zztoop_message(struct object *me, struct object *them, char *message);

//PASSAGE
#define ZZT_PASSAGE_UPDATE NULL
#define ZZT_PASSAGE_MESSAGE (msg_handler)passage_message
#define ZZT_PASSAGE_CREATE (create_handler)passage_create
#define ZZT_PASSAGE_SHAPE 0xF0
#define ZZT_PASSAGE_NAME "passage"
#define ZZT_PASSAGE_FLAGS F_GLOW|F_OBJECT
#define PASSAGE_DEST arg3
msg_handler passage_message(struct object *me, struct object *them, char *message);
create_handler passage_create(struct object *me);

//DUPLICATOR
#define ZZT_DUPLICATOR_UPDATE (update_handler)duplicator_update
#define ZZT_DUPLICATOR_MESSAGE NULL
#define ZZT_DUPLICATOR_CREATE (create_handler)duplicator_create
#define ZZT_DUPLICATOR_SHAPE '.'
#define ZZT_DUPLICATOR_NAME "duplicator"
#define ZZT_DUPLICATOR_FLAGS F_OBJECT
create_handler duplicator_create(struct object *me);
update_handler duplicator_update(struct object *me);
