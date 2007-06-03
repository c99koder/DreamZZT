/* lua.cpp - Lua interpreter object
 * Copyright (c) 2000 - 2007 Sam Steele
 *
 * This file is part of DreamZZT.
 *
 * DreamZZT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * DreamZZT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */ 

#include <Tiki/tiki.h>
#include <Tiki/hid.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Audio;

#include "object.h"
#include "board.h"
#include "status.h"
#include "sound.h"
#include "debug.h"
#include "window.h"

extern ZZTMusicStream *zm;

extern struct board_info_node *currentbrd;
extern struct world_header world;

extern "C" {

#if TIKI_PLAT == TIKI_OSX
#include "Lua/lualib.h"
#else
#include <lualib.h>
#endif

#include "zzt_lua.h" 
}

static int pop_message (lua_State *L) {
	ZZTLUA *me = ((ZZTLUA*)  tolua_tousertype(L,1,0));
	ZZTLUA::message_queue_item i;
	
	if(me->message_count() < 1) {
		lua_pushnil(L);
		return 1;
	} else {
		i = me->pop_message();
		tolua_pushusertype(L,(void*)i.them,"ZZTObject");
		lua_pushstring(L, i.message.c_str());
		return 2;
	}
}


ZZTLUA::ZZTLUA(int type, int x, int y, int shape, int flags, std::string name) : ZZTObject(type, x, y, shape, flags, name) {
	m_luaVM = lua_open();

#ifdef LUA_VERSION_NUM	
	luaL_openlibs(m_luaVM);
#else
	luaopen_base(m_luaVM);
	luaopen_io(m_luaVM);
	luaopen_string(m_luaVM);
	luaopen_math(m_luaVM);
#endif

	tolua_zzt_open(m_luaVM);
	lua_pushcfunction(m_luaVM, ::pop_message);
    lua_setglobal(m_luaVM, "pop_message");
}

ZZTLUA::~ZZTLUA() {
	lua_close(m_luaVM);
}

void ZZTLUA::create() {
	tolua_pushusertype(m_luaVM,(void*)this,"ZZTObject");
	lua_setglobal(m_luaVM, "me");
	
	if(luaL_loadbuffer (m_luaVM, (m_prog.c_str()) + m_progpos, m_prog.length() - m_progpos, "ZZTLUA") || lua_pcall (m_luaVM, 0, 0, 0)) {
		printf("LUA error: %s\n",lua_tostring (m_luaVM, -1));
		lua_pop(m_luaVM, 1);
	}
}

void ZZTLUA::update() {
	tolua_pushusertype(m_luaVM,(void*)this,"ZZTObject");
	lua_setglobal(m_luaVM, "me");

	lua_getglobal(m_luaVM, "wake");

	if(lua_pcall(m_luaVM, 0, 0, 0) != 0) {
		printf("LUA error: %s\n",lua_tostring (m_luaVM, -1));
		lua_pop(m_luaVM, 1);
	}
}