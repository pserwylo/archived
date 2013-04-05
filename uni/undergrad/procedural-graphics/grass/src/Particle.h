/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <osg/Array>

namespace grass
{

/**
 * These particles keep track of their paths so that we can
 * render a line following that path.
 * Doesn't do much other than integrate the position and velocity,
 * but as mentioned in the lectures, the update method can easily be refactored
 * to support a more advanced technique.
 * Having said that, there is no need for an extremely "correct" trajectory,
 * given we are just using this for small blades of grace.
 */
class Particle
{
public:
	Particle(osg::Vec3 initialVelocity, float mass, osg::Vec3 forces );

	virtual ~Particle(){};

	void update(float secondsPassed);

	// For grass, we are interested in the history of the
	// particles trajectory.
	inline osg::Vec3Array* getTrail() { return this->trail; };

private:
	osg::Vec3Array* trail;

	// The values which will get integrated.
	osg::Vec3 position;
	osg::Vec3 velocity;

	osg::Vec3 forces;
	float mass;

	// just gravity...
	static osg::Vec3 acceleration;

};

};

#endif /* PARTICLE_H_ */
