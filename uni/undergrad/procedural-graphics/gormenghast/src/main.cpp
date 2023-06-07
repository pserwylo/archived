/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#define OSG_USE_DEPRECATED_GEOMETRY_METHODS

#include "TerrainGenerator.h"
#include "TerrainGeometry.h"
#include "TerrainSaver.h"

#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>

#include "ParticleSystem.h"
#include "TuftNode.h"
#include "TowerGenerator.h"

#include <iostream>
#include <vector>

int main()
{

	int resolution = 128;
	int size = 500;

	osg::ref_ptr<osg::Group> root = new osg::Group();

	std::cout << "Generating terrain (" << resolution << " vertices squared) (" << size << " metres squared)...";
	std::cout.flush();
	TerrainGenerator generator;
	generator.setSize(resolution);
	generator.setSmoothness(0.1);
	generator.generate();
	std::vector<double>* heightMap = generator.getDsm();
	std::cout << " Done." << std::endl;

	std::cout << "Creating terrain geometry...";
	std::cout.flush();
	osg::ref_ptr<TerrainGeometry> terrainGeometry = new TerrainGeometry( heightMap, size );
	osg::ref_ptr<osg::Geode> terrain = new osg::Geode();
	terrain->addDrawable(terrainGeometry.get());
	root->addChild(terrain.get());
	std::cout << " Done." << std::endl;

	osg::ref_ptr<osg::Group> grassLayer = new osg::Group();
	grass::ParticleSystem* system = grass::ParticleSystem::createDefaultSystem();
	system->run( 10 );

	/*
	std::cout << "Planting grass...";
	std::cout.flush();
	for (int i = 0; i < 10000; i ++)
	{
		osg::Vec3 location;
		do
		{
			location = terrainGeometry->getRandomPointOnTerrain();
		} while ( location.z() > 2 );
		grassLayer->addChild( grass::TuftNode::generateTuftLOD( system->getBlades(), 10, location ) );
	}
	root->addChild( grassLayer.get() );
	std::cout << "Done." << std::endl;
	*/

	std::cout << "Building castle towers...";
	std::cout.flush();
	osg::ref_ptr<osg::Group> castleLayer = new osg::Group();

	for (int i = 0; i < 40; i ++)
	{
		double x = rand() % 50 + size/2 - 100;
		double y = rand() % 150 + size/2 - 100;
		double z = terrainGeometry->getHeightAt( osg::Vec2( x, y ) );
		buildings::TowerGenerator interpreter( osg::Vec3( x, y, z ) );
		interpreter.setStartWidth( 10 );
		interpreter.setMinWidth( 6 );
		interpreter.setSplitProbability( 0.3 );
		interpreter.generate();
		root->addChild( interpreter.getTower() );
	}

	for (int i = 0; i < 40; i ++)
	{
		double x = rand() % 100 + 50 + size/2 - 100;
		double y = rand() % 50 + size/2 - 100;
		double z = terrainGeometry->getHeightAt(osg::Vec2(x, y));
		buildings::TowerGenerator interpreter( osg::Vec3( x, y, z ) );
		interpreter.setStartWidth( 15 );
		interpreter.setMinWidth( 8 );
		interpreter.setSplitProbability( 0.3 );
		interpreter.generate();
		root->addChild( interpreter.getTower() );
	}

	root->addChild( castleLayer.get() );
	std::cout << " Done." << std::endl;

	std::cout << "Finished generation.\nRunning visualisation...";
	std::cout.flush();
	osgViewer::Viewer viewer;
    viewer.getCamera()->setCullingMode(osg::CullSettings::CullingModeValues::NO_CULLING);
	viewer.setSceneData( root );
    viewer.setUpViewInWindow( 0, 0, 800, 600, 0 );
	viewer.run();
	std::cout << " Over." << std::endl;

	return 0;
}
