/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include <osgViewer/Viewer>

#include <osg/LOD>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Shape>

#include <FernGrower.h>
#include <iostream>

// #include <FernLeaf.h>
#include <FernSegment.h>

#include <time.h>

int main()
{
	srand( time( NULL ) );

	osg::Group* root = new osg::Group();


	/*
	float length = 0.2 * 2 * 1;
	float width = 0.2 * 3/4 * 1;
	osg::ref_ptr<FernLeafHigh> highGeometry = new FernLeafHigh( length, width );
	osg::ref_ptr<FernLeafMedium> mediumGeometry = new FernLeafMedium( length, width );
	osg::ref_ptr<FernLeafLow> lowGeometry = new FernLeafLow( length, width );

	osg::ref_ptr<osg::Geode> high = new osg::Geode();
	osg::ref_ptr<osg::Geode> medium = new osg::Geode();
	osg::ref_ptr<osg::Geode> low = new osg::Geode();

	high->addDrawable( highGeometry.get() );
	medium->addDrawable( mediumGeometry.get() );
	low->addDrawable( lowGeometry.get() );

	osg::ref_ptr<osg::LOD> lod = new osg::LOD();

	lod->addChild( low.get(), 100.0f, 500.0f );
	lod->addChild( medium.get(), 20.0f, 100.0f );
	lod->addChild( high.get(), 00.0f, 20.0f);

	root->addChild( lod.get() );
	*/

	/*
	plants::FernSegmentLow* segment = new plants::FernSegmentLow( 0.10, 0.02 );
	osg::Geode* geode = new osg::Geode();
	geode->addDrawable( segment );
	root->addChild( geode );
	*/

	//plants::FernSegment* segment = new plants::FernSegment( 2, 0.5 );
	//root->addChild( segment );

	/*
	FernLeafHigh* leaf = new FernLeafHigh( 10, 2 );
	osg::Geode* geode = new osg::Geode();
	geode->addDrawable( leaf );
	root->addChild( geode );
	*/
	/*
	FernLeaf* leaf = new FernLeaf( 0.4, 0.2 * 3 / 4 );
	root->addChild( leaf );
	*/


	plants::FernGrower grower;
	grower.generate();
	root->addChild( grower.getFern() );

	int numOfFerns = 5;
	int numOfLayers = 2;

	//for (int j = 0; j < numOfLayers; j ++)
	//{

	//	osg::ref_ptr<osg::PositionAttitudeTransform> accentNode = new osg::PositionAttitudeTransform();
	//	accentNode->setAttitude( osg::Quat( 30 * osg::PI / 180 * (float)j / numOfLayers, osg::Vec3( 0, 1, 0 ) ) );

	/*
		for (int i = 0; i < numOfFerns; i ++)
		{

			osg::ref_ptr<osg::PositionAttitudeTransform> rotateNode = new osg::PositionAttitudeTransform();
			rotateNode->setAttitude( osg::Quat( osg::PI * 2 * (float)i / numOfFerns, osg::Vec3( 1, 0, 0 ) ) );

			rotateNode->addChild( grower.getFern() );

//			accentNode->addChild( rotateNode.get() );

			root->addChild( rotateNode.get() );
		}
*/
//	}


    osgViewer::Viewer viewer;
    // viewer.setLightingMode( osg::View::SKY_LIGHT );
    viewer.setSceneData( root );
    viewer.setUpViewInWindow( 0, 0, 800, 600, 0 );
    viewer.run();
	return 0;

}
