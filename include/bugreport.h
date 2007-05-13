/* bugreport.h - A C++ interface for the Trac XML-RPC plugin
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