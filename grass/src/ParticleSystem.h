/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>
#include <osg/Vec3>

namespace grass
{

class Particle;

/**
 * Class which initialises and updates particles.
 * The particles will keep track of their trails which can then be used
 * to render the grass blades.
 */
class ParticleSystem
{
public:
	// Helper function to setup a sane particle system which
	// is appropriate for grass generation.
	static ParticleSystem* createDefaultSystem();

public:
	ParticleSystem( int numOfParticles ) : numOfParticles( numOfParticles ){};
	virtual ~ParticleSystem(){};

	// Let the particle system run for however many time steps...
	void run(int timesteps);

	// Reference to each of the generated particles (and their trails
	// if the system has been run).
	inline std::vector<grass::Particle*>& getBlades() { return this->blades; };

	inline void setMass(float value) { this->mass = value; };
	inline void setSpeed(float value) { this->speed = value; };
	inline void setDirection(osg::Vec3 value) { this->direction = value; this->direction.normalize(); };
	inline void setForces(osg::Vec3 value) { this->forces = value; };

private:
	void update();

	// Takes a value and returns a random variation
	// between 50% and 150% of that value.
	// Used for randomly varying each parameter on initialisation.
	static double getRandomFactor(double value);


private:
	int numOfParticles;
	float mass;
	float speed;
	osg::Vec3 direction;
	osg::Vec3 forces;

	std::vector<grass::Particle*> blades;
};

};

#endif /* PARTICLESYSTEM_H_ */
