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

void Pusher::update() {
  if(m_step.x==1) move(RIGHT);
  if(m_step.y==1) move(DOWN);
  if(m_step.x==-1) move(LEFT);
  if(m_step.y==-1) move(UP);
	//zm->setTune("t--f");
	//zm->start();
}

void Pusher::create() {
  if(m_step.x==1) m_shape=0x10;
  if(m_step.y==1) m_shape=0x1f;
  if(m_step.x==-1) m_shape=0x11;
  if(m_step.y==-1) m_shape=0x1e;
}