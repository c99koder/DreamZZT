/* vmu.h - Manage the Dreamcast Visual Memory Unit
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

extern "C" {
	void zzt_vmu_init();
	void update_lcds();
	int zinflate(const char *source, const char *dest);
	int zdeflate(const char *source, const char *dest, int level);
	void vmuify(const char *filename_in, const char *filename_out, const char *shortname, const char *longname);
	void unvmuify(const char *filename_in, const char *filename_out);
}