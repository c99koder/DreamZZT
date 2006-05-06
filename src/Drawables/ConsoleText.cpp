/*
 Tiki
 
 ConsoleText.cpp
 
 Copyright (C)2001 - 2006 Sam Steele
 */

//#include "pch.h"

#include "Tiki/Tiki.h"
#include "ConsoleText.h"
#include "Tiki/plxcompat.h"

using namespace Tiki::GL;
using namespace Tiki::GL::Plxcompat;

ConsoleText::ConsoleText(int cols, int rows, Texture * texture) {
	m_texture = texture;
	m_rows = rows;
	m_cols = cols;

	m_palette[0] = Color(0,0,0);
	m_palette[1] = Color(0,0,0.5);
	m_palette[2] = Color(0,0.5,0);
	m_palette[3] = Color(0,0.5,0.5);
	m_palette[4] = Color(0.5,0,0);
	m_palette[5] = Color(0.5,0,0.5);
	m_palette[6] = Color(0.5,0.5,0);
	m_palette[7] = Color(0.5,0.5,0.5);
		
	m_charData.resize(rows*cols, 0);
	m_colorData.resize(rows*cols, 0);
	
	m_texture->select();
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	m_cursor_x = 0;
	m_cursor_y = 0;
	
	m_fg = 7;
	m_bg = 0;
}

ConsoleText::~ConsoleText() {
}

void ConsoleText::setTexture(Texture * txr) {
	m_texture = txr;
}

void ConsoleText::setSize(float w, float h) {
	m_w = w;
	m_h = h;
}

void ConsoleText::clear() {
	int x;
	
	for(x=0; x<m_rows*m_cols; x++) {
		m_charData[x] = 0;
		m_colorData[x] = m_fg + (m_bg * 8);
	}
	
	m_cursor_x = 0;
	m_cursor_y = 0;
}

void ConsoleText::locate(int x, int y) {
	m_cursor_x = x;
	m_cursor_y = y;
}

void ConsoleText::color(int fg, int bg) {
	m_fg = fg;
	m_bg = bg;
}

void ConsoleText::printf(const char *fmt, ...) {
	char buf[1024];
	va_list args;
	
	va_start(args,fmt);
	vsprintf(buf,fmt,args);
	va_end(args);

	for(int i=0; i<strlen(buf); i++) {
		m_charData[(m_cursor_y * m_cols) + m_cursor_x] = buf[i];
		m_colorData[(m_cursor_y * m_cols) + m_cursor_x] = m_fg + (m_bg * 16);
		
		m_cursor_x++;
		
		if(buf[i] == '\n') {
			m_cursor_x = 0;
			m_cursor_y++;
		}	
	}
}

void ConsoleText::renderCharacter(float x, float y, float w, float h, unsigned char c, int color) {
	plx_vertex_t vert;
	float u = (c % 16) * 8;
	float v = (c / 16) * 8;

	const Vector & sv = getScale();
	w *= sv.x;
	h *= sv.y;
	
	const Vector & tv = getPosition() + Vector(x, y, -0.0001);

	m_texture->select();

	if(color>7) {
		vert.argb = Color(0.25, 0.25, 0.25);
		color -= 8;
	} else {
		vert.argb = Color(0,0,0);
	}
	vert.argb += m_palette[color];
	vert.oargb = 0;
	
	vert.flags = PLX_VERT;
	vert.x = tv.x-m_w/2;
	vert.y = tv.y+h-m_h/2;
	vert.z = tv.z;
	vert.u = u/m_texture->getW();
	vert.v = (v+8)/m_texture->getH();
	plx_prim(&vert, sizeof(vert));
	
	vert.y = tv.y-m_h/2;
	vert.u = u/m_texture->getH();
	vert.v = v/m_texture->getW();
	plx_prim(&vert, sizeof(vert));
	
	vert.x = tv.x+w-m_w/2;
	vert.y = tv.y+h-m_h/2;
	vert.u = (u+8)/m_texture->getW();
	vert.v = (v+8)/m_texture->getH();
	plx_prim(&vert, sizeof(vert));
	
	vert.flags = PLX_VERT_EOS;
	vert.y = tv.y-m_h/2;
	vert.u = (u+8)/m_texture->getW();
	vert.v = v/m_texture->getH();
	plx_prim(&vert, sizeof(vert));	
}

void ConsoleText::renderBackground(float x, float y, float w, float h, int color) {
	plx_vertex_t vert;
	const Vector & sv = getScale();
	w *= sv.x;
	h *= sv.y;
	
	const Vector & tv = getPosition() + Vector(x, y, 0);
	
	m_texture->deselect();
	
	vert.argb = m_palette[color];
	vert.oargb = 0;
	
	vert.flags = PLX_VERT;
	vert.x = tv.x-m_w/2;
	vert.y = tv.y+h-m_h/2;
	vert.z = tv.z;
	plx_prim(&vert, sizeof(vert));
	
	vert.y = tv.y-m_h/2;
	plx_prim(&vert, sizeof(vert));
	
	vert.x = tv.x+w-m_w/2;
	vert.y = tv.y+h-m_h/2;
	plx_prim(&vert, sizeof(vert));
	
	vert.flags = PLX_VERT_EOS;
	vert.y = tv.y-m_h/2;
	plx_prim(&vert, sizeof(vert));	
}

void ConsoleText::draw(ObjType list) {
	int x=0,y=0;
	float x_step=(m_w / m_cols);
	float y_step=(m_h / m_rows);
	
	glDisable(GL_DEPTH_TEST);
	
	for(y=0; y<m_rows; y++) {
		for(x=0; x<m_cols; x++) {
			if(list == Trans) { //Characters!
				renderCharacter(x*x_step, y*y_step, x_step, y_step, m_charData[y*(m_cols) + x], m_colorData[y*(m_cols) + x] % 16);
			} else { //Background blocks!
				renderBackground(x*x_step, y*y_step, x_step, y_step, m_colorData[y*(m_cols) + x] / 16);
			}
		}
	}
}