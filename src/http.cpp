/* http.cpp - HTTP file downloader
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

#include <string>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <Tiki/tiki.h>
#include <Tiki/file.h>

using namespace Tiki;

#include "http.h"

#ifdef DREAMCAST
#define USER_AGENT "DreamZZT/3.0.6 (Dreamcast)"
#endif
#if TIKI_PLAT == TIKI_SDL
#define USER_AGENT "DreamZZT/3.0.6 (Linux)"
#endif
#if TIKI_PLAT == TIKI_WIN32
#define USER_AGENT "DreamZZT/3.0.6 (Windows)"
#endif
#if TIKI_PLAT == TIKI_OSX
#define USER_AGENT "DreamZZT/3.0.6 (Macintosh)"
#endif

int hex_to_int(char c) {
  if(c>='0' && c <='9') {
    return c-'0';
  }
  if(c>='a' && c <='f') {
    return 10+(c-'a');
  }
  if(c>='A' && c <='F') {
    return 10+(c-'A');
  }
  return -1;
}

char *strtolower(char *str) {
  int x;
  static char buf[256];
  if(str==NULL) return NULL;
  strcpy(buf,str);
  for(x=0;x<strlen(buf);x++) {
    if(buf[x]>='A' && buf[x]<='Z') {
      buf[x]+=('a'-'A');
    }
  }
  return buf;
}

struct MemoryStruct {
  char *memory;
  size_t size;
};

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;
	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}

size_t WriteTikiFileCallback(void *ptr, size_t size, size_t nmemb, void *data) {
	return ((File *)data)->write(ptr,size * nmemb);
}

std::string http_get_string(std::string URL) {
	CURL *curl_handle;
  struct MemoryStruct chunk;
	std::string output;
	
  chunk.memory=NULL; /* we expect realloc(NULL, size) to work */
	chunk.size = 0;    /* no data at this point */
	
  /* init the curl session */
  curl_handle = curl_easy_init();
	
  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, URL.c_str());
	
  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	
  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
  
	/* some servers don't like requests that are made without a user-agent
		field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT);
	
  /* get it! */
  curl_easy_perform(curl_handle);
	
  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

	output.append((const char *)chunk.memory);
	free(chunk.memory);
	return output;
}

bool http_get_file(std::string filename, std::string URL) {
	CURL *curl_handle;
	File f(filename,"wb");
	
  /* init the curl session */
  curl_handle = curl_easy_init();
	
  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, URL.c_str());
	
  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteTikiFileCallback);
	
  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&f);
  
	/* some servers don't like requests that are made without a user-agent
		field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT);
	
	/* get it! */
  curl_easy_perform(curl_handle);
	
  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);
	
	f.close();
}
