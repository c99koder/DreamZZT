/*
 *  bugreport.h
 *  DreamZZT
 *
 *  Created by Sam Steele on 5/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#define TRAC_XMLRPC_HOME "http://dev.c99.org/DreamZZT/xmlrpc"

class TracBug {
public:
	TracBug();
	TracBug(int num) { get(num); }
	
	void setProperty(std::string name, std::string value, std::string type);
	std::string getProperty(std::string name);
	void delProperty(std::string name);
	int getNum() { return m_num; }
	
	bool create();
	bool get(int num);
	void attach(std::string filename, std::string description);
private:
	struct propEntry { 
		std::string name;
		std::string value;
		std::string type;
	};
	
	std::list<propEntry> m_props;
	int m_num;
};

std::list<TracBug> search_tickets(std::string query);

bool submit_bug(std::string email, std::string title, std::string desc);