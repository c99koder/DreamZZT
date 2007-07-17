TIKI_DIR = ../../tiki
TIKI_PLAT = nds
DEPSDIR = ../.deps

OBJS = ../src/DreamZZT.o ../src/main.o ../src/console_nds.o ../src/debug.o ../src/sound.o ../src/task.o ../src/word.o \
../src/window.o ../src/board.o ../src/status.o ../src/object.o ../src/editor.o \
../src/objects/passage.o ../src/objects/terrain.o ../src/objects/bullet.o ../src/objects/enemy.o \
../src/objects/dup.o ../src/objects/zztoop.o ../src/objects/pusher.o ../src/objects/bomb.o ../src/objects/items.o \
../src/objects/conveyer.o ../src/objects/player.o ../src/objects/centipede.o sound9.o

../$(TARGET).arm9: $(TARGET).arm9.elf
	@$(OBJCOPY) -O binary $< $@
	@echo built ... $(notdir $@)

../include/version.h: ../include/version.h.in
	sed "s/\$WCREV\\$/`svnversion .`/" < ../include/version.h.in > ../include/version.h

$(TARGET).arm9.elf: ../resources/zztascii.bin.o ../resources/zztascii_pal.bin.o ../include/version.h $(OBJS)
	$(CXX) $(LDFLAGS) -L$(TIKI_DIR)/$(TIKI_PLAT) $(OBJS) ../resources/zztascii.bin.o ../resources/zztascii_pal.bin.o -lfat $(TIKI_BASE_LIBS) -o $(TARGET).arm9.elf

clean:
	rm -f $(OBJS) dreamzzt.arm9.elf

include $(TIKI_DIR)/$(TIKI_PLAT)/Makefile.rules
CXXFLAGS+=-I../include -I. -DDZZT_LITE

#---------------------------------------------------------------------------------
%.bin.o :       %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)
