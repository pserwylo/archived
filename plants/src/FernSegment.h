/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef FERNSEGMENT_H_
#define FERNSEGMENT_H_

#include <osg/Geometry>
#include <osg/LOD>

namespace plants
{

/**
 * A fern branch is made up of several FernSegment's.
 * They are arranged like so (where each * is the end/start of a segment):
 *
 *   __--*----*----*--__
 *  *                   *--_
 *                          *
 *
 * The LOD is the same setup as the FernLeaf.
 */
class FernSegment : public osg::LOD
{
public:
	FernSegment( float length, float width );
	virtual ~FernSegment() {};
};

/**
 * High quality semgents are cylinders.
 */
class FernSegmentHigh : public osg::Geometry
{
public:
	FernSegmentHigh( float length, float width );
	virtual ~FernSegmentHigh() {};
};

// There is no medium segment, because nothing useful
// really falls between a cylinder and a line

/**
 * Low quality segments are a line.
 */
class FernSegmentLow : public osg::Geometry
{
public:
	FernSegmentLow( float length, float width ) : length( length ), width( width ) {};
	virtual ~FernSegmentLow() {};

public:
	virtual void drawImplementation( osg::RenderInfo& renderInfo ) const;
	virtual osg::BoundingBox getBound() const;

private:
	float length;
	float width;
};

};

#endif /* FERNSEGMENT_H_ */
