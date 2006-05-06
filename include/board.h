#include "object.h"

#define BOARD_X 60
#define BOARD_Y 25

struct board_data {
  struct object *obj;
  struct object *under;
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
	char message[60];
	int msgcount;
	struct board_data board[BOARD_X][BOARD_Y];
  struct board_info_node *next;
};

struct zzt_string {
	unsigned char len;
	unsigned char string[20];
} __attribute ((packed));

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
} __attribute ((packed));

int load_zzt(char *filename, int titleonly);
void switch_board(int num);
void update_brd();
void free_world();
int is_empty(struct board_info_node *curbrd, int x, int y);
int block_bg(int x, int y);
void draw_block(int x, int y);
void draw_board();
struct world_header *get_world();
unsigned char block_code(int x, int y);
void remove_from_board(struct board_info_node *brd, struct object *me);
struct board_info_node *get_board(int num);
struct board_info_node *get_board_list();
struct object *get_obj_by_type(struct board_info_node *board, int type);
struct board_data *get_block_by_type(char *type, int *x, int *y);
struct board_info_node *get_current_board();
int board_up();
int board_down();
int board_left();
int board_right();
