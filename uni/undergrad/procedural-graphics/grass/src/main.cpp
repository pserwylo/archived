/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

// Example of how to use the grass stuff.
#include <osgViewer/Viewer>

#include <Blade.h>
#include <TuftNode.h>
#include <ParticleSystem.h>
#include <Particle.h>

#include <iostream>

#include <math.h>

int main()
{

	osg::Group* root = new osg::Group();

	osg::Geode* grassNode = new osg::Geode();

	grass::ParticleSystem* system = grass::ParticleSystem::createDefaultSystem();
	system->run(10);

	for (int i = 0; i < 10000; i ++)
	{
		float x = (float)(rand() % 20000) / 1000;
		float y = (float)(rand() % 20000) / 1000;
		float z = 0;
		root->addChild( grass::TuftNode::generateTuftLOD( system->getBlades(), 10, osg::Vec3(x, y, z) ) );
	}

	root->addChild( grassNode );

    osgViewer::Viewer viewer;
    viewer.setSceneData( root );
    viewer.setUpViewInWindow( 0, 0, 800, 600, 0 );
    viewer.run();
	return 0;

}
