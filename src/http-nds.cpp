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
#include <Tiki/tikitime.h>
#include <Tiki/hid.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;

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
#if TIKI_PLAT == TIKI_NDS
#include <nds.h>
#include <dswifi9.h>
#else
#define closesocket close
#endif
using namespace std;

#include "console.h"
#include "http.h"
#include "version.h"
#include "object.h"
#include "window.h"

#define USER_AGENT (std::string("DreamZZT/") + std::string(VERSION) + std::string(" (Nintendo DS)")).c_str()

std::string curl_auth_string = "";
bool networkEnabled = false;
extern ConsoleText *ct;
extern ConsoleText *st;

void render();

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
	if(c>='0' && c <='9') return c-'0';
	if(c>='a' && c <='f') return 10+(c-'a');
	if(c>='A' && c <='F') return 10+(c-'A');
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

char *http_recieve_chunked(int s, int *len) {
	int x,z,chunksize,total=*len;
	unsigned int y=0;
	char *tmp;
	char *output = (char *)malloc(100);

	do {
		tmp=(char *)malloc(100);
		do { //sometimes apache sends us blank lines, so keep looping until we reach another chunk
			if(net_readline(s,tmp,100)==-1) return NULL; //chunk size
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
		free(tmp);

		z=0;
		if(chunksize>0) {
			output = (char *)realloc(output, y + chunksize + 1);
			while(z<chunksize) {
				z+=recv(s, output + y + z,chunksize - z,0);
				y+=z;
				ct->locate(6,12);
				ct->color(14,1);
				*ct << "Received " << (int)y << " bytes";
				render();
				Time::sleep(50000);
			}
		}
	} while(chunksize>0);
	output[y] = '\0';
	*len = y;
	return output;
}

char *http_get(std::string URL, int *length) {
	int s;
	struct sockaddr_in sinRemote;
	char *tmp;
	char name[256];
	char value[256];
	int x,z=0;
	unsigned int y=0;
	int mode=0,len=0;
	char msg[300];
	char *output=NULL;
	char *auth;

	base64_encode(curl_auth_string.c_str(),curl_auth_string.length(),&auth);
#if TIKI_PLAT == TIKI_NDS
  if(Wifi_AssocStatus() != ASSOCSTATUS_ASSOCIATED) { // simple WFC connect:
		ct->color(15,1);
		ct->locate(0,0);
		ct->printf("Connecting via WFC data\n");
		render();
		int i;
		Wifi_AutoConnect(); // request connect
		while(1) {
			i=Wifi_AssocStatus(); // check status
			if(i==ASSOCSTATUS_ASSOCIATED) {
				networkEnabled = true;
				ct->printf("Connected successfully!\n");
				render();
				break;
			}
			if(i==ASSOCSTATUS_CANNOTCONNECT) {
				ct->printf("Could not connect!\n");
				render();
                return NULL;
				break;
			}
		}
	} // if connected, you can now use the berkley sockets interface to connect to the internet!
#endif

	if(URL.find("http://") == 0) {
		URL = URL.substr(7);	
	}

	std::string host = URL.substr(0, URL.find("/"));
	std::string path = URL.substr(URL.find("/"));

	s = socket(AF_INET, SOCK_STREAM, 0);

	sinRemote.sin_family = AF_INET;
	sinRemote.sin_addr.s_addr = resolve((char *)host.c_str());
	if(sinRemote.sin_addr.s_addr==0) return NULL;
	sinRemote.sin_port = htons(80);

	connect(s, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in));

	tmp=(char *)malloc(1024);
	sprintf(tmp,"GET %s HTTP/1.1\r\n\
Host: %s:%i\r\n\
User-Agent: %s\r\n\
Connection: keep-alive\r\n\
Authorization: Basic %s\r\n\
\r\n",path.c_str(),host.c_str(),80,USER_AGENT,auth);
	if(net_writeline(s,tmp)<0) return NULL;

	draw_box(ct, 4, 11, 24, 1, 15, 1, false);
	ct->locate(10,11);
	ct->color(15,1);
	*ct << " Downloading ";
	
	net_readline(s,tmp,255);
	strtok(tmp," "); //HTTP/1.1

	do {
		tmp[0]='\0';
		if(net_readline(s,tmp,255)==-1) break;
		if(tmp[0]!='\0') {
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
					*length = len;
				}
				if(!strcmp("content-type",name)) {
					//strcpy(content_type,strtok(value,";"));
				}
				if(!strcmp("location",name)) {
					shutdown(s,1);
					closesocket(s);
					free(tmp);	
					return http_get(value,length);
				}
			}
		}
	} while(tmp[0]!='\0');

	z=0;
	free(tmp);

	if(mode==1) {
		output = http_recieve_chunked(s,length);
	} else if(len>0) {
		TUIMeter m(&y,len,22);
		output=(char *)malloc(len+1);
		while(y<len) {
			do {
				x=recv(s,output+y,255,0);
			} while (x<1);
			output[y+x]='\0';
			y+=x;
			ct->locate(6,12);
			ct->color(14,1);
			m.draw(ct);
			render();
			Time::sleep(50000);
		}
	}
	shutdown(s,1);
	closesocket(s);
	return output;
}

std::string http_get_string(std::string URL) {
	int len;
	char *data = http_get(URL, &len);
	if(data != NULL) {
		std::string output = data;
		free(data);
		return output;
	} else {
		return "";
	}
}

bool http_get_file(std::string filename, std::string URL) {
	File f;
	int len;
	char *data = http_get(URL, &len);
	if(data != NULL) {
		f.open(filename, "wb");
		f.write(data,len);
		f.close();
		free(data);
		return true;
	} else {
		return false;
	}
}

std::string http_post_file(std::string filename, std::string contentType, std::string URL) {
	int s;
	struct sockaddr_in sinRemote;
	char *tmp;
	char name[256];
	char value[256];
	int x,z=0;
	unsigned int y=0;
	int mode=0,len=0;
	char msg[300];
	char *output=NULL;
	char *auth;
	File f(filename,"rb");

	base64_encode(curl_auth_string.c_str(),curl_auth_string.length(),&auth);
#if TIKI_PLAT == TIKI_NDS
	if(Wifi_AssocStatus() != ASSOCSTATUS_ASSOCIATED) { // simple WFC connect:
		ct->locate(0,0);
		ct->color(15,1);
		ct->printf("Connecting via WFC data...\n");
		render();
		int i;
		Wifi_AutoConnect(); // request connect
		while(1) {
			i=Wifi_AssocStatus(); // check status
			if(i==ASSOCSTATUS_ASSOCIATED) {
				networkEnabled = true;
				ct->printf("Connected successfully!\n");
				render();
				break;
			}
			if(i==ASSOCSTATUS_CANNOTCONNECT) {
				ct->printf("Could not connect!\n");
				render();
				return "";
				break;
			}
		}
	} // if connected, you can now use the berkley sockets interface to connect to the internet!
#endif

	if(URL.find("http://") == 0) {
		URL = URL.substr(7);	
	}

	std::string host = URL.substr(0, URL.find("/"));
	std::string path = URL.substr(URL.find("/"));
	s = socket(AF_INET, SOCK_STREAM, 0);
	sinRemote.sin_family = AF_INET;
	sinRemote.sin_addr.s_addr = resolve((char *)host.c_str());
	if(sinRemote.sin_addr.s_addr==0) return "";
	sinRemote.sin_port = htons(80);

	connect(s, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in));

	draw_box(ct, 4, 11, 24, 1, 15, 1, false);
	ct->locate(12,11);
	ct->color(15,1);
	*ct << " Uploading ";

	std::string multipart_header = "------------------------------537a746a2d15\r\n\
Content-Disposition: form-data; name=\"File\"; filename=\""+filename.substr(filename.find_last_of("/\\")+1)+"\"\r\n\
Content-Type: "+contentType+"\r\n\r\n";

	tmp=(char *)malloc(f.total()+strlen("\r\n------------------------------537a746a2d15--\r\n"+1));
	sprintf(tmp,"POST %s HTTP/1.1\r\n\
Host: %s:%i\r\n\
User-Agent: %s\r\n\
Pragma: no-cache\r\n\
Accept: */*\r\n\
Authorization: Basic %s\r\n\
Content-Length: %i\r\n\
Expect: 100-continue\r\n\
Content-Type: multipart/form-data; boundary=----------------------------537a746a2d15\
\r\n\r\n",path.c_str(),host.c_str(),80,USER_AGENT,auth,f.total()+multipart_header.length()+strlen("\r\n------------------------------537a746a2d15--\r\n"));
	
	if(net_writeline(s,tmp)<0) return "";

	net_readline(s,tmp,255);

	net_writeline(s,(char *)multipart_header.c_str());
	f.read(tmp,f.total());
	strcpy(tmp+f.total(),"\r\n------------------------------537a746a2d15--\r\n");
	TUIMeter m(&y, f.total(), 22);
	for(y=0; y<f.total() + strlen("\r\n------------------------------537a746a2d15--\r\n"); y+=255) {
		send(s,tmp+y,255,0);
		ct->locate(6,12);
		ct->color(14,1);
		m.draw(ct);
		render();
		Time::sleep(50000);
	}

	f.close();
	net_readline(s,tmp,255);
	net_readline(s,tmp,255);

	draw_box(ct, 4, 11, 24, 1, 15, 1, false);
	ct->locate(10,11);
	ct->color(15,1);
	*ct << " Downloading ";

	do {
		tmp[0]='\0';
		if(net_readline(s,tmp,255)==-1) break;
		if(tmp[0]!='\0') {
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
			}
		}
	} while(tmp[0]!='\0');

	z=0; y=0;
	free(tmp);

	if(mode==1) {
		output = http_recieve_chunked(s, &len);
	} else if(len>0) {
		TUIMeter m2(&y, len, 22);
		output=(char *)malloc(len+1);
		while(y<len) {
			do {
				x=recv(s,output+y,255,0);
			} while (x<1);
			output[y+x]='\0';
			y+=x;
			ct->locate(6,12);
			ct->color(14,1);
			m2.draw(ct);
			render();
			Time::sleep(50000);
		}
	}
	shutdown(s,1);
	closesocket(s);
	std::string out = output;
	free(output);
	return out;
}

std::string http_post_data(std::string data, std::string contentType, std::string URL) {
	  int s;
	  struct sockaddr_in sinRemote;
	  char *tmp;
	  char name[256];
	  char value[256];
	  int x,y=0,z=0;
	  int mode=0,len=0;
	  char msg[300];
	char *output=NULL;
	char *auth;

	base64_encode(curl_auth_string.c_str(),curl_auth_string.length(),&auth);
#if TIKI_PLAT == TIKI_NDS
	  if(Wifi_AssocStatus() != ASSOCSTATUS_ASSOCIATED)
		{ // simple WFC connect:
			st->printf("\nConnecting via WFC data\n");
			render();
			int i;
			Wifi_AutoConnect(); // request connect
			while(1) {
				i=Wifi_AssocStatus(); // check status
				if(i==ASSOCSTATUS_ASSOCIATED) {
					networkEnabled = true;
					st->printf("Connected successfully!\n");
					render();
					break;
				}
				if(i==ASSOCSTATUS_CANNOTCONNECT) {
					st->printf("Could not connect!\n");
					render();
	                return NULL;
					break;
				}
			}
		} // if connected, you can now use the berkley sockets interface to connect to the internet!
#endif

	  if(URL.find("http://") == 0) {
		URL = URL.substr(7);	
	  }

	  std::string host = URL.substr(0, URL.find("/"));
	  std::string path = URL.substr(URL.find("/"));

	  s = socket(AF_INET, SOCK_STREAM, 0);

	  sinRemote.sin_family = AF_INET;
	  sinRemote.sin_addr.s_addr = resolve((char *)host.c_str());
	  if(sinRemote.sin_addr.s_addr==0) return NULL;
	  sinRemote.sin_port = htons(80);

	  connect(s, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in));

	  tmp=(char *)malloc(data.length() + 1024);
	  sprintf(tmp,"POST %s HTTP/1.1\r\n\
Host: %s:%i\r\n\
User-Agent: %s\r\n\
Connection: keep-alive\r\n\
Authorization: Basic %s\r\n\
Content-Type: %s\r\n\
Content-length: %i\r\n\
\r\n\
%s",path.c_str(),host.c_str(),80,USER_AGENT,auth,contentType.c_str(),data.length(),data.c_str());

for(y=0; y<strlen(tmp); y+=255) {
	send(s,tmp+y,255,0);
	*st << ".";
	render();
	Time::sleep(50000);
}

	  net_readline(s,tmp,255);
	  strtok(tmp," "); //HTTP/1.1

	  do {
		tmp[0]='\0';
	    if(net_readline(s,tmp,255)==-1) break;
	    if(tmp[0]!='\0') {
	      //st->printf("Header: %s (%i)\n",tmp,strlen(tmp));
		//render();
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
	      }
	    }
	  } while(tmp[0]!='\0');

	  z=0;
	  free(tmp);

	  if(mode==1) {
	    output = http_recieve_chunked(s, &len);
	  } else if(len>0) {
	    output=(char *)malloc(len+1);
		while(y<len) {
		  do {
		  x=recv(s,output+y,255,0);
		  } while (x<1);
		  output[y+x]='\0';
		  y+=x;
		*st << ".";
	render();
	Time::sleep(50000);
		}
	  }
	  shutdown(s,1);
	  closesocket(s);
	std::string out = output;
	free(output);
	  return out;	
}