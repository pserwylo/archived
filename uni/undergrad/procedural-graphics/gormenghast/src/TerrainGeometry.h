/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef TERRAINGEOMETRY_H_
#define TERRAINGEOMETRY_H_

#include <osg/Geometry>
#include <osg/RenderInfo>
#include <osg/BoundingBox>

#include <vector>

class TerrainGeometry : public osg::Geometry
{
public:
	TerrainGeometry(std::vector<double>* dsm, int size);
	virtual ~TerrainGeometry();

	osg::Vec3 getRandomPointOnTerrain();
	osg::Vec3 getRandomPointOnTerrain( osg::Vec2 centre, float radius );
	double getHeightAt(osg::Vec2 xyLocation);

	virtual void drawImplementation(osg::RenderInfo &info) const;
	virtual osg::BoundingBox computefBounds() const;

private:
	int numOfPoints;
	std::vector<double>* dsm;
	double maxHeight;
	int size;
	double scaleFactor;

	osg::Vec4Array* colours;
	osg::Vec3Array* faceNormals;
	osg::Vec3Array* vertexNormals;

};

#endif /* TERRAINGEOMETRY_H_ */
