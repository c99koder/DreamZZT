/* player.cpp - Player object
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
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/tikitime.h>
#include <Tiki/thread.h>
#include <Tiki/eventcollector.h>
#include <string.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;
using namespace Tiki::Audio;
using namespace Tiki::Thread;

#include "console.h"
#include "object.h"
#include "board.h"
#include "http.h"
#include "status.h"
#include "sound.h"

extern ZZTMusicStream *zm;
extern ConsoleText *ct;
extern ConsoleText *dt;
extern ConsoleText *st;
extern Mutex zzt_screen_mutex;

extern int switchbrd;
extern struct world_header world;
Player *player=NULL;
extern struct board_info_node *currentbrd;
void status(char *text);

#define MENU "Select an option:\r\
!save;Save game\r\
!hints;Online hints\r\
!quit;Quit game"

EventCollector *playerEventCollector=NULL;
extern float zoom;

void Player::processEvent(const Event & evt) {
	if (evt.type == Hid::Event::EvtQuit) {
		switchbrd = -2;
	}
	if (evt.type == Hid::Event::EvtAxis && evt.axis == Event::AxisRight) {
		zoom = 1 + ((float)evt.axisValue / 256.0f);
	}
	if (evt.type == Hid::Event::EvtKeyUp || evt.type == Hid::Event::EvtBtnRelease) {
		if(evt.key == 's' || evt.btn == Event::BtnY) {
				switchbrd=-4;
		}	else if(evt.key == 't' || evt.btn == Event::BtnX) {
			if(world.torches>0) {
				if(currentbrd->dark==1) {
					world.torch_cycle=200;
					take_torch(1);
				} else {
					set_msg("Don't need torch - room is not dark!");
				}
			} else {
				set_msg("You don't have any torches!");
			}
		} else if(evt.key == 'p') {
			setFlag(F_SLEEPING);
		} else if(evt.key == Event::KeyEsc) {
			switchbrd=-3;
		} else if((m_move == UP && (evt.key == Event::KeyUp || evt.btn == Event::BtnUp)) ||
							(m_move == DOWN && (evt.key == Event::KeyDown || evt.btn == Event::BtnDown)) ||
							(m_move == LEFT && (evt.key == Event::KeyLeft || evt.btn == Event::BtnLeft)) ||
							(m_move == RIGHT && (evt.key == Event::KeyRight || evt.btn == Event::BtnRight))) {
			m_move = IDLE;
		} else if(evt.btn == Event::BtnA || evt.key == 32) {
			m_shoot = IDLE;
		}
	} else if ((evt.type == Hid::Event::EvtKeypress || evt.type == Hid::Event::EvtBtnPress) && world.health > 0) {
		m_flags&=~F_SLEEPING;

		if(evt.key == 32 || evt.btn == Event::BtnA) {
			if(m_move != IDLE) {
				m_move = IDLE;
				m_shoot = m_move;
			} else {
				m_shoot = SHOOTING;
			}
		}

		if(evt.btn == Event::MouseWheelUp) {
			if(zoom < 2) zoom += 0.1f;
		}

		if(evt.btn == Event::MouseWheelDown) {
			if(zoom > 1) zoom -= 0.1f;
		}
		
		switch(evt.key) {
			case ']':
				if(zoom < 6) zoom += 0.1f;
				break;			
			case '[':
				if(zoom > 1) zoom -= 0.1f;
				break;
			case Event::KeyUp:
				m_heading=UP;
				if(m_shoot != IDLE) {
					m_shoot=UP;
				} else {
					m_move=UP;
				}
				break;
			case Event::KeyDown:
				m_heading=DOWN;
				if(m_shoot != IDLE) {
					m_shoot=DOWN;
				} else {
					m_move=DOWN;
				}
				break;
			case Event::KeyLeft:
				m_heading=LEFT;
				if(m_shoot != IDLE) {
					m_shoot=LEFT;
				} else {
					m_move=LEFT;
				}
				break;
			case Event::KeyRight:
				m_heading=RIGHT;
				if(m_shoot != IDLE) {
					m_shoot=RIGHT;
				} else {
					m_move=RIGHT;
				}
				break;			
			case 32:
				if(currentbrd->maxshots==0) {
					set_msg("Can't shoot in here");
				} else {
					m_shoot=m_heading;
				}
				break;
		}
	}
}

void Player::create() {
	m_shape=2;
	m_fg=15;
	m_bg=1;
	m_shoot = m_move = IDLE;

	//if(!m_model) {
	//	m_model = new AMFModelInstance("block.amf", m_position, Vector(0.0f, 1.0f, 0.0f));
	//	gl->insert(m_model);
	//}

	if(m_cycle == 0) m_cycle = 1;
	if(playerEventCollector==NULL) {
		playerEventCollector = new EventCollector();
	}
}

void Player::message(ZZTObject *them, std::string message) {
	if(message == "shot" || message == "bombed" || message == "time") {
		if(world.energizer_cycle==0 || message == "time") {
			take_health(10);
			if(message != "time" && world.health > 0) {
				set_msg("Ouch!");
				zm->setTune("t--c+c---c+d#");
				zm->start();
			}
			if(currentbrd->reenter && world.health>0) {
				world.time=currentbrd->time;
				draw_time();
				currentbrd->board[(int)m_position.x][(int)m_position.y].obj=currentbrd->board[(int)m_position.x][(int)m_position.y].under;
				currentbrd->board[(int)m_position.x][(int)m_position.y].under=NULL;
				draw_block((int)m_position.x, (int)m_position.y);
				m_position.x = currentbrd->reenter_x;
				m_position.y = currentbrd->reenter_y;				
				currentbrd->board[(int)m_position.x][(int)m_position.y].under=currentbrd->board[(int)m_position.x][(int)m_position.y].obj;
				currentbrd->board[(int)m_position.x][(int)m_position.y].obj=this;
				draw_block((int)m_position.x, (int)m_position.y);
				m_flags|=F_SLEEPING;
			}
		}
	}
}

void whip(struct object *me, int x, int y, char shape) {
/*	struct object *them=currentbrd->board[x][y].obj;
	ct->locate(x,y);
	ct->color(7,0); ct->printf("%c",shape);
	if(them!=NULL && them->message!=NULL) {
		them->message(them,me,"whipped");
	} else if(currentbrd->board[x][y].code==I_BREAKABLE) {
		currentbrd->board[x][y].code=I_EMPTY;
		currentbrd->board[x][y].color=0;
		currentbrd->board[x][y].solid=0;
	}
	video_refresh();
	draw_block(x,y);*/
}

void Player::update() {
	int s=0;
	Event evt;
	struct board_info_node *brd;
	ZZTObject *obj;
	direction oldMove = m_move;
	direction oldShoot = m_shoot;
	
	if(m_flags&F_SLEEPING && world.health > 0) {
		st->locate(4,6);
		st->color(15,1);
		st->printf("Pausing...");
		do {
			if(s==0) {
				draw();
				s++;
			} else {
				s=0;
				ct->locate((int)m_position.x, (int)m_position.y);
				ct->color(currentbrd->board[(int)m_position.x][(int)m_position.y].under->fg(),currentbrd->board[(int)m_position.x][(int)m_position.y].under->bg());
				ct->printf("%c",currentbrd->board[(int)m_position.x][(int)m_position.y].under->shape());
			}
			draw_msg();
			render();
			Time::sleep(100000);
			while (playerEventCollector->getEvent(evt)) {
				processEvent(evt);
			}	
		} while(m_flags&F_SLEEPING);
		st->locate(4,6);
		st->color(15,1);
		st->printf("          ");
		draw();
	}

	if(world.energizer_cycle > 0) {
		if(world.energizer_cycle%2==1) {
			m_shape=2;
			m_bg=1;
			m_fg=15;
		} else {
			m_shape=1;
			m_fg=15;
			m_bg=5;
		}
	}

	while (playerEventCollector->getEvent(evt)) {
		processEvent(evt);
		if((m_move != IDLE || m_shoot != IDLE) && (m_move != oldMove || m_shoot != oldShoot)) break;
	}	
	
	switch(m_move) {
		case UP:
			if(m_position.y==0&&board_up()>0) {
				brd=get_board(board_up());
				decompress(brd);
				obj=get_obj_by_type(brd,ZZT_PLAYER);
				brd->board[(int)obj->position().x][(int)obj->position().y].obj=brd->board[(int)obj->position().x][(int)obj->position().y].under;
				brd->board[(int)obj->position().x][(int)obj->position().y].under = NULL;
				obj->setPosition(Vector(m_position.x, BOARD_Y-1,0));
				brd->board[(int)obj->position().x][(int)obj->position().y].under=brd->board[(int)obj->position().x][(int)obj->position().y].obj;
				brd->board[(int)obj->position().x][(int)obj->position().y].obj=obj;
				switchbrd=board_up();
				m_move=IDLE;
			}
			break;
		case DOWN:
			if(m_position.y>=BOARD_Y-1&&board_down()>0) {
				brd=get_board(board_down());
				decompress(brd);
				obj=get_obj_by_type(brd,ZZT_PLAYER);
				brd->board[(int)obj->position().x][(int)obj->position().y].obj=brd->board[(int)obj->position().x][(int)obj->position().y].under;
				brd->board[(int)obj->position().x][(int)obj->position().y].under = NULL;
				obj->setPosition(Vector(m_position.x,0,0));
				brd->board[(int)obj->position().x][(int)obj->position().y].under=brd->board[(int)obj->position().x][(int)obj->position().y].obj;
				brd->board[(int)obj->position().x][(int)obj->position().y].obj=obj;
				switchbrd=board_down();
				m_move=IDLE;
			}
			break;
		case LEFT:
			if(m_position.x==0&&board_left()>0) {
				brd=get_board(board_left());
				decompress(brd);
				obj=get_obj_by_type(brd,ZZT_PLAYER);
				brd->board[(int)obj->position().x][(int)obj->position().y].obj=brd->board[(int)obj->position().x][(int)obj->position().y].under;
				brd->board[(int)obj->position().x][(int)obj->position().y].under = NULL;
				obj->setPosition(Vector(BOARD_X-1, m_position.y,0));
				brd->board[(int)obj->position().x][(int)obj->position().y].under=brd->board[(int)obj->position().x][(int)obj->position().y].obj;
				brd->board[(int)obj->position().x][(int)obj->position().y].obj=obj;
				switchbrd=board_left();
				m_move=IDLE;
			}
			break;
		case RIGHT:
			if(m_position.x>=BOARD_X-1&&board_right()>0) {
				brd=get_board(board_right());
				decompress(brd);
				obj=get_obj_by_type(brd,ZZT_PLAYER);
				brd->board[(int)obj->position().x][(int)obj->position().y].obj=brd->board[(int)obj->position().x][(int)obj->position().y].under;
				brd->board[(int)obj->position().x][(int)obj->position().y].under = NULL;
				obj->setPosition(Vector(0,m_position.y,0));
				brd->board[(int)obj->position().x][(int)obj->position().y].under=brd->board[(int)obj->position().x][(int)obj->position().y].obj;
				brd->board[(int)obj->position().x][(int)obj->position().y].obj=obj;
				switchbrd=board_right();
				m_move=IDLE;
			}
			break;
	}
	if(m_move!=IDLE) {
		//gl->remove(m_model);
		move(m_move);
		//m_model = new AMFModelInstance("block.amf", m_position, Vector(0,1,0));
		//gl->insert(m_model);
		//m_model->position = m_position;
	}
	
	if(m_shoot!=IDLE) {
		shoot(m_shoot);
	}
}

Player::~Player()
{
	//if(m_model) {
	//	gl->remove(m_model);
	//}
}