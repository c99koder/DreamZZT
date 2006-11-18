/* window.h - ZZT-formatted text popup window
 * Copyright (c) 2000-2006 Sam Steele
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

class TUIWidget {
public:
	TUIWidget() {
		m_ec = new Hid::EventCollector(false);
	}
	
	virtual void draw(ConsoleText *ct) {};
	void focus(bool f) {
		m_focus = f;
		if(f) {
			m_ec->start();
		} else {
			m_ec->stop();
		}
	}
	
	void update() {
		// Check for regular HID events.
		Hid::Event evt;
		while (m_ec->getEvent(evt)) {
			processHidEvent(evt);
		}
	}
	
	virtual void processHidEvent(const Hid::Event &evt) {};
	
	virtual const std::string getHelpText() { return "\0"; }
	virtual const std::string getReturnValue() { return "\0"; }
	
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

class TUICheckBox : public TUIWidget {
public:
	TUICheckBox(std::string text, bool checked=false) {
		m_text=text;
		m_checked=checked;
	}
	
	void draw(ConsoleText *ct) {
		ct->color(WHITE|HIGH_INTENSITY,BLUE);
		*ct << "  [" << (m_checked?"\xfb":" ") << "] " << m_text;
	}
	
	const std::string getHelpText() { return "Press SPACE to toggle this"; }
	
	void processHidEvent(const Hid::Event &evt) {
		if(evt.type == Event::EvtKeypress && evt.key == ' ') m_checked = !m_checked;
	}
private:
	std::string m_text;
	bool m_checked;
};

class TUIRadioGroup : public TUIWidget {
public:
	TUIRadioGroup(std::string text) {
		m_text=text;
		m_selected=0;
	}
	
	void add(std::string text) {
		m_options.push_back(text);
	}
	
	void draw(ConsoleText *ct) {
		std::list<std::string>::iterator options_iter;
		int i=0;
		
		ct->color(YELLOW|HIGH_INTENSITY,BLUE);
		*ct << m_text;
		
		ct->color(WHITE|HIGH_INTENSITY,BLUE);
		
		for(options_iter = m_options.begin(); options_iter != m_options.end(); options_iter++) {
			*ct << "  (" << ((m_selected==i++)?"\x7":" ") << ") " << (*options_iter);
		}
	}
	
	const std::string getHelpText() { return "Press LEFT or RIGHT to change this"; }

	void processHidEvent(const Hid::Event &evt) {
		if(evt.type == Event::EvtKeypress && evt.key == Event::KeyLeft) {
			m_selected--;
			if(m_selected < 0) m_selected=0;
		}
		if(evt.type == Event::EvtKeypress && evt.key == Event::KeyRight) {
			m_selected++;
			if(m_selected >= m_options.size()) m_selected=m_options.size()-1;
		}
	}

private:
	std::string m_text;
	int m_selected;
	std::list<std::string> m_options;
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
};