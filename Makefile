TIKI_DIR = ../tiki
TIKI_PLAT = sdl

CFLAGS=-I$(TIKI_DIR)/$(TIKI_PLAT)/include -I$(TIKI_DIR)/include -Iinclude
OBJS = src/DreamZZT.o src/debug.o src/sound.o src/http.o src/word.o src/main.o src/txtwin.o src/board.o src/status.o src/object.o \
src/objects/passage.o src/objects/terrain.o src/objects/bullet.o src/objects/enemy.o \
src/objects/dup.o src/objects/zztoop.o src/objects/pusher.o src/objects/bomb.o src/objects/items.o \
src/objects/conveyer.o src/objects/player.o

all: dreamzzt

dreamzzt: $(OBJS)
	$(CXX) -L$(TIKI_DIR)/$(TIKI_PLAT) $(OBJS) $(TIKI_BASE_LIBS) -o dreamzzt
	
clean:
	-rm -f $(OBJS) dreamzzt

package: dreamzzt
	cp dreamzzt loki/bin/Linux/x86/glibc-2.1 
	~/loki_setup/makeself/makeself.sh --follow loki dreamzzt-$(VERSION).run dreamzzt ./setup.sh

include $(TIKI_DIR)/$(TIKI_PLAT)/Makefile.rules
