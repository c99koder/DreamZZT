function main()
	me.name = "tutor"
	w = zzt.TUIWindow:new("DreamZZT Tutorial")
	w:buildFromString("  Welcome to the DreamZZT tutorial!  I'm\
going to introduce you to some of the\
items and concepts in typical ZZT games.\
\
$Text Windows\
\
  These little windows are used\
throughout the game to provide you with\
information.  They may also present you\
with a list of choices, like this:\
\
!part2;Select this to continue")
	w:doMenu()

	me.cycle = 20
	coroutine.yield();
	set_msg("Tutor: Sometimes messages are displayed")
	set_msg("at the bottom of your screen.")
	coroutine.yield();
	w = zzt.TUIWindow:new("DreamZZT Tutorial")
	w:buildFromString("$Basic Controls\
\
  You can use up, down, left, and right\
to control your player ().  To activate\
an object, simply walk into it.  This is\
referred to as touching and object.\
\
$Status\
\
  The status area on the right contains\
important information, such as your\
health, ammo, gems, and keys.  It also\
provides you with quick help, such as\
what button to use to fire bullets, or\
light a torch.\
\
$Health\
\
  Health is very important!  You will die\
if this reaches zero, and will have to\
start the game over or restore a saved\
game.  Remember to save often!\
\
$Ammo\
\
  This is how many bullets you have\
remaining.  You can pick up boxes of\
bullets (Ñ) which give you 15 shots each.\
Ammo can sometimes be purchased from\
stores.\
\
$Torches\
\
  Torches are used to light up dark\
rooms.  You'll have a chance to play with\
these later on in the tutorial!\
\
$Gems\
\
  Gems are sometimes used as currency in\
games.  You can usually use them to\
purchase items, bribe characters, etc.\
They also give you 1 health point when\
you pick them up!\
\
$Score\
\
  This is your score.  Try to get the\
highest score, and compete with other\
DreamZZT players worldwide with DreamZZT\
Online!\
\
$Keys\
\
  These are used to unlock doors.  We'll\
start our demonstration off with these!")
	w:doMenu()
	coroutine.yield()
	set_msg("We'll start our tutorial off with keys and")
	set_msg("doors.  Watch as I open the door!")
	me.cycle = 2
	move(DOWN) move(DOWN) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT)
	o = me:create_object(ZZT_EMPTY, RIGHT)
	zzt.board.put(o)
	move(RIGHT)
	set_msg("The cyan door is now open!")
	me.cycle = 20
	coroutine.yield()
	me.cycle = 2
	move(LEFT)
	o = me:create_object(ZZT_DOOR, RIGHT)
	o.color = CYAN
	zzt.board.put(o)
	move(LEFT) move(LEFT) move(LEFT) move(LEFT) move(UP) move(UP)
	set_msg("Now, pick up this key and open the door.")
	o = me:create_object(ZZT_KEY, DOWN)
	o.co = CYAN
	zzt.board.put(o)
	while (zzt.board.get_obj_by_color(zzt.board.current, ZZT_DOOR, CYAN)) do
		coroutine.yield()
	end
	set_msg("Great job!")
	move(DOWN) move(DOWN) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(DOWN) move(DOWN)
	me.cycle = 20
	set_msg("Now try picking up some items and")
	set_msg("watch your status area on the right!")
	set_msg("Open the blue door to continue.")
	me.cycle = 2
	while (zzt.board.get_obj_by_color(zzt.board.current, ZZT_DOOR, BLUE)) do
		coroutine.yield()
	end
	set_msg("Great work!")
	move(UP) move(UP)
	move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT)
	set_msg("Now lets play around with pushers,")
	move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT)
	set_msg("sliders, and boulders.")
	move(DOWN) move(DOWN)
	o = zzt.board.get_obj_by_name(zzt.board.current, "doorstop2")
	o:message(me, "go")
	me.cycle = 20
	set_msg("Push the slider on the right into the")
	set_msg("wall to open the door!")
	me.cycle = 2
	wait_for_message("loop3")
	move(UP) move(UP)
	move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT)
	set_msg("Great!")
	move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT)
	set_msg("Now lets try torches!")
	move(RIGHT) move(RIGHT) move(RIGHT) move(RIGHT)
	move(UP) move(UP)
	set_msg("I'm going to turn out the lights.")
	move(UP) move(UP) move(UP) move(UP) move(UP) move(UP)
	me.cycle = 20
	set_msg("Try to find the green key and open the")
	set_msg("door!  Good luck!")
	me.cycle = 2
	zzt.board.current.dark = 1
	while (zzt.board.get_obj_by_color(zzt.board.current, ZZT_DOOR, GREEN)) do
		coroutine.yield()
	end
	me.cycle = 20
	zzt.board.current.dark = 0
	set_msg("Great!")
	w = zzt.TUIWindow:new("DreamZZT Tutorial")
	w:buildFromString("  Our final room will introduce you to two\
of the enemys in ZZT.  If you run out of\
ammo or are low on health, just touch me\
and I'll give you more!\
\
$Lions\
\
  Lions (Í) simply run around trying to\
maul you.  Don't let them touch you or\
you'll lose health!\
\
$Tigers\
\
  Like lions, tigers („) also run around,\
but they can also shoot!  Shoot them\
before they shoot you!")
	w:doMenu()
	zzt.status.give_health(100)
	zzt.status.give_ammo(100)
	set_msg("RELEASE THE LIONS!!!!!")
	change(ZZT_INVISIBLE, RED+8, zzt.create_object(ZZT_EMPTY,0,0))
	while (zzt.board.get_obj_by_type(zzt.board.current, ZZT_LION)) do
		them,msg = pop_message(me)
		if (not (them == nil)) and ((them.type == ZZT_PLAYER) and (msg == "touch")) then
			health_ammo_menu()
		end
		coroutine.yield()
	end
	me.cycle = 20
	set_msg("Very good!  Now...")
	set_msg("RELEASE THE TIGERS!")
	me.cycle = 2
	change(ZZT_INVISIBLE, BLUE+8, zzt.create_object(ZZT_EMPTY,0,0))
	while (zzt.board.get_obj_by_type(zzt.board.current, ZZT_TIGER)) do
		them,msg = pop_message(me)
		if (not (them == nil)) and ((them.type == ZZT_PLAYER) and (msg == "touch")) then
			health_ammo_menu()
		end
		coroutine.yield()
	end
	w = zzt.TUIWindow:new("Congratulations")
	w:buildFromString("Congratulations!  You have successfully\
completed the DreamZZT tutorial.  From\
here, you should be able to start playing\
real games.  If you need more help, feel\
free to visit the DreamZZT website at\
$http://dev.c99.org/DreamZZT/")
	w:doMenu()
end

function health_ammo_menu()
	local w = zzt.TUIWindow:new("First Aid")
	w:buildFromString("What do you need?\
\
!health;More health\
!ammo;More ammo")
	w:doMenu()
	
	if(w:getLabel() == "health") then 
		zzt.status.give_health(100)
		set_msg("Enjoy!")
	end
	if(w:getLabel() == "ammo") then 
		zzt.status.give_ammo(15)
		set_msg("Enjoy!")
	end			
end