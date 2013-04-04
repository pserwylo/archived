/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "PlantGrower.h"

#include <osg/Geode>

using namespace plants;

PlantGrower::PlantGrower( std::string rules ) :
	rules( rules )
{
	this->plant = new osg::Group();
}

void PlantGrower::generate()
{

	// Rectangle goes into two smaller rectangles, smaller, stacked ontop of the original.
	for ( unsigned int i = 0; i < this->rules.size(); i ++ )
	{

	}

}

void PlantGrower::turnLeft( osg::Group* parent )
{

}

void PlantGrower::turnRight( osg::Group* parent )
{

}

void PlantGrower::moveForward( osg::Group* parent )
{

}

void PlantGrower::reduce( osg::Group* parent )
{

}
