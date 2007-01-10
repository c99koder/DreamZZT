/* task.cpp - DreamZZT Online tasks
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

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Hid;

#include <iostream>
#include <string>
#include <sstream>
#include <Tiki/drawables/console.h>
#include "board.h"
#include "object.h"
#include "task.h"
#include "status.h"
#include "http.h"

extern ConsoleText *ct;
extern Player *player;
extern struct board_info_node *currentbrd;
extern struct world_header world;

bool TaskCollect::check() {
	if(m_count <= 0) {
		m_complete = true;
	}
	
	return m_complete;
}

void TaskCollect::get(ZZTObject *obj) {
	if(obj->getType() == m_type) {
		if(m_color > 0 && obj->getColor() != m_color && (obj->getColor() - 8) != m_color) return;
		m_count--;
	}
}

bool TaskKillEnemy::check() {
	if(m_count <= 0) {
		m_complete = true;
	}
	
	return m_complete;
}

void TaskKillEnemy::kill(ZZTObject *obj) {
	if(obj->getType() == m_type) {
		if(m_color > 0 && obj->getColor() != m_color && (obj->getColor() - 8) != m_color) return;
		m_count--;
	}
}

bool TaskKillObject::check() {
	if(m_count <= 0) {
		m_complete = true;
	}
	
	return m_complete;
}

void TaskKillObject::kill(ZZTObject *obj) {
	if(obj->getType() == ZZT_OBJECT && ((ZZTOOP*)obj)->get_zztobj_name() == m_name) {
		if(m_color > 0 && obj->getColor() != m_color && (obj->getColor() - 8) != m_color) return;
		m_count--;
	}
}

bool TaskShootObject::check() {
	if(m_count <= 0) {
		m_complete = true;
	}
	
	return m_complete;
}

void TaskShootObject::shoot(ZZTObject *obj) {
	if(obj->getType() == ZZT_OBJECT && ((ZZTOOP*)obj)->get_zztobj_name() == m_name) {
		if(m_color > 0 && obj->getColor() != m_color && (obj->getColor() - 8) != m_color) return;
		m_count--;
	}
}

bool TaskTouchObject::check() {
	if(m_count <= 0) {
		m_complete = true;
	}
	
	return m_complete;
}

void TaskTouchObject::touch(ZZTObject *obj) {
	if(obj->getType() == ZZT_OBJECT && ((ZZTOOP*)obj)->get_zztobj_name() == m_name) {
		if(m_color > 0 && obj->getColor() != m_color && (obj->getColor() - 8) != m_color) return;
		m_count--;
	}
}

bool TaskPlayerPosition::check() {
	Vector p = player->getPosition();
	if((m_board==0 || m_board==currentbrd->num)  && (p.x <= m_max.x && p.y <= m_max.y && p.x >= m_min.x && p.y >= m_min.y))
		m_complete = true;
	
	return m_complete;
}

bool TaskUseTorch::check() {
	if(currentbrd->dark == 1 && world.torch_cycle > 0) {
		m_complete = true; 
	}
	
	return m_complete;
}

std::list<Task*> taskList;

void add_task(Task *task, bool complete) {
	if(complete) world.task_points += task->getValue();
	task->setComplete(complete);
	taskList.push_back(task);
}

template <typename T>
std::string ToString(T aValue)
{
	std::stringstream ss;
	ss << aValue;
	return ss.str();
}


void check_tasks() {
#ifdef NET
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(!((*task_iter)->getComplete())) {
			if((*task_iter)->check()) {
				Debug::printf("Task complete: %s\n",(*task_iter)->getTitle().c_str());
				std::string s = http_get_string(DZZTNET_HOST + DZZTNET_HOME + std::string("?PostBackAction=CompleteTask&TaskID=") + ToString((*task_iter)->getID()));
				if(s=="OK") {
					world.task_points += (*task_iter)->getValue();
					draw_score();
					TUIWindow t("Task Complete");
					t.buildFromString("Congratulations!  You have completed\r\
the following task:\r\
\r\
$" + ((*task_iter)->getTitle()) + "\r\r" + ((*task_iter)->getDescription()) + "\r\
\r\
You've earned a bonus of " + ToString((*task_iter)->getValue()) + " points.\r");
					t.doMenu(ct);
				} else {
					TUIWindow t("Task Submission Error");
					t.buildFromString("The task may already be complete or\nhas been removed from the server.\n\nThis may also indicate an invalid\nusername or password.\n\nTaskID: " + ToString((*task_iter)->getID()) + "\n");
					t.doMenu(ct);
				}
			}
		}
	}
#endif
}

void task_touch(ZZTObject *obj) {
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(((*task_iter)->getBoard() == 0 || (*task_iter)->getBoard() == currentbrd->num) && !((*task_iter)->getComplete())) {
			(*task_iter)->touch(obj);
		}
	}
}

void task_get(ZZTObject *obj) {
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(((*task_iter)->getBoard() == 0 || (*task_iter)->getBoard() == currentbrd->num) && !((*task_iter)->getComplete())) {
			(*task_iter)->get(obj);
		}
	}
}

void task_kill(ZZTObject *obj) {
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(((*task_iter)->getBoard() == 0 || (*task_iter)->getBoard() == currentbrd->num) && !((*task_iter)->getComplete())) {
			(*task_iter)->kill(obj);
		}
	}
}

void task_shoot(ZZTObject *obj) {
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(((*task_iter)->getBoard() == 0 || (*task_iter)->getBoard() == currentbrd->num) && !((*task_iter)->getComplete())) {
			(*task_iter)->shoot(obj);
		}
	}
}
