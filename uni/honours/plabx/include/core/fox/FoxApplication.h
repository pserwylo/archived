/*
 * FoxApplication.h
 *
 *  Created on: 11/05/2009
 *      Author: Peter Serwylo
 */

#ifndef FOXAPPLICATION_H_
#define FOXAPPLICATION_H_

#include "api/UiApplication.h"
#include "fox/fx.h"

class FoxApplicationWindow;

class FoxApplication: public UiApplication
{

public:
	FoxApplication();
	virtual ~FoxApplication();

	virtual void init(std::string title, int x, int y, int width, int height);
	virtual void setup(UiInputHandler* input, UiOutput* output);
	virtual void run(int argc = 0, char** argv = NULL);

	inline FXApp* getFoxApp() { return this->app; };
	inline FoxApplicationWindow* getFoxAppWindow() { return this->window; };

protected:
	FXApp* app;
	FoxApplicationWindow* window;
};

#endif /* FOXAPPLICATION_H_ */
