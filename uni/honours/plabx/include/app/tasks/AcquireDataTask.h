/*
 * ReadDataTask.h
 *
 *  Created on: 16/05/2009
 *      Author: pete
 */

#ifndef ACQUIREDATATASK_H_
#define ACQUIREDATATASK_H_

#include <api/Task.h>

class AcquireDataTask : public Task
{

public:
	static const int PROCESS_DATA = 0;

public:
	AcquireDataTask();
	virtual ~AcquireDataTask(){};

public:
	virtual void run();

protected:
	virtual void end();

};

#endif /* ACQUIREDATATASK_H_ */
