TIKI_DIR = ../tiki
ifeq ($(strip $(TIKI_PLAT)),)
TIKI_PLAT = sdl
endif

BIN = $(DESTDIR)/usr/bin
DATA = $(DESTDIR)/usr/share/dreamzzt
PIXMAPS = $(DESTDIR)/usr/share/pixmaps
ICONS = $(DESTDIR)/usr/share/icons/hicolor
DOC = $(DESTDIR)/usr/share/doc/dreamzzt
APPLICATIONS = $(DESTDIR)/usr/share/applications

OBJS = src/DreamZZT.o src/debug.o src/sound.o src/word.o src/main.o src/window.o src/board.o src/task.o src/status.o src/object.o src/editor.o \
src/objects/passage.o src/objects/terrain.o src/objects/bullet.o src/objects/enemy.o \
src/objects/dup.o src/objects/zztoop.o src/objects/pusher.o src/objects/bomb.o src/objects/items.o \
src/objects/conveyer.o src/objects/player.o src/objects/centipede.o src/objects/lua.o \
src/libtolua++/tolua_event.o src/libtolua++/tolua_push.o src/libtolua++/tolua_is.o src/libtolua++/tolua_to.o src/libtolua++/tolua_map.o src/pluto.o

ifeq ($(TIKI_PLAT),nds)
OBJS += src/console_nds.o resources/zztascii.bin.o
endif

ifeq ($(TIKI_PLAT),dc)
OBJS += src/vmu.o src/lcd.o $(KOS_BASE)/../kos-ports/lua/src/*.o $(KOS_BASE)/../kos-ports/lua/src/lib/*.o
else
OBJS += src/bugreport.o
OBJS += lua/lapi.o lua/lib/lauxlib.o lua/lib/ltablib.o lua/lstring.o lua/lcode.o lua/lib/lbaselib.o lua/llex.o lua/ltable.o \
lua/ldebug.o lua/lib/ldblib.o lua/lmem.o lua/ltests.o lua/ldo.o lua/lib/liolib.o lua/lobject.o lua/ltm.o lua/ldump.o lua/lib/lmathlib.o lua/lopcodes.o lua/lundump.o \
lua/lfunc.o lua/lib/loadlib.o lua/lparser.o lua/lvm.o lua/lgc.o lua/lib/lstrlib.o lua/lstate.o lua/lzio.o
endif

ifeq ($(TIKI_PLAT),nds)
NDS_CART_CODE ?= DZZT
NDS_CART_ID ?= SS
NDS_CART_NAME ?= DreamZZT
NDS_CART_VERSION ?= 310
endif

ifeq ($(TIKI_PLAT),sdl)
EXTRA_LIBS = -lqt-mt
endif

all: dreamzzt

VERSION:
	sed "s/\$$WCREV\\$$/`svnversion .`/" < include/version.h.in > include/version.h

dreamzzt: VERSION $(OBJS)
	$(CXX) $(LDFLAGS) -L$(TIKI_DIR)/$(TIKI_PLAT) -L$(TIKI_DIR)/$(TIKI_PLAT)/lib $(OBJS) $(TIKI_BASE_LIBS) $(EXTRA_LIBS) -o dreamzzt$(PLATFORM_BINARY_EXT)
	$(post_build)
	
clean:
	-rm -f $(OBJS) dreamzzt$(PLATFORM_BINARY_EXT)
ifeq ($(TIKI_PLAT),nds)
	-rm -f dreamzzt.nds dreamzzt.ds.gba
endif

install: dreamzzt
ifeq ($(TIKI_PLAT),sdl)
	install -d $(BIN) $(DATA) $(PIXMAPS) $(ICONS)/48x48/mimetypes $(APPLICATIONS) $(DOC)
	install -s dreamzzt $(BIN)
	install -m644 resources/*.zzt resources/*.szt resources/*.png $(DATA)
	install -m644 dreamzzt.png $(PIXMAPS)
	install -m644 application-x-zzt-*.png $(ICONS)/48x48/mimetypes
	install -m644 dreamzzt.desktop $(APPLICATIONS)
	install -m644 ChangeLog doc/* $(DOC)
endif

include $(TIKI_DIR)/$(TIKI_PLAT)/Makefile.rules
CXXFLAGS += -I$(TIKI_DIR)/$(TIKI_PLAT)/include -I$(TIKI_DIR)/include -Iinclude -O2

ifeq ($(TIKI_PLAT),dc)
CFLAGS += -I$(KOS_BASE)/../kos-ports/lua/src -I$(KOS_BASE)/../kos-ports/lua/include -Iinclude
CXXFLAGS += -I$(KOS_BASE)/../kos-ports/lua/src -I$(KOS_BASE)/../kos-ports/lua/include
else
CFLAGS += -Ilua -Ilua/lib -Iinclude
CXXFLAGS += -Ilua -Ilua/lib -DNET
endif

ifeq ($(TIKI_PLAT),sdl)
CXXFLAGS += -DDATA_PATH="\"$(DATA)\""
endif
 
ifneq ($(TIKI_PLAT),nds)
CXXFLAGS += -DUSE_OPENGL
endif

DEPSDIR = $(CURDIR)

#---------------------------------------------------------------------------------
%.bin.o :       %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)
