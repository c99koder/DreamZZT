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
