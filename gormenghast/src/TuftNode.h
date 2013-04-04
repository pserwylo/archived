/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef TUFT_H_
#define TUFT_H_

#include <vector>
#include <osg/Geode>
#include <osg/Node>

namespace grass
{

class Blade;
class Particle;

/**
 * A TuftNode is a scene graph node which as a number of grass blades
 * and provides LOD also.
 */
class TuftNode : public osg::Geode
{
public:
	TuftNode(std::vector<Blade*> blades);
	virtual ~TuftNode(){};

public:
	/* Two helper functions which take the particles from a
	 * ParticleSystem run and turns them into usable OSG nodes.
	 */
	static TuftNode* generateTuft( std::vector<Particle*>& particlePool, unsigned int noOfBlades, osg::Vec3 location );
	static osg::Node* generateTuftLOD( std::vector<Particle*>& particlePool, unsigned int noOfBlades, osg::Vec3 location );

};

}

#endif /* TUFT_H_ */
