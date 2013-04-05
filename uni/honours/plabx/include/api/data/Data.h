/*
 * Data.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef DATA_H_
#define DATA_H_

#include <string>
#include <osg/BoundingBox>

class Data
{
protected:
	Data(std::string name);
public:
	virtual ~Data();
	inline std::string getName() { return this->name; };
	inline osg::BoundingBox getBounds() { return this->bounds; };

protected:
	std::string name;
	osg::BoundingBox bounds;
};

#endif /* DATA_H_ */
