/* 
 * This software is the original work of Peter Serwylo. ID: 20266057 
 * This software is submitted in partial fulfillment of the 
 * requirements for the degree of BITS (Honours), Monash University
 */ 

#ifndef TOWERGENERATOR_H_
#define TOWERGENERATOR_H_

#include <osg/Geode>
#include <osg/Shape>

namespace buildings
{

/**
 * Randomly generates one tower for a castle.
 * Collecting many random towers in close proximity will
 * result in a fantasy-style-castle.
 */
class TowerGenerator
{
public:
	TowerGenerator( osg::Vec3 location );
	virtual ~TowerGenerator() {};

	void generate();
	inline osg::Geode* getTower() { return this->tower.get(); };

	inline void setStartWidth( int startWidth ) { this->arg_START_WIDTH = startWidth; };
	inline void setMinWidth( int minWidth ) { this->arg_MIN_WIDTH = minWidth; };
	inline void setTerminationProbability( float terminationProbability ) { this->arg_TERMINATION_PROBABILITY = terminationProbability; };
	inline void setRoofProbability( float roofProbability ) { this->arg_ROOF_PROBABILITY = roofProbability; };
	inline void setSplitProbability( float splitProbability ) { this->arg_SPLIT_PROBABILITY = splitProbability; };

private:
	// Add a new block to the tower and recursively continue on.
	void addSegment( osg::Box* parent );

	// From this point on, the segment will be split into two.
	// Each of those two will then continue to grow in the same fashion.
	void splitSegment( osg::Box* parent );

	// Put a rof structure on top of <code>child</code>
	void setRoof( osg::Box* child );

	// A window room hangs off the side of a section.
	void addWindowRoom( osg::Box* parent );

	void applyTexture( osg::Drawable* drawable );

private:
	// Randomly choose which corner or side of the tower to place this window room...
	// Assigns either a 1 or -1 to both axis'.
	static void chooseCorner( int& xAxis, int& yAxis );

private:
	// Reference to the scene graph node which is of interest to the
	// user of this class...
	osg::ref_ptr<osg::Geode> tower;

	// This is the bottom, centre of the tower.
	osg::Vec3 location;


	/* List of parameters which control the random process */

	// Initial base segment's width (in metres).
	int arg_START_WIDTH;

	// Force the process to stop when segments reach this width.
	int arg_MIN_WIDTH;

	// Although MIN_WIDTH *forces* the process to stop, we want
	// all towers to be random heights, so this is the probability
	// that at any segment, the growing procedure will stop.
	float arg_TERMINATION_PROBABILITY;

	// My girlfriend commented that there were too many roofs, so
	// now this is the probability that when finishing a tower, it
	// will have a roof...
	float arg_ROOF_PROBABILITY;

	// Probability that at any new segment, the tower will split into two
	// new growth streams of segments.
	float arg_SPLIT_PROBABILITY;
};

};

#endif /* TOWERGENERATOR_H_ */
