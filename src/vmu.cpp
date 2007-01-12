#include <kos.h>
#include <zlib/zlib.h>

void dbgio_callback(const char *msg);

void vmuify(char *filename) {
  int fd;
  gzFile gfd;
  int len;
  uint8 *buf;
  uint8 *icon;
  uint8 *pkg_out;
  vmu_pkg_t pkg;
  int pkg_size;
  color(15,1);

  fs_chdir("/ram");
  c99_printf("VMUifying %s\n",filename);
  video_refresh();
  video_refresh();
  fd=fs_open(filename,O_RDONLY);
  len=fs_total(fd);
  buf=malloc(len);
  fs_read(fd,buf,len);
  fs_close(fd);
  c99_printf("Loaded %i bytes\n",len);
  video_refresh();
  video_refresh();

  gfd=gzopen("tmp.gz","wb");
  gzwrite(gfd,buf,len);
  gzclose(gfd);
  free(buf);
  c99_printf("wrote /ram/tmp.gz\n");
  video_refresh();
  video_refresh();

  fd=fs_open("/rd/icon.hex",O_RDONLY);
  c99_printf("got icon handle: %i\n",fd);
  icon=malloc(512);
  len=fs_read(fd,&pkg.icon_pal[0],32);
  len+=fs_read(fd,icon,512);
  pkg.icon_data=icon;
  c99_printf("Icon loaded (%i bytes)\n",len);
  fs_close(fd);
  video_refresh();
  video_refresh();

  fd=fs_open("tmp.gz",O_RDONLY);
  buf=malloc(fs_total(fd));
  fs_read(fd,buf,fs_total(fd));
  c99_printf("Data loaded (%i)\n",fs_total(fd));
  video_refresh();
  video_refresh();
 
  strcpy(pkg.desc_short, filename);
  strcpy(pkg.desc_long, "DreamZZT Saved Game");
  strcpy(pkg.app_id, "DZZT");
  pkg.icon_cnt = 1;
  pkg.icon_anim_speed = 1;
  pkg.eyecatch_type = VMUPKG_EC_NONE;
  pkg.data_len = fs_total(fd);
  pkg.data=buf;
  vmu_pkg_build(&pkg, &pkg_out, &pkg_size);
  fs_close(fd);

  c99_printf("Writing to /vmu/a1/%s\n",filename);
  video_refresh();
  video_refresh();
  fs_chdir("/vmu/a1");
  fd=fs_open(filename,O_WRONLY);
  fs_write(fd,pkg_out,pkg_size);
  fs_close(fd);
  free(buf);
  free(icon);
  c99_printf("vmuify complete\n");
  video_refresh();
  video_refresh();
  fs_chdir("/ram");
  thd_sleep(5000);
}
