/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "ParticleSystem.h"
#include "Particle.h"

#include <stdlib.h>
#include <math.h>

using namespace grass;

ParticleSystem* ParticleSystem::createDefaultSystem()
{
	ParticleSystem* system = new ParticleSystem(100);
	system->setDirection( osg::Vec3( 0.0f, 0.0f, 0.5f ) );
	system->setSpeed( 10 );
	system->setForces( osg::Vec3( 0.0f, 0.0f, -9.8f ) );
	system->setMass( 3 );
	return system;
}

void ParticleSystem::run(int timesteps)
{


	// Calculate random direction...
	// AB = this->direction
	// BC = width of radius of the projection cone, perpendicular to AB

	// TODO: Rotate BC around AB...

	/*
	 *
	 *       __-----__
		 *    _--    B    --_ C
	 *    \--__  *  __--/
	 *     \   --|--   /
	 *      \    |    /
	 *       \   |   /
	 *        \  |  /
	 *         \ | /
	 *          \|/
	 *           *
	 *           A
	 *
	 *
	 *      _______________
	 *     /              /
	 *    / _____/____   /
	 *   /      / B     / C
	 *  /______________/
	 *      \  D |    /
	 *       \   |   /
	 *        \  |  /
	 *         \ | /
	 *          \|/
	 *           *
	 *           A
	 */
	osg::Vec3 BC = this->direction ^ osg::Vec3(1.0f, 1.0f, 0.0f);
	BC.normalize();

	osg::Vec3 BD = this->direction ^ BC;
	BD.normalize();

	for (int i = 0; i < this->numOfParticles; i ++)
	{
		float particleMass = ParticleSystem::getRandomFactor(this->mass);
		float particleSpeed = ParticleSystem::getRandomFactor(this->speed);

		float offsetX = rand() % (int)(this->speed/2) - this->speed/4;
		float offsetY = rand() % (int)(this->speed/2) - this->speed/4;
		osg::Vec3 offset = BC * offsetX + BD * offsetY;

		osg::Vec3 particleDirection = (this->direction * particleSpeed) + offset;

		this->blades.push_back( new Particle( particleDirection, particleMass, this->forces ) );
	}

	for (int i = 0; i < timesteps; i ++)
	{
		this->update();
	}
}

double ParticleSystem::getRandomFactor(double value)
{
	return value * (1 + (double)(rand() % 100) / 100)/5 - 0.5f;
}

void ParticleSystem::update()
{
	for (std::vector<grass::Particle*>::iterator it = this->blades.begin(); it != this->blades.end(); it ++)
	{
		(*it)->update(0.01f);
	}
}
