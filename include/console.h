/*
 Tiki
 
 console.h
 
 Copyright (C)2001 - 2006 Sam Steele
 */

#ifndef __TIKI_DRW_CONSOLETEXT_H
#define __TIKI_DRW_CONSOLETEXT_H

#include "Tiki/gl.h"
#include "Tiki/drawable.h"
#include "Tiki/texture.h"
#include "Tiki/color.h"

#if defined(DZZT_LITE) && TIKI_PLAT != TIKI_NDS
#if TIKI_PLAT == TIKI_OSX
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#endif

#include <vector>

namespace Tiki {
	namespace GL {

#define endl '\n'
		
		enum ConsoleColor {
			BLACK = 0x00,
			BLUE = 0x01,
			GREEN = 0x02,
			CYAN = 0x03,
			RED = 0x04,
			MAGENTA = 0x05,
			YELLOW = 0x06,
			GREY = 0x07,
			GRAY = 0x07,
			WHITE = 0x07,
			HIGH_INTENSITY = 0x1000,
			UNDERLINE = 0x2000,
			UNDERSCORE = 0x2000,
			BLINK = 0x4000,
			REVERSE = 0x8000,
			INVISIBLE = 0xA000
		};
		
		/** ConsoleText -- ConsoleText displays an array of fixed width characters. */
		class ConsoleText : public Drawable {
public:
#ifdef DZZT_LITE
#if TIKI_PLAT == TIKI_NDS
			ConsoleText(int cols, int rows, bool sub);
#else
			ConsoleText(int cols, int rows, SDL_Surface * font);
#endif
#else
			ConsoleText(int cols, int rows, Texture * texture);
#endif
			virtual ~ConsoleText();
			
			void setSize(float w, float h);
			
			void clear();
			void locate(int x, int y);
			void color(int fg, int bg);
			void printf(const char *fmt, ...);
			void scroll(int rows, int top, int left, int bottom, int right);
			void scroll(int rows);
			
			void setAutoScroll(bool s) {
				m_autoScroll = s;
			}
			
			void setAutoWrap(bool w) {
				m_autoWrap = w;
			}
			
			void setAutoRefresh(bool r) {
				m_autoRefresh = r;
			}
			
			void setANSI(bool a) {
				m_ansi = a;
			}
			
			char getChar(int x, int y) { 
				assert(x<m_cols && y<m_rows);
				return m_charData[(y*m_cols) + x];
			}
			
			short getColor(int x, int y) {
				assert(x<m_cols && y<m_rows);
				return m_colorData[(y*m_cols) + x];
			}				
			
			void putChar(int x, int y, unsigned char c) {
				assert(x<m_cols && y<m_rows);
				m_charData[(y*m_cols) + x] = c;
			}
			
			void putColor(int x, int y, int attr) {
				assert(x<m_cols && y<m_rows);
				m_colorData[(y*m_cols) + x] = attr;
			}
			
			ConsoleText& operator <<(std::string input) {
				printf("%s",input.c_str());
				
				return *this;
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
#ifdef DZZT_LITE
#if TIKI_PLAT == TIKI_NDS
			virtual void draw();
#else
			virtual void draw(SDL_Surface *screen);
#endif
#else
			virtual void draw(ObjType t);
			void renderCharacter(float x, float y, float w, float h, unsigned char c, int color);
			void renderBackground(float x, float y, float w, float h, int color);
#endif
			Tiki::Math::Vector getSize() const;
			Color getConsoleColor(const int colorNumber) const;
protected:
#ifdef DZZT_LITE
#if TIKI_PLAT == TIKI_NDS
			bool m_sub;
#else
			SDL_Surface *m_font;
#endif
#else
			RefPtr<Texture>		m_texture;
#endif
			int m_rows, m_cols;
			std::vector<unsigned char> m_charData;
			std::vector<unsigned short> m_colorData;

			Color m_palette[8];
			
private:		
			void processAnsiString();
			bool m_autoScroll, m_autoWrap, m_autoRefresh;
			bool m_ansi;
			int m_cursor_x, m_cursor_y;
			int m_save_x, m_save_y;
			int m_attr;
			float	m_w, m_h;
			unsigned char ansistr[51];
			unsigned char ansiptr;
		};
	};
};

#endif	// __TIKI_DRW_CONSOLETEXT_H

