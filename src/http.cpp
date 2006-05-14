#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#include <fcntl.h>
#include <sys/stat.h>
#define uint32_t u_long
#endif
#ifdef UNIX
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SOCKET int
#endif
#ifdef DREAMCAST
#include <kos.h>
#include <lwip/lwip.h>
#include <lwip/sockets.h>
#define SOCKET int
#endif
#include <string.h>
#include <math.h>
#include <stdio.h>
#ifndef DREAMCAST
#include <fstream>
using namespace std;
#endif

#ifdef NET
#include "http.h"

#ifdef DREAMCAST
#define USER_AGENT "DCSquares/2.0.0 (Dreamcast)"
#endif
#ifdef LINUX
#define USER_AGENT "DCSquares/2.0.0 (Linux)"
#endif
#ifdef WIN32
#define USER_AGENT "DCSquares/2.0.0 (Windows)"
#endif
#ifdef MACOS
#define USER_AGENT "DCSquares/2.0.0 (Macintosh)"
#endif

void (*status_callback)(char *)=NULL;
fd_set socks;

void set_status_callback(http_status_callback *fn) {
  status_callback=fn;
}

int net_readline(SOCKET sock, char *buf, int bufsize) {
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

int net_writeline(SOCKET sock, char *buf) {
  return send(sock,buf,strlen(buf),0);
}
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

#ifdef NET

#ifdef WIN32
uint32_t resolve(char *name) {
  char tmp[200];
  struct hostent *h=gethostbyname(name);
  if(h==NULL) {
		printf("Gethostbyname: %i\n",WSAGetLastError());
		return 0;
	}
  return *(int *)h->h_addr;
}
#endif
#ifdef UNIX
long unsigned int resolve(char *name) {
  char tmp[200];
  struct hostent *h=gethostbyname(name);
  if(h==NULL) {
		perror("gethostbyname");
		return 0;
	}
  return *(int *)h->h_addr;
}
#endif
#ifdef DREAMCAST
uint32 resolve(char *name) {
    struct sockaddr_in sinRemote;
	char msg[200];
	uint8 ip[4];
  uint32 host_ip;
#define READIP(dst, src) \
        dst = ((src[0]) << 24) | \
                ((src[1]) << 16) | \
                ((src[2]) << 8) | \
                ((src[3]) << 0)
				
  sinRemote.sin_family = AF_INET;
  sinRemote.sin_addr.s_addr = htonl(lwip_netcfg.dns[0]);
  sinRemote.sin_port = htons(53);
  status_callback("Resolving host...");
  if(lwip_gethostbyname(&sinRemote,name,ip)<0) return 0;
  READIP(host_ip,ip);
  sprintf(msg,"%i.%i.%i.%i",ip[0],ip[1],ip[2],ip[3]);
  status_callback(msg);
  return htonl(host_ip);
}
#endif

#endif

#ifdef NET
void http_get_file(char *fn, const char *host, int port, const char *filename, char *content_type, int *content_length) {
  SOCKET s;
  struct sockaddr_in sinRemote;
  char *tmp;
  char name[256];
  char value[256];
  int x,y=0,z=0;
  FILE *fd;
  int mode=0,len=0;
  char msg[300];

  if(status_callback!=NULL) {
    sprintf(msg,"Connecting to %s...",host);
    status_callback(msg);
  }

  s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

  sinRemote.sin_family = AF_INET;
  sinRemote.sin_addr.s_addr = resolve((char *)host);
	if(sinRemote.sin_addr.s_addr==0) return;
  sinRemote.sin_port = htons(port);
  
  connect(s, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in));
  
  if(status_callback!=NULL) {
    sprintf(msg,"Requesting %s...",filename);
    status_callback(msg);
  }

  tmp=(char *)malloc(1024);
  sprintf(tmp,"GET %s HTTP/1.1\r\n\
Host: %s:%i\r\n\
User-Agent: %s\r\n\
Connection: keep-alive\r\n\
\r\n",filename,host,port,USER_AGENT);
  if(net_writeline(s,tmp)<0) return;

  net_readline(s,tmp,255);
  strtok(tmp," "); //HTTP/1.1

  *content_length=0;
  strcpy(content_type,"unknown");

if(!strcmp("200",strtok(NULL," "))) {

  do {
	tmp[0]='\0';
    if(net_readline(s,tmp,255)==-1) break;
    if(tmp[0]!='\0') {
      //printf("Header: %s (%i)\n",tmp,strlen(tmp));
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
          *content_length=len;
        }
        if(!strcmp("content-type",name)) {
          strcpy(content_type,strtok(value,";"));
        }
      }
    }
  } while(tmp[0]!='\0');

  z=0;
  free(tmp);

  if(status_callback!=NULL) {
    sprintf(msg,"Downloading...");
    status_callback(msg);
  }
  fd=fopen(fn,"wb");
  if(!fd) {
    status_callback("Cannot open file.\n");
    return;
  }
  //printf("Opened %s as %i\n",fn,fd);

  if(mode==1) {
    *content_length=http_recieve_chunked(s,fd);
  } else if(len>0) {
    tmp=(char *)malloc(2048);
    while(y<len) {
	  do {
      x=recv(s,tmp,2048,0);
	  } while (x<1);
      fwrite(tmp,1,x,fd);
      y+=x;
      if(status_callback!=NULL) {
        sprintf(msg,"Downloading %s (%i%%)...",filename,(int)(((float)y/(float)len)*100.0f));
        status_callback(msg);
      }
    }
    free(tmp);
  }
} else {
  if(status_callback!=NULL) {
    sprintf(msg,"Unable to download file");
    status_callback(msg);
  }
}
  shutdown(s,1);
#ifdef WIN32
  closesocket(s);
#else
  close(s);
#endif
fclose(fd);
  //printf("Payload: %s\n",buf);
  if(status_callback!=NULL) {
    sprintf(msg,"Done!");
    status_callback(msg);
  }
}

int http_recieve_chunked(SOCKET s, FILE *fd) {
  int x,z,y,chunksize;
  char *tmp;

  y=0;
  do {
    tmp=(char *)malloc(100);
	do { //sometimes apache sends us blank lines, so keep looping until we reach another chunk
		if(net_readline(s,tmp,255)==-1) return -1; //chunk size
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
				fwrite(tmp,1,x,fd);
				z+=x;
			}
    }
    free(tmp);
    y+=chunksize;
  } while(chunksize>0);
  return y;
}
#endif
