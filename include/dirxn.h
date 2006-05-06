#ifndef _DIRECTION_H
#define _DIRECTION_H

#include "object.h"

enum direction { LEFT, RIGHT, UP, DOWN };
enum direction opposite(enum direction dir);
enum direction toward(struct object *me, struct object *them);
enum direction word_to_direction(struct object *me, int word);
int distance(struct object *me, struct object *them);
int dist_x(struct object *me, struct object *them);
int dist_y(struct object *me, struct object *them);
void move(struct object *me, enum direction d);
int try_move(struct object *me, enum direction dir);
#endif
