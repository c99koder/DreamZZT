#!/bin/sh

# genobjects.sh - Generate the object database
# Copyright (c) 2000-2006 Sam Steele
#
# This file is part of DreamZZT.
#
# DreamZZT is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2
# as published by the Free Software Foundation.
#
# DreamZZT is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
# 

objlist="PLAYER OBJECT PASSAGE SLIDER_NS SLIDER_EW PUSHER \
KEY GEM TORCH AMMO SCROLL ENERGIZER \
DOOR CONVEYER_CW CONVEYER_CCW BOMB EXPLOSION BULLET DUPLICATOR TRANSPORTER \
BOULDER EMPTY BREAKABLE LINE SOLID NORMAL FAKE FOREST INVISIBLE WATER BLINK HORIZONTAL_BLINK VERTICAL_BLINK \
BLUE_TEXT GREEN_TEXT CYAN_TEXT RED_TEXT PURPLE_TEXT YELLOW_TEXT WHITE_TEXT \
LION SHARK BEAR RUFFIAN TIGER SPINNING_GUN CENTIPEDE_HEAD CENTIPEDE_BODY"

echo "
//Generated on `date` by `whoami` 

int str_to_obj(std::string str) { 
  if(str == \"\") {
    return -1;" > objdb;
for i in $objlist; do 
echo "  } else if(str.find(ZZT_${i}_NAME) == 0) {" >> objdb 
echo "    return ZZT_$i;" >> objdb 
done 
echo "  } else {
    return -1;
  }
}

ZZTObject *create_object(int type, int x, int y) {
  if(type == -1) {
    return NULL;
" >> objdb
for i in $objlist; do 
echo "  } else if(type == ZZT_${i}) {" >> objdb 
echo "    return new ZZT_${i}_CLASS(type, x, y, ZZT_${i}_SHAPE, ZZT_${i}_FLAGS, ZZT_${i}_NAME);" >> objdb 
done 
echo "  } else {
    return NULL;
  }
}

ZZTObject *create_copy(ZZTObject *source) {
  if(source == NULL) {
    return NULL;
" >> objdb
for i in $objlist; do 
echo "  } else if(source->getType() == ZZT_${i}) {" >> objdb 
echo "    return new ZZT_${i}_CLASS(*(ZZT_${i}_CLASS *)source);" >> objdb 
done 
echo "  } else {
    return NULL;
  }
}" >> objdb