/*
 * UiInputHandler.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef UIINPUTHANDLER_H_
#define UIINPUTHANDLER_H_

#include <osg/Vec3>
#include <osg/Array>
#include <sigc++/sigc++.h>

class UiApplication;
class UiOutput;

class UiInputHandler
{
public:
	UiInputHandler();
	virtual ~UiInputHandler();

	/*
	 * The following functions return pointers to the signals required for you
	 * to connect to in order to receive high level input events.
	 * They live from
	 * Usage:
	 *   inputHandler->pointSelectedSignal()->connect(sigc::mem_func(object, &Class::function));
	 */
	sigc::signal<void, osg::Vec3>* getPointSelectedSignal();
	sigc::signal<void, osg::Vec3Array*>* getPolygonDrawnSignal();

protected:
	sigc::signal<void, osg::Vec3> pointSelectedSignal;
	sigc::signal<void, osg::Vec3Array*> polygonDrawnSignal;
};

#endif /* UIINPUTHANDLER_H_ */
