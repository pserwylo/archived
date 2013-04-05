/*
 * ReadDataTask.h
 *
 *  Created on: 16/05/2009
 *      Author: pete
 */

#ifndef PROCESSDATATASK_H_
#define PROCESSDATATASK_H_

#include <api/Task.h>
#include <sigc++/connection.h>
#include <osg/Vec3>

class ProcessDataTask : public Task
{

public:
	/**
	 * When there is an error processing the data, we go and
	 * ask the user for some more data.
	 */
	static const int RE_FETCH_DATA = 0;

	static const int EXPORT_DATA = 1;

public:
	ProcessDataTask();
	virtual ~ProcessDataTask(){};

public:
	virtual void run();
	void onPointClicked(osg::Vec3 point);

protected:
	virtual void end();

private:
	sigc::connection pointClickedConnection;

};

#endif /* PROCESSDATATASK_H_ */
