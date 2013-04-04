/* 
 * This software is the original work of Peter Serwylo. ID: 20266057 
 * This software is submitted in partial fulfillment of the 
 * requirements for the degree of BITS (Honours), Monash University
 */ 

#ifndef BUILDINGBLOCK_H_
#define BUILDINGBLOCK_H_

#include <osg/Geometry>

/**
 * Geometry class to represent one tower segment.
 * Implemented here so that if need be, texturing and such can be implemented
 * in the one place.
 */
class BuildingBlock : public osg::Geometry
{
public:
	BuildingBlock( osg::Box* box, bool isWindow = false );
	virtual ~BuildingBlock();

private:
	// The shape to model this segment on...
	osg::Box* box;

private:
	// Not currently used, but these lazily loaded images represent two different images
	// for textures, one for a regular segment, one for a window setment.
	static osg::ref_ptr<osg::Image> wallImage;
	static osg::ref_ptr<osg::Image> windowImage;

};

#endif /* BUILDINGBLOCK_H_ */
