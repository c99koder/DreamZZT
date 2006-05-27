/* DreamZZT.cpp - Tiki main function
 * Copyright (c) 2000-2006 Sam Steele
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

#if TIKI_PLAT == TIKI_WIN32
#include <windows.h>

#include "pch.h"

static char szAppName[] = "DreamZZT";
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
extern "C" int tiki_main(int argc, char *argv[]);
int main(int argc, char *argv[])
#endif
{
#if TIKI_PLAT != TIKI_WIN32
	return tiki_main(argc, argv);
#else
	return Tiki::DoMain(szAppName, hInst, hPrevInstance, lpCmdLine, nCmdShow);
#endif
}
