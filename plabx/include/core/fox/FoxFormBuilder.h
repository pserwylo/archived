/*
 * FoxFormBuilder.h
 *
 *  Created on: 12/05/2009
 *      Author: pete
 */

#ifndef FOXFORMBUILDER_H_
#define FOXFORMBUILDER_H_

#include "api/UiFormBuilder.h"

#include <vector>

struct FoxFormFieldWrapper
{

	FormField* field;

	FXSlider* slider;
	FXRealSpinner* spinner;
	FXTextField* textField;
	FXCheckButton* checkBox;
};

class FoxFormBuilder : public UiFormBuilder
{
public:
	FoxFormBuilder(FXApp* foxApp) : foxApp(foxApp) {};
	virtual ~FoxFormBuilder() {};

	virtual bool displayForm(Form* form);

private:
	std::vector<FoxFormFieldWrapper*> fieldWrappers;
	FXApp* foxApp;
};

#endif /* FOXFORMBUILDER_H_ */
