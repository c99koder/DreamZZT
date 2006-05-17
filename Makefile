TIKI_DIR = ../tiki
TIKI_PLAT = sdl

CFLAGS=-I$(TIKI_DIR)/$(TIKI_PLAT)/include -I$(TIKI_DIR)/include -Iinclude
OBJS = src/DreamZZT.o src/debug.o src/sound.o src/http.o src/word.o src/main.o src/txtwin.o src/board.o src/dirxn.o src/status.o src/object.o \
src/objects/passage.o src/objects/terrain.o src/objects/bullet.o src/objects/enemy.o \
src/objects/dup.o src/objects/zztoop.o src/objects/pusher.o src/objects/bomb.o src/objects/items.o \
src/objects/spinner.o src/objects/player.o

all: $(OBJS)
	$(CXX) -L$(TIKI_DIR)/$(TIKI_PLAT) $(OBJS) $(TIKI_BASE_LIBS) -o dreamzzt

clean:
	-rm -f $(OBJS) dreamzzt

include $(TIKI_DIR)/$(TIKI_PLAT)/Makefile.rules
