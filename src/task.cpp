/* task.cpp - Manage DZZT.NET tasks
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

extern ConsoleText *ct;

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
		m_count--;
	}
}

bool TaskUseTorch::check() {
	if(currentbrd->dark == 1 && world.torch_cycle > 0) {
		m_complete = true; 
	}
	
	return m_complete;
}

std::list<Task*> taskList;

void add_task(Task *task) {
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
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(!((*task_iter)->getComplete())) {
			if((*task_iter)->check()) {
				Debug::printf("Task complete: %s\n",(*task_iter)->getTitle().c_str());
				give_score((*task_iter)->getValue());
				TUIWindow t("Task Complete");
				t.buildFromString("Congratulations!  You have completed\r\
the following task:\r\
\r\
$" + ((*task_iter)->getTitle()) + "\r" + ((*task_iter)->getDescription()) + "\r\
\r\
You've earned a bonus of " + ToString((*task_iter)->getValue()) + " points.\r");
				t.doMenu(ct);
			}
		}
	}
}

void task_touch(ZZTObject *obj) {
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(!((*task_iter)->getComplete())) {
			(*task_iter)->touch(obj);
		}
	}
}

void task_get(ZZTObject *obj) {
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(!((*task_iter)->getComplete())) {
			(*task_iter)->get(obj);
		}
	}
}

void task_kill(ZZTObject *obj) {
	std::list<Task*>::iterator task_iter;
	
	for(task_iter = taskList.begin(); task_iter != taskList.end(); task_iter++) {
		if(!((*task_iter)->getComplete())) {
			(*task_iter)->kill(obj);
		}
	}
}
