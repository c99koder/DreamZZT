//BOULDER
#define ZZT_BOULDER_UPDATE NULL
#define ZZT_BOULDER_MESSAGE NULL
#define ZZT_BOULDER_CREATE NULL
#define ZZT_BOULDER_SHAPE 0xFE
#define ZZT_BOULDER_NAME "boulder"
#define ZZT_BOULDER_FLAGS F_PUSHABLE

//LINE
#define ZZT_LINE_UPDATE NULL
#define ZZT_LINE_MESSAGE NULL
#define ZZT_LINE_CREATE NULL
#define ZZT_LINE_SHAPE 206
#define ZZT_LINE_NAME "line"
#define ZZT_LINE_FLAGS F_NONE

//NORMAL
#define ZZT_NORMAL_UPDATE NULL
#define ZZT_NORMAL_MESSAGE NULL
#define ZZT_NORMAL_CREATE NULL
#define ZZT_NORMAL_SHAPE 0xB2
#define ZZT_NORMAL_NAME "normal"
#define ZZT_NORMAL_FLAGS F_NONE

//SOLID
#define ZZT_SOLID_UPDATE NULL
#define ZZT_SOLID_MESSAGE NULL
#define ZZT_SOLID_CREATE NULL
#define ZZT_SOLID_SHAPE 0xDB
#define ZZT_SOLID_NAME "solid"
#define ZZT_SOLID_FLAGS F_NONE

//EMPTY
#define ZZT_EMPTY_UPDATE NULL
#define ZZT_EMPTY_MESSAGE NULL
#define ZZT_EMPTY_CREATE (create_handler)terrain_create
#define ZZT_EMPTY_SHAPE 32
#define ZZT_EMPTY_NAME "empty"
#define ZZT_EMPTY_FLAGS F_EMPTY
create_handler terrain_create(struct object *me);

//FAKE
#define ZZT_FAKE_UPDATE NULL
#define ZZT_FAKE_MESSAGE NULL
#define ZZT_FAKE_CREATE NULL
#define ZZT_FAKE_SHAPE 0xB2
#define ZZT_FAKE_NAME "fake"
#define ZZT_FAKE_FLAGS F_EMPTY

//BREAKABLE
#define ZZT_BREAKABLE_UPDATE NULL
#define ZZT_BREAKABLE_MESSAGE NULL
#define ZZT_BREAKABLE_CREATE NULL
#define ZZT_BREAKABLE_SHAPE 0xB1
#define ZZT_BREAKABLE_NAME "breakable"
#define ZZT_BREAKABLE_FLAGS F_NONE

//FOREST
#define ZZT_FOREST_UPDATE NULL
#define ZZT_FOREST_MESSAGE (msg_handler)terrain_message
#define ZZT_FOREST_CREATE NULL
#define ZZT_FOREST_SHAPE 0xb0
#define ZZT_FOREST_NAME "forest"
#define ZZT_FOREST_FLAGS F_NONE

//WATER
#define ZZT_WATER_UPDATE (update_handler)water_update
#define ZZT_WATER_MESSAGE (msg_handler)terrain_message
#define ZZT_WATER_CREATE (create_handler)terrain_create
#define ZZT_WATER_SHAPE 0xb1
#define ZZT_WATER_NAME "water"
#define ZZT_WATER_FLAGS F_NONE
update_handler water_update(struct object *me);

//INVISIBLE
#define ZZT_INVISIBLE_UPDATE NULL
#define ZZT_INVISIBLE_MESSAGE (msg_handler)terrain_message
#define ZZT_INVISIBLE_CREATE NULL
#define ZZT_INVISIBLE_SHAPE ' '
#define ZZT_INVISIBLE_NAME "invisible"
#define ZZT_INVISIBLE_FLAGS F_NONE
msg_handler terrain_message(struct object *me, struct object *them, char *message);

//BLINK
#define ZZT_BLINK_UPDATE (update_handler)blink_update
#define ZZT_BLINK_MESSAGE NULL
#define ZZT_BLINK_CREATE (create_handler)blink_create
#define ZZT_BLINK_SHAPE 206
#define ZZT_BLINK_NAME "blink"
#define ZZT_BLINK_FLAGS F_OBJECT
create_handler blink_create(struct object *me);
update_handler blink_update(struct object *me);

//VERTICAL LASER
#define ZZT_VERTICAL_BLINK_UPDATE (update_handler)laser_update
#define ZZT_VERTICAL_BLINK_MESSAGE NULL
#define ZZT_VERTICAL_BLINK_CREATE NULL
#define ZZT_VERTICAL_BLINK_SHAPE 0xBA
#define ZZT_VERTICAL_BLINK_NAME "vertical blink"
#define ZZT_VERTICAL_BLINK_FLAGS F_NONE
update_handler laser_update(struct object *me);

//HORIZONTAL LASER
#define ZZT_HORIZONTAL_BLINK_UPDATE (update_handler)laser_update
#define ZZT_HORIZONTAL_BLINK_MESSAGE NULL
#define ZZT_HORIZONTAL_BLINK_CREATE NULL
#define ZZT_HORIZONTAL_BLINK_SHAPE 0xCD
#define ZZT_HORIZONTAL_BLINK_NAME "horizontal blink"
#define ZZT_HORIZONTAL_BLINK_FLAGS F_NONE

//BLUE
#define ZZT_BLUE_TEXT_UPDATE NULL
#define ZZT_BLUE_TEXT_MESSAGE NULL
#define ZZT_BLUE_TEXT_CREATE (create_handler)text_create
#define ZZT_BLUE_TEXT_SHAPE 0xB2
#define ZZT_BLUE_TEXT_NAME "text"
#define ZZT_BLUE_TEXT_FLAGS F_NONE

//GREEN
#define ZZT_GREEN_TEXT_UPDATE NULL
#define ZZT_GREEN_TEXT_MESSAGE NULL
#define ZZT_GREEN_TEXT_CREATE (create_handler)text_create
#define ZZT_GREEN_TEXT_SHAPE 0xB2
#define ZZT_GREEN_TEXT_NAME "text"
#define ZZT_GREEN_TEXT_FLAGS F_NONE

//CYAN
#define ZZT_CYAN_TEXT_UPDATE NULL
#define ZZT_CYAN_TEXT_MESSAGE NULL
#define ZZT_CYAN_TEXT_CREATE (create_handler)text_create
#define ZZT_CYAN_TEXT_SHAPE 0xB2
#define ZZT_CYAN_TEXT_NAME "text"
#define ZZT_CYAN_TEXT_FLAGS F_NONE

//RED
#define ZZT_RED_TEXT_UPDATE NULL
#define ZZT_RED_TEXT_MESSAGE NULL
#define ZZT_RED_TEXT_CREATE (create_handler)text_create
#define ZZT_RED_TEXT_SHAPE 0xB2
#define ZZT_RED_TEXT_NAME "text"
#define ZZT_RED_TEXT_FLAGS F_NONE

//PURPLE
#define ZZT_PURPLE_TEXT_UPDATE NULL
#define ZZT_PURPLE_TEXT_MESSAGE NULL
#define ZZT_PURPLE_TEXT_CREATE (create_handler)text_create
#define ZZT_PURPLE_TEXT_SHAPE 0xB2
#define ZZT_PURPLE_TEXT_NAME "text"
#define ZZT_PURPLE_TEXT_FLAGS F_NONE

//YELLOW
#define ZZT_YELLOW_TEXT_UPDATE NULL
#define ZZT_YELLOW_TEXT_MESSAGE NULL
#define ZZT_YELLOW_TEXT_CREATE (create_handler)text_create
#define ZZT_YELLOW_TEXT_SHAPE 0xB2
#define ZZT_YELLOW_TEXT_NAME "text"
#define ZZT_YELLOW_TEXT_FLAGS F_NONE

//WHITE
#define ZZT_WHITE_TEXT_UPDATE NULL
#define ZZT_WHITE_TEXT_MESSAGE NULL
#define ZZT_WHITE_TEXT_CREATE (create_handler)text_create
#define ZZT_WHITE_TEXT_SHAPE 0xB2
#define ZZT_WHITE_TEXT_NAME "text"
#define ZZT_WHITE_TEXT_FLAGS F_NONE
create_handler text_create(struct object *me);
