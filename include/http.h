/* http.h - HTTP file downloader
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

#ifndef WIN32
#define SOCKET int
#else
#include <windows.h>
#define uint32_t unsigned long
#endif
typedef void (http_status_callback)(char *msg);
int net_readline(SOCKET sock, char *buf, int bufsize);
int net_writeline(SOCKET sock, char *buf);
int http_recieve_chunked(SOCKET s, FILE *fd);
void set_status_callback(http_status_callback *fn);
void http_get_file(char *fn, const char *host, int port, const char *filename, char *content_type, int *content_length);
unsigned long resolve(char *name);
char *strtolower(char *str);

