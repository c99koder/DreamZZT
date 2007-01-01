/* window.cpp - ZZT-formatted text popup window
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

#include <Tiki/tiki.h>
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/eventcollector.h>
#include <Tiki/tikitime.h>
#include <Tiki/thread.h>
#include <string.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Thread;

#include <Tiki/drawables/console.h>

#include "window.h"
#include "object.h"
#include "board.h"

void render();
extern EventCollector *playerEventCollector;
extern Player *player;
extern int switchbrd;
extern struct board_info_node *board_list;

void TUIWidget::focus(bool f) {
	m_focus = f;
	if(f) {
		m_ec->start();
	} else {
		m_ec->stop();
	}
}

void TUIWidget::update() {
	// Check for regular HID events.
	Hid::Event evt;
	while (m_ec->getEvent(evt)) {
		processHidEvent(evt);
	}
}

TUIRadioGroup::TUIRadioGroup(std::string text, int *selected) {
	m_text=text;
	m_selectedi=selected;
	m_selectedf=NULL;
	m_selecteds=NULL;
	m_selectedc=NULL;
}

TUIRadioGroup::TUIRadioGroup(std::string text, unsigned short *selected) {
	m_text=text;
	m_selectedi=NULL;
	m_selectedf=NULL;
	m_selecteds=selected;
	m_selectedc=NULL;
}

TUIRadioGroup::TUIRadioGroup(std::string text, float *selected) {
	m_text=text;
	m_selectedi=NULL;
	m_selectedf=selected;
	m_selecteds=NULL;
	m_selectedc=NULL;
}

TUIRadioGroup::TUIRadioGroup(std::string text, unsigned char *selected) {
	m_text=text;
	m_selectedi=NULL;
	m_selectedf=NULL;
	m_selecteds=NULL;
	m_selectedc=selected;
}

void TUIRadioGroup::draw(ConsoleText *ct) {
	std::list<std::string>::iterator options_iter;
	int i=0;
	if(m_selectedi!=NULL) m_val = (*m_selectedi);
	if(m_selecteds!=NULL) m_val = (*m_selecteds);
	if(m_selectedf!=NULL) m_val = (*m_selectedf);
	if(m_selectedc!=NULL) m_val = (*m_selectedc);
	
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << m_text;
	
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	*ct << m_options[m_val];
}

void TUIRadioGroup::processHidEvent(const Hid::Event &evt) {
	if(m_selectedi!=NULL) m_val = (*m_selectedi);
	if(m_selecteds!=NULL) m_val = (*m_selecteds);
	if(m_selectedf!=NULL) m_val = (*m_selectedf);
	if(m_selectedc!=NULL) m_val = (*m_selectedc);
	
	if(evt.type == Event::EvtKeypress && evt.key == Event::KeyLeft) {
		if(m_val == 0) m_val=m_options.size();
		m_val--;
	}
	if(evt.type == Event::EvtKeypress && evt.key == Event::KeyRight) {
		m_val++;
		if(m_val >= m_options.size()) m_val=0;
	}
	
	if(m_selectedi!=NULL) (*m_selectedi) = m_val;
	if(m_selecteds!=NULL) (*m_selecteds) = m_val;
	if(m_selectedf!=NULL) (*m_selectedf) = m_val;
	if(m_selectedc!=NULL) (*m_selectedc) = m_val;		
}

TUIBoardList::TUIBoardList(std::string text, unsigned char *board) : TUIRadioGroup(text, board) {
	struct board_info_node *current=board_list;

	add("(None)");
	if(current!=NULL) current=current->next;
	
	while(current!=NULL) {
		add(current->title);
		current=current->next;
	}
}

TUIDirection::TUIDirection(std::string text, Vector *direction) : TUIRadioGroup(text, &m_select) {
	m_direction = direction;
	add("North");
	add("South");
	add("East");
	add("West");
	add("Idle");
	if(direction->x == 0 && direction->y == -1) m_select = 0;
	else if(direction->x == 0 && direction->y == 1) m_select = 1;
	else if(direction->x == 1 && direction->y == 0) m_select = 2;
	else if(direction->x == -1 && direction->y == 0) m_select = 3;
	else if(direction->x == 0 && direction->y == 0) m_select = 4;
	else m_select=4;
}

void TUIDirection::processHidEvent(const Hid::Event &evt) {
	TUIRadioGroup::processHidEvent(evt);
	
	switch(m_select) {
		case 0:
			m_direction->x = 0;
			m_direction->y = -1;
			break;
		case 1:
			m_direction->x = 0;
			m_direction->y = 1;
			break;
		case 2:
			m_direction->x = 1;
			m_direction->y = 0;
			break;
		case 3:
			m_direction->x = -1;
			m_direction->y = 0;
			break;
		case 4:
			m_direction->x = 0;
			m_direction->y = 0;
			break;
	}
}		

TUINumericInput::TUINumericInput(std::string text, int *num, int min, int max, int step) {
	m_text=text;
	m_min=min;
	m_max=max;
	m_step=step;
	m_numi=num;
	m_nums=NULL;
	m_numf=NULL;
	m_numc=NULL;
}

TUINumericInput::TUINumericInput(std::string text, unsigned short int *num, int min, int max, int step) {
	m_text=text;
	m_min=min;
	m_max=max;
	m_step=step;
	m_numi=NULL;
	m_nums=num;
	m_numf=NULL;
	m_numc=NULL;
}

TUINumericInput::TUINumericInput(std::string text, float *num, int min, int max, int step) {
	m_text=text;
	m_min=min;
	m_max=max;
	m_step=step;
	m_numi=NULL;
	m_nums=NULL;
	m_numf=num;
	m_numc=NULL;
}

TUINumericInput::TUINumericInput(std::string text, unsigned char *num, int min, int max, int step) {
	m_text=text;
	m_min=min;
	m_max=max;
	m_step=step;
	m_numi=NULL;
	m_nums=NULL;
	m_numf=NULL;
	m_numc=num;
}	

void TUINumericInput::draw(ConsoleText *ct) {
	int i=0;
	if(m_numi!=NULL) m_val=(*m_numi);
	if(m_nums!=NULL) m_val=(*m_nums);
	if(m_numf!=NULL) m_val=(*m_numf);
	if(m_numc!=NULL) m_val=(*m_numc);
	
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << m_text;
	
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	*ct << m_val;
}

void TUINumericInput::processHidEvent(const Hid::Event &evt) {
	if(m_numi!=NULL) m_val=(*m_numi);
	if(m_nums!=NULL) m_val=(*m_nums);
	if(m_numf!=NULL) m_val=(*m_numf);
	if(m_numc!=NULL) m_val=(*m_numc);
	
	if(evt.type == Event::EvtKeypress && evt.key == Event::KeyLeft) {
		m_val-=m_step;
		if(m_val < m_min) m_val=m_min;
	}
	if(evt.type == Event::EvtKeypress && evt.key == Event::KeyRight) {
		m_val+=m_step;
		if(m_val > m_max) m_val=m_max;
	}
	
	if(m_numi!=NULL) (*m_numi)=m_val;
	if(m_nums!=NULL) (*m_nums)=m_val;
	if(m_numf!=NULL) (*m_numf)=m_val;
	if(m_numc!=NULL) (*m_numc)=m_val;
}

void TUITextInput::draw(ConsoleText *ct) {
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << m_label;
	
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	*ct << *m_text;
	if(m_blink && getFocus()) *ct << "_";
	m_blinkTimer--;
	if(m_blinkTimer == 0) {
		m_blink = !m_blink;
		m_blinkTimer = 10;
	}
	if(!getFocus()) {
		m_blink=false;
		m_blinkTimer = 1;
	}
}

void TUITextInput::processHidEvent(const Hid::Event &evt) {
	if(evt.type == Event::EvtKeypress) {
		if(evt.key >= 32 && evt.key < 127 && (m_text->length() + m_label.length() <= 40)) {
			*m_text += evt.key;
		}
		if((evt.key == 8 || evt.key == 127) && m_text->length() > 0) {
			m_text->erase(m_text->end() - 1);
		}
	}
}

void TUIPasswordInput::draw(ConsoleText *ct) {
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << m_label;
	
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	for(int i=0; i<m_text->length(); i++) {
		*ct << "*";
	}
	if(m_blink && getFocus()) *ct << "_";
	m_blinkTimer--;
	if(m_blinkTimer == 0) {
		m_blink = !m_blink;
		m_blinkTimer = 10;
	}
	if(!getFocus()) {
		m_blink=false;
		m_blinkTimer = 1;
	}
}

void TUIWindow::draw_shadow(ConsoleText *console, int x, int y) {
  //int fg=(console->getColor(x,y)/16)-8;
  //if(fg<0) fg=8;
	console->putColor(x,y, BLACK|HIGH_INTENSITY);
}

void TUISlider::draw(ConsoleText *ct) {
	int i=0;
	if(m_numi!=NULL) m_val=(*m_numi);
	if(m_nums!=NULL) m_val=(*m_nums);
	if(m_numf!=NULL) m_val=(*m_numf);
	if(m_numc!=NULL) m_val=(*m_numc);
	
	ct->color(YELLOW|HIGH_INTENSITY,BLUE);
	*ct << m_text;
	
	ct->color(WHITE|HIGH_INTENSITY,BLUE);
	
	*ct << "1 ";
	for(i=0; i<m_val; i++) {
		*ct << "\xf9";
	}
	*ct << "\xfe";
	for(i=0; i<8-m_val; i++) {
		*ct << "\xf9";
	}
	*ct << " 9";
}

void TUIWindow::processHidEvent(const Hid::Event &evt) {
	if(evt.type == Event::EvtQuit) {
		m_loop = false;
		switchbrd = -2;
	} else if(evt.type == Event::EvtKeypress) {
		switch(evt.key) {
			case Event::KeyUp:
				m_widgets[m_offset]->focus(false);
				m_offset--;
				if(m_offset < 0) m_offset = 0;
				m_widgets[m_offset]->focus(true);
				break;
			case 13:
				if(m_widgets[m_offset]->getCloseOnEnter()) {
					m_loop = false;
					m_label = m_widgets[m_offset]->getReturnValue();
					break;
				}
			case Event::KeyDown:
				m_widgets[m_offset]->focus(false);
				m_offset++;
				if(m_offset >= m_widgets.size()) m_offset = m_widgets.size() - 1;
					m_widgets[m_offset]->focus(true);
				break;
		} 
	} else if(evt.type == Event::EvtKeyUp && evt.key == Event::KeyEsc) {
			m_loop = false;
	}
}

void TUIWindow::buildFromString(std::string s) {
	int i=0,j=0;
	std::string label,text;
	
	do {
		j=0;
		label="";
		text="";
		
		switch(s[i+j++]) {
			case '!':
				while(((i+j) < s.length()) && s[i+j]!=';') label += s[i+j++];
				j++;
				while(((i+j) < s.length()) && s[i+j]!='\r' && s[i+j]!='\n') text += s[i+j++];
				addWidget(new TUIHyperLink(label,text));
				break;
			case '$':
				while(((i+j) < s.length()) && s[i+j]!='\r' && s[i+j]!='\n') text += s[i+j++];
				for(int x=0; x< (m_w - text.length() - 4) / 2; x++) {
					label += " ";
				}
				addWidget(new TUILabel(label + text,true));
				break;	
			default:
				j--;
				while(((i+j) < s.length()) && s[i+j]!='\r' && s[i+j]!='\n') {
					if(s[i+j] == '\t') {
						for(int t=(10 - (text.length() % 10)); t>0; t--) {
							text += " ";
						}
					} else {
						text += s[i+j];
					}
					j++;
				}
				addWidget(new TUILabel(text));
				break;	
		}
		i+=j+1;
	} while (i < s.length());
}

void TUIWindow::draw_box(ConsoleText *console, int x, int y,int w,int h,int fg,int bg, bool shadow) {
  int i,j,f;
  //draw a box using IBM extended ASCII
  console->putColor(x,y,fg | (bg << 8));
	console->putChar(x,y,218);
	
  for(i=0;i<w;i++) {
		console->putColor(x+i+1,y,fg | (bg << 8));
		console->putChar(x+i+1,y,196);
  }
	
	console->putColor(x+w+1,y,fg | (bg << 8));
	console->putChar(x+w+1,y,191);
  
	for(i=0;i<h;i++) {
		console->putColor(x,y+i+1,fg | (bg << 8));
		console->putChar(x,y+i+1,179);
		
    for(j=0;j<w;j++) {
			console->putColor(x+j+1,y+i+1,fg | (bg << 8));
			console->putChar(x+j+1,y+i+1,' ');
    }
    
		console->putColor(x+j+1,y+i+1,fg | (bg << 8));
		console->putChar(x+j+1,y+i+1,179);
		
    if(shadow) draw_shadow(console,x+w+2,y+i+1);
    if(shadow) draw_shadow(console,x+w+3,y+i+1);
    console->color(fg,bg);
  }
  
	console->locate(x,y+h+1);
  console->printf("%c",192);
  for(i=0;i<w;i++)
    console->printf("%c",196);
  console->printf("%c",217);
  draw_shadow(console,x+w+2,y+h+1);
  draw_shadow(console,x+w+3,y+h+1);
	
  for(i=0;i<w+2;i++)
    if (shadow) draw_shadow(console,x+2+i,y+h+2);
}

void TUIWindow::doMenu(ConsoleText *ct) {
	EventCollector ec;
	Event evt;
	std::vector<TUIWidget *>::iterator widget_iter;
	int i=0;
	m_dirty = 1;
	m_loop = true;
	m_widgets[m_offset]->focus(true);
	if(playerEventCollector != NULL && playerEventCollector->listening()) playerEventCollector->stop();

	draw_box(ct, m_x, m_y, m_w, m_h, WHITE|HIGH_INTENSITY, BLUE);
	
	do {
		//if(m_dirty) {
			//m_dirty=0;
			i=0;
			draw_box(ct, m_x, m_y, m_w, m_h, WHITE|HIGH_INTENSITY, BLUE, false);
			ct->color(YELLOW | HIGH_INTENSITY, BLUE);
			if(m_widgets[m_offset]->getHelpText() != "\0") {
				ct->locate(m_x+(m_w-m_widgets[m_offset]->getHelpText().length()-1)/2,m_y+1);
				*ct << "\xae " << m_widgets[m_offset]->getHelpText() << " \xaf";
			} else {
				ct->locate(m_x+(m_w-m_title.length()+2)/2,m_y+1);
				*ct << m_title;
			}
			ct->locate(m_x,m_y+2);
			ct->color(WHITE|HIGH_INTENSITY,BLUE);
			ct->printf("%c",195);
			for(int x=0;x<m_w;x++)
				ct->printf("%c",196);
			ct->printf("%c",180); 
			ct->color(RED|HIGH_INTENSITY,BLUE);
			ct->locate(m_x+1,m_h/2+m_y+1);
			ct->printf("%c",175);
			ct->locate(m_x+m_w,m_h/2+m_y+1);
			ct->printf("%c",174);
			
			if(m_offset < m_h/2-2) {
				for(i=0; i < m_h/2-2-m_offset; i++) {
					ct->locate(m_x+(m_w/2)-20,m_y+3+i);
					if(m_h/2-2 - m_offset - i == 4) {
						ct->setANSI(true);
						*ct << " \x1b[1;36mUse \x1b[37m\x18 \x1b[36mand \x1b[37m\x19 \x1b[36mto scroll and press \x1b[37m" << ((TIKI_PLAT == TIKI_DC) ? "Start " : "Enter ") << "\x1b[36mor";
						ct->setANSI(false);
					}
					if(m_h/2-2 - m_offset - i == 3) {
						ct->setANSI(true);
						*ct << " \x1b[1;37m" << ((TIKI_PLAT == TIKI_DC) ? "Fire " : "Space ") << "\x1b[36mto select.  Press \x1b[37m" << ((TIKI_PLAT == TIKI_DC) ? " B  " : "ESC ") << "\x1b[36mto close.    ";
						ct->setANSI(false);
					}
					if(m_h/2-2 - m_offset - i == 1) {
						ct->color(YELLOW|HIGH_INTENSITY,BLUE);
						for(int u=0;u<m_w-3;u++) {
							ct->printf("%c",u%5==0?7:' ');
						}
					}
				}
			}		
			
			for(widget_iter = m_widgets.begin() + ((m_offset <=  m_h/2-2)?0:(m_offset -  m_h/2 + 2)); widget_iter != m_widgets.end() && i < m_h-2; widget_iter++) {
				ct->locate(m_x+3,m_y+3+i++);
				(*widget_iter)->update();
				(*widget_iter)->draw(ct);
			}

			if(i < m_h-2) {
				ct->locate(m_x+2,m_y+3+i++);
				ct->color(YELLOW|HIGH_INTENSITY,BLUE);
				for(int u=0;u<m_w-3;u++) {
					ct->printf("%c",u%5==0?7:' ');
				}
			}
		//}
		render();
		while (ec.getEvent(evt)) {
			processHidEvent(evt);
		}
	} while(m_loop);

	if(playerEventCollector != NULL && !playerEventCollector->listening()) playerEventCollector->start();
}
