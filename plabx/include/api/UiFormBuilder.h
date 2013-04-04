/*
 * UiFormBuilder.h
 *
 *  Created on: 12/05/2009
 *      Author: pete
 */

#ifndef UIFORMBUILDER_H_
#define UIFORMBUILDER_H_

#include "api/Form.h"

class UiFormBuilder {
public:
	UiFormBuilder();
	virtual ~UiFormBuilder();

	virtual bool displayForm(Form* form) = 0;

};

#endif /* UIFORMBUILDER_H_ */
