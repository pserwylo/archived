/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef FERNLEAF_H_
#define FERNLEAF_H_

#include <osg/Geometry>
#include <osg/LOD>

namespace plants
{

/**
 * Fern leaf implementing LOD to try and tackle the big problems
 * I have with too much recursive depth in the fern, resulting
 * in tiny leaves where there probably wouldn't be any...
 */
class FernLeaf : public osg::LOD
{
public:
	FernLeaf( float length, float width );
	virtual ~FernLeaf() {};
};

/**
 * High quality fern leaves are flat leaves with a bend in the middle
 * resulting in a three dimensional, correctly lit leaf.
 */
class FernLeafHigh : public osg::Geometry
{
public:
	FernLeafHigh( float length, float width );
	virtual ~FernLeafHigh() {};
};

/**
 * Medium leaves are just a two dimensional triangle.
 */
class FernLeafMedium : public osg::Geometry
{
public:
	FernLeafMedium( float length, float width );
	virtual ~FernLeafMedium() {};
};

/**
 * Low quality leaves are simple green lines.
 */
class FernLeafLow : public osg::Geometry
{
public:
	FernLeafLow( float length, float width );
	virtual ~FernLeafLow() {};
};

};

#endif /* FERNLEAF_H_ */
