/*
 * Task.h
 *
 *  Created on: 13/05/2009
 *      Author: pete
 */

#ifndef TASK_H_
#define TASK_H_

#include <map>
#include <sigc++/sigc++.h>

class Task {
public:
	Task() {};
	virtual ~Task() {};

public:
	// This is the starting point for any task, where it initializes itself
	// and begins executing...
	// Once entering this function, the state of the application is solely in
	// this task, even once the function returns, until it passes the torch by
	// calling moveTo(int).
	virtual void run() = 0;

	// A neighbour is a node in the graph (flowchart) of tasks which can be invoked
	// from this task.
	void addNeighbour(int identifier, Task* task);

	inline sigc::signal<void, Task*> getCompletedSignal() { return this->completedSignal; };

protected:
	// Jump to another node in the flowchart.
	void moveTo(int taskIdentifier);

	// Clean up any input listeners or the like, to make sure that
	// no input is directed to this task (given that it is now not the
	// task being executed). Will be called just before moving onto the
	// next task in the flowchart.
	virtual void end() = 0;

private:
	// List of all tasks which can be invoked from this particular one.
	std::map<int, Task*> neighbouringTasks;

	// This signal is emitted when the task ends, and is used initially
	// so that the gui can display the status of the events...
	sigc::signal<void, Task*> completedSignal;

};

#endif /* TASK_H_ */
