/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <osg/Group>
#include <string>
#include <FernLeaf.h>
#include <FernSegment.h>

namespace plants
{

/**
 * This class will generate one fern on calling the generate() function.
 * It is very deterministic in structure.
 * Although it is not hard to add a bit of randomness to each parameter at different
 * points during the generation, I find the striking self-similarity of the fern one
 * of the most distinctive thing about the plant.
 */
class FernGrower
{
public:
	FernGrower( );
	virtual ~FernGrower() {};

	void generate();
	float generateBase();
	inline osg::Node* getFern() { return this->fern.get(); };

private:
	void branch( osg::Group* parent, int depth, float accumulatedScale = 1 );

private:
	osg::ref_ptr<osg::Group> fern;
	osg::ref_ptr<plants::FernSegment> templateSegment;
	osg::ref_ptr<plants::FernLeaf> templateLeaf;
	osg::ref_ptr<osg::Group> templateBase;

	int arg_NUM_OF_SEGMENTS;
	float arg_SEGMENT_LENGTH;
	float arg_SEGMENT_THICKNESS;
	float arg_STEP_SCALE_FACTOR;
	int arg_MAX_DEPTH;
	float arg_MIN_SEGMENT_LENGTH;
	float arg_BEND_ANGLE;


};

};

#endif /* INTERPRETER_H_ */
