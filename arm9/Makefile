TIKI_DIR = ../../tiki
TIKI_PLAT = nds
DEPSDIR = ../.deps

OBJS = ../src/DreamZZT.o ../src/main.o ../src/console_nds.o ../src/debug.o ../src/sound.o ../src/task.o ../src/word.o ../src/bugreport.o \
../src/window.o ../src/board.o ../src/status.o ../src/object.o ../src/editor.o \
../src/objects/passage.o ../src/objects/terrain.o ../src/objects/bullet.o ../src/objects/enemy.o \
../src/objects/dup.o ../src/objects/zztoop.o ../src/objects/pusher.o ../src/objects/bomb.o ../src/objects/items.o \
../src/objects/conveyer.o ../src/objects/player.o ../src/objects/centipede.o ../src/objects/lua.o \
../src/libtolua++/tolua_event.o ../src/libtolua++/tolua_push.o ../src/libtolua++/tolua_is.o ../src/libtolua++/tolua_to.o ../src/libtolua++/tolua_map.o ../src/pluto.o \
../lua/lapi.o ../lua/lib/lauxlib.o ../lua/lib/ltablib.o ../lua/lstring.o ../lua/lcode.o ../lua/lib/lbaselib.o ../lua/llex.o ../lua/ltable.o \
../lua/ldebug.o ../lua/lib/ldblib.o ../lua/lmem.o ../lua/ltests.o ../lua/ldo.o ../lua/lib/liolib.o ../lua/lobject.o ../lua/ltm.o ../lua/ldump.o ../lua/lib/lmathlib.o ../lua/lopcodes.o ../lua/lundump.o \
../lua/lfunc.o ../lua/lib/loadlib.o ../lua/lparser.o ../lua/lvm.o ../lua/lgc.o ../lua/lib/lstrlib.o ../lua/lstate.o ../lua/lzio.o

../$(TARGET).arm9: $(TARGET).arm9.elf
	@$(OBJCOPY) -O binary $< $@
	@echo built ... $(notdir $@)

VERSION:
	sed "s/\$$WCREV\\$$/`svnversion .`/" < ../include/version.h.in > ../include/version.h

$(TARGET).arm9.elf: ../resources/zztascii.bin.o VERSION $(OBJS)
	$(CXX) $(LDFLAGS) -L$(TIKI_DIR)/$(TIKI_PLAT) $(OBJS) ../resources/zztascii.bin.o $(TIKI_BASE_LIBS) -o $(TARGET).arm9.elf
	arm-eabi-strip $(TARGET).arm9.elf
clean:
	rm -f $(OBJS) dreamzzt.arm9.elf

include $(TIKI_DIR)/$(TIKI_PLAT)/Makefile.rules
CXXFLAGS+=-I../include -I. -I../lua -I../src/libtolua++ -DDZZT_LITE -O2

#---------------------------------------------------------------------------------
%.bin.o :       %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)
