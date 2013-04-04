/*
 * FormField.h
 *
 *  Created on: 12/05/2009
 *      Author: pete
 */

#ifndef FORMFIELD_H_
#define FORMFIELD_H_

#include <string>

class FormField
{
public:
	FormField(std::string name, std::string description, int type) : name(name), description(description), type(type) {}
	virtual ~FormField(){}

	inline std::string getName() { return this->name; };
	inline std::string getDescription() { return this->description; };
	inline int getType()	{ return this->type; };

	// Was going to use templates here, but will not work because the
	// type information is not just used for this, but also to deduce which
	// type of widget to use...
	inline void* getValue() { return this->value; }
	inline void setValue(void* newValue) { this->value = newValue; }

	/**
	 * These are the different type of fields which can live
	 * on one of these generic forms.
	 */
	enum
	{
		TYPE_INTEGER,
		TYPE_DOUBLE,
		TYPE_COLOUR,
		TYPE_STRING,
		TYPE_BOOLEAN
	} FieldType;

private:
	std::string name;
	std::string description;
	int type;

	void* value;

};

#endif /* FORMFIELD_H_ */
