/*
 * UiDialogs.h
 *
 *  Created on: 16/05/2009
 *      Author: pete
 */

#ifndef UIDIALOGS_H_
#define UIDIALOGS_H_

#include <string>

class UiDialogs
{

public:
	static const int INFO = 1;
	static const int WARN = 2;
	static const int ERROR = 3;

public:
	static const int OK_CANCEL = 1;
	static const int YES_NO = 2;

public:
	virtual void displayMessage(std::string message, std::string title, int type = UiDialogs::INFO) = 0;
	virtual bool getConfirmation(std::string message, std::string title, int type = UiDialogs::OK_CANCEL) = 0;
	virtual std::string getUri() = 0;

};

#endif /* UIDIALOGS_H_ */
