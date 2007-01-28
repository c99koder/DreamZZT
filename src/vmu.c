/* vmu.cpp - Manage the Dreamcast Visual Memory Unit
 * Copyright (c) 2001- 2007 Sam Steele
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

#include <kos.h>
#include <zlib/zlib.h>

static char vmu_logo[] =
"\
                                                \
                                                \
                                                \
                                                \
                                                \
               ...................              \
               ...................              \
             ..                   ..            \
             ..                   ..            \
             ..                   ..            \
             ..                   ..            \
             ..    ....   ....    ..            \
             ..    ....   ....    ..            \
             ..    ....   ....    ..            \
             ..                   ..            \
             ..                   ..            \
             ..        ...        ..            \
             ..        ...        ..            \
             ..                   ..            \
             ..                   ..            \
             ..    ...........    ..            \
             ..    ...........    ..            \
             ..        ...        ..            \
             ..                   ..            \
             ..                   ..            \
             ..                   ..            \
               ...................              \
               ...................              \
                                                \
                                                \
                                                \
                                                ";

const unsigned char icon_data[544] = {
	0x08, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x10, 0x00, 0x11, 0x11, 
	0x11, 0x11, 0x00, 0x01, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x01, 0x11, 
	0x11, 0x10, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x01, 0x11, 
	0x11, 0x10, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x01, 0x11, 
	0x11, 0x10, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x10, 0x00, 0x11, 0x11, 
	0x11, 0x11, 0x00, 0x01, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x10, 0x00, 0x11, 0x11, 
	0x11, 0x11, 0x00, 0x01, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x10, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

};

maple_device_t *lcds[8];
uint8   bitmap[48*32/8];
int lcdcount=0;

static void zzt_vmu_detach(maple_driver_t *drv, maple_device_t *dev) {
}

                                                                                
static int zzt_vmu_attach(maple_driver_t *drv, maple_device_t *dev) {
	dev->status_valid = 1;
	lcds[lcdcount++]=dev; 
  return 0;
}

void update_lcds() {
	int i;
	if(lcdcount==0) return;
	for(i=0;i<lcdcount;i++) {
  	vmu_draw_lcd(lcds[i], bitmap);
	}
	lcdcount=0;
}
                                                                                
/* Device Driver Struct */
static maple_driver_t dcb_vmu_drv = {
        functions:      MAPLE_FUNC_MEMCARD | MAPLE_FUNC_LCD | MAPLE_FUNC_CLOCK,
        name:           "DreamZZT VMU Driver",
        periodic:       NULL,
        attach:         zzt_vmu_attach,
        detach:         zzt_vmu_detach
};

void zzt_vmu_init() {
  int x, y, xi, xb, i;
	maple_device_t *dev;
  memset(bitmap, 0, 48*32/8);
  if (vmu_logo) {
	  for (y=0; y<32; y++)
      for (x=0; x<48; x++) {
        xi = x / 8;
        xb = 0x80 >> (x % 8);
        if (vmu_logo[(31-y)*48+(47-x)] == '.')
          bitmap[y*(48/8)+xi] |= xb;
      }
  }
  i = 0;
  while ( (dev = maple_enum_type(i++, MAPLE_FUNC_LCD)) ) {
          vmu_draw_lcd(dev, bitmap);
  }

  vmu_shutdown();
  maple_driver_reg(&dcb_vmu_drv);
}

#define CHUNK 16384

/* Compress from file source to file dest until EOF on source.
   zdeflate() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
int zdeflate(const char *source, const char *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    char in[CHUNK];
    char out[CHUNK];
		int fd_in,fd_out;
		
		fd_in = fs_open(source,O_RDONLY);
		fd_out = fs_open(dest,O_WRONLY);

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fs_read(fd_in, in, CHUNK);
        flush = (fs_tell(fd_in) == fs_total(fd_in)) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            have = CHUNK - strm.avail_out;
            if (fs_write(fd_out, out, have) != have) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when last data in file processed */
    } while (flush != Z_FINISH);

		fs_close(fd_in);
		fs_close(fd_out);

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

/* Decompress from file source to file dest until stream ends or EOF.
   zinflate() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
int zinflate(const char *source, const char *dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    char in[CHUNK];
    char out[CHUNK];
		int fd_in, fd_out;
		
		fd_in = fs_open(source,O_RDONLY);
		fd_out = fs_open(dest,O_WRONLY);

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fs_read(fd_in, in, CHUNK);
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fs_write(fd_out, out, have) != have) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

		fs_close(fd_in);
		fs_close(fd_out);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void unvmuify(const char *filename_in, const char *filename_out) {
	int fd;
	uint8 *buf1;
	uint8 *buf2;
	unsigned long sourceLen,destLen;
	vmu_hdr_t pkg;
	fd=fs_open(filename_in,O_RDONLY);
	fs_read(fd,&pkg,sizeof(vmu_hdr_t));
	fs_seek(fd,128 + (pkg.icon_cnt * 512), SEEK_SET);
	fs_read(fd,&destLen,sizeof(unsigned long));
	sourceLen = pkg.data_len - sizeof(unsigned long);
	printf("sourceLen: %li\ndestLen: %li\n",sourceLen,destLen);
	buf1 = (uint8 *)malloc(sourceLen);
	buf2 = (uint8 *)malloc(destLen);
	fs_read(fd,buf1,sourceLen);
	fs_close(fd);
	if(uncompress(buf2, &destLen, buf1, sourceLen) != Z_OK) {
		printf("Decompression error!\n");
		free(buf1);
		free(buf2);
		return;
	}
	printf("Writing output file\n");
	fd=fs_open(filename_out,O_WRONLY);
	fs_write(fd,buf2,destLen);
	fs_close(fd);
	printf("Done\n");
}

void vmuify(const char *filename_in, const char *filename_out, const char *shortname, const char *longname) {
  int fd,pkg_size;
  uint8 *buf1;
	uint8 *buf2;
  uint8 *pkg_out;
  vmu_pkg_t pkg;
  unsigned long sourceLen,destLen;
  
  printf("Opening source file\n");
  fd=fs_open(filename_in,O_RDONLY);
  buf1=(uint8 *)malloc(fs_total(fd));
  sourceLen = fs_read(fd,buf1,fs_total(fd));
  printf("Read %i bytes\n",sourceLen);
  buf1[sourceLen]='\0';
  fs_close(fd);
  printf("Closing source file\n");

	destLen = 20+(int)((float)sourceLen * 1.2f) + sizeof(unsigned long);
	buf2=(uint8 *)malloc(destLen);
	memcpy(buf2,&sourceLen,sizeof(unsigned long));
	printf("Compressing data\n");
	if(compress(buf2 + sizeof(unsigned long), &destLen, buf1, sourceLen) != Z_OK) {
		printf("Compression error!\n");
		free(buf2);
		free(buf1);
		return;
	} else {
		printf("Compressed to %i bytes\n",destLen);
		free(buf1);
	}
  strcpy(pkg.desc_short, shortname);
  strcpy(pkg.desc_long, longname);
  strcpy(pkg.app_id, "DZZT");
  pkg.icon_cnt = 1;
  pkg.icon_anim_speed = 1;
  memcpy(&pkg.icon_pal[0],icon_data,32);
  pkg.icon_data=icon_data+32;
  pkg.eyecatch_type = VMUPKG_EC_NONE;
  pkg.data_len = destLen + sizeof(unsigned long);
  pkg.data=buf2;
  printf("Building package\n");
  vmu_pkg_build(&pkg, &pkg_out, &pkg_size);

  printf("Opening output file\n");
	fs_unlink(filename_out);
  fd=fs_open(filename_out,O_WRONLY);
	if(fd==-1) {
	  printf("Error opening output file.\n"); 
		return;
	}
  printf("Writing..\n");
  fs_write(fd,pkg_out,pkg_size);
  printf("Closing output file\n");
  fs_close(fd);
  free(buf2);
  printf("VMUification complete\n");
}
