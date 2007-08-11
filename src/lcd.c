/* lcd.c - Manage the Dreamcast Visual Memory Unit's LCD
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

maple_device_t *lcds[8];
uint8   bitmap[48*32/8];
int lcdcount=0;

static void zzt_vmu_detach(maple_driver_t *drv, maple_device_t *dev) {}


static int zzt_vmu_attach(maple_driver_t *drv, maple_device_t *dev) {
	dev->status_valid = 1;
	lcds[lcdcount++]=dev;
	return 0;
}

void update_lcds() {
	int i;
	if(lcdcount==0)
		return;
	for(i=0;i<lcdcount;i++) {
		vmu_draw_lcd(lcds[i], bitmap);
	}
	lcdcount=0;
}

/* Device Driver Struct */
static maple_driver_t dcb_vmu_drv = {
                                        functions:
                                        MAPLE_FUNC_MEMCARD | MAPLE_FUNC_LCD | MAPLE_FUNC_CLOCK,
                                        name:           "DreamZZT VMU Driver"
                                        ,
                                        periodic:
                                        NULL,
                                        attach:
                                        zzt_vmu_attach,
                                        detach:
                                        zzt_vmu_detach
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
