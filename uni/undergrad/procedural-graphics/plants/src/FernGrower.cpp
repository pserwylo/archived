/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "FernGrower.h"

#include <osg/Quat>
#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>

#include <osgUtil/PrintVisitor>

#include <iostream>
#include <stdlib.h>

using namespace plants;

// Initialise to sane default parameters.
FernGrower::FernGrower( ) :
	arg_NUM_OF_SEGMENTS( 20 ), // 20 segments before terminating a sub-tree.
	arg_SEGMENT_LENGTH( 0.2 ), // 20 centimetres.
	arg_SEGMENT_THICKNESS( 0.01 ), // Half a centimetre.
	arg_STEP_SCALE_FACTOR( 0.97 ), // Factor to scale down segments each step forwards.
	arg_MAX_DEPTH( 2 ), // Only recurse down twice before appending leaves.
	arg_MIN_SEGMENT_LENGTH( 0.01), // One centimetre.
	arg_BEND_ANGLE( 5 * osg::PI / 180 ) // Simulate the effect of gravity so that the tree is not flat.
{
	this->fern = new osg::Group();

	// Used many times, but only the one object is required for each...
	this->templateSegment = new FernSegment( this->arg_SEGMENT_LENGTH, this->arg_SEGMENT_THICKNESS );
	this->templateLeaf = new FernLeaf( this->arg_SEGMENT_LENGTH * 2, this->arg_SEGMENT_LENGTH * 4 / 4 );

}

/*
 * Create the fern.
 *
 *             D
 *            / \           C
 * __---___        ___---__
 *-        --_  _--        -
 *           _\/_    __
 *          \|  |/     }
 *          \|  |/      }___ B
 *      ___ \|  |/      }
 * A --{___ \|__|/   __}
 *
 *
 * A) Create one base section of the fern.
 * B) Then, continue stacking these on top of eachother until we are at a nice height.
 * C) Begin a branch structure.
 * D) Rotate around and continue adding branch structures until we've completed the full 360.
 *
 */
void FernGrower::generate()
{
	// Start of with the base, and record the height it terminates at...
	float height = this->generateBase();

	// Using that height, transform the ferns "height" high.
	osg::ref_ptr<osg::PositionAttitudeTransform> heightTransform = new osg::PositionAttitudeTransform();
	heightTransform->setPosition( osg::Vec3( height, 0, 0 ) );
	this->fern->addChild( heightTransform.get() );

	// numOfBranches = how many fern branches to append in one 360 spin around the base...
	int numOfBranches = 5;
	osg::ref_ptr<osg::PositionAttitudeTransform> branchTransform = new osg::PositionAttitudeTransform();
	heightTransform->addChild( branchTransform.get() );
	for ( int i = 0; i < numOfBranches; i ++ )
	{

		osg::ref_ptr<osg::PositionAttitudeTransform> branchTransformNode = new osg::PositionAttitudeTransform();

		// Random adjustment to the size ( multiply by factor: 0.85 < factor < 1.15 )
		branchTransformNode->setScale( osg::Vec3(
			(double)(rand() % 30) / 100 + 0.85,
			(double)(rand() % 30) / 100 + 0.85,
			(double)(rand() % 30) / 100 + 0.85) );

		branchTransformNode->setAttitude( osg::Quat(
			2 * osg::PI * (float)i / numOfBranches,
			osg::Vec3( 1.0, 0.0, 0.0 ) ) );

		// Tilt upwards so that they don't start at the horizontal...
		osg::ref_ptr<osg::PositionAttitudeTransform> upTransform = new osg::PositionAttitudeTransform();
		upTransform->setAttitude( osg::Quat( this->arg_BEND_ANGLE * 4 , osg::Vec3( 0, 1, 0 ) ) );

		branchTransform->addChild( branchTransformNode.get() );
		branchTransformNode->addChild( upTransform.get() );

		// ... now that all of the transforms are done, we can actually build the branch.
		this->branch( upTransform, 0 );
	}

}

/*
 * Construct a series of cylinders pointing outwards like so:
 *
 *      | |
 *   \  o o  /
 *    o     o
 * - o       o -
 * - o       o -
 *    o     o
 *   /  o o  \
 *      | |
 *
 * Then stack them on top of each other, rotated a bit, until a certain height.
 */
float FernGrower::generateBase()
{
	// Construct the base of the fern...
	osg::TessellationHints* hints = new osg::TessellationHints();
	hints->setCreateBottom( false );
	hints->setDetailRatio( 0.2 );

	// This is the little sticky outie bit which is aggregated numerous
	// times to create the trunk of the fern...
	osg::ref_ptr<osg::ShapeDrawable> baseStickyOutieBit =
		new osg::ShapeDrawable(
			new osg::Cylinder(
				osg::Vec3( 0, 0, this->arg_SEGMENT_LENGTH ),
				this->arg_SEGMENT_LENGTH / 8,
				this->arg_SEGMENT_LENGTH / 2 ),
			hints );
	osg::ref_ptr<osg::Geode> stickyOutieBit = new osg::Geode();
	stickyOutieBit->addDrawable( baseStickyOutieBit );
	baseStickyOutieBit->setColor( osg::Vec4( (float)118/255, (float)31/255, (float)31/255, 1.0f ) );

	// The outer loop stacks the collections of cyliders...
	osg::ref_ptr<osg::PositionAttitudeTransform> transformNode = new osg::PositionAttitudeTransform();
	this->fern->addChild( transformNode.get() );

	int height = 0;
	// Make sure that we are at least 5 bits high, and then after than, keep going until a random termination
	// condition (fern tree's can really be of any height and still be natural.
	while ( height < 5 || rand() % 100 < 90 )
	{
		height ++;

		transformNode->setAttitude( osg::Quat(
			10 * osg::PI / 180 * (double)(rand() % 30) / 100 + 0.85,
			osg::Vec3( 1, 0, 0 ) ) );
		transformNode->setPosition( osg::Vec3( this->arg_SEGMENT_LENGTH / 6, 0, 0 ) );
		// transformNode->addChild( this->templateBase.get() );

		// inner loop constructs the cylinder circles...
		int numOfBaseBits = 12;
		for ( int i = 0; i < numOfBaseBits; i ++ )
		{
			osg::ref_ptr<osg::PositionAttitudeTransform> localTransformNode = new osg::PositionAttitudeTransform();

			// Random adjustment to the size ( multiply by factor: 0.85 < factor < 1.15 )
			localTransformNode->setScale( osg::Vec3(
				(double)(rand() % 30) / 100 + 0.85,
				(double)(rand() % 30) / 100 + 0.85,
				(double)(rand() % 30) / 100 + 0.85) );

			localTransformNode->setAttitude( osg::Quat(
				2 * osg::PI * (float)i / numOfBaseBits,
				osg::Vec3( 1.0, 0.0, 0.0 ) ) );

			localTransformNode->addChild( stickyOutieBit );
			transformNode->addChild( localTransformNode.get() );
		}

		osg::ref_ptr<osg::PositionAttitudeTransform> newTransform = new osg::PositionAttitudeTransform();
		transformNode->addChild( newTransform.get() );
		transformNode = newTransform;
	}

	// This is a cylinder to slot down the middle of the tower we
	// just created, so that it is not possible to see through it...
	osg::ref_ptr<osg::ShapeDrawable> trunkCentre =
		new osg::ShapeDrawable(
			new osg::Cylinder(
				osg::Vec3( 0, 0, height * this->arg_SEGMENT_LENGTH / 12 ),
				this->arg_SEGMENT_LENGTH,
				height * this->arg_SEGMENT_LENGTH / 6 ),
			hints );
	osg::ref_ptr<osg::Geode> trunkGeode = new osg::Geode();
	trunkGeode->addDrawable( trunkCentre.get() );
	trunkCentre->setColor( osg::Vec4( (float)118/255, (float)31/255, (float)31/255, 1.0f ) );

	// Rotate around until it is the right angle...
	osg::ref_ptr<osg::PositionAttitudeTransform> trunkTransform = new osg::PositionAttitudeTransform();
	trunkTransform->setAttitude( osg::Quat( osg::PI / 2, osg::Vec3( 0, 1, 0 ) ) );
	trunkTransform->addChild( trunkGeode.get() );

	this->fern->addChild( trunkTransform.get() );

	// THis was how high the base became...
	return height * this->arg_SEGMENT_LENGTH / 6;
}

/**
 * Begin a recursive branch. Will iterate down in length, adding
 * more branches to the sides as it goes along.
 * If the recursive depth is already too deep, then instead of recursing
 * further down, then add green leaves.
 */
void FernGrower::branch( osg::Group* parent, int depth, float accumulatedScale )
{
	osg::ref_ptr<osg::PositionAttitudeTransform> transformNode = new osg::PositionAttitudeTransform();
	parent->addChild( transformNode );

	float scale = 1;
	for ( int i = 0; i < this->arg_NUM_OF_SEGMENTS; i ++ )
	{

		osg::ref_ptr<osg::PositionAttitudeTransform> newTransform = new osg::PositionAttitudeTransform();

		// ...move forward...
		newTransform = new osg::PositionAttitudeTransform();
		newTransform->setPosition( osg::Vec3( 0, 0, this->arg_SEGMENT_LENGTH * scale ) );

		// Scale down...
		scale *= this->arg_STEP_SCALE_FACTOR;
		newTransform->setScale( osg::Vec3( scale, scale, scale ) );

		// ... bend a little bit so it sags...
		newTransform->setAttitude( osg::Quat( -this->arg_BEND_ANGLE, osg::Vec3f( 0, 1, 0 ) ) );

		// ... but terminate before getting too small...
		// REMOVED: I found that the end of the leaf gets crappinated
		// if this approach is taken...
		// if ( accumulatedScale * scale * this->arg_SEGMENT_LENGTH < this->arg_MIN_SEGMENT_LENGTH )
		// {
		// 	std::cout << "Bailing! (" << (accumulatedScale * scale * this->arg_SEGMENT_LENGTH) << ") < " << this->arg_MIN_SEGMENT_LENGTH << std::endl;
		// 	return;
		// }

		// ...add segment...
		transformNode->addChild( this->templateSegment );

		// ...then branch to the left and to the right...
		osg::ref_ptr<osg::PositionAttitudeTransform> leftTransform = new osg::PositionAttitudeTransform();
		osg::ref_ptr<osg::PositionAttitudeTransform> rightTransform = new osg::PositionAttitudeTransform();

		// This tilts each side up, before they are then brought down by their
		// own BEND_ANGLE.
		osg::ref_ptr<osg::PositionAttitudeTransform> leftUpTransform = new osg::PositionAttitudeTransform();
		osg::ref_ptr<osg::PositionAttitudeTransform> rightUpTransform = new osg::PositionAttitudeTransform();

		leftUpTransform->setAttitude( osg::Quat( this->arg_BEND_ANGLE * 3 , osg::Vec3( 0, 1, 0 ) ) );
		rightUpTransform->setAttitude( osg::Quat( this->arg_BEND_ANGLE * 3 , osg::Vec3( 0, 1, 0 ) ) );

		// Rotate to the side, then bend up...
		leftTransform->addChild( leftUpTransform );
		rightTransform->addChild( rightUpTransform );

		leftTransform->setAttitude( osg::Quat( 60 * osg::PI / 180, osg::Vec3( 1, 0, 0 ) ) );
		rightTransform->setAttitude( osg::Quat( -60 * osg::PI / 180, osg::Vec3( 1, 0, 0 ) ) );

		// Append to the branch we are currently working on.
		newTransform->addChild( leftTransform );
		newTransform->addChild( rightTransform );

		// Recurse further down, adding another tree like structure...
		if ( depth < this->arg_MAX_DEPTH )
		{
			double majorScale = scale * 3 / 8;
			leftTransform->setScale( osg::Vec3( majorScale, majorScale, majorScale ) );
			rightTransform->setScale( osg::Vec3( majorScale, majorScale, majorScale ) );

			this->branch( leftUpTransform, depth + 1, majorScale );
			this->branch( rightUpTransform, depth + 1, majorScale );
		}
		else
		{
			// If we are yay many steps deep, then we add leaves instead of
			// recursing further with branches.
			double majorScale = scale * 3 / 4;
			leftTransform->setScale( osg::Vec3( majorScale, majorScale, majorScale ) );
			rightTransform->setScale( osg::Vec3( majorScale, majorScale, majorScale ) );

			leftUpTransform->addChild( this->templateLeaf );
			rightUpTransform->addChild( this->templateLeaf );
		}

		transformNode->addChild( newTransform );
		transformNode = newTransform.get();

	}
}
