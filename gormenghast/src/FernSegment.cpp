/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "FernSegment.h"
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/Geode>
#include <osg/BoundingBox>

using namespace plants;

FernSegment::FernSegment( float length, float width )
{

	static const int HIGH = 10;
	static const int LOW = 500;

	osg::TessellationHints* hints = new osg::TessellationHints();
	hints->setCreateTop( false );
	hints->setCreateBottom( false );
	hints->setDetailRatio( 0.2f );

	osg::ref_ptr<osg::ShapeDrawable> highGeometry =
		new osg::ShapeDrawable(
			/*new osg::Box(
				osg::Vec3(0, 0, length / 2),
				width, width, length),*/
			new osg::Cylinder(
				osg::Vec3(0, 0, length / 2),
				width/2, length),
			hints );
	osg::ref_ptr<FernSegmentLow> lowGeometry = new FernSegmentLow( length, width );

	osg::ref_ptr<osg::Geode> high = new osg::Geode();
	osg::ref_ptr<osg::Geode> low = new osg::Geode();

	high->addDrawable( highGeometry.get() );
	low->addDrawable( lowGeometry.get() );

	this->addChild( low.get(), HIGH, LOW );
	this->addChild( high.get(), 00.0f, HIGH );
}

void FernSegmentLow::drawImplementation( osg::RenderInfo& renderInfo ) const
{

	glLineWidth( this->width );
	glColor4f( 0.1f, 0.7f, 0.1f, 1.0f );
	glBegin( GL_LINES );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, this->length );
	glEnd( );

}

osg::BoundingBox FernSegmentLow::getBound() const
{
	return osg::BoundingBox( osg::Vec3( 0, 0, 0 ), osg::Vec3( 0, 0, this->length ) );
}
