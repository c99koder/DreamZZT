/* controller.m - Cocoa window controller
 * Copyright (c) 2000-2006 Sam Steele
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

#import "Controller.h"
#import <Tiki/TikiMain.h>
#import <assert.h>

void tiki_main();
extern char *default_file;

@implementation Controller
- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename
{
	openFileName = filename;
}

- (void) applicationDidFinishLaunching: (NSNotification *) note
{
	NSString * resPath = [[NSBundle mainBundle] resourcePath];
	chdir([resPath cString]);
	TikiMain * otm = [[TikiMain alloc] retain];
	tm = otm;
	[tm doMainWithWindow: mainWindow andView: mainView andMainFunc: tiki_main andOpenFile: openFileName];
	tm = nil;
	[otm release];
}

- (NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication *)sender
{
	assert( tm );
	[tm quitSoon];
	return NSTerminateNow;
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *)theApplication
{
        return YES;
}

@end
