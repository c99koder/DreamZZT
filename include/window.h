/* window.h - ZZT-formatted text popup window
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

#ifndef _WINDOW_H
#define _WINDOW_H

#include <Tiki/eventcollector.h>
#include "console.h"

class TUIWidget {
public:
	TUIWidget() {
		m_ec = new Hid::EventCollector(false);
		m_focus = false;
		m_bg = BLUE;
	}
	
	virtual ~TUIWidget() {
	}
	
	virtual void draw(ConsoleText *ct) {};
	void focus(bool f);	
	void update();	
	bool getFocus() { return m_focus; }
	void setBg(unsigned char bg) { m_bg = bg; }
	virtual void processHidEvent(const Hid::Event &evt) {};
	virtual const std::string getHelpText() { return "\0"; }
	virtual const std::string getReturnValue() { return "\0"; }
	virtual const bool getCloseOnEnter() { return true; }
	
private:
	bool m_focus;
	RefPtr<Hid::EventCollector> m_ec;
protected:
	unsigned char m_bg;
};

class TUILabel : public TUIWidget {
public:
	TUILabel(std::string text, bool bold=false, bool ANSI=false) {
		m_text=text;
		m_bold=bold;
		m_ansi=ANSI;
	}
	
	~TUILabel() {}
	
	void draw(ConsoleText *ct) {
		ct->color((m_bold?WHITE:YELLOW)|HIGH_INTENSITY,m_bg);
		if(m_ansi) ct->setANSI(true);
		*ct << m_text;
		if(m_ansi) ct->setANSI(false);
	}
private:
	std::string m_text;
	bool m_bold, m_ansi;
};

class TUITextInput : public TUIWidget {
public:
	TUITextInput(std::string label, std::string *text, bool centered=false) {
		m_label=label;
		m_text=text;
		m_blink=false;
		m_blinkTimer=1;
		m_center = centered;
	}
	
	void draw(ConsoleText *ct);	
	const std::string getHelpText() { return "Use keyboard to edit text"; }
	const bool getCloseOnEnter() { return false; }
	
	void processHidEvent(const Hid::Event &evt);
protected:
	std::string m_label;
	std::string *m_text;
	bool m_blink,m_center;
	uint64 m_blinkTimer;
};

class TUIPasswordInput : public TUITextInput {
public:
	TUIPasswordInput(std::string label, std::string *text) : TUITextInput(label,text) { }	
	void draw(ConsoleText *ct);	
};

class TUICheckBox : public TUIWidget {
public:
	TUICheckBox(std::string text, bool *checked) {
		m_text=text;
		m_checked_b=checked;
		m_checked_uc=NULL;
	}

	TUICheckBox(std::string text, unsigned char *checked) {
		m_text=text;
		m_checked_b=NULL;
		m_checked_uc=checked;
	}
	
	void draw(ConsoleText *ct) {
		ct->color(WHITE|HIGH_INTENSITY,m_bg);
		if(m_checked_b != NULL) *ct << "  [" << ((*m_checked_b)?"\xfb":" ") << "] " << m_text;
		if(m_checked_uc != NULL) *ct << "  [" << ((*m_checked_uc)?"\xfb":" ") << "] " << m_text;
	}
	
	const std::string getHelpText() { return "Press SPACE to toggle this"; }
	
	void processHidEvent(const Hid::Event &evt) {
		if(evt.type == Event::EvtKeypress && evt.key == ' ') {
			if(m_checked_b != NULL) (*m_checked_b) = !(*m_checked_b);
			if(m_checked_uc != NULL) (*m_checked_uc) = !(*m_checked_uc);
		}
	}
private:
	std::string m_text;
	bool *m_checked_b;
	unsigned char *m_checked_uc;
};

class TUIRadioGroup : public TUIWidget {
public:
	TUIRadioGroup(std::string text, int *selected);	
	TUIRadioGroup(std::string text, unsigned short *selected);	
	TUIRadioGroup(std::string text, float *selected);	
	TUIRadioGroup(std::string text, unsigned char *selected);	
	
	void add(std::string text) {
		m_options.push_back(text);
	}
	
	void draw(ConsoleText *ct);	
	const std::string getHelpText() { return "Press LEFT or RIGHT to change this"; }

	void processHidEvent(const Hid::Event &evt);
protected:
	std::string m_text;
	int *m_selectedi;
	unsigned short int *m_selecteds;
	float *m_selectedf;
	unsigned char *m_selectedc;
	int m_val;
	std::vector<std::string> m_options;
};

class TUIBoardList : public TUIRadioGroup {
public:
	TUIBoardList(std::string text, unsigned char *board);
};

class TUIDirection : public TUIRadioGroup {
public:
	TUIDirection(std::string text, Vector *direction);	
	void processHidEvent(const Hid::Event &evt);
private:
	unsigned short int m_select;
	Vector *m_direction;
};
	
class TUINumericInput : public TUIWidget {
public:
	TUINumericInput(std::string text, int *num, int min, int max, int step=1);
	TUINumericInput(std::string text, unsigned short int *num, int min, int max, int step=1);	
	TUINumericInput(std::string text, float *num, int min, int max, int step=1);	
	TUINumericInput(std::string text, unsigned char *num, int min, int max, int step=1);	
	void draw(ConsoleText *ct);	
	const std::string getHelpText() { return "Press LEFT or RIGHT to change this"; }
	void processHidEvent(const Hid::Event &evt);	
protected:
	int *m_numi;
	unsigned short int *m_nums;
	float *m_numf;
	unsigned char *m_numc;
	int m_val;
	std::string m_text;
	int m_min, m_max, m_step;
};

class TUISlider : public TUINumericInput {
public:
	TUISlider(std::string text, int *slide) : TUINumericInput(text, slide, 0, 8) {
		m_blinkTimer = 4;
		m_blink = false;
	}
	
	void draw(ConsoleText *ct);
private:
	bool m_blink;
	int m_blinkTimer;
};

class TUIHyperLink : public TUIWidget {
public:
	TUIHyperLink(std::string label, std::string text) {
		m_text=text;
		m_label=label;
	}
	
	void draw(ConsoleText *ct) {
		ct->color(MAGENTA|HIGH_INTENSITY,m_bg);
		*ct << " \x10 ";
#if TIKI_PLAT != TIKI_NDS
		*ct << "   ";
#endif
		ct->color(WHITE|HIGH_INTENSITY,m_bg);
		*ct << m_text;
	}
	
	const std::string getHelpText() { 
#if TIKI_PLAT == TIKI_DC || TIKI_PLAT == TIKI_NDS
		return "Press A to select this"; 
#else
		return "Press ENTER to select this"; 
#endif
	}
	const std::string getReturnValue() { return m_label; }
	
private:
	std::string m_text;
	std::string m_label;
};

class TUIWindow {
public:
#if TIKI_PLAT == TIKI_NDS
	TUIWindow(std::string title,int x=0, int y=0, int w=32, int h=22);
#else
#ifdef DZZT_LITE
	TUIWindow(std::string title,int x=6, int y=3, int w=45, int h=17);
#else
	TUIWindow(std::string title,int x=6, int y=6, int w=45, int h=17);
#endif
#endif
	
	~TUIWindow() {
		TUIWidget *t;
		while(!m_widgets.empty()) {
			t = m_widgets[m_widgets.size()-1];
			m_widgets.pop_back();
			delete t;
		}
	}

	void draw_shadow(ConsoleText *console, int x, int y);
	void draw_box(ConsoleText *console, int x, int y, int w, int h, int fg, int bg, bool shadow=true);
	void buildFromString(std::string s, bool ANSI=false);
	void doMenu();
	void addWidget(TUIWidget *w) {
		m_widgets.push_back(w);
	}
	void processHidEvent(const Hid::Event &evt);
	
	std::string getLabel() { return m_label; }
	
private:
	std::string m_title,m_label;
	int m_x, m_y, m_w, m_h, m_offset;
	std::vector<TUIWidget *> m_widgets;
	bool m_loop;
	bool m_dirty;
};

#endif