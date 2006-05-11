#ifndef WIN32
#define SOCKET int
#else
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
