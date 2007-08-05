/* os.mm - OS hooks for Mac OS X
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

#include <Cocoa/Cocoa.h>
#include <string>
#include "os.h"

std::string os_select_file(std::string title, std::string filter) {
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	std::string output;
	
	[panel setTitle:[NSString stringWithCString:title.c_str()]];
	
	if([panel runModalForTypes:[NSArray arrayWithObjects:[NSString stringWithCString:filter.c_str()],NULL]]) {
		output = [[panel filename] cString]; 
	} else {
		output = "";
	}
	[panel release];
	return output;
}

std::string os_save_file(std::string title, std::string filename, std::string filter) {
	NSSavePanel *panel = [NSSavePanel savePanel];
	std::string output;
	
	[panel setTitle:[NSString stringWithCString:title.c_str()]];
	
	if([panel runModalForDirectory:nil file:[NSString stringWithCString:filename.c_str()]]) {
		output = [[panel filename] cString]; 
	} else {
		output = "";
	}
	[panel release];
	return output;
}