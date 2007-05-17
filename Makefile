TIKI_DIR = ../tiki
TIKI_PLAT = sdl
BIN = $(DESTDIR)/usr/bin
DATA = $(DESTDIR)/usr/share/dreamzzt
PIXMAPS = $(DESTDIR)/usr/share/pixmaps
ICONS = $(DESTDIR)/usr/share/icons/hicolor
DOC = $(DESTDIR)/usr/share/doc/dreamzzt
APPLICATIONS = $(DESTDIR)/usr/share/applications

CFLAGS = -I$(TIKI_DIR)/$(TIKI_PLAT)/include -I$(TIKI_DIR)/include -Iinclude -DDATA_PATH="\"$(DATA)\""
OBJS = src/DreamZZT.o src/debug.o src/sound.o src/http.o src/word.o src/main.o src/window.o src/board.o src/console.o src/task.o src/status.o src/object.o src/editor.o src/bugreport.o \
src/objects/passage.o src/objects/terrain.o src/objects/bullet.o src/objects/enemy.o \
src/objects/dup.o src/objects/zztoop.o src/objects/pusher.o src/objects/bomb.o src/objects/items.o \
src/objects/conveyer.o src/objects/player.o src/objects/centipede.o

all: dreamzzt

dreamzzt: $(OBJS)
	$(CXX) -L$(TIKI_DIR)/$(TIKI_PLAT) $(OBJS) $(TIKI_BASE_LIBS) -lalut -lqt-mt `curl-config --libs` -o dreamzzt
	
clean:
	-rm -f $(OBJS) dreamzzt

install: dreamzzt
	install -d $(BIN) $(DATA) $(PIXMAPS) $(ICONS)/48x48/mimetypes $(APPLICATIONS) $(DOC)
	install -s dreamzzt $(BIN)
	install -m644 resources/* $(DATA)
	install -m644 dreamzzt.png $(PIXMAPS)
	install -m644 application-x-zzt-*.png $(ICONS)/48x48/mimetypes
	install -m644 dreamzzt.desktop $(APPLICATIONS)
	install -m644 ChangeLog doc/* $(DOC)

package: dreamzzt
	strip dreamzzt
	cp dreamzzt loki/bin/Linux/x86/glibc-2.1 
	~/loki_setup/makeself/makeself.sh --follow loki dreamzzt-$(VERSION).run dreamzzt ./setup.sh

include $(TIKI_DIR)/$(TIKI_PLAT)/Makefile.rules
