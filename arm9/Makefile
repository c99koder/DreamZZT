TIKI_DIR = ../../tiki
TIKI_PLAT = nds
DEPSDIR = ../.deps

OBJS = ../src/DreamZZT.o ../src/main.o ../src/console_nds.o ../src/debug.o ../src/sound.o ../src/task.o ../src/word.o \
../src/window.o ../src/board.o ../src/status.o ../src/object.o ../src/editor.o \
../src/objects/passage.o ../src/objects/terrain.o ../src/objects/bullet.o ../src/objects/enemy.o \
../src/objects/dup.o ../src/objects/zztoop.o ../src/objects/pusher.o ../src/objects/bomb.o ../src/objects/items.o \
../src/objects/conveyer.o ../src/objects/player.o ../src/objects/centipede.o sound9.o

../$(TARGET).arm9: $(TARGET).elf
	@$(OBJCOPY) -O binary $< $@
	@echo built ... $(notdir $@)

$(TARGET).elf: ../resources/zztascii.bin.o $(OBJS)
	$(CXX) $(LDFLAGS) -L$(TIKI_DIR)/$(TIKI_PLAT) $(OBJS) ../resources/zztascii.bin.o -lfat $(TIKI_BASE_LIBS) -o $(TARGET).elf

clean:
	rm -f $(OBJS) dreamzzt.elf

include $(TIKI_DIR)/$(TIKI_PLAT)/Makefile.rules
CXXFLAGS+=-I../include -I. -DDEBUG -DDZZT_LITE

#---------------------------------------------------------------------------------
%.bin.o :       %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)
