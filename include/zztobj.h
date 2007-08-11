#ifndef _ZZTOBJ_H
#define _ZZTOBJ_H
// We also need to know the index numbers for all items on
// the board. How else would we be able to see the difference
// between a Fake and a Normal?
// In other words..these are the ZZT coded things

// Items available through F1. 21
#define	I_EMPTY			0x00
#define	I_EDGE			0x01
#define	I_DOWN_ARROW	0x02
#define	I_RIGHT_ARROW	0x03
#define	I_PLAYER		0x04
#define	I_AMMO			0x05
#define	I_TORCH			0x06
#define	I_GEM			0x07
#define	I_KEY			0x08
#define	I_DOOR			0x09
#define	I_SCROLL		0x0a
#define	I_PASSAGE		0x0b
#define	I_DUPLICATOR	0x0c
#define	I_BOMB			0x0d
#define	I_ENERGIZER		0x0e
#define	I_LEFT_ARROW	0x0f
#define	I_CONVEYOR_C	0x10
#define	I_CONVEYOR_CC	0x11
#define	I_DOUBLE_CROSS	0x12

// Terrains available through F3. 13
#define	I_WATER			0x13
#define	I_FOREST		0x14
#define	I_SOLID			0x15
#define	I_NORMAL		0x16
#define	I_BREAKABLE		0x17
#define	I_BOULDER		0x18
#define	I_SLIDER_NS		0x19
#define	I_SLIDER_EW		0x1a
#define	I_FAKE			0x1b
#define	I_INVISIBLE		0x1c
#define	I_BLINK_WALL	0x1d
#define	I_TRANSPORTER	0x1e
#define	I_LINE			0x1f
#define	I_RICOCHET		0x20
#define	I_DOWN_TRIAN	0x21

// Creatures available through F2. 13
#define	I_BEAR			0x22
#define	I_RUFFIAN		0x23
#define	I_OBJECT		0x24
#define	I_SLIME			0x25
#define	I_SHARK			0x26
#define	I_SPINNING_GUN	0x27
#define	I_PUSHER		0x28
#define	I_LION			0x29
#define	I_TIGER			0x2a
#define	I_HEAD			0x2c
#define	I_SEGMENT		0x2d
/*
struct obj {
	int x;
	int y;
	int xstep;
	int ystep;
	int cycle;
	unsigned char code;
	unsigned char color;
	unsigned char p1;
	unsigned char p2;
	unsigned char p3;
	int p4;
	int progpos;
	unsigned char ut;
	unsigned char uc;
	int proglen;
	char *prog;
	struct obj *next;
};
 
struct obj *get_obj(int x, int y);
void set_objlist(struct obj *list);
*/
#endif

