/* 
 * This software is the original work of Peter Serwylo. ID: 20266057 
 * This software is submitted in partial fulfillment of the 
 * requirements for the degree of BITS (Honours), Monash University
 */ 

#include "TowerGenerator.h"
#include "BuildingBlock.h"

#include <osg/Geode>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Image>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace buildings;

/*
 * Initialize the parameters to sane defaults, but they can also be
 * changed via their respective getters.
 */
TowerGenerator::TowerGenerator( osg::Vec3 location ) :
	location( location ),
	arg_START_WIDTH( 10 ),
	arg_MIN_WIDTH( 3 ),
	arg_TERMINATION_PROBABILITY( 0.2 ),
	arg_ROOF_PROBABILITY( 0.4 ),
	arg_SPLIT_PROBABILITY( 0.1 )
{
	this->tower = new osg::Geode();
}

void TowerGenerator::generate()
{
	// Begin with just a segment, and then recurse further from there...
	this->addSegment( NULL );
}

/*
 * Window rooms are little brick rooms protruding from the walls of the tower.
 * They also have a pointy roof.
 */
void TowerGenerator::addWindowRoom( osg::Box* parent )
{
	// Create a tower about 1/4 of the width of the parent, 1/2 the height.
	// Translate to one side of the building.

	float width = parent->getHalfLengths().x();
	float height = parent->getHalfLengths().z() * 1.5;

	float offset = parent->getHalfLengths().x();
	int xAxis = 0;
	int yAxis = 0;

	// Ranodmly choose which corner or side of the tower to place this window room...
	TowerGenerator::chooseCorner( xAxis, yAxis );

	osg::Vec3 centre = parent->getCenter();
	centre.set(
		centre.x() + offset * xAxis,
		centre.y() + offset * yAxis,
		centre.z() + height / 5 );

	osg::ref_ptr<osg::Box> windowBit = new osg::Box( centre, width, width, height );
	this->tower->addDrawable( new BuildingBlock( windowBit.get(), true ) );

	// Add Triangle to the top of the edge, same width, length of the edge,
	// and about half the height...
	this->setRoof( windowBit.get() );

}

// Construct the new shape.
// Resize it to a little bit smaller than the previous one.
// Rotate it just a smidge to get that crazy old feel.
void TowerGenerator::addSegment( osg::Box* parent )
{

	// Parameters for the new segment to be added...
	osg::Vec3 centre;
	float width;
	float height;

	// The first segment of a tower...
	if ( parent == NULL )
	{
		centre = this->location;
		width = this->arg_START_WIDTH * ( (double)( rand() % 500 ) / 1000 + 1 );
		height = width * 1.5;
	}
	// ... and any successive segment...
	else
	{
		// This decides how much to scale the old segment down to get the new segment...
		float widthRandomFactor = 1 - (float)( rand() % 400 ) / 1000; // 0.6 -> 1
		width = parent->getHalfLengths().x() * 2 * widthRandomFactor;
		height = width * 1.5;
		centre = parent->getCenter();

		// Slide the new segment left, right, back or forth a little bit so that the towers
		// end up more organic...
		float xRandomShift = ( (float)( rand() % 2000 ) / 1000 - 1 ) * ( width / 5 );
		float yRandomShift = ( (float)( rand() % 2000 ) / 1000 - 1 ) * ( width / 5 );
		centre.set( centre.x() + xRandomShift, centre.y() + yRandomShift, centre.z() + height );
	}

	// Termination condition (note that we can still terminate *before* this condition
	// is reached, but not after).
	if ( width < this->arg_MIN_WIDTH )
	{
		this->setRoof( parent );
	}
	else
	{
		osg::ref_ptr<osg::Box> child = new osg::Box( centre, width, width, height );
		osg::ref_ptr<osg::Drawable> drawable = new BuildingBlock( child.get() );// new osg::ShapeDrawable( child.get() );

		// Rotate the segment around a bit so again, the tower takes on a sort of
		// organic feel...
		float angle = (double)(rand() % 1000) / 1000 * 30 - 15;
		angle *= osg::PI / 180;

		child->setRotation( osg::Quat( angle, osg::Vec3( 0, 0, 1.0f ) ) );

		// Stack it on top of the old box.
		this->tower->addDrawable( drawable );

		if ( rand() % 100 < 50 )
		{
			this->addWindowRoom( child.get() );
		}

		// Now, either go put another box on top, or finish off with a roof.
		if ( rand() % 100 < this->arg_TERMINATION_PROBABILITY * 10 )
		{
			this->setRoof( child.get() );
		}
		else if ( rand() % 100 < this->arg_SPLIT_PROBABILITY * 10 )
		{
			this->splitSegment( child.get() );
		}
		else
		{
			this->addSegment( child );
		}
	}
}

void TowerGenerator::splitSegment( osg::Box* parent )
{

	float width = parent->getHalfLengths().x() * 1.5;
	float height = width * 1.3;

	float offset = parent->getHalfLengths().x();
	int xAxis = 0;
	int yAxis = 0;
	TowerGenerator::chooseCorner( xAxis, yAxis );

	osg::Vec3 centre = parent->getCenter();
	centre.set(
		centre.x() + offset * xAxis,
		centre.y() + offset * yAxis,
		centre.z() + height / 2 );

	osg::ref_ptr<osg::Box> child1 = new osg::Box( centre, width, width, height );

	float angle = (double)(rand() % 1000) / 1000 * 90 - 45;
	angle *= osg::PI / 180;
	child1->setRotation( osg::Quat( angle, osg::Vec3( 0, 0, 1.0f ) ) );


	centre = parent->getCenter();
	centre.set(
		centre.x() + offset * -xAxis,
		centre.y() + offset * -yAxis,
		centre.z() + height / 2 );

	osg::ref_ptr<osg::Box> child2 = new osg::Box( centre, width, width, height );

	angle = (double)(rand() % 1000) / 1000 * 90 - 45;
	angle *= osg::PI / 180;
	child2->setRotation( osg::Quat( angle, osg::Vec3( 0, 0, 1.0f ) ) );


	// Stack it on top of the old box.
	this->tower->addDrawable( new osg::ShapeDrawable( child1.get() ) );
	this->tower->addDrawable( new osg::ShapeDrawable( child2.get() ) );

	if ( rand() % 100 < 50 )
	{
		this->addWindowRoom( child1.get() );
	}

	if ( rand() % 100 < 50 )
	{
		this->addWindowRoom( child2.get() );
	}

	// Now, either go put another box on top, or finish off with a roof.
	if ( rand() % 100 > 80 )
	{
		this->setRoof( child1.get() );
	}
	else
	{
		this->addSegment( child1 );
	}

	// Now, either go put another box on top, or finish off with a roof.
	if ( rand() % 100 > 80 )
	{
		this->setRoof( child2.get() );
	}
	else
	{
		this->addSegment( child2 );
	}

}

void TowerGenerator::setRoof( osg::Box* child )
{
	if ( rand() % 100 < this->arg_ROOF_PROBABILITY * 100 )
	{
		return;
	}

	osg::Vec3 centre = child->getCenter();
	centre.set( centre.x(), centre.y(), centre.z() + child->getHalfLengths().z() );

	float radius = ( child->getHalfLengths().x() < child->getHalfLengths().y() ) ? child->getHalfLengths().x() : child->getHalfLengths().y();
	radius *= 2;
	float height = child->getHalfLengths().z() * 2;
	osg::ref_ptr<osg::Cone> cone = new osg::Cone( centre, radius, height );

	this->tower->addDrawable( new osg::ShapeDrawable( cone.get() ) );
}

void TowerGenerator::chooseCorner( int& xAxis, int& yAxis )
{
	xAxis = 1;
	yAxis = 1;

	int random = rand() % 800;
	if ( random < 100 )	{ xAxis = 1; yAxis = 0; }
	else if ( random < 200 ) { xAxis = -1; yAxis = 0; }
	else if ( random < 300 ) { xAxis = -1; yAxis = -1; }
	else if ( random < 400 ) { xAxis = -1; yAxis = 1; }
	else if ( random < 500 ) { xAxis = 1; yAxis = -1; }
	else if ( random < 600 ) { xAxis = 1; yAxis = 1; }
	else if ( random < 700 ) { yAxis = 1; yAxis = 0; }
	else { yAxis = -1; yAxis = 0; }

}

void TowerGenerator::applyTexture( osg::Drawable* drawable )
{
	return;
	osg::StateSet* state = drawable->getOrCreateStateSet();

	// Load the texture image
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile( "/home/pete/Pictures/textures/bricks.jpg" );

	// Attach the image in a Texture2D object
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setImage( image.get() );

	// Attach the texture attribute for texture unit 0.
	state->setTextureAttribute( 0, texture.get() );
}
