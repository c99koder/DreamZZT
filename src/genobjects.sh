#!/bin/sh

objlist="PLAYER OBJECT PASSAGE SLIDER_NS SLIDER_EW PUSHER \
KEY GEM TORCH AMMO SCROLL ENERGIZER \
DOOR SPINNER_CW SPINNER_CCW BOMB EXPLOSION BULLET DUPLICATOR TRANSPORTER \
BOULDER EMPTY BREAKABLE LINE SOLID NORMAL FAKE FOREST INVISIBLE WATER BLINK HORIZONTAL_BLINK VERTICAL_BLINK \
BLUE_TEXT GREEN_TEXT CYAN_TEXT RED_TEXT PURPLE_TEXT YELLOW_TEXT WHITE_TEXT \
LION BEAR RUFFIAN TIGER SPINNING_GUN"

echo "void objects_init() {" > objdb 
echo "  //Generated on `date` by `whoami`" >> objdb 
echo "  int x;
  for(x=0;x<ZZT_TYPE_COUNT;x++) {
    object_templates[x].create=NULL;
    object_templates[x].update=NULL;
    object_templates[x].message=NULL;
    object_templates[x].shape=' ';
    object_templates[x].fg=15;
    object_templates[x].bg=0;
    object_templates[x].name=NULL;
		 object_templates[x].flags=F_EMPTY;
  }
" >> objdb;
for i in $objlist; do echo "  //$i" >> objdb 
echo "  object_templates[ZZT_$i].update=ZZT_${i}_UPDATE;" >> objdb 
echo "  object_templates[ZZT_$i].message=ZZT_${i}_MESSAGE;" >> objdb 
echo "  object_templates[ZZT_$i].create=ZZT_${i}_CREATE;" >> objdb 
echo "  object_templates[ZZT_$i].shape=ZZT_${i}_SHAPE;" >> objdb 
echo "  object_templates[ZZT_$i].fg=15;" >> objdb 
echo "  object_templates[ZZT_$i].bg=0;" >> objdb 
echo "  object_templates[ZZT_$i].name=NULL;" >> objdb 
echo "  set_name(&object_templates[ZZT_$i],ZZT_${i}_NAME);" >> objdb 
echo "  object_templates[ZZT_$i].flags=ZZT_${i}_FLAGS;" >> objdb 
done 
echo "}" >> objdb
