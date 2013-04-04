/*
 * TaskManager.h
 *
 *  Created on: 13/05/2009
 *      Author: pete
 */

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <vector>

class Task;

class TaskManager
{
public:
	TaskManager(){};
	virtual ~TaskManager(){};

public:
	// This is used as an alternative to exposing the underlying list
	// because we want to add listeners and what not when adding new
	// tasks...
	void addTask(Task* task);

private:
	std::vector<Task*> taskList;
};

#endif /* TASKMANAGER_H_ */
