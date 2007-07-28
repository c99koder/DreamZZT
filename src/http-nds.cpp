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
#include <Tiki/tiki.h>
#include <Tiki/file.h>

using namespace Tiki;
using namespace Tiki::GL;

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <nds.h>
#include <dswifi9.h>
using namespace std;

#include "console.h"
#include "http.h"
#include "version.h"

#define USER_AGENT (std::string("DreamZZT/") + std::string(VERSION) + std::string(" (Nintendo DS)")).c_str()

std::string curl_auth_string = "sam:027325";
bool networkEnabled = false;
extern ConsoleText *st;

int base64_encode(const void *inp, int insize, char **outptr);

void (*status_callback)(char *)=NULL;
fd_set socks;

int net_readline(int sock, char *buf, int bufsize) {
  int r, rt;
  char c;

  rt = 0;
  buf[0]='\0';
  do {
    r = recv(sock, &c, 1,0);
    if (r<1)
      return -1;
    if (rt < bufsize)
      buf[rt++] = c;
  } while (c != '\n');

  buf[rt-1] = 0;
  if (buf[rt-2] == '\r')
    buf[rt-2] = 0;
  return 0;
}

int net_writeline(int sock, char *buf) {
  return send(sock,buf,strlen(buf),0);
}

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

long unsigned int resolve(char *name) {
  char tmp[200];
  struct hostent *h=gethostbyname(name);
  if(h==NULL) {
		perror("gethostbyname");
		return 0;
	}
  return *( (unsigned long *)h->h_addr_list[0]);
}

std::string http_recieve_chunked(int s) {
  int x,z,y,chunksize;
  char *tmp;
  std::string output = "";

  y=0;
  do {
    tmp=(char *)malloc(100);
	do { //sometimes apache sends us blank lines, so keep looping until we reach another chunk
		if(net_readline(s,tmp,255)==-1) return ""; //chunk size
	} while(strlen(tmp)<1);
    for(z=0;z<strlen(tmp);z++) { //eliminate whitespace
      if(hex_to_int(tmp[z])==-1) {
        tmp[z]='\0';
        break;
      }
    }
    chunksize=0;
    if(strlen(tmp)>0) {
      for(z=0;z<strlen(tmp);z++) {
        x=pow(16.0f,int(strlen(tmp)-z-1));
        chunksize+=hex_to_int(tmp[z])*x;
      }
    }
    //printf("Chunksize: %i bytes (%s)\n",chunksize,tmp);
    free(tmp);

    tmp=(char *)malloc(chunksize+1);
		z=0;
    if(chunksize>0) {
			while(z<chunksize) {
				x=recv(s,tmp,chunksize,0);
				output.append(tmp,chunksize);
				z+=x;
			}
    }
    free(tmp);
    y+=chunksize;
  } while(chunksize>0);
  return output;
}

std::string http_get_string(std::string URL) {
  int s;
  struct sockaddr_in sinRemote;
  char *tmp;
  char name[256];
  char value[256];
  int x,y=0,z=0;
  int mode=0,len=0;
  char msg[300];
  std::string output;
char *auth;

base64_encode(curl_auth_string.c_str(),curl_auth_string.length(),&auth);

  if(Wifi_AssocStatus() != ASSOCSTATUS_ASSOCIATED)
	{ // simple WFC connect:
		st->printf("Connecting via WFC data\n");
		st->draw();
		int i;
		Wifi_AutoConnect(); // request connect
		while(1) {
			i=Wifi_AssocStatus(); // check status
			if(i==ASSOCSTATUS_ASSOCIATED) {
				networkEnabled = true;
				st->printf("Connected successfully!\n");
				st->draw();
				break;
			}
			if(i==ASSOCSTATUS_CANNOTCONNECT) {
				st->printf("Could not connect!\n");
				st->draw();
                return "";
				break;
			}
		}
	} // if connected, you can now use the berkley sockets interface to connect to the internet!


  if(URL.find("http://") == 0) {
	URL = URL.substr(7);	
  }

  std::string host = URL.substr(0, URL.find("/"));
  std::string path = URL.substr(URL.find("/"));

st->printf("Host: %s\nPath: %s\n",host.c_str(),path.c_str());
	st->draw();

  s = socket(AF_INET, SOCK_STREAM, 0);

  sinRemote.sin_family = AF_INET;
st->printf("Looking up hostname...\n");
st->draw();
  sinRemote.sin_addr.s_addr = resolve((char *)"192.168.11.2"/*host.c_str()*/);
st->printf("Got an IP!\n");
st->draw();
  if(sinRemote.sin_addr.s_addr==0) return "";
  sinRemote.sin_port = htons(80);
  
  connect(s, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in));

st->printf("Connected to server!\n");
st->draw();

  tmp=(char *)malloc(1024);
  sprintf(tmp,"GET %s HTTP/1.1\r\n\
Host: %s:%i\r\n\
User-Agent: %s\r\n\
Connection: keep-alive\r\n\
Authorization: Basic %s\r\n\
\r\n",path.c_str(),host.c_str(),80,USER_AGENT,auth);
st->printf("Sending: %s\n", tmp);
st->draw();
  if(net_writeline(s,tmp)<0) return "";

st->printf("Sent our request!\n");
st->draw();

  net_readline(s,tmp,255);
st->printf("Received: %s\n", tmp);
	st->draw();
  strtok(tmp," "); //HTTP/1.1

  do {
	tmp[0]='\0';
    if(net_readline(s,tmp,255)==-1) break;
    if(tmp[0]!='\0') {
      //st->printf("Header: %s (%i)\n",tmp,strlen(tmp));
	//st->draw();
      strcpy(name,strtolower(strtok(tmp,":")));
      strcpy(value,strtolower(strtok(NULL," ")));
      if(value[0]==' ') {
        for(x=0;x<strlen(value)-1;x++) {
          value[x]=value[x+1];
        }
      }
      if(name!=NULL) {
        if(!strcmp("transfer-encoding",name) && !strcmp("chunked",value)) {
          mode=1;
        }
        if(!strcmp("content-length",name)) {
          len=atoi(value);
        }
        if(!strcmp("content-type",name)) {
          //strcpy(content_type,strtok(value,";"));
        }
        if(!strcmp("location",name)) {
		  shutdown(s,1);
		  closesocket(s);
		free(tmp);	
			return http_get_string(value);
        }
      }
    }
  } while(tmp[0]!='\0');

  z=0;
  free(tmp);

  if(mode==1) {
    output = http_recieve_chunked(s);
  } else if(len>0) {
    tmp=(char *)malloc(2048);
	output = "";
    while(y<len) {
	  do {
      x=recv(s,tmp,2048,0);
	  tmp[x]='\0';
	  } while (x<1);
      output.append(tmp);
      y+=x;
    }
    free(tmp);
  }
  shutdown(s,1);
  closesocket(s);
  return output;
}

bool http_get_file(std::string filename, std::string URL) {
}

std::string http_post_file(std::string filename, std::string contentType, std::string URL) {
}

std::string http_post_data(std::string data, std::string contentType, std::string URL) {
}