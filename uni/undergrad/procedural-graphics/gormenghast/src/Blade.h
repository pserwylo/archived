/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef GRASSBLADE_H_
#define GRASSBLADE_H_

#include <osg/Geometry>

namespace grass
{

/**
 * This class performs the rendering of the particle trails.
 * Used to conceptually *and* physically separate the particle logic from
 * the rendering.
 */
class Blade : public osg::Geometry
{

public:
	// Constants to let the blade support LOD.
	static const int DETAIL_LOW = 0;
	static const int DETAIL_MEDIUM = 1;
	static const int DETAIL_HIGH = 2;

public:
	Blade(osg::Vec3Array* points, osg::Vec3 location, int detail = Blade::DETAIL_HIGH);
	virtual ~Blade();

public:
	// Switches based on <code>detail</code> and passes the rendering on
	// to the draw*Quality function.
	virtual void drawImplementation(osg::RenderInfo &info) const;
	virtual osg::BoundingSphere computeBound() const;

private:
	void drawLowQuality() const;
	void drawMediumQuality() const;
	void drawHighQuality() const;

private:
	osg::Vec3Array* points;
	osg::Vec3 location;
	double widthBase;
	osg::BoundingBox bounds;
	osg::Vec3 extrudeDirection;
	osg::Vec3 colour;
	int detail;
};

}

#endif /* GRASSBLADE_H_ */
