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
extern Player *player;

extern "C" {
#include "lualib.h"
#if 0
#include "zzt_lua.h"
#endif
#include "pluto.h"
}

#include <machine/endian.h>
#include <algorithm>

inline void ByteSwap(void * b, int n) {
#if BYTE_ORDER == LITTLE_ENDIAN
	register int i = 0;
	register int j = n-1;
	while (i<j) {
		std::swap(((unsigned char *)b)[i], ((unsigned char *)b)[j]);
		i++, j--;
	}
#endif
}

int base64_encode(const void *inp, int insize, char **outptr);
int base64_decode(const char *src, unsigned char **outptr);


static int pop_message (lua_State *L) {
#if 0
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
#endif
	return 0;
}

ZZTLUA::ZZTLUA() {
	m_shape = 0x01;
	m_name = "lua";
	m_flags = F_OBJECT | F_PUSHER;

	m_luaVM = lua_open();

#ifdef LUA_VERSION_NUM

	luaL_openlibs(m_luaVM);
#else

	luaopen_base(m_luaVM);
	luaopen_io(m_luaVM);
	luaopen_string(m_luaVM);
	luaopen_math(m_luaVM);
#endif

	//tolua_zzt_open(m_luaVM);
	pluto_open(m_luaVM);
	lua_pushcfunction(m_luaVM, ::pop_message);
	lua_setglobal(m_luaVM, "pop_message");

	m_serialized = false;
}

ZZTLUA::~ZZTLUA() {
	lua_close(m_luaVM);
}

void ZZTLUA::create() {
#if 0
	tolua_pushusertype(m_luaVM,(void*)this,"ZZTObject");
	lua_setglobal(m_luaVM, "me");

	tolua_pushusertype(m_luaVM,(void*)player,"ZZTObject");
	lua_setglobal(m_luaVM, "player");

	if(m_serialized) {
		LoadInfo li;
		unsigned char *out;
		li.size = base64_decode(m_prog.c_str(),&out);
		li.buf = (const char *)out;

		lua_getglobal(m_luaVM, "setup_perms");

		if(lua_pcall(m_luaVM, 0, 0, 0) != 0) {
			printf("LUA error: %s\n",lua_tostring (m_luaVM, -1));
			lua_pop(m_luaVM, 1);
		}

		lua_getglobal(m_luaVM, "perms");

		pluto_unpersist(m_luaVM, bufreader, &li);

		free(out);
		m_serialized = false;
		lua_settop(m_luaVM, 2);
		lua_setglobal(m_luaVM, "savestate");
	} else if(luaL_loadbuffer (m_luaVM, (m_prog.c_str()) + m_progpos, m_prog.length() - m_progpos, "ZZTLUA") || lua_pcall (m_luaVM, 0, 0, 0)) {
		printf("LUA error: %s\n",lua_tostring (m_luaVM, -1));
		lua_pop(m_luaVM, 1);
	}
#endif
}

void ZZTLUA::setParam(int arg, unsigned char val) {
	if(arg==1)
		m_serialized = val;
}

unsigned char ZZTLUA::param(int arg) {
	if(arg==1) {
		if(!m_serialized) {
			WriterInfo wi;
			char *out;

			wi.buf = NULL;
			wi.buflen = 0;

			lua_getglobal(m_luaVM, "setup_perms");

			if(lua_pcall(m_luaVM, 0, 0, 0) != 0) {
				printf("LUA error: %s\n",lua_tostring (m_luaVM, -1));
				lua_pop(m_luaVM, 1);
			}

			lua_settop(m_luaVM, 0);
			lua_getglobal(m_luaVM, "perms");
			lua_getglobal(m_luaVM, "savestate");

			pluto_persist(m_luaVM, bufwriter, &wi);

			base64_encode(wi.buf, wi.buflen, &out);
			m_prog = out;
			m_proglen = m_prog.length();
			free(out);
			m_serialized = true;
		}
		return m_serialized;
	}
	return 0;
}

void ZZTLUA::update() {
	/*tolua_pushusertype(m_luaVM,(void*)this,"ZZTObject");
	lua_setglobal(m_luaVM, "me");

	tolua_pushusertype(m_luaVM,(void*)player,"ZZTObject");
	lua_setglobal(m_luaVM, "player");*/

	lua_getglobal(m_luaVM, "wake");

	if(lua_pcall(m_luaVM, 0, 0, 0) != 0) {
		printf("LUA error: %s\n",lua_tostring (m_luaVM, -1));
		lua_pop(m_luaVM, 1);
	}
}
