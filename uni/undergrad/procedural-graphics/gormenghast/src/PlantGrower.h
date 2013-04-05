/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <osg/Group>
#include <string>

namespace plants
{

class PlantGrower
{
public:
	PlantGrower( std::string rules );
	virtual ~PlantGrower() {};

	void generate();
	inline osg::Node* getPlant() { return this->plant.get(); };

private:
	void turnLeft( osg::Group* parent );
	void turnRight( osg::Group* parent );
	void moveForward( osg::Group* child );
	void reduce( osg::Group* child );

private:
	osg::ref_ptr<osg::Group> plant;
	std::string rules;
};

};

#endif /* INTERPRETER_H_ */
