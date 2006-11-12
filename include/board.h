/* board.h - World load / save and utility functions
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

#include "object.h"

#define BOARD_X 60
#define BOARD_Y 25

struct board_data {
  ZZTObject *obj;
  ZZTObject *under;
};

struct board_info_node {
  char title[34];
  int num;
  char maxshots;
  char dark;
  char board_up;
  char board_down;
  char board_left;
  char board_right;
  char reenter;
  short int time;
	char animatedWater;
	char message[60];
	int msgcount;
	struct board_data board[BOARD_X][BOARD_Y];
  struct board_info_node *next;
};

struct zzt_string {
	unsigned char len;
	unsigned char string[20];
};

struct world_header {
  unsigned short int magic;
  unsigned short int board_count;
  unsigned short int ammo;
  unsigned short int gems;
  char keys[7];
  unsigned short int health;
  unsigned short int start;
  unsigned short int torches;
  unsigned short int torch_cycle;
  unsigned short int energizer_cycle;
  unsigned short int pad1;
  unsigned short int score;
  struct zzt_string title;
  struct zzt_string flag[10];
  unsigned short int time;
	unsigned char saved;
};

int load_zzt(char *filename, int titleonly);
void save_game(char *filename);
void switch_board(int num);
void update_brd();
void free_world();
void new_world();
void new_board(char *title);
void put(ZZTObject *o);
int is_empty(struct board_info_node *curbrd, int x, int y, bool ignorePlayer=false);
int block_bg(int x, int y);
void draw_block(int x, int y);
void draw_board();
struct world_header *get_world();
unsigned char block_code(int x, int y);
void remove_from_board(struct board_info_node *brd, ZZTObject *me);
struct board_info_node *get_board(int num);
struct board_info_node *get_board_list();
struct board_data *get_block_by_type(int type, int &x, int &y);
ZZTObject *get_obj_by_type(struct board_info_node *board, int type);
ZZTObject *get_obj_by_color(struct board_info_node *board, int type, int color);
ZZTObject *get_obj_by_color(struct board_info_node *board, int type, int fg, int bg);
struct board_info_node *get_current_board();
int board_up();
int board_down();
int board_left();
int board_right();
void render();