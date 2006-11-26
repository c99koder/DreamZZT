//
//  os.mm
//  DreamZZT
//
//  Created by Sam Steele on 11/25/06.
//  Copyright 2006 __MyCompanyName__. All rights reserved.
//

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
	
	return output;
}