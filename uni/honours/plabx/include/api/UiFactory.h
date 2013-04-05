/*
 * UiFactory.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef UIFACTORY_H_
#define UIFACTORY_H_

class UiApplication;
class UiInputHandler;
class UiOutput;
class UiFormBuilder;
class UiDialogs;

class UiFactory
{

public:
	static UiApplication* getApplication();
	static UiInputHandler* getInputHandler();
	static UiOutput* getOutput();
	static UiFormBuilder* getFormBuilder();
	static UiDialogs* getDialogs();

private:
	static UiApplication* application;
	static UiInputHandler* inputHandler;
	static UiOutput* output;
	static UiFormBuilder* formBuilder;
	static UiDialogs* dialogs;

};

#endif /* UIFACTORY_H_ */
