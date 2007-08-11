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
#include "console.h"
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

void TaskCollect::get
	(ZZTObject *obj) {
	if(obj->type() == m_type) {
		if(m_color > 0 && obj->color() != m_color && (obj->color() - 8) != m_color)
			return;
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
	if(obj->type() == m_type) {
		if(m_color > 0 && obj->color() != m_color && (obj->color() - 8) != m_color)
			return;
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
	if(obj->type() == ZZT_OBJECT && ((ZZTOOP*)obj)->get_zztobj_name() == m_name) {
		if(m_color > 0 && obj->color() != m_color && (obj->color() - 8) != m_color)
			return;
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
	if(obj->type() == ZZT_OBJECT && ((ZZTOOP*)obj)->get_zztobj_name() == m_name) {
		if(m_color > 0 && obj->color() != m_color && (obj->color() - 8) != m_color)
			return;
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
	if(obj->type() == ZZT_OBJECT && ((ZZTOOP*)obj)->get_zztobj_name() == m_name) {
		if(m_color > 0 && obj->color() != m_color && (obj->color() - 8) != m_color)
			return;
		m_count--;
	}
}

bool TaskPlayerPosition::check() {
	Vector p = player->position();
	if(p.x <= m_max.x && p.y <= m_max.y && p.x >= m_min.x && p.y >= m_min.y)
		m_complete = true;

	return m_complete;
}

bool TaskUseTorch::check() {
	if(currentbrd->dark == 1 && world.torch_cycle > 0) {
		m_complete = true;
	}

	return m_complete;
}

bool TaskObjectCount::check() {
	int x = BOARD_X-1, y = BOARD_Y-1;
	ZZTObject *obj;
	int count=0;

	do {
		obj=get_obj_by_type(m_type, x, y);
		if(obj!=NULL) {
			if(m_color == 0 || obj->color() == m_color || obj->color() == m_color + 8) {
				count++;
				x--;
			}
		}
	} while(obj != NULL);

	if(count == m_count)
		m_complete = true;

	return m_complete;
}

std::list<Task*> taskList;

void add_task(Task *task, bool complete) {
	if(complete)
		world.task_points += task->getValue();
	task->setComplete(complete);
	taskList.push_back(task);
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
			(*task_iter)->get
			(obj);
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
