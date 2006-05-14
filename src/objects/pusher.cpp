/*
 *  pusher.cpp
 *  Pusher object
 *
 *  Copyright 2000 - 2006 Sam Steele. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/hid.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Tiki;
using namespace Tiki::Hid;
using namespace Tiki::Audio;

#include "object.h"
#include "sound.h"

extern ZZTMusicStream *zm;

msg_handler pusher_message(struct object *me, struct object *them, char *message) {
  return 0;
}

update_handler pusher_update(struct object *me) {
  if(me->xstep==1) move(me,RIGHT);
  if(me->ystep==1) move(me,DOWN);
  if(me->xstep==-1) move(me,LEFT);
  if(me->ystep==-1) move(me,UP);
	//zm->setTune("t--f");
	//zm->start();
  return 0;
}

create_handler pusher_create(struct object *me) {
  if(me->xstep==65535) me->xstep=-1;
  if(me->ystep==65535) me->ystep=-1;
  if(me->xstep==1) me->shape=0x10;
  if(me->ystep==1) me->shape=0x1f;
  if(me->xstep==-1) me->shape=0x11;
  if(me->ystep==-1) me->shape=0x1e;
  return 0;
}