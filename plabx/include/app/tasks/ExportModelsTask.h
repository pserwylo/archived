/*
 * ReadDataTask.h
 *
 *  Created on: 16/05/2009
 *      Author: pete
 */

#ifndef EXPORTMODELSTASK_H_
#define EXPORTMODELSTASK_H_

#include <api/Task.h>
#include <sigc++/connection.h>

class ExportModelsTask : public Task
{

public:
	ExportModelsTask();
	virtual ~ExportModelsTask(){};

public:
	virtual void run();

protected:
	virtual void end();
	void showForm();

};

#endif /* EXPORTMODELSTASK_H_ */
