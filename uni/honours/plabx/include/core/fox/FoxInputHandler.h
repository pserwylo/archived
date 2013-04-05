/*
 * FoxUiInputHandler.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef FOXUIINPUTHANDLER_H_
#define FOXUIINPUTHANDLER_H_

#include <api/UiInputHandler.h>

class FoxApplication;
class FoxOutput;

class FoxInputHandler: public UiInputHandler
{
public:
	FoxInputHandler();
	virtual ~FoxInputHandler();

	virtual void setup(FoxApplication* app, FoxOutput* output);


	// Collection of functions which are called from the FoxApplicationWindow when certain user actions
	// take place. Each one just emits its specified signal.
public:
	void pointSelected(osg::Vec3 point);
	void polygonDrawn(osg::Vec3Array* polygon);

};

#endif /* FOXUIINPUTHANDLER_H_ */
