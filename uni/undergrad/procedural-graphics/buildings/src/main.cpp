/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include <osgViewer/Viewer>
#include <TowerGenerator.h>
#include <iostream>

#include <BuildingBlock.h>
#include <osg/Shape>
#include <osg/Geode>

#include <time.h>

int main()
{
	srand( time( NULL ) );

	osg::Group* root = new osg::Group();

	/*
	osg::Box* box = new osg::Box( osg::Vec3( 0, 0, 0 ), 10, 100, 50 );
	osg::ref_ptr<BuildingBlock> block = new BuildingBlock( box );
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable( block.get() );
	root->addChild( geode.get() );
	*/


	for (int i = 0; i < 40; i ++)
	{
		buildings::TowerGenerator interpreter( osg::Vec3( rand() % 50, rand() % 150, 0 ) );
		interpreter.setStartWidth( 10 );
		interpreter.setMinWidth( 6 );
		interpreter.setSplitProbability( 0.3 );
		interpreter.generate();
		root->addChild( interpreter.getTower() );
	}


	for (int i = 0; i < 40; i ++)
	{
		buildings::TowerGenerator interpreter( osg::Vec3( rand() % 50 + 150, rand() % 150, 0 ) );
		interpreter.setStartWidth( 10 );
		interpreter.setMinWidth( 6 );
		interpreter.setSplitProbability( 0.3 );
		interpreter.generate();
		root->addChild( interpreter.getTower() );
	}

	for (int i = 0; i < 40; i ++)
	{
		buildings::TowerGenerator interpreter( osg::Vec3( rand() % 100 + 50, rand() % 50, 0 ) );
		interpreter.setStartWidth( 15 );
		interpreter.setMinWidth( 8 );
		interpreter.setSplitProbability( 0.3 );
		interpreter.generate();
		root->addChild( interpreter.getTower() );
	}

    osgViewer::Viewer viewer;
    viewer.setSceneData( root );
    // viewer.setUpViewerAsEmbeddedInWindow( 0, 0, 800, 600 );
    viewer.setUpViewInWindow( 0, 0, 400, 300, 0 );
    viewer.run();

	return 0;

}
