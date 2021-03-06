/* window.cpp - ZZT-formatted text popup window
 * Copyright (C) 2000 - 2007 Sam Steele
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
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/eventcollector.h>
#include <Tiki/tikitime.h>
#include <Tiki/thread.h>
#include <string.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Thread;

#include "console.h"
#include "status.h"
#include "window.h"
#include "object.h"
#include "board.h"
#include "word.h"

#ifdef USE_3DMODEL
#include "GraphicsLayer.h"

extern GraphicsLayer *gl;
#endif

extern Console *st;
const unsigned char lowercasekbd[946] = {
                                            0x1b, 0x5b, 0x30, 0x3b, 0x33, 0x37, 0x3b, 0x34,
                                            0x30, 0x6d, 0x1b, 0x5b, 0x32, 0x4a, 0x1b, 0x5b,
                                            0x31, 0x3b, 0x34, 0x34, 0x6d, 0x1b, 0x5b, 0x31,
                                            0x3b, 0x31, 0x48, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x1b, 0x5b, 0x32, 0x3b, 0x31,
                                            0x48, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x1b, 0x5b, 0x33, 0x3b, 0x31, 0x48, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b,
                                            0x5b, 0x34, 0x3b, 0x31, 0x48, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x1b, 0x5b, 0x35,
                                            0x3b, 0x31, 0x48, 0x20, 0xda, 0xc4, 0xc2, 0xc4,
                                            0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4,
                                            0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4,
                                            0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4,
                                            0xc4, 0xc4, 0xbf, 0x1b, 0x5b, 0x36, 0x3b, 0x31,
                                            0x48, 0x20, 0xb3, 0x60, 0xb3, 0x31, 0xb3, 0x32,
                                            0xb3, 0x33, 0xb3, 0x34, 0xb3, 0x35, 0xb3, 0x36,
                                            0xb3, 0x37, 0xb3, 0x38, 0xb3, 0x39, 0xb3, 0x30,
                                            0xb3, 0x2d, 0xb3, 0x3d, 0xb3, 0x3c, 0xcd, 0xcd,
                                            0xb3, 0x0d, 0x0a, 0x1b, 0x5b, 0x37, 0x3b, 0x31,
                                            0x48, 0x20, 0xc3, 0xc4, 0xc1, 0xc4, 0xc5, 0xc4,
                                            0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4,
                                            0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4,
                                            0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4, 0xc2, 0xc4,
                                            0xb4, 0x1b, 0x5b, 0x38, 0x3b, 0x31, 0x48, 0x20,
                                            0xb3, 0x54, 0x41, 0x42, 0xb3, 0x71, 0xb3, 0x77,
                                            0xb3, 0x65, 0xb3, 0x72, 0xb3, 0x74, 0xb3, 0x79,
                                            0xb3, 0x75, 0xb3, 0x69, 0xb3, 0x6f, 0xb3, 0x70,
                                            0xb3, 0x5b, 0xb3, 0x5d, 0xb3, 0x5c, 0xb3, 0x1b,
                                            0x5b, 0x39, 0x3b, 0x31, 0x48, 0x20, 0xc3, 0xc4,
                                            0xc4, 0xc4, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2,
                                            0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2,
                                            0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2,
                                            0xc1, 0xc2, 0xc1, 0xc4, 0xb4, 0x1b, 0x5b, 0x31,
                                            0x30, 0x3b, 0x31, 0x48, 0x20, 0xb3, 0x43, 0x41,
                                            0x50, 0x53, 0xb3, 0x61, 0xb3, 0x73, 0xb3, 0x64,
                                            0xb3, 0x66, 0xb3, 0x67, 0xb3, 0x68, 0xb3, 0x6a,
                                            0xb3, 0x6b, 0xb3, 0x6c, 0xb3, 0x3b, 0xb3, 0x27,
                                            0xb3, 0x3c, 0xd9, 0xb3, 0x1b, 0x5b, 0x31, 0x31,
                                            0x3b, 0x31, 0x48, 0x20, 0xc3, 0xc4, 0xc4, 0xc4,
                                            0xc4, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1,
                                            0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1,
                                            0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1,
                                            0xc4, 0xc4, 0xb4, 0x1b, 0x5b, 0x31, 0x32, 0x3b,
                                            0x31, 0x48, 0x20, 0xb3, 0x53, 0x48, 0x49, 0x46,
                                            0x54, 0xb3, 0x7a, 0xb3, 0x78, 0xb3, 0x63, 0xb3,
                                            0x76, 0xb3, 0x62, 0xb3, 0x6e, 0xb3, 0x6d, 0xb3,
                                            0x2c, 0xb3, 0x2e, 0xb3, 0x2f, 0xb3, 0x53, 0x46,
                                            0x54, 0xb3, 0x1b, 0x5b, 0x31, 0x33, 0x3b, 0x31,
                                            0x48, 0x20, 0xc0, 0xc2, 0xc4, 0xc2, 0xc4, 0xc2,
                                            0xc1, 0xc4, 0xc5, 0xc4, 0xc1, 0xc2, 0xc1, 0xc4,
                                            0x0d, 0x0a, 0x1b, 0x5b, 0x31, 0x33, 0x3b, 0x31,
                                            0x36, 0x48, 0xc1, 0xc4, 0xc1, 0xc4, 0xc1, 0xc2,
                                            0xc1, 0xc4, 0xc1, 0xc4, 0xc5, 0xc4, 0xc1, 0xc4,
                                            0xc4, 0xc2, 0xd9, 0x1b, 0x5b, 0x31, 0x34, 0x3b,
                                            0x31, 0x48, 0x20, 0x20, 0xb3, 0x18, 0xb3, 0x19,
                                            0xb3, 0x3c, 0x2d, 0xb3, 0x2d, 0x3e, 0xb3, 0x20,
                                            0x53, 0x50, 0x41, 0x43, 0x45, 0x20, 0xb3, 0x50,
                                            0x67, 0x55, 0x70, 0xb3, 0x50, 0x67, 0x44, 0x6e,
                                            0xb3, 0x20, 0x1b, 0x5b, 0x31, 0x35, 0x3b, 0x31,
                                            0x48, 0x20, 0x20, 0xc0, 0xc4, 0xc1, 0xc4, 0xc1,
                                            0xc4, 0xc4, 0xc1, 0xc4, 0xc4, 0xc1, 0xc4, 0xc4,
                                            0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc1, 0xc4, 0xc4,
                                            0xc4, 0xc4, 0xc1, 0xc4, 0xc4, 0xc4, 0xc4, 0xd9,
                                            0x20, 0x1b, 0x5b, 0x31, 0x36, 0x3b, 0x31, 0x48,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x1b, 0x5b, 0x31, 0x37, 0x3b, 0x31, 0x48, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b,
                                            0x5b, 0x31, 0x38, 0x3b, 0x31, 0x48, 0x20, 0x20,
                                            0x20, 0x55, 0x73, 0x65, 0x20, 0x74, 0x68, 0x65,
                                            0x20, 0x73, 0x74, 0x79, 0x6c, 0x75, 0x73, 0x20,
                                            0x74, 0x6f, 0x20, 0x74, 0x61, 0x70, 0x20, 0x61,
                                            0x20, 0x6b, 0x65, 0x79, 0x20, 0x20, 0x1b, 0x5b,
                                            0x31, 0x39, 0x3b, 0x31, 0x48, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0d,
                                            0x0a, 0x1b, 0x5b, 0x31, 0x39, 0x3b, 0x32, 0x37,
                                            0x48, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b,
                                            0x5b, 0x32, 0x30, 0x3b, 0x31, 0x48, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x61, 0x62, 0x6f,
                                            0x76, 0x65, 0x20, 0x74, 0x6f, 0x20, 0x70, 0x72,
                                            0x65, 0x73, 0x73, 0x20, 0x69, 0x74, 0x2e, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b, 0x5b,
                                            0x32, 0x31, 0x3b, 0x31, 0x48, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x1b, 0x5b, 0x32,
                                            0x32, 0x3b, 0x31, 0x48, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x1b, 0x5b, 0x32, 0x33,
                                            0x3b, 0x31, 0x48, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x1b, 0x5b, 0x32, 0x33, 0x3b,
                                            0x31, 0x48, 0x1b, 0x5b, 0x30, 0x6d, 0x0d, 0x0a,
                                            0x1a, 0x00
                                        };

const unsigned char uppercasekbd[946] = {
                                            0x1b, 0x5b, 0x30, 0x3b, 0x33, 0x37, 0x3b, 0x34,
                                            0x30, 0x6d, 0x1b, 0x5b, 0x32, 0x4a, 0x1b, 0x5b,
                                            0x31, 0x3b, 0x34, 0x34, 0x6d, 0x1b, 0x5b, 0x31,
                                            0x3b, 0x31, 0x48, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x1b, 0x5b, 0x32, 0x3b, 0x31,
                                            0x48, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x1b, 0x5b, 0x33, 0x3b, 0x31, 0x48, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b,
                                            0x5b, 0x34, 0x3b, 0x31, 0x48, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x1b, 0x5b, 0x35,
                                            0x3b, 0x31, 0x48, 0x20, 0xda, 0xc4, 0xc2, 0xc4,
                                            0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4,
                                            0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4,
                                            0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4, 0xc2, 0xc4,
                                            0xc4, 0xc4, 0xbf, 0x1b, 0x5b, 0x36, 0x3b, 0x31,
                                            0x48, 0x20, 0xb3, 0x7e, 0xb3, 0x21, 0xb3, 0x40,
                                            0xb3, 0x23, 0xb3, 0x24, 0xb3, 0x25, 0xb3, 0x5e,
                                            0xb3, 0x26, 0xb3, 0x2a, 0xb3, 0x28, 0xb3, 0x29,
                                            0xb3, 0x5f, 0xb3, 0x2b, 0xb3, 0x3c, 0xcd, 0xcd,
                                            0xb3, 0x0d, 0x0a, 0x1b, 0x5b, 0x37, 0x3b, 0x31,
                                            0x48, 0x20, 0xc3, 0xc4, 0xc1, 0xc4, 0xc5, 0xc4,
                                            0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4,
                                            0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4,
                                            0xc5, 0xc4, 0xc5, 0xc4, 0xc5, 0xc4, 0xc2, 0xc4,
                                            0xb4, 0x1b, 0x5b, 0x38, 0x3b, 0x31, 0x48, 0x20,
                                            0xb3, 0x54, 0x41, 0x42, 0xb3, 0x51, 0xb3, 0x57,
                                            0xb3, 0x45, 0xb3, 0x52, 0xb3, 0x54, 0xb3, 0x59,
                                            0xb3, 0x55, 0xb3, 0x49, 0xb3, 0x4f, 0xb3, 0x50,
                                            0xb3, 0x7b, 0xb3, 0x7d, 0xb3, 0x7c, 0xb3, 0x1b,
                                            0x5b, 0x39, 0x3b, 0x31, 0x48, 0x20, 0xc3, 0xc4,
                                            0xc4, 0xc4, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2,
                                            0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2,
                                            0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2,
                                            0xc1, 0xc2, 0xc1, 0xc4, 0xb4, 0x1b, 0x5b, 0x31,
                                            0x30, 0x3b, 0x31, 0x48, 0x20, 0xb3, 0x43, 0x41,
                                            0x50, 0x53, 0xb3, 0x41, 0xb3, 0x53, 0xb3, 0x44,
                                            0xb3, 0x46, 0xb3, 0x47, 0xb3, 0x48, 0xb3, 0x4a,
                                            0xb3, 0x4b, 0xb3, 0x4c, 0xb3, 0x3a, 0xb3, 0x22,
                                            0xb3, 0x3c, 0xd9, 0xb3, 0x1b, 0x5b, 0x31, 0x31,
                                            0x3b, 0x31, 0x48, 0x20, 0xc3, 0xc4, 0xc4, 0xc4,
                                            0xc4, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1,
                                            0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1,
                                            0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1, 0xc2, 0xc1,
                                            0xc4, 0xc4, 0xb4, 0x1b, 0x5b, 0x31, 0x32, 0x3b,
                                            0x31, 0x48, 0x20, 0xb3, 0x53, 0x48, 0x49, 0x46,
                                            0x54, 0xb3, 0x5a, 0xb3, 0x58, 0xb3, 0x43, 0xb3,
                                            0x56, 0xb3, 0x42, 0xb3, 0x4e, 0xb3, 0x4d, 0xb3,
                                            0x3c, 0xb3, 0x3e, 0xb3, 0x3f, 0xb3, 0x53, 0x46,
                                            0x54, 0xb3, 0x1b, 0x5b, 0x31, 0x33, 0x3b, 0x31,
                                            0x48, 0x20, 0xc0, 0xc2, 0xc4, 0xc2, 0xc4, 0xc2,
                                            0xc1, 0xc4, 0xc5, 0xc4, 0xc1, 0xc2, 0xc1, 0xc4,
                                            0x0d, 0x0a, 0x1b, 0x5b, 0x31, 0x33, 0x3b, 0x31,
                                            0x36, 0x48, 0xc1, 0xc4, 0xc1, 0xc4, 0xc1, 0xc2,
                                            0xc1, 0xc4, 0xc1, 0xc4, 0xc5, 0xc4, 0xc1, 0xc4,
                                            0xc4, 0xc2, 0xd9, 0x1b, 0x5b, 0x31, 0x34, 0x3b,
                                            0x31, 0x48, 0x20, 0x20, 0xb3, 0x18, 0xb3, 0x19,
                                            0xb3, 0x3c, 0x2d, 0xb3, 0x2d, 0x3e, 0xb3, 0x20,
                                            0x53, 0x50, 0x41, 0x43, 0x45, 0x20, 0xb3, 0x50,
                                            0x67, 0x55, 0x70, 0xb3, 0x50, 0x67, 0x44, 0x6e,
                                            0xb3, 0x20, 0x1b, 0x5b, 0x31, 0x35, 0x3b, 0x31,
                                            0x48, 0x20, 0x20, 0xc0, 0xc4, 0xc1, 0xc4, 0xc1,
                                            0xc4, 0xc4, 0xc1, 0xc4, 0xc4, 0xc1, 0xc4, 0xc4,
                                            0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc1, 0xc4, 0xc4,
                                            0xc4, 0xc4, 0xc1, 0xc4, 0xc4, 0xc4, 0xc4, 0xd9,
                                            0x20, 0x1b, 0x5b, 0x31, 0x36, 0x3b, 0x31, 0x48,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x1b, 0x5b, 0x31, 0x37, 0x3b, 0x31, 0x48, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b,
                                            0x5b, 0x31, 0x38, 0x3b, 0x31, 0x48, 0x20, 0x20,
                                            0x20, 0x55, 0x73, 0x65, 0x20, 0x74, 0x68, 0x65,
                                            0x20, 0x73, 0x74, 0x79, 0x6c, 0x75, 0x73, 0x20,
                                            0x74, 0x6f, 0x20, 0x74, 0x61, 0x70, 0x20, 0x61,
                                            0x20, 0x6b, 0x65, 0x79, 0x20, 0x20, 0x1b, 0x5b,
                                            0x31, 0x39, 0x3b, 0x31, 0x48, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0d,
                                            0x0a, 0x1b, 0x5b, 0x31, 0x39, 0x3b, 0x32, 0x37,
                                            0x48, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b,
                                            0x5b, 0x32, 0x30, 0x3b, 0x31, 0x48, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x61, 0x62, 0x6f,
                                            0x76, 0x65, 0x20, 0x74, 0x6f, 0x20, 0x70, 0x72,
                                            0x65, 0x73, 0x73, 0x20, 0x69, 0x74, 0x2e, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1b, 0x5b,
                                            0x32, 0x31, 0x3b, 0x31, 0x48, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x1b, 0x5b, 0x32,
                                            0x32, 0x3b, 0x31, 0x48, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x1b, 0x5b, 0x32, 0x33,
                                            0x3b, 0x31, 0x48, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                            0x20, 0x20, 0x20, 0x1b, 0x5b, 0x32, 0x33, 0x3b,
                                            0x31, 0x48, 0x1b, 0x5b, 0x30, 0x6d, 0x0d, 0x0a,
                                            0x1a, 0x00
                                        };

void render();
extern EventCollector *playerEventCollector;
extern Player *player;
extern int switchbrd;
extern struct board_info_node *board_list;
extern float zoom;
Console *mt = NULL;
#if defined(USE_SDL)
extern SDL_Surface *zzt_font;
#elif defined(USE_OPENGL)
extern Texture *zzt_font;
#endif

TUIWindow *activeWindow;

void TUIWidget::focus(bool f) {
	m_focus = f;
	if(f) {
		if(!m_ec->listening())
			m_ec->start();
		update();
	} else {
		if(m_ec->listening())
			m_ec->stop();
	}
}

void TUIWidget::update() {
	// Check for regular HID events.
	Hid::Event evt;
	while (m_ec->getEvent(evt)) {
		processHidEvent(evt);
	}
}

TUIRadioGroup::TUIRadioGroup(std::string text, int *selected) {
	m_text=text;
	m_selectedi=selected;
	m_selectedf=NULL;
	m_selecteds=NULL;
	m_selectedc=NULL;
}

TUIRadioGroup::TUIRadioGroup(std::string text, unsigned short *selected) {
	m_text=text;
	m_selectedi=NULL;
	m_selectedf=NULL;
	m_selecteds=selected;
	m_selectedc=NULL;
}

TUIRadioGroup::TUIRadioGroup(std::string text, float *selected) {
	m_text=text;
	m_selectedi=NULL;
	m_selectedf=selected;
	m_selecteds=NULL;
	m_selectedc=NULL;
}

TUIRadioGroup::TUIRadioGroup(std::string text, unsigned char *selected) {
	m_text=text;
	m_selectedi=NULL;
	m_selectedf=NULL;
	m_selecteds=NULL;
	m_selectedc=selected;
}

void TUIRadioGroup::draw(Console *ct, int top, int bottom, int y_pos) {
	std::list<std::string>::iterator options_iter;
	int i=0;
	if(m_selectedi!=NULL)
		m_val = (*m_selectedi);
	if(m_selecteds!=NULL)
		m_val = (*m_selecteds);
	if(m_selectedf!=NULL)
		m_val = (int)(*m_selectedf);
	if(m_selectedc!=NULL)
		m_val = (*m_selectedc);

	ct->color(YELLOW|HIGH_INTENSITY,m_bg);
	*ct << m_text;

	ct->color(WHITE|HIGH_INTENSITY,m_bg);
	*ct << m_options[m_val];
}

void TUIRadioGroup::processHidEvent(const Hid::Event &evt) {
	if(m_selectedi!=NULL)
		m_val = (*m_selectedi);
	if(m_selecteds!=NULL)
		m_val = (*m_selecteds);
	if(m_selectedf!=NULL)
		m_val = (int)(*m_selectedf);
	if(m_selectedc!=NULL)
		m_val = (*m_selectedc);

	if(evt.type == Event::EvtKeypress && evt.key == Event::KeyLeft) {
		if(m_val == 0)
			m_val = (int)m_options.size();
		m_val--;
	}
	if(evt.type == Event::EvtKeypress && evt.key == Event::KeyRight) {
		m_val++;
		if(m_val >= (int)m_options.size())
			m_val=0;
	}

	if(m_selectedi!=NULL)
		(*m_selectedi) = m_val;
	if(m_selecteds!=NULL)
		(*m_selecteds) = m_val;
	if(m_selectedf!=NULL)
		(*m_selectedf) = (float)m_val;
	if(m_selectedc!=NULL)
		(*m_selectedc) = m_val;
}

TUIBoardList::TUIBoardList(std::string text, unsigned char *board) : TUIRadioGroup(text, board) {
	struct board_info_node *current=board_list;

	add
		("(None)");
	if(current!=NULL)
		current=current->next;

	while(current!=NULL) {
		add
			(current->title);
		current=current->next;
	}
}

TUIDirection::TUIDirection(std::string text, Vector *direction) : TUIRadioGroup(text, &m_select) {
	m_direction = direction;
	add
		("North");
	add
		("South");
	add
		("East");
	add
		("West");
	add
		("Idle");
	if(direction->x == 0 && direction->y == -1)
		m_select = 0;
	else if(direction->x == 0 && direction->y == 1)
		m_select = 1;
	else if(direction->x == 1 && direction->y == 0)
		m_select = 2;
	else if(direction->x == -1 && direction->y == 0)
		m_select = 3;
	else if(direction->x == 0 && direction->y == 0)
		m_select = 4;
	else
		m_select=4;
}

void TUIDirection::processHidEvent(const Hid::Event &evt) {
	TUIRadioGroup::processHidEvent(evt);

	switch(m_select) {
	case 0:
		m_direction->x = 0;
		m_direction->y = -1;
		break;
	case 1:
		m_direction->x = 0;
		m_direction->y = 1;
		break;
	case 2:
		m_direction->x = 1;
		m_direction->y = 0;
		break;
	case 3:
		m_direction->x = -1;
		m_direction->y = 0;
		break;
	case 4:
		m_direction->x = 0;
		m_direction->y = 0;
		break;
	}
}

TUINumericInput::TUINumericInput(std::string text, int *num, int min, int max, int step) {
	m_text=text;
	m_min=min;
	m_max=max;
	m_step=step;
	m_numi=num;
	m_nums=NULL;
	m_numf=NULL;
	m_numc=NULL;
}

TUINumericInput::TUINumericInput(std::string text, unsigned short int *num, int min, int max, int step) {
	m_text=text;
	m_min=min;
	m_max=max;
	m_step=step;
	m_numi=NULL;
	m_nums=num;
	m_numf=NULL;
	m_numc=NULL;
}

TUINumericInput::TUINumericInput(std::string text, float *num, int min, int max, int step) {
	m_text=text;
	m_min=min;
	m_max=max;
	m_step=step;
	m_numi=NULL;
	m_nums=NULL;
	m_numf=num;
	m_numc=NULL;
}

TUINumericInput::TUINumericInput(std::string text, unsigned char *num, int min, int max, int step) {
	m_text=text;
	m_min=min;
	m_max=max;
	m_step=step;
	m_numi=NULL;
	m_nums=NULL;
	m_numf=NULL;
	m_numc=num;
}

void TUINumericInput::draw(Console *ct, int top, int bottom, int y_pos) {
	int i=0;
	if(m_numi!=NULL)
		m_val=(*m_numi);
	if(m_nums!=NULL)
		m_val=(*m_nums);
	if(m_numf!=NULL)
		m_val=(int)(*m_numf);
	if(m_numc!=NULL)
		m_val=(*m_numc);

	ct->color(YELLOW|HIGH_INTENSITY,m_bg);
	*ct << m_text;

	ct->color(WHITE|HIGH_INTENSITY,m_bg);
	*ct << m_val;
}

void TUINumericInput::processHidEvent(const Hid::Event &evt) {
	if(m_numi!=NULL)
		m_val=(*m_numi);
	if(m_nums!=NULL)
		m_val=(*m_nums);
	if(m_numf!=NULL)
		m_val=(int)(*m_numf);
	if(m_numc!=NULL)
		m_val=(*m_numc);

	if(evt.type == Event::EvtKeypress && evt.key == Event::KeyLeft) {
		m_val-=m_step;
		if(m_val < m_min)
			m_val=m_min;
	}
	if(evt.type == Event::EvtKeypress && evt.key == Event::KeyRight) {
		m_val+=m_step;
		if(m_val > m_max)
			m_val=m_max;
	}

	if(m_numi!=NULL)
		(*m_numi)=m_val;
	if(m_nums!=NULL)
		(*m_nums)=m_val;
	if(m_numf!=NULL)
		(*m_numf)=(float)m_val;
	if(m_numc!=NULL)
		(*m_numc)=m_val;
}

int TUITextInput::YtoX(int y) {
	size_t pos=0,oldpos=0;

	while((pos = m_text->find('\r', oldpos)) != std::string::npos && --y >= 0) {
		oldpos = pos+1;
	}

	return oldpos;
}

void TUITextInput::draw(Console *ct, int top, int bottom, int y_pos) {
	int i=0;
	std::vector<std::string> lines = wordify(*m_text, '\r', true);

	m_cursor_y = y_pos;
	if(y_pos >=0 && y_pos < getHeight() && m_cursor_x > lines[y_pos].length())
		m_cursor_x = lines[y_pos].length();

	for(std::vector<std::string>::iterator line = lines.begin() + top; line != lines.end() && i < bottom; line++) {
		ct->setANSI(true);
		*ct << "\x1b[s"; //Save cursor position
		if(i > 0)
			*ct << "\x1b[" << i << "B"; //Move i rows down
		ct->setANSI(false);
		if(m_center && ((*line).length() + m_label.length() < 40)) {
			for(size_t i=0; i< 20 - (((*line).length() + m_label.length()) / 2); i++) {
				*ct << " ";
			}
		}
		ct->color(YELLOW|HIGH_INTENSITY,m_bg);
		*ct << m_label;

		ct->color(WHITE|HIGH_INTENSITY,m_bg);
		*ct << *line;
		ct->setANSI(true);
		*ct << "\x1b[K\x1b[u"; //Clear EOL, restore cursor position
		ct->setANSI(false);
		i++;
	}
	if(m_blink && getFocus()) {
		ct->setANSI(true)
		;
		*ct << "\x1b[s"; //Save cursor position
		if(m_cursor_y - top > 0)
			*ct << "\x1b[" << m_cursor_y - top << "B"; //Move i rows down
		if(m_cursor_x > 0 || m_label.length() > 0)
			*ct << "\x1b[" << int(m_cursor_x + m_label.length()) << "C"; //Move i cols right
		*ct << (char)0xDB;
		*ct << "\x1b[u"; //restore cursor position
		ct->setANSI(false);
	}
	if(Time::gettime() - m_blinkTimer > 400000) {
		m_blink = !m_blink;
		m_blinkTimer = Time::gettime()
		               ;
	}
	if(!getFocus()) {
		m_blink=false;
		m_blinkTimer = Time::gettime()
		               ;
	}
}

void TUITextInput::focus(bool f) {
	if(!f) {
		m_blink = true;
		m_blinkTimer = Time::gettime();
	}
	TUIWidget::focus(f);
}

void TUITextInput::processHidEvent(const Hid::Event &evt) {
	char key = evt.key;

	if(evt.type == Event::EvtKeypress) {
		m_blink = true;
		m_blinkTimer = Time::gettime();
		if(key >= 32 && key < 127 && (m_text->length() + m_label.length() <= 40 || m_multiline)) {
			if(evt.mod & Event::KeyShift) {
				if(key >= 'a' && key <= 'z') {
					key -= 32;
				} else {
					switch(key) {
					case '`':
						key='~';
						break;
					case '0':
						key=')';
						break;
					case '1':
						key='!';
						break;
					case '2':
						key='@';
						break;
					case '3':
						key='#';
						break;
					case '4':
						key='$';
						break;
					case '5':
						key='%';
						break;
					case '6':
						key='^';
						break;
					case '7':
						key='&';
						break;
					case '8':
						key='*';
						break;
					case '9':
						key='(';
						break;
					case '-':
						key='_';
						break;
					case '=':
						key='+';
						break;
					case '[':
						key='{';
						break;
					case ']':
						key='}';
						break;
					case '\\':
						key='|';
						break;
					case ';':
						key=':';
						break;
					case '\'':
						key='\"';
						break;
					case ',':
						key='<';
						break;
					case '.':
						key='>';
						break;
					case '/':
						key='?';
						break;
					}
				}
			}
			std::string tmp;
			tmp += key;
			m_text->insert(YtoX(m_cursor_y) + m_cursor_x, tmp);
			m_cursor_x++;
		} else if((key == 8 || key == 127) && m_text->length() > 0) {
			m_text->erase(YtoX(m_cursor_y) + m_cursor_x - 1, 1);
			m_cursor_x--;
		} else if(m_multiline && key == 13) {
			m_text->insert(YtoX(m_cursor_y) + m_cursor_x, "\r");
			m_cursor_y++;
			m_cursor_x=0;
			if(activeWindow != NULL)
				activeWindow->scroll(1);
		} else if(evt.key == Event::KeyLeft) {
			m_cursor_x--;
			if(m_cursor_x < 0)
				m_cursor_x=0;
		} else if(evt.key == Event::KeyRight) {
			m_cursor_x++;
		}
	}
}

void TUIPasswordInput::draw(Console *ct, int top, int bottom, int y_pos) {
	ct->color(YELLOW|HIGH_INTENSITY,m_bg);
	*ct << m_label;

	ct->color(WHITE|HIGH_INTENSITY,m_bg);
	for(size_t i=0; i<m_text->length(); i++) {
		*ct << "*";
	}
	if(m_blink && getFocus()) {
		*ct << (char)0xDB;
	}
	if(Time::gettime() - m_blinkTimer > 400000) {
		m_blink = !m_blink;
		m_blinkTimer = Time::gettime();
	}
	if(!getFocus()) {
		m_blink=false;
		m_blinkTimer = Time::gettime();
	}
}

void TUISlider::draw(Console *ct, int top, int bottom, int y_pos) {
	int i=0;
	if(m_numi!=NULL)
		m_val=(*m_numi);
	if(m_nums!=NULL)
		m_val=(*m_nums);
	if(m_numf!=NULL)
		m_val=(int)(*m_numf);
	if(m_numc!=NULL)
		m_val=(*m_numc);

	ct->color(YELLOW|HIGH_INTENSITY,m_bg);
	*ct << m_text;

	ct->color(WHITE|HIGH_INTENSITY,m_bg);

	*ct << "1 ";
	for(i=0; i<m_val; i++) {
		*ct << "\xf9";
	}
	if(m_blink) {
		*ct << " ";
	} else {
		*ct << "\xfe";
	}
	for(i=0; i<8-m_val; i++) {
		*ct << "\xf9";
	}
	*ct << " 9";
	m_blinkTimer--;
	if(m_blinkTimer == 0) {
		m_blink = !m_blink;
		m_blinkTimer = 4;
	}
	if(!getFocus()) {
		m_blink=false;
		m_blinkTimer = 1;
	}
}

void TUIMeter::draw(Console *ct, int top, int bottom, int y_pos) {
	int x;
	int val = (*m_val > m_max) ? m_max : (*m_val);

	for(x=0;x<val / (m_max / m_width);x++) {
		ct->printf("%c",177);
	}
	for(x=0;x<m_width-(val / (m_max / m_width));x++) {
		ct->printf(" ");
	}
}

TUIWindow::TUIWindow(std::string title,int x, int y, int w, int h) {
	m_x=x;
	m_y=y;
	m_w=w;
	m_h=h;
	m_title=title;
	m_label="\0";
	m_offset=0;
	m_dirty=0;
	m_delta=0;
	m_repeatTimer=0;
}

TUIWindow::~TUIWindow() {
	TUIWidget *t;
	while(!m_widgets.empty()) {
		t = m_widgets[m_widgets.size()-1];
		m_widgets.pop_back();
		delete t;
	}
	if(mt != NULL) {
		delete mt;
		mt = NULL;
	}
}

#define CHECK_TOUCHKEY(X,Y,W,H,BUTTON) \
	if(evt.x >= X && evt.x < X+W && evt.y >= Y && evt.y < Y+H) { \
		evtPress.key = BUTTON; \
		evtPress.port = 1; \
		shift = false; \
		for(int ctk = ((X)/8)+1; ctk < (((X)+(W))/8); ctk++) { \
			st->putColor(ctk, ((Y)/8) + 1, BLUE | (WHITE << 8)); \
		} \
		st->draw(); \
		swiWaitForVBlank(); \
		swiWaitForVBlank(); \
		swiWaitForVBlank(); \
		swiWaitForVBlank(); \
		swiWaitForVBlank(); \
		sendEvent(evtPress); \
	}

void TUIWindow::processHidEvent(const Hid::Event &evt, bool canClose) {
	Event evtPress(Event::EvtKeypress);

	if(evt.type == Event::EvtQuit) {
		m_loop = false;
		switchbrd = -2;
	} else if(evt.type == Event::EvtKeyDown || evt.type == Event::EvtBtnPress) {
		switch(evt.key) {
		case Event::KeyUp:
			m_delta = -1;
			m_repeatTimer = Time::gettime() + 1000000;
			break;
		case Event::KeyDown:
			m_delta = 1;
			m_repeatTimer = Time::gettime() + 1000000;
			break;
		}
		if(evt.key == 13 || evt.btn == Event::BtnA) {
			if(widgetAtOffset(m_offset)->getCloseOnEnter()) {
				m_loop = false;
				m_label = widgetAtOffset(m_offset)->getReturnValue();
			}
		}
	} else if(evt.type == Event::EvtKeyUp || evt.type == Event::EvtBtnRelease) {
		if(evt.btn == Event::MouseLeftBtn) {
#if TIKI_PLAT == TIKI_NDS
			static bool shift = false;
			static bool caps = false;

			if(caps)
				shift = true;

			CHECK_TOUCHKEY(2*8-4,5*8-4,16,16,shift?'~':'`');
			CHECK_TOUCHKEY(4*8-4,5*8-4,16,16,shift?'!':'1');
			CHECK_TOUCHKEY(6*8-4,5*8-4,16,16,shift?'@':'2');
			CHECK_TOUCHKEY(8*8-4,5*8-4,16,16,shift?'#':'3');
			CHECK_TOUCHKEY(10*8-4,5*8-4,16,16,shift?'$':'4');
			CHECK_TOUCHKEY(12*8-4,5*8-4,16,16,shift?'%':'5');
			CHECK_TOUCHKEY(14*8-4,5*8-4,16,16,shift?'^':'6');
			CHECK_TOUCHKEY(16*8-4,5*8-4,16,16,shift?'&':'7');
			CHECK_TOUCHKEY(18*8-4,5*8-4,16,16,shift?'*':'8');
			CHECK_TOUCHKEY(20*8-4,5*8-4,16,16,shift?'(':'9');
			CHECK_TOUCHKEY(22*8-4,5*8-4,16,16,shift?')':'0');
			CHECK_TOUCHKEY(24*8-4,5*8-4,16,16,shift?'_':'-');
			CHECK_TOUCHKEY(26*8-4,5*8-4,16,16,shift?'+':'=');
			CHECK_TOUCHKEY(28*8-4,5*8-4,8*3 + 8,16,0x08);

			CHECK_TOUCHKEY(2*8-4,7*8-4,8*3 + 8,16,'\t');
			CHECK_TOUCHKEY(6*8-4,7*8-4,16,16,shift?'Q':'q');
			CHECK_TOUCHKEY(8*8-4,7*8-4,16,16,shift?'W':'w');
			CHECK_TOUCHKEY(10*8-4,7*8-4,16,16,shift?'E':'e');
			CHECK_TOUCHKEY(12*8-4,7*8-4,16,16,shift?'R':'r');
			CHECK_TOUCHKEY(14*8-4,7*8-4,16,16,shift?'T':'t');
			CHECK_TOUCHKEY(16*8-4,7*8-4,16,16,shift?'Y':'y');
			CHECK_TOUCHKEY(18*8-4,7*8-4,16,16,shift?'U':'u');
			CHECK_TOUCHKEY(20*8-4,7*8-4,16,16,shift?'I':'i');
			CHECK_TOUCHKEY(22*8-4,7*8-4,16,16,shift?'O':'o');
			CHECK_TOUCHKEY(24*8-4,7*8-4,16,16,shift?'P':'p');
			CHECK_TOUCHKEY(26*8-4,7*8-4,16,16,shift?'{':'[');
			CHECK_TOUCHKEY(28*8-4,7*8-4,16,16,shift?'}':']');
			CHECK_TOUCHKEY(30*8-4,7*8-4,16,16,shift?'|':'\\');

			CHECK_TOUCHKEY(7*8-4,9*8-4,16,16,shift?'A':'a');
			CHECK_TOUCHKEY(9*8-4,9*8-4,16,16,shift?'S':'s');
			CHECK_TOUCHKEY(11*8-4,9*8-4,16,16,shift?'D':'d');
			CHECK_TOUCHKEY(13*8-4,9*8-4,16,16,shift?'F':'f');
			CHECK_TOUCHKEY(15*8-4,9*8-4,16,16,shift?'G':'g');
			CHECK_TOUCHKEY(17*8-4,9*8-4,16,16,shift?'H':'h');
			CHECK_TOUCHKEY(19*8-4,9*8-4,16,16,shift?'J':'j');
			CHECK_TOUCHKEY(21*8-4,9*8-4,16,16,shift?'K':'k');
			CHECK_TOUCHKEY(23*8-4,9*8-4,16,16,shift?'L':'l');
			CHECK_TOUCHKEY(25*8-4,9*8-4,16,16,shift?':':';');
			CHECK_TOUCHKEY(27*8-4,9*8-4,16,16,shift?'\"':'\'');
			CHECK_TOUCHKEY(29*8-4,9*8-4,8*2 + 8,16,13);

			CHECK_TOUCHKEY(8*8-4,11*8-4,16,16,shift?'Z':'z');
			CHECK_TOUCHKEY(10*8-4,11*8-4,16,16,shift?'X':'x');
			CHECK_TOUCHKEY(12*8-4,11*8-4,16,16,shift?'C':'c');
			CHECK_TOUCHKEY(14*8-4,11*8-4,16,16,shift?'V':'v');
			CHECK_TOUCHKEY(16*8-4,11*8-4,16,16,shift?'B':'b');
			CHECK_TOUCHKEY(18*8-4,11*8-4,16,16,shift?'N':'n');
			CHECK_TOUCHKEY(20*8-4,11*8-4,16,16,shift?'M':'m');
			CHECK_TOUCHKEY(22*8-4,11*8-4,16,16,shift?'<':',');
			CHECK_TOUCHKEY(24*8-4,11*8-4,16,16,shift?'>':'.');
			CHECK_TOUCHKEY(26*8-4,11*8-4,16,16,shift?'?':'/');

			CHECK_TOUCHKEY(3*8-4,13*8-4,16,16,Event::KeyUp);
			CHECK_TOUCHKEY(5*8-4,13*8-4,16,16,Event::KeyDown);
			CHECK_TOUCHKEY(7*8-4,13*8-4,8*2 + 8,14,Event::KeyLeft);
			CHECK_TOUCHKEY(10*8-4,13*8-4,8*2 + 8,14,Event::KeyRight);
			CHECK_TOUCHKEY(13*8-4,13*8-4,8*7 + 8,14,' ');

			if(evt.x >= 2*8-4 && evt.x < 2*8-4 + 8*4 + 6 && evt.y >= 9*8 -4 && evt.y < 9*8 - 4 + 14) {
				caps = !caps;
				shift = caps;
			}

			if((evt.x >= 2*8-4 && evt.x < 2*8-4 + 8*5 + 6 && evt.y >= 11*8 -4 && evt.y < 11*8 - 4 + 14) ||
			        (evt.x >= 28*8-4 && evt.x < 28*8-4 + 8*3 + 6 && evt.y >= 11*8 -4 && evt.y < 11*8 - 4 + 14)) {
				shift = !shift;
			}

			st->clear();
			*st << (const char *)((shift||caps)?uppercasekbd:lowercasekbd);
#endif

		} else {
			switch(evt.key) {
			case Event::KeyEsc:
				if(canClose) {
					m_loop = false;
					m_label = "";
				}
				break;
			case Event::KeyUp:
			case Event::KeyDown:
				m_delta = 0;
				break;
			}
		}
	}
}

void TUIWindow::buildFromString(std::string s, bool ANSI) {
	int i=0,j=0;
	std::string label,text;

	do {
		j=0;
		label="";
		text="";

		switch(s[i+j++]) {
		case '!':
			while(((i+j) < (int)s.length()) && s[i+j]!=';')
				label += s[i+j++];
			j++;
			while(((i+j) < (int)s.length()) && s[i+j]!='\r' && s[i+j]!='\n')
				text += s[i+j++];
			addWidget(new TUIHyperLink(label,text));
			break;
		case '$':
			while(((i+j) < (int)s.length()) && s[i+j]!='\r' && s[i+j]!='\n')
				text += s[i+j++];
			for(int x=0; x< (m_w - (int)text.length() - 4) / 2; x++) {
				label += " ";
			}
			addWidget(new TUILabel(label + text,true,ANSI));
			break;
		default:
			j--;
			while(((i+j) < (int)s.length()) && s[i+j]!='\r' && s[i+j]!='\n') {
				if(s[i+j] == '\t') {
					for(int t=(10 - (int)(text.length() % 10)); t>0; t--) {
						text += " ";
					}
				} else {
					text += s[i+j];
				}
				j++;
				if(j>=m_w - 3 && s[i+j] != '\r' && s[i+j] != '\n') {
					addWidget(new TUILabel(text,false,ANSI));
					text = "";
					i+=j;
					j=0;
				}
			}
			addWidget(new TUILabel(text,false,ANSI));
			break;
		}
		i+=j+1;
	} while (i < (int)s.length());
}

void draw_shadow(Console *console, int x, int y) {
	console->putColor(x,y, BLACK|HIGH_INTENSITY);
}

void draw_box(Console *console, int x, int y,int w,int h,int fg,int bg, bool shadow) {
	int i,j;
	//draw a box using IBM extended ASCII
	console->putColor(x,y,fg | (bg << 8));
	console->putChar(x,y,218);
#ifdef USE_3DMODEL

	gl->clear(x,y);
#endif

	for(i=0;i<w;i++) {
		console->putColor(x+i+1,y,fg | (bg << 8));
		console->putChar(x+i+1,y,196);
#ifdef USE_3DMODEL

		gl->clear(x+i+1,y);
#endif

	}

	console->putColor(x+w+1,y,fg | (bg << 8));
	console->putChar(x+w+1,y,191);
#ifdef USE_3DMODEL

	gl->clear(x+w+1,y);
#endif

	for(i=0;i<h;i++) {
		if(shadow)
			draw_shadow(console,x+w+2,y+i+1);
		if(shadow)
			draw_shadow(console,x+w+3,y+i+1);
		console->putColor(x,y+i+1,fg | (bg << 8));
		console->putChar(x,y+i+1,179);
#ifdef USE_3DMODEL

		gl->clear(x,y+i+1);
#endif

		for(j=0;j<w;j++) {
			console->putColor(x+j+1,y+i+1,fg | (bg << 8));
			console->putChar(x+j+1,y+i+1,' ');
#ifdef USE_3DMODEL

			gl->clear(x+j+1, y+i+1);
#endif

		}

		console->putColor(x+j+1,y+i+1,fg | (bg << 8));
		console->putChar(x+j+1,y+i+1,179);
#ifdef USE_3DMODEL

		gl->clear(x+j+1,y+i+1);
#endif

		console->color(fg,bg);
	}

	console->locate(x,y+h+1);
	console->printf("%c",192);
	for(i=0;i<w;i++) {
		console->printf("%c",196);
#ifdef USE_3DMODEL

		gl->clear(x+i,y+h+1);
#endif

	}

	console->printf("%c",217);
	if(shadow) {
		draw_shadow(console,x+w+2,y+h+1);
		draw_shadow(console,x+w+3,y+h+1);

		for(i=0;i<w+2;i++)
			draw_shadow(console,x+2+i,y+h+2);
	}
}

#if TIKI_PLAT == TIKI_NDS
extern int disp_off_x,disp_off_y;
#endif

TUIWidget *TUIWindow::widgetAtOffset(int offset) {
	std::vector<TUIWidget *>::iterator widget_iter = m_widgets.begin();
	int i = 0;
	do {
		i += (*widget_iter)->getHeight();
	} while(offset >= i && widget_iter++ != m_widgets.end());
	return (*widget_iter);
}

int TUIWindow::widgetY(TUIWidget *widget) {
	int i = 0;
	std::vector<TUIWidget *>::iterator widget_iter = m_widgets.begin();
	while(widget_iter != m_widgets.end() && widget != (*widget_iter)) {
		i += (*widget_iter)->getHeight();
		widget_iter++;
	}
	return i;
}

void TUIWindow::doMenu(bool canClose) {
	EventCollector ec;
	Event evt;
	std::vector<TUIWidget *>::iterator widget_iter;
	int i=0;
	m_dirty = 1;
	m_loop = true;
	widgetAtOffset(m_offset)->focus(true);
	if(playerEventCollector != NULL && playerEventCollector->listening()) {
		playerEventCollector->stop();
		if(player != NULL)
			player->setHeading(player->heading());
	}
	activeWindow = this;
#if TIKI_PLAT == TIKI_NDS

	BG0_X0 = 8;
	BG1_X0 = 8;

	SUB_BG0_X0 = 0;
	SUB_BG1_X0 = 0;

	lcdMainOnTop();
	st->setANSI(true);
	st->clear();
	*st << (const char *)lowercasekbd;
	mt = new Console(m_w+2, m_h+2, false);
#else

	mt = new Console(m_w+2, m_h+2, zzt_font);
#endif
#if defined(USE_SDL)

	mt->setSize((m_w+2) * 8, (m_h+2) * 16);
	mt->translate(Vector(m_x*8,m_y*16,0.8) + Vector((m_w+2)*4 , (m_h+2)*8, 0));
#else

	mt->setSize((m_w+2) * 8, (m_h+2) * 20);
	mt->translate(Vector(m_x*8,m_y*20,0.8) + Vector((m_w+2)*4 , (m_h+2)*10, 0.1));
#endif

	zoom = 1;

	do {
		i=0;
		draw_box(mt, 0, 0, m_w, m_h, WHITE|HIGH_INTENSITY, BLUE, false);
		mt->color(YELLOW | HIGH_INTENSITY, BLUE);
		if(widgetAtOffset(m_offset)->getHelpText() != "\0") {
			mt->locate(((int)(m_w-widgetAtOffset(m_offset)->getHelpText().length())-1)/2,1);
			*mt << "\xae " << widgetAtOffset(m_offset)->getHelpText() << " \xaf";
		} else {
			mt->locate((m_w-(int)m_title.length()+2)/2,1);
			*mt << m_title;
		}
		mt->locate(0,2);
		mt->color(WHITE|HIGH_INTENSITY,BLUE);
		mt->printf("%c",195);
		for(int x=0;x<m_w;x++)
			mt->printf("%c",196);
		mt->printf("%c",180);
		mt->color(RED|HIGH_INTENSITY,BLUE);
		mt->locate(1,m_h/2+1);
		mt->printf("%c",175);
		mt->locate(m_w,m_h/2+1);
		mt->printf("%c",174);

		mt->setANSI(true);

		if(m_offset < m_h/2-2) {
			for(i=0; i < m_h/2-2-m_offset; i++) {
				mt->locate(2,3+i);
				if(m_h/2-2 - m_offset - i == 4) {
#if TIKI_PLAT == TIKI_NDS
					mt->locate(0,3+i);
#endif

					*mt << " \x1b[1;36mUse \x1b[37m\x18 \x1b[36mand \x1b[37m\x19 \x1b[36mto scroll and press \x1b[37m" << ((TIKI_PLAT == TIKI_DC) ? "Start " : "Enter ") << "\x1b[36mor";
				}
				if(m_h/2-2 - m_offset - i == 3) {
#if TIKI_PLAT == TIKI_NDS
					mt->locate(0,3+i);
#endif

					*mt << " \x1b[1;37m" << ((TIKI_PLAT == TIKI_DC || TIKI_PLAT == TIKI_NDS) ? "A " : "Space ") << "\x1b[36mto select.";
					if(canClose)
						*mt << "  Press \x1b[37m" << ((TIKI_PLAT == TIKI_DC || TIKI_PLAT == TIKI_NDS) ? "B" : "ESC ") << "\x1b[36m to close.    ";
				}
				if(m_h/2-2 - m_offset - i == 1) {
					mt->color(YELLOW|HIGH_INTENSITY,BLUE);
					for(int u=0;u<m_w-3;u++) {
						mt->printf("%c",u%5==0?7:' ');
					}
				}
			}
		}

		mt->setANSI(false);

		for(widget_iter = find(m_widgets.begin(),m_widgets.end(),widgetAtOffset(m_offset-(m_h/2-2))); widget_iter != m_widgets.end() && i < m_h-2; widget_iter++) {
			mt->locate(3,3+i);
			(*widget_iter)->update();
			(*widget_iter)->draw(mt,  m_offset + i - widgetY(*widget_iter) - (m_h/2-2), m_h-2-i, m_offset-widgetY(*widget_iter));
			i += (*widget_iter)->getHeight() - (m_offset + i - widgetY(*widget_iter) - (m_h/2-2));
		}

		mt->color(RED|HIGH_INTENSITY,BLUE);
		mt->locate(1,m_h/2+1);
		mt->printf("%c",175);
		mt->locate(m_w,m_h/2+1);
		mt->printf("%c",174);

		if(i < m_h-2) {
			mt->locate(2,3+i++);
			mt->color(YELLOW|HIGH_INTENSITY,BLUE);
			for(int u=0;u<m_w-3;u++) {
				mt->printf("%c",u%5==0?7:' ');
			}
		}
		render();
		while (ec.getEvent(evt)) {
			processHidEvent(evt,canClose);
		}
		if(m_delta != 0 && Time::gettime() - m_repeatTimer > 150000) {
			widgetAtOffset(m_offset)->focus(false);
			m_offset+=m_delta;
			m_repeatTimer = Time::gettime();
		}
		if(m_offset < 0)
			m_offset = 0;
		if(m_offset >= widgetsHeight())
			m_offset = widgetsHeight() - 1;
		widgetAtOffset(m_offset)->focus(true);
	} while(m_loop);

	activeWindow = NULL;
#if TIKI_PLAT == TIKI_NDS

	BG0_X0 = 0;
	BG1_X0 = 0;

	SUB_BG0_X0 = -48;
	SUB_BG1_X0 = -48;

	lcdMainOnBottom();
	st->color(15,1);
	st->clear();
	st->draw();
	dzzt_logo();
#endif

	if(playerEventCollector != NULL && !playerEventCollector->listening()) {
		playerEventCollector->start();
		draw_hud_ingame();
	}
	delete mt;
	mt = NULL;
	render();
}
