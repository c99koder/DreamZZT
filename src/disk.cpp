#ifdef DREAMCAST
#include <kos.h>

#define open fs_open
#define close fs_close
#define read fs_read
#else
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#endif
#if defined (LINUX) || defined (MSDOS)
#include <dirent.h>
#endif

unsigned char file_read_byte(int fd);

void get_string_from_file(int in, char *text, int len) {
	int x;
	for(x=0;x<len;x++) {
		text[x]=file_read_byte(in);
	}
	text[len]='\0';
}

int c99_open(char *filename, int mode) {
	return open(filename,mode);
}

void c99_close(int fd) {
	close(fd);
}

unsigned char file_read_byte(int fd) {
	unsigned char buf[5];
	read(fd,buf,1);
	return buf[0];
}

void file_init() {}

void file_shutdown() {}

#ifndef WIN32
char *select_file(char *path, char *ext, int save) {
	char buf[1000];
	char tmp[50];
	static char ret[50];
	char input[1][2][50];
#ifdef DREAMCAST
  #define d_name name
	#define readdir fs_readdir

	dirent_t *de;
	uint32 d;
#else

	struct dirent *de;
	struct DIR *d;
#endif

	if(save==1) {
		strcpy(buf,"%filename;:Filename:\r!go;Go!\r\r");
	} else {
		buf[0]='\0';
	}
	strcat(buf,"$Directory listing for\r$");
	strcat(buf,path);
	strcat(buf,"\r\r");
	strcat(buf,"!..;..\r");
#ifdef DREAMCAST

	d = fs_open(path,O_RDONLY|O_DIR);
#else

	d = opendir(path);
#endif

	while ( (de=readdir(d)) ) {
		strcpy(tmp,strtolower(de->d_name));
		if(tmp[strlen(de->d_name)-3]==ext[0] &&
		        tmp[strlen(de->d_name)-2]==ext[1] &&
		        tmp[strlen(de->d_name)-1]==ext[2]) {
			strcat(buf,"!");
			strcat(buf,de->d_name);
			strcat(buf,";");
			strcat(buf,de->d_name);
			strcat(buf,"\r");
		}
	}
#ifdef DREAMCAST
	fs_close(d);
#else

	closedir(d);
#endif

	input[1][1][0]='\0';
	text_window("Select a File",buf,tmp,input);
	printf("%i\n",input[1][1][0]);
	if(input[1][1][0]!='\0')
		strcpy(ret,input[1][1]);
	else
		strcpy(ret,tmp);
	return &ret;
}
#endif
