TIKI_DIR = ../tiki
TIKI_PLAT = sdl
BIN = $(DESTDIR)/usr/bin
DATA = $(DESTDIR)/usr/share/dreamzzt

CFLAGS=-I$(TIKI_DIR)/$(TIKI_PLAT)/include -I$(TIKI_DIR)/include -Iinclude -g -DDATA_PATH="\"$(DATA)\""
OBJS = src/DreamZZT.o src/debug.o src/sound.o src/http.o src/word.o src/main.o src/window.o src/board.o src/status.o src/object.o src/editor.o \
src/objects/passage.o src/objects/terrain.o src/objects/bullet.o src/objects/enemy.o \
src/objects/dup.o src/objects/zztoop.o src/objects/pusher.o src/objects/bomb.o src/objects/items.o \
src/objects/conveyer.o src/objects/player.o src/objects/centipede.o

all: dreamzzt

dreamzzt: $(OBJS)
	$(CXX) -L$(TIKI_DIR)/$(TIKI_PLAT) $(OBJS) $(TIKI_BASE_LIBS) -lalut -lqt-mt -o dreamzzt
	
clean:
	-rm -f $(OBJS) dreamzzt

install: dreamzzt
	install -D -s dreamzzt $(BIN)/dreamzzt
	install -D resources/zzt-ascii.png $(DATA)/zzt-ascii.png
	install -D resources/tutorial.zzt $(DATA)/tutorial.zzt
	install -D resources/town.zzt $(DATA)/town.zzt

package: dreamzzt
	strip dreamzzt
	cp dreamzzt loki/bin/Linux/x86/glibc-2.1 
	~/loki_setup/makeself/makeself.sh --follow loki dreamzzt-$(VERSION).run dreamzzt ./setup.sh

include $(TIKI_DIR)/$(TIKI_PLAT)/Makefile.rules
