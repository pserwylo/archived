/*
 * FoxDialogs.h
 *
 *  Created on: 16/05/2009
 *      Author: pete
 */

#ifndef FOXDIALOGS_H_
#define FOXDIALOGS_H_

#include <api/UiDialogs.h>

#include <fox/fx.h>

class FoxApplication;

class FoxDialogs : public UiDialogs
{

public:
	FoxDialogs(FoxApplication* app);
	virtual ~FoxDialogs() {};

	virtual void displayMessage(std::string message, std::string title, int type = UiDialogs::INFO);
	virtual bool getConfirmation(std::string message, std::string title, int type = UiDialogs::OK_CANCEL);
	virtual std::string getUri();

private:
	FoxApplication* app;

};

#endif /* FOXDIALOGS_H_ */
