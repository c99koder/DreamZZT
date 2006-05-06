/*
 Tiki
 
 ConsoleText.h
 
 Copyright (C)2001 - 2006 Sam Steele
 */

#ifndef __TIKI_DRW_CONSOLETEXT_H
#define __TIKI_DRW_CONSOLETEXT_H

#include "Tiki/gl.h"
#include "Tiki/drawable.h"
#include "Tiki/texture.h"
#include "Tiki/color.h"

#include <vector>

namespace Tiki {
	namespace GL {

#define endl '\n'
		
		enum ConsoleColor {
			BLACK = 0,
			BLUE,
			GREEN,
			CYAN,
			RED,
			MAGENTA,
			YELLOW,
			GREY=7,
			GRAY=7,
			HIGH_INTENSITY
		};
		
		/** ConsoleText -- ConsoleText displays an array of fixed width characters.  It takes a texture to use as the font.	*/
		class ConsoleText : public Drawable {
public:
			ConsoleText(int cols, int rows, Texture * texture);
			virtual ~ConsoleText();
			
			void setTexture(Texture * txr);
			void setSize(float w, float h);
			
			void clear();
			void locate(int x, int y);
			void color(int fg, int bg);
			void printf(const char *fmt, ...);
			
			char getChar(int x, int y) { 
				assert(x<m_cols && y<m_rows);
				return m_charData[(y*m_cols) + x];
			}
			
			char getColor(int x, int y) {
				assert(x<m_cols && y<m_rows);
				return m_colorData[(y*m_cols) + x];
			}				
			
			ConsoleText& operator <<(const char *input) {
				printf("%s",input);
				
				return *this;
			}

			ConsoleText& operator <<(int input) {
				printf("%i",input);
				
				return *this;
			}			

			ConsoleText& operator <<(char input) {
				printf("%c",input);
				
				return *this;
			}
			
			ConsoleText& operator <<(float input) {
				printf("%f",input);
				
				return *this;
			}			
			virtual void draw(ObjType t);
			void ConsoleText::renderCharacter(float x, float y, float w, float h, unsigned char c, int color);
			void ConsoleText::renderBackground(float x, float y, float w, float h, int color);
			
protected:
			RefPtr<Texture>		m_texture;
			int m_rows, m_cols;
			std::vector<char> m_charData;
			std::vector<char> m_colorData;

			Color m_palette[8];
			
private:			
			int m_cursor_x, m_cursor_y;
			int m_fg, m_bg;
			float	m_w, m_h;
		};

		/** ConsoleStream -- ConsoleStream handles all the mysterious frame things so you don't have to. */
		class ConsoleStream {
public:
			ConsoleStream(ConsoleText *ct) { m_ct = ct; }
			~ConsoleStream() {}
			
			ConsoleStream operator <<(const char *input) {
				m_ct->printf("%s",input);
				refresh();
				return *this;
			}			

			ConsoleStream operator <<(const int input) {
				m_ct->printf("%i",input);
				refresh();
				return *this;  
			}						
			
private:
			void refresh() {
				Frame::begin();
				m_ct->draw(Drawable::Opaque);
				Frame::transEnable();
				m_ct->draw(Drawable::Trans);
				Frame::finish();
			}
			
			ConsoleText *m_ct;
		};
		
		
	};
};

#endif	// __TIKI_DRW_CONSOLETEXT_H

