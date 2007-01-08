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
#include <Tiki/drawables/console.h>

class TUIWidget {
public:
	TUIWidget() {
		m_ec = new Hid::EventCollector(false);
		m_focus = false;
	}
	
	virtual void draw(ConsoleText *ct) {};
	void focus(bool f);	
	void update();	
	bool getFocus() { return m_focus; }
	virtual void processHidEvent(const Hid::Event &evt) {};
	virtual const std::string getHelpText() { return "\0"; }
	virtual const std::string getReturnValue() { return "\0"; }
	virtual const bool getCloseOnEnter() { return true; }
	
private:
	bool m_focus;
	RefPtr<Hid::EventCollector> m_ec;
};

class TUILabel : public TUIWidget {
public:
	TUILabel(std::string text, bool bold=false) {
		m_text=text;
		m_bold=bold;
	}
	
	void draw(ConsoleText *ct) {
		ct->color((m_bold?WHITE:YELLOW)|HIGH_INTENSITY,BLUE);
		*ct << m_text;
	}
private:
	std::string m_text;
	bool m_bold;
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
	int m_blinkTimer;
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
		m_checked=checked;
	}
	
	void draw(ConsoleText *ct) {
		ct->color(WHITE|HIGH_INTENSITY,BLUE);
		*ct << "  [" << ((*m_checked)?"\xfb":" ") << "] " << m_text;
	}
	
	const std::string getHelpText() { return "Press SPACE to toggle this"; }
	
	void processHidEvent(const Hid::Event &evt) {
		if(evt.type == Event::EvtKeypress && evt.key == ' ') (*m_checked) = !(*m_checked);
	}
private:
	std::string m_text;
	bool *m_checked;
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
	}
	
	void draw(ConsoleText *ct);
};

class TUIHyperLink : public TUIWidget {
public:
	TUIHyperLink(std::string label, std::string text) {
		m_text=text;
		m_label=label;
	}
	
	void draw(ConsoleText *ct) {
		ct->color(MAGENTA|HIGH_INTENSITY,BLUE);
		*ct << " \x10   ";
		ct->color(WHITE|HIGH_INTENSITY,BLUE);
		*ct << m_text;
	}
	
	const std::string getHelpText() { return "Press ENTER to select this"; }
	const std::string getReturnValue() { return m_label; }
	
private:
	std::string m_text;
	std::string m_label;
};

class TUIWindow {
public:
	TUIWindow(std::string title,int x=6, int y=3, int w=45, int h=17) {
		m_x=x;
		m_y=y;
		m_w=w;
		m_h=h;
		m_title=title;
		m_label="\0";
		m_offset=0;
		m_dirty=0;
	}

	void draw_shadow(ConsoleText *console, int x, int y);
	void draw_box(ConsoleText *console, int x, int y, int w, int h, int fg, int bg, bool shadow=true);
	void buildFromString(std::string s);
	void doMenu(ConsoleText *ct);
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