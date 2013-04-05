/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "TuftNode.h"
#include "Blade.h"
#include "Particle.h"

#include <stdlib.h>

#include <osg/LOD>

namespace grass {

TuftNode::TuftNode( std::vector<Blade*> blades )
{
	for (std::vector<Blade*>::iterator it = blades.begin(); it != blades.end(); it ++)
	{
		this->addDrawable( *it );
	}
}

TuftNode* TuftNode::generateTuft( std::vector<Particle*>& particlePool, unsigned int noOfBlades, osg::Vec3 location )
{
	std::vector<Blade*> blades;
	for (unsigned int i = 0; i < noOfBlades; i ++)
	{
		int particleIndex = rand() % ( particlePool.size() - 1 );
		blades.push_back( new Blade( particlePool.at( particleIndex )->getTrail(), location ) );
	}

	return new TuftNode( blades );
}

osg::Node* TuftNode::generateTuftLOD( std::vector<Particle*>& particlePool, unsigned int noOfBlades, osg::Vec3 location )
{
	std::vector<Blade*> blades_low;
	std::vector<Blade*> blades_medium;
	std::vector<Blade*> blades_high;
	for (unsigned int i = 0; i < noOfBlades; i ++)
	{
		int particleIndex = rand() % ( particlePool.size() - 1 );

		// Only show a small amount of blades in a low res tuft.
		if (i % 6 == 0)
		{
			blades_low.push_back( new Blade( particlePool.at( particleIndex )->getTrail(), location, Blade::DETAIL_LOW ) );
		}

		// Only show a small amount of blades in a mediums res tuft.
		if ( i % 5 == 0 )
		{
			blades_medium.push_back( new Blade( particlePool.at( particleIndex )->getTrail(), location, Blade::DETAIL_MEDIUM ) );
		}

		blades_high.push_back( new Blade( particlePool.at( particleIndex )->getTrail(), location, Blade::DETAIL_HIGH ) );
	}

	// Set up the viewing distances which will trigger different LOD's
	static const int HIGH = 10;
	static const int MEDIUM = 50;
	static const int LOW = 100;
	osg::LOD* lodNode = new osg::LOD();
	lodNode->addChild(new TuftNode(blades_low), MEDIUM, LOW);
	lodNode->addChild(new TuftNode(blades_medium), HIGH, MEDIUM);
	lodNode->addChild(new TuftNode(blades_high), 0, HIGH );

	return lodNode;
}

}
