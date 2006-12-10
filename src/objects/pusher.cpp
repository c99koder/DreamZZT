/* pusher.cpp - Pusher object
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

#include <Tiki/tiki.h>
#include <Tiki/hid.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Tiki;
using namespace Tiki::GL;
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