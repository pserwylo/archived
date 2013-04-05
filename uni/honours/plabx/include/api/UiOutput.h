/*
 * UiOutput.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef UIOUTPUT_H_
#define UIOUTPUT_H_

#include <vector>
#include <osg/Group>

class Data;
class UiApplication;
class UiInputHandler;


/**
 * This struct holds extra information ontop of just
 * the data itself. At time of writing, it also keeps track
 * as to whether the data is visible or not.
 */
struct DataView
{
	Data* data;
	bool isVisible;
};

class UiOutput
{
public:
	UiOutput();
	virtual ~UiOutput();

public:
	virtual void addData(Data* data) = 0;
	void setVisible(Data* data, bool isVisible);

	// This is public so that users of the library can also add whatever they
	// want to the scenegraph.
	osg::Group* getSceneRoot();

private:
	std::vector<DataView> dataList;
	osg::ref_ptr<osg::Group> sceneRoot;
};

#endif /* UIOUTPUT_H_ */
