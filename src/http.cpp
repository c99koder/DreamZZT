/* http.cpp - HTTP file downloader
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

#include <string>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <Tiki/tiki.h>
#include <Tiki/file.h>
#include "console.h"

using namespace Tiki;
using namespace Tiki::GL;

#include "http.h"

#ifdef DREAMCAST
#define USER_AGENT "DreamZZT/3.0.6.1 (Dreamcast)"
#endif
#if TIKI_PLAT == TIKI_SDL
#define USER_AGENT "DreamZZT/3.0.6.1 (Linux)"
#endif
#if TIKI_PLAT == TIKI_WIN32
#define USER_AGENT "DreamZZT/3.0.6.1 (Windows)"
#endif
#if TIKI_PLAT == TIKI_OSX
#define USER_AGENT "DreamZZT/3.0.6.1 (Macintosh)"
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
	size_t x;
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

std::string curl_auth_string = "";

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
	return ((File *)data)->write(ptr, (int)(size * nmemb));
}

extern ConsoleText *ct;
void render();

int http_progress(void *clientp,
									double dltotal,
									double dlnow,
									double ultotal,
									double ulnow) {
	ct->locate(0,24);
	ct->color(0,7);
	for(int i=0; i<60; i++) {
		*ct << " ";
	}
	ct->locate(0,24);
	if(dltotal > 0) {
		*ct << "Downloading... (" << (int)(((float)dlnow/(float)dltotal) * 100.0f) << "%)";
	} else if(ultotal > 0) {
		*ct << "Uploading... (" << (int)(((float)ulnow/(float)ultotal) * 100.0f) << "%)";
	}
	render();
	return 0;
}

int debug_callback(CURL *curl_handle, curl_infotype type, char *txt, size_t len, void *data) {
	char tmp[1024];

	strncpy(tmp,txt,len);
	tmp[len]='\0';
	Debug::printf("%s\n",tmp);
	return 0;
}

CURL *http_begin(std::string URL) {
	CURL *curl_handle;
	static char url[256];
	static char auth[256];

	strncpy(url,URL.c_str(),256); //Windows... sigh.
	strncpy(auth,curl_auth_string.c_str(),256); //Windows... sigh.

	ct->locate(0,24);
	ct->color(0,7);
	for(int i=0; i<60; i++) {
		*ct << " ";
	}
	ct->locate(0,24);
	*ct << "Loading...";
	render();
	
	/* init the curl session */
	curl_handle = curl_easy_init();

	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);

	/* some servers don't like requests that are made without a user-agent
		field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT);
	
	/* DreamZZT Online authentication */
	curl_easy_setopt(curl_handle, CURLOPT_USERPWD, auth);
	
	/* Progress function */
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(curl_handle, CURLOPT_PROGRESSFUNCTION, http_progress);
	
	return curl_handle;
}

void http_finish(CURL *curl_handle) {
	/* get it! */
	curl_easy_perform(curl_handle);
	
	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);
	
	ct->locate(0,24);
	ct->color(0,7);
	for(int i=0; i<60; i++) {
		*ct << " ";
	}
	ct->locate(0,24);

	render();	
}

std::string http_get_string(std::string URL) {
	CURL *curl_handle = http_begin(URL);
	struct MemoryStruct chunk;
	std::string output;
	
	chunk.memory=NULL; /* we expect realloc(NULL, size) to work */
	chunk.size = 0;		/* no data at this point */
	
	
	/* send all data to this function	*/
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	http_finish(curl_handle);
	
	if(chunk.memory != NULL) {
		output.append((const char *)chunk.memory);
		free(chunk.memory);
	}
	return output;
}

bool http_get_file(std::string filename, std::string URL) {
	CURL *curl_handle = http_begin(URL);
	static File f(filename,"wb");
	
	/* send all data to this function	*/
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteTikiFileCallback);
	
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&f);
	
	http_finish(curl_handle);
	
	f.close();
	return true;
}

std::string http_post_file(std::string filename, std::string contentType, std::string URL) {
	CURL *curl_handle = http_begin(URL);
	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;
	struct MemoryStruct chunk;
	std::string output;
	
	chunk.memory=NULL; /* we expect realloc(NULL, size) to work */
	chunk.size = 0;		/* no data at this point */	
	
	/* send all data to this function	*/
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	
	/* Add file/contenttype section */
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "File",
							 CURLFORM_FILE, filename.c_str(),
							 CURLFORM_CONTENTTYPE, contentType.c_str(), CURLFORM_END);
	
	/* Set the form info */
	curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
	
	http_finish(curl_handle);
	
	if(chunk.memory != NULL) {
		output.append((const char *)chunk.memory);
		free(chunk.memory);
	}
	return output;
}

std::string http_post_data(std::string data, std::string contentType, std::string URL) {
	CURL *curl_handle = http_begin(URL);
	struct MemoryStruct chunk;
	struct curl_slist *slist=NULL;
	std::string output;
	
	chunk.memory=NULL; /* we expect realloc(NULL, size) to work */
	chunk.size = 0;		/* no data at this point */	
	
	/* send all data to this function	*/
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	
	/* Set the form info */
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data.c_str());


	slist = curl_slist_append(slist, (std::string("Content-Type: ") + contentType).c_str());
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);
	
	http_finish(curl_handle);
	
	if(chunk.memory != NULL) {
		output.append((const char *)chunk.memory);
		free(chunk.memory);
	}
	curl_slist_free_all(slist);
	
	return output;
}
