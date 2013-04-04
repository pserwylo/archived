/*
 * FoxDataOutput.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef FOXDATAOUTPUT_H_
#define FOXDATAOUTPUT_H_

#include <api/UiOutput.h>
#include <fox/fx.h>
#include <osgViewer/Viewer>
#include <osg/Group>

class FoxApplication;
class FoxInputHandler;
class GraphicsWindowFOX;

class FoxOutput: public FXObject, public UiOutput
{
public:
	FoxOutput();
	virtual ~FoxOutput();

public:
	FXCanvas* getMainView();
	FXCanvas* getMinimapView();
	FXPacker* getLayersView();

	virtual void addData(Data* data);

	virtual void setup(FoxApplication* app, FoxInputHandler* input);

	enum
	{
		ID_GL_VIEW = FXCanvas::ID_LAST,
		ID_IDLE,
		ID_LAST
	};

	long onIdle(FXObject*, FXSelector, void* ptr);

protected:
	FXDECLARE(FoxOutput);

private:
	GraphicsWindowFOX* mainView;
	GraphicsWindowFOX* minimapView;
	FXVerticalFrame* layersView;

	// Used to give mainView and minimapView temporary parents until
	// they are added to the application (via "reparent()")
	FXHorizontalFrame* dummyParent;

	osgViewer::Viewer* sceneViewer;
	osg::ref_ptr<osg::Group> sceneRoot;

};

#endif /* FOXDATAOUTPUT_H_ */
