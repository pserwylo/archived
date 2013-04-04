/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "Particle.h"

using namespace grass;

osg::Vec3 Particle::acceleration = osg::Vec3(0.0f, 0.0f, -9.8f);

Particle::Particle(osg::Vec3 initialVelocity, float mass, osg::Vec3 forces ) :
	trail(new osg::Vec3Array),
	mass(mass),
	velocity( initialVelocity ),
	forces( forces )
{
	this->trail->push_back(osg::Vec3(this->position));
};

void Particle::update(float secondsPassed)
{
	this->position += this->velocity * secondsPassed;
	this->velocity += (forces / mass) * secondsPassed;

	this->trail->push_back(osg::Vec3(this->position));
}
