/* word.cpp - Turn a string into a vector of words
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

#include <vector>
#include <string>
#include "word.h"

std::vector<std::string> wordify(std::string s, char seperator) {
	std::vector<std::string> list;
	size_t pos=0,oldpos=0;
	
	while((pos = s.find(seperator, oldpos)) != std::string::npos) {
		if(pos != oldpos) list.push_back(s.substr(oldpos,(pos-oldpos)));
		oldpos = pos+1;
	}
	
	list.push_back(s.substr(oldpos)); //Stuff in the last word
	return list;
}
