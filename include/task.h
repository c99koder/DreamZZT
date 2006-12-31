/*
 *  task.h
 *  DreamZZT
 *
 *  Created by Sam Steele on 12/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TASK_H
#define _TASK_H

#define TASK_BOARD_ANY -1

#define TASK_COLLECT 1
#define TASK_TORCH 2
#define TASK_KILL_ENEMY 3
#define TASK_KILL_OBJECT 4
#define TASK_TOUCH_OBJECT 5
#define TASK_SHOOT_OBJECT 6
#define TASK_PLAYER_POSITION 7

class Task {
public:
	Task(int id, int board, std::string title, std::string description, int value) {
		m_id = id;
		m_title = title;
		m_desc = description;
		m_value = value;
		m_board = board;
		m_complete = false;
	}

	Task(std::vector<std::string> params) {
		m_id = atoi(params[0].c_str());
		m_board = atoi(params[1].c_str());
		m_title = params[2];
		m_desc = params[3];
		m_value = atoi(params[4].c_str());
		m_complete = false;
	}
	
	virtual bool check() { return false; };
	virtual void touch(ZZTObject *obj) { };
	virtual void get(ZZTObject *obj) { };
	virtual void shoot(ZZTObject *obj) { };	
	virtual void kill(ZZTObject *obj) { };

	void setComplete(bool complete) { m_complete = complete; }
	bool getComplete() { return m_complete; }
	std::string getTitle() { return m_title; }
	std::string getDescription() { return m_desc; }
	int getValue() { return m_value; }
	int getBoard() { return m_board; }
	int getID() { return m_id; }
	
protected:
	std::string m_title, m_desc;
	int m_id, m_value, m_board;
	bool m_complete;
};

class TaskCollect : public Task {
public:
	TaskCollect(int id, int board, std::string title, std::string description, int value, int type, int color, int count) : Task(id,board,title,description,value) {
		m_type = type;
		m_color = color;
		m_count = count;
	}
	
	TaskCollect(std::vector<std::string> params) : Task(params) {
		m_type = atoi(params[5].c_str());
		m_color = atoi(params[6].c_str());
		m_count = atoi(params[7].c_str());
	}
	
	bool check();
	void get(ZZTObject *obj);
private:
	int m_type, m_count, m_color;
};

class TaskKillEnemy : public Task {
public:
	TaskKillEnemy(int id, int board, std::string title, std::string description, int value, int type, int color, int count) : Task(id,board,title,description,value) {
		m_type = type;
		m_count = count;
	}
	
	TaskKillEnemy(std::vector<std::string> params) : Task(params) {
		m_type = atoi(params[5].c_str());
		m_color = atoi(params[6].c_str());
		m_count = atoi(params[7].c_str());
	}
	
	bool check();
	void kill(ZZTObject *obj);
private:
		int m_type, m_count, m_color;
};

class TaskKillObject : public Task {
public:
	TaskKillObject(int id, int board, std::string title, std::string description, int value, std::string name, int color, int count) : Task(id,board,title,description,value) {
		m_name = name;
		m_color = color;
		m_count = count;
	}
	
	TaskKillObject(std::vector<std::string> params) : Task(params) {
		m_name = params[5];
		m_color = atoi(params[6].c_str());
		m_count = atoi(params[7].c_str());
	}
	
	bool check();
	void kill(ZZTObject *obj);
private:
	int m_count, m_color;
	std::string m_name;
};

class TaskTouchObject : public Task {
public:
	TaskTouchObject(int id, int board, std::string title, std::string description, int value, std::string name, int color, int count) : Task(id,board,title,description,value) {
		m_name = name;
		m_color = color;
		m_count = count;
	}
	
	TaskTouchObject(std::vector<std::string> params) : Task(params) {
		m_name = params[5];
		m_color = atoi(params[6].c_str());
		m_count = atoi(params[7].c_str());
	}
	
	bool check();
	void touch(ZZTObject *obj);
private:
	int m_count, m_color;
	std::string m_name;
};

class TaskShootObject : public Task {
public:
	TaskShootObject(int id, int board, std::string title, std::string description, int value, std::string name, int color, int count) : Task(id,board,title,description,value) {
		m_name = name;
		m_color = color;
		m_count = count;
	}
	
	TaskShootObject(std::vector<std::string> params) : Task(params) {
		m_name = params[5];
		m_color = atoi(params[6].c_str());
		m_count = atoi(params[7].c_str());
	}
	
	bool check();
	void shoot(ZZTObject *obj);
private:
	int m_count, m_color;
	std::string m_name;
};

class TaskPlayerPosition : public Task {
public:
	TaskPlayerPosition(int id, int board, std::string title, std::string description, int value, Vector min, Vector max) : Task(id,board,title,description,value) {
		m_min = min;
		m_max = max;
	}
	
	TaskPlayerPosition(std::vector<std::string> params) : Task(params) {
		m_min.y = atoi(params[5].c_str());
		m_min.x = atoi(params[6].c_str());
		m_max.y = atoi(params[7].c_str());
		m_max.x = atoi(params[8].c_str());
	}
	
	bool check();
private:
	Vector m_min, m_max;
};

class TaskUseTorch : public Task {
public:
	TaskUseTorch(int id, int board, std::string title, std::string description, int value) : Task(id,board,title,description,value) { };
	TaskUseTorch(std::vector<std::string> params) : Task(params) { };
	bool check();
};

void add_task(Task *task, bool complete);
void delete_tasks();
void check_tasks();
void task_touch(ZZTObject *obj);
void task_get(ZZTObject *obj);
void task_shoot(ZZTObject *obj);
void task_kill(ZZTObject *obj);
#endif