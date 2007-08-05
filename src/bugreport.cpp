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

using namespace Tiki;

#include <string>
#include <list>
#include <sstream>
#include "http.h"
#include "bugreport.h"

/* ---- Base64 Encoding --- */
static char table64[]=
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  
/*
 * base64_encode()
 *
 * Returns the length of the newly created base64 string. The third argument
 * is a pointer to an allocated area holding the base64 data. If something
 * went wrong, -1 is returned.
 *
 * Copyright (C) 2001, Andrew Francis and Daniel Stenberg
 */
int base64_encode(const void *inp, int insize, char **outptr)
{
  unsigned char ibuf[3];
  unsigned char obuf[4];
  int i;
  int inputparts;
  char *output;
  char *base64data;

  char *indata = (char *)inp;

  if(0 == insize)
    insize = (int)strlen(indata);

  base64data = output = (char*)malloc(insize*4/3+4);
  if(NULL == output)
    return -1;

  while(insize > 0) {
    for (i = inputparts = 0; i < 3; i++) { 
      if(insize > 0) {
        inputparts++;
        ibuf[i] = *indata;
        indata++;
        insize--;
      } else
        ibuf[i] = 0;
    }
                       
    obuf [0] = (ibuf [0] & 0xFC) >> 2;
    obuf [1] = ((ibuf [0] & 0x03) << 4) | ((ibuf [1] & 0xF0) >> 4);
    obuf [2] = ((ibuf [1] & 0x0F) << 2) | ((ibuf [2] & 0xC0) >> 6);
    obuf [3] = ibuf [2] & 0x3F;

    switch(inputparts) {
    case 1: /* only one byte read */
      sprintf(output, "%c%c==", 
              table64[obuf[0]],
              table64[obuf[1]]);
      break;
    case 2: /* two bytes read */
      sprintf(output, "%c%c%c=", 
              table64[obuf[0]],
              table64[obuf[1]],
              table64[obuf[2]]);
      break;
    default:
      sprintf(output, "%c%c%c%c", 
              table64[obuf[0]],
              table64[obuf[1]],
              table64[obuf[2]],
              table64[obuf[3]] );
      break;
    }
    output += 4;
  }
  *output=0;
  *outptr = base64data; /* make it return the actual data memory */

  return (int)strlen(base64data); /* return the length of the new data */
}
/* ---- End of Base64 Encoding ---- */

/*
 * base64_decode()
 *
 * Given a base64 string at src, decode it and return an allocated memory in
 * the *outptr. Returns the length of the decoded data.
 *
 * Copyright (C) 1998 - 2007, Daniel Stenberg, <daniel@haxx.se>, et al.
 */
static void decodeQuantum(unsigned char *dest, const char *src)
{
  unsigned int x = 0;
  int i;
  char *found;

  for(i = 0; i < 4; i++) {
    if((found = strchr(table64, src[i])))
      x = (x << 6) + (unsigned int)(found - table64);
    else if(src[i] == '=')
      x = (x << 6);
  }

  dest[2] = (unsigned char)(x & 255);
  x >>= 8;
  dest[1] = (unsigned char)(x & 255);
  x >>= 8;
  dest[0] = (unsigned char)(x & 255);
}
 
int base64_decode(const char *src, unsigned char **outptr)
{
  int length = 0;
  int equalsTerm = 0;
  int i;
  int numQuantums;
  unsigned char lastQuantum[3];
  size_t rawlen=0;
  unsigned char *newstr;

  *outptr = NULL;

  while((src[length] != '=') && src[length])
    length++;
  /* A maximum of two = padding characters is allowed */
  if(src[length] == '=') {
    equalsTerm++;
    if(src[length+equalsTerm] == '=')
      equalsTerm++;
  }
  numQuantums = (length + equalsTerm) / 4;

  /* Don't allocate a buffer if the decoded length is 0 */
  if (numQuantums <= 0)
    return 0;

  rawlen = (numQuantums * 3) - equalsTerm;

  /* The buffer must be large enough to make room for the last quantum
  (which may be partially thrown out) and the zero terminator. */
  newstr = (unsigned char *)malloc(rawlen+4);
  if(!newstr)
    return 0;

  *outptr = newstr;

  /* Decode all but the last quantum (which may not decode to a
  multiple of 3 bytes) */
  for(i = 0; i < numQuantums - 1; i++) {
    decodeQuantum((unsigned char *)newstr, src);
    newstr += 3; src += 4;
  }

  /* This final decode may actually read slightly past the end of the buffer
  if the input string is missing pad bytes.  This will almost always be
  harmless. */
  decodeQuantum(lastQuantum, src);
  for(i = 0; i < 3 - equalsTerm; i++)
    newstr[i] = lastQuantum[i];

  newstr[i] = 0; /* zero terminate */
  return rawlen;
}

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
		if((*pi).name == name) return (*pi).value;
	}
	
	return "";
}

void TracBug::delProperty(std::string name) {
	for(std::list<propEntry>::iterator pi = m_props.begin(); pi != m_props.end(); pi++) {
		if((*pi).name == name) pi = m_props.erase(pi);
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
	
	std::string result = http_post_data(bug, "text/xml", TRAC_XMLRPC_HOME);
	
	std::string::size_type pos = result.find( "<param>\n<value><int>", 0);
	if(pos == std::string::npos) return false;
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
	
	std::string result = http_post_data(q.str(), "text/xml", TRAC_XMLRPC_HOME);
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
	File f(filename, "rb");
	char *buf = (char *)malloc(f.total());
	char *fileData;
	f.read(buf,f.total());
	base64_encode(buf,f.total(),&fileData);
	f.close();

	std::stringstream attach;
	attach << "<?xml version=\"1.0\"?><methodCall><methodName>ticket.putAttachment</methodName><params>";
	attach << "<param><value><int>" << m_num << "</int></value></param>";
	attach << "<param><value><string>" << filename << "</string></value></param>";
	attach << "<param><value><string>" << description << "</string></value></param>";
	attach << "<param><value><base64>" << fileData << "</base64></value></param>";
	attach << "</params></methodCall>";
	http_post_data(attach.str(), "text/xml", TRAC_XMLRPC_HOME);
	free(fileData);
}

std::list<TracBug> search_tickets(std::string query) {
	std::list<TracBug> bugs;
	TracBug bug;
	
	std::string q = "<?xml version=\"1.0\"?><methodCall><methodName>ticket.query</methodName><params>";
	q += "<param><value><string>" + query + "</string></value></param>";
	q += "</params></methodCall>";
	
	std::string result = http_post_data(q, "text/xml", TRAC_XMLRPC_HOME);
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
