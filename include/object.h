#ifndef _OBJECT_H
#define _OBJECT_H

#include "dirxn.h"

typedef void (*update_handler)(struct object *me);
typedef void (*msg_handler)(struct object *me, struct object *them, char *msg); //universal messaging
typedef void (*create_handler)(struct object *me);

struct object {
  char x;
  char y;
  short xstep;
  short ystep;
  short cycle;
  int tick;
  int fg;
  int bg;
  int *color;
  int board;
  int heading;
  char type;
  unsigned char shape;
  char *name; 
  char arg1;
  char arg2;
  char arg3;
  int arg4;
  int arg5;
  int flags;
  char *prog;
  short proglen;
  short progpos;
  int updated;
  void (*update)(struct object *me);
  void (*message)(struct object *me, struct object *them, char *message);
  void (*create)(struct object *me);
};

void null_handler();
update_handler default_update();
msg_handler default_msg();

#define ZZT_TYPE_COUNT 0x3e
#define ZZT_EMPTY 0x00
#define ZZT_EDGE 0x01
#define ZZT_EXPLOSION 0x02
#define ZZT_PLAYER 0x04
#define ZZT_AMMO 0x05
#define ZZT_TORCH 0x06
#define ZZT_GEM 0x07
#define ZZT_KEY 0x08
#define ZZT_DOOR 0x09
#define ZZT_SCROLL 0x0A
#define ZZT_PASSAGE 0x0B
#define ZZT_DUPLICATOR 0x0C
#define ZZT_BOMB 0x0D
#define ZZT_ENERGIZER 0x0E
#define ZZT_STAR 0x0F
#define ZZT_SPINNER_CW 0x10
#define ZZT_SPINNER_CCW 0x11
#define ZZT_BULLET 0x12
#define ZZT_WATER 0x13
#define ZZT_FOREST 0x14
#define ZZT_SOLID 0x15
#define ZZT_NORMAL 0x16
#define ZZT_BREAKABLE 0x17
#define ZZT_BOULDER 0x18
#define ZZT_SLIDER_NS 0x19
#define ZZT_SLIDER_EW 0x1A
#define ZZT_FAKE 0x1B
#define ZZT_INVISIBLE 0x1C
#define ZZT_BLINK 0x1D
#define ZZT_TRANSPORTER 0x1E
#define ZZT_LINE 0x1F
#define ZZT_RICOCHET 0x20
#define ZZT_HORIZONTAL_BLINK 0x21
#define ZZT_BEAR 0x22
#define ZZT_RUFFIAN 0x23
#define ZZT_OBJECT 0x24
#define ZZT_SLIME 0x25
#define ZZT_SHARK 0x26
#define ZZT_SPINNING_GUN 0x27
#define ZZT_PUSHER 0x28
#define ZZT_LION 0x29
#define ZZT_TIGER 0x2A
#define ZZT_VERTICAL_BLINK 0x2B
#define ZZT_CENTIPEDE_HEAD 0x2C
#define ZZT_CENTIPEDE_BODY 0x2D
#define ZZT_BLUE_TEXT 0x2F
#define ZZT_GREEN_TEXT 0x30
#define ZZT_CYAN_TEXT 0x31
#define ZZT_RED_TEXT 0x32
#define ZZT_PURPLE_TEXT 0x33
#define ZZT_YELLOW_TEXT 0x34
#define ZZT_WHITE_TEXT 0x35
/*#define ZZT_EMPTY 0x36   | (set in colour byte) | White blinking text
#define ZZT_EMPTY 0x37   | (set in colour byte) | Blue blinking text
#define ZZT_EMPTY 0x38   | (set in colour byte) | Green blinking text
#define ZZT_EMPTY 0x39   | (set in colour byte) | Cyan blinking text
#define ZZT_EMPTY 0x3A   | (set in colour byte) | Red blinking text
#define ZZT_EMPTY 0x3B   | (set in colour byte) | Purple blinking text
#define ZZT_EMPTY 0x3C   | (set in colour byte) | Yellow blinking text
#define ZZT_EMPTY 0x3D   | (set in colour byte) | Grey blinking text*/

#define F_NONE 0
#define F_EMPTY 1
#define F_PUSHABLE 2
#define F_DELETED 4
#define F_SLEEPING 8
#define F_ITEM 16
#define F_ENEMY 32
#define F_GLOW 64
#define F_OBJECT 128

#include "objects/objects.h"
#include "objects/terrain.h"
#include "objects/items.h"
#include "objects/enemy.h"

void objects_init();
struct object *create_object(int type, int x, int y);
void set_name(struct object *obj, char *name);
struct object *get_obj_by_id(int id);
struct object *get_obj_by_color(struct board_info_node *board, int type, int color);
int str_to_obj(char *str);
int str_to_color(char *color);
char *int_to_color(int col);
#endif
