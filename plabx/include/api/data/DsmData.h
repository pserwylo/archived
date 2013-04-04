/*
 * DsmData.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef DSMDATA_H_
#define DSMDATA_H_

#include <api/data/Data.h>

#include <osg/Array>
#include <osg/BoundingBox>
#include <string>

class DsmData: public Data
{
public:
	DsmData( std::string name );
	virtual ~DsmData();

private:
	osg::Vec3Array* dataPoints;


public:

	osg::Node* getDisplayNode();

	getPointAt(x, y)
	{
		assert( this->isGridded );

	}
public:
	friend class DsmReader;
};

#endif /* DSMDATA_H_ */
