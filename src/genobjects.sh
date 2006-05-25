#!/bin/sh

objlist="PLAYER OBJECT PASSAGE SLIDER_NS SLIDER_EW PUSHER \
KEY GEM TORCH AMMO SCROLL ENERGIZER \
DOOR CONVEYER_CW CONVEYER_CCW BOMB EXPLOSION BULLET DUPLICATOR TRANSPORTER \
BOULDER EMPTY BREAKABLE LINE SOLID NORMAL FAKE FOREST INVISIBLE WATER BLINK HORIZONTAL_BLINK VERTICAL_BLINK \
BLUE_TEXT GREEN_TEXT CYAN_TEXT RED_TEXT PURPLE_TEXT YELLOW_TEXT WHITE_TEXT \
LION BEAR RUFFIAN TIGER SPINNING_GUN"

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