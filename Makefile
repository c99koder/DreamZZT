TIKI_DIR = ../tiki
TIKI_PLAT = sdl
BIN = $(DESTDIR)/usr/bin
DATA = $(DESTDIR)/usr/share/dreamzzt
PIXMAPS = $(DESTDIR)/usr/share/pixmaps
ICONS = $(DESTDIR)/usr/share/icons/hicolor
DOC = $(DESTDIR)/usr/share/doc/dreamzzt
APPLICATIONS = $(DESTDIR)/usr/share/applications

CFLAGS = -I$(TIKI_DIR)/$(TIKI_PLAT)/include -I$(TIKI_DIR)/include -Iinclude -Ilua -Ilua/lib -DDATA_PATH="\"$(DATA)\"" -DNET
OBJS = src/DreamZZT.o src/GraphicsLayer.o src/AMFModel.o src/AMFModelInstance.o src/ModelManager.o src/debug.o src/sound.o src/http.o src/word.o src/main.o src/window.o src/board.o src/console.o src/task.o src/status.o src/object.o src/editor.o src/bugreport.o \
src/objects/passage.o src/objects/terrain.o src/objects/bullet.o src/objects/enemy.o \
src/objects/dup.o src/objects/zztoop.o src/objects/pusher.o src/objects/bomb.o src/objects/items.o \
src/objects/conveyer.o src/objects/player.o src/objects/centipede.o src/objects/lua.o \
src/libtolua++/tolua_event.o src/libtolua++/tolua_push.o src/libtolua++/tolua_is.o src/libtolua++/tolua_to.o src/libtolua++/tolua_map.o src/pluto.o \
lua/lapi.o lua/lib/lauxlib.o lua/lib/ltablib.o lua/lstring.o lua/lcode.o lua/lib/lbaselib.o lua/llex.o lua/ltable.o \
lua/ldebug.o lua/lib/ldblib.o lua/lmem.o lua/ltests.o lua/ldo.o lua/lib/liolib.o lua/lobject.o lua/ltm.o lua/ldump.o lua/lib/lmathlib.o lua/lopcodes.o lua/lundump.o \
lua/lfunc.o lua/lib/loadlib.o lua/lparser.o lua/lvm.o lua/lgc.o lua/lib/lstrlib.o lua/lstate.o lua/lzio.o

all: dreamzzt

VERSION:
	sed "s/\$$WCREV\\$$/`svnversion .`/" < include/version.h.in > include/version.h

dreamzzt: VERSION $(OBJS)
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
