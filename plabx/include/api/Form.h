/*
 * Form.h
 *
 *  Created on: 12/05/2009
 *      Author: pete
 */

#ifndef FORM_H_
#define FORM_H_

#include <string>
#include <vector>
#include "api/FormField.h"

class Form
{
public:
	Form(std::string name) : name(name) {}
	virtual ~Form(){}

	void addField(FormField* field) { this->fields.push_back(field); };
	std::vector<FormField*>* getFields() { return &(this->fields); };
	std::string getName() { return this->name; };

private:
	std::vector<FormField*> fields;
	std::string name;
};

#endif /* FORM_H_ */
