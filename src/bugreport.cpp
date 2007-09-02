/* bugreport.cpp - A C++ interface for the Trac XML-RPC plugin
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

#include <Tiki/tiki.h>
#include <Tiki/file.h>
#include <Tiki/net.h>
#include <Tiki/net/http/useragent.h>
#include <Tiki/net/http/request.h>

using namespace Tiki;
using namespace Tiki::Net;
using namespace Tiki::Net::Http;

#include <string>
#include <list>
#include <sstream>
#include "bugreport.h"

#ifdef NET

TracBug::TracBug() {
	m_num = -1;
}

void TracBug::setProperty(std::string name, std::string value, std::string type) {
	propEntry pe;
	pe.name = name;
	pe.value = value;
	pe.type = type;
	delProperty(name);
	m_props.push_back(pe);
}

std::string TracBug::getProperty(std::string name) {
	for(std::list<propEntry>::iterator pi = m_props.begin(); pi != m_props.end(); pi++) {
		if((*pi).name == name)
			return (*pi).value;
	}

	return "";
}

void TracBug::delProperty(std::string name) {
	for(std::list<propEntry>::iterator pi = m_props.begin(); pi != m_props.end(); pi++) {
		if((*pi).name == name)
			pi = m_props.erase(pi);
	}
}

bool TracBug::create() {
	std::string bug = "<?xml version='1.0'?><methodCall><methodName>ticket.create</methodName><params>";
	bug += "<param><value><string>" + getProperty("summary") + "</string></value></param>";
	bug += "<param><value><string>" + getProperty("description") + "</string></value></param>";
	bug += "<param><value><struct>";
	for(std::list<propEntry>::iterator pi = m_props.begin(); pi != m_props.end(); pi++) {
		if((*pi).name != "summary" && (*pi).name != "description") {
			bug += "<member><name>" + (*pi).name + "</name><value><" + (*pi).type + ">" + (*pi).value + "</" + (*pi).type + "></value></member>";
		}
	}
	bug += "</struct></value></param><param><value><boolean>1</boolean></value></param></params></methodCall>";

	HttpUserAgent ua;
	Request r;
	Response *response;
	
	r.addContentPart(new Buffer(bug.length(), (uint8 *)bug.c_str(), std::string("text/xml")), "TracXML");
	r.setUrl(TRAC_XMLRPC_HOME);
	response = ua.post(&r);
	
	std::string result = std::string((char *)(response->getContentPart(DEFAULT_CONTENT_PART)->getData()));
	delete response;
	
	std::string::size_type pos = result.find( "<param>\n<value><int>", 0);
	if(pos == std::string::npos)
		return false;
	std::string ticket = result.substr(pos + 20, result.find("</int>", pos) - pos - 20);
	m_num = atoi(ticket.c_str());
	printf("Ticket number: %i\n",m_num);
	return true;
}

bool TracBug::get(int num) {
	std::stringstream q;
	q << "<?xml version=\"1.0\"?><methodCall><methodName>ticket.get</methodName><params>";
	q << "<param><value><int>" << num << "</int></value></param>";
	q << "</params></methodCall>";

	m_props.clear();

	HttpUserAgent ua;
	Request r;
	Response *response;
	
	r.addContentPart(new Buffer(q.str().length(), (uint8 *)q.str().c_str(), std::string("text/xml")), "TracXML");
	r.setUrl(TRAC_XMLRPC_HOME);
	response = ua.post(&r);
	
	std::string result = std::string((char *)(response->getContentPart(DEFAULT_CONTENT_PART)->getData()));
	delete response;

	std::string::size_type pos = result.find("<value><struct>", 0), pos2 = 0;

	while(pos < result.length() && ((pos = result.find("<member>", pos)) != std::string::npos)) {
		pos = result.find("<name>", pos) + 6;
		std::string name = result.substr(pos, (pos2 = result.find("</name>", pos)) - pos);
		pos = pos2;
		pos = result.find("<value>", pos) + 8;
		std::string type = result.substr(pos, (pos2 = result.find(">", pos)) - pos);
		pos = pos2;
		pos++;
		std::string value = result.substr(pos, (pos2 = result.find("</", pos)) - pos);
		pos = pos2;
		setProperty(name, value, type);
	}

	m_num = num;
	return true;
}

void TracBug::attach(std::string filename, std::string description) {
	Buffer file(filename, "application/x-zzt-save");
	Base64 b64;
	Buffer *encodedFile = b64.encode(&file);
	
	std::stringstream attach;
	attach << "<?xml version=\"1.0\"?><methodCall><methodName>ticket.putAttachment</methodName><params>";
	attach << "<param><value><int>" << m_num << "</int></value></param>";
	attach << "<param><value><string>" << filename << "</string></value></param>";
	attach << "<param><value><string>" << description << "</string></value></param>";
	attach << "<param><value><base64>" << (char *)encodedFile->getData() << "</base64></value></param>";
	attach << "</params></methodCall>";

	HttpUserAgent ua;
	Request r;
	Response *response;
	
	r.addContentPart(new Buffer(attach.str().length(), (uint8 *)attach.str().c_str(), std::string("text/xml")), "TracXML");
	r.setUrl(TRAC_XMLRPC_HOME);
	response = ua.post(&r);
	
	delete response;
	delete encodedFile;
}

std::list<TracBug> search_tickets(std::string query) {
	std::list<TracBug> bugs;
	TracBug bug;

	std::string q = "<?xml version=\"1.0\"?><methodCall><methodName>ticket.query</methodName><params>";
	q += "<param><value><string>" + query + "</string></value></param>";
	q += "</params></methodCall>";

	HttpUserAgent ua;
	Request r;
	Response *response;
	
	r.addContentPart(new Buffer(q.length(), (uint8 *)q.c_str(), std::string("text/xml")), "TracXML");
	r.setUrl(TRAC_XMLRPC_HOME);
	response = ua.post(&r);
	
	std::string result = std::string((char *)(response->getContentPart(DEFAULT_CONTENT_PART)->getData()));
	delete response;

	std::string::size_type pos = result.find("<value><array><data>", 0);

	while(pos < result.length() && ((pos = result.find("<value><int>", pos)) != std::string::npos)) {
		std::string ticket = result.substr(pos + 12, result.find("</int></value>", pos) - pos - 12);
		bug.get(atoi(ticket.c_str()));
		bugs.push_back(bug);
		pos += 14;
	}

	return bugs;
}
#endif
