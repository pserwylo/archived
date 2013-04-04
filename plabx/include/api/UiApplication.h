/*
 * Application.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <string>

class UiInputHandler;
class UiOutput;

/**
 * This pure virtual class is a base class representing the Main application
 * (usually a Window which housed the rest of the app).
 */
class UiApplication
{

public:
	UiApplication();
	virtual ~UiApplication();

	virtual void init(std::string title, int x, int y, int width, int height) = 0;
	virtual void run(int argc = 0, char** argv = NULL) = 0;

};

#endif /* APPLICATION_H_ */
