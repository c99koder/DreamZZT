/* word.cpp - Turn a string into a vector of words
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

#include <vector>
#include <string>
#include "word.h"

std::vector<std::string> wordify(std::string s, char seperator) {
  unsigned int x=0;
	std::vector<std::string> list;
	std::string tmp;
	
  for(x=0;x<s.length();x++) {
    if(s[x]==seperator) {
			list.push_back(tmp);
			tmp = "";
      continue;
    } else {
			tmp += s[x];
    }
  }
	
	list.push_back(tmp); //Stuff in the last word
	
	return list;
}
