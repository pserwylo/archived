/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "TerrainGeometry.h"

#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <osg/Point>
#include <iostream>
#include <stdlib.h>
#include <osg/PositionAttitudeTransform>

osg::Vec3 TerrainGeometry::getRandomPointOnTerrain()
{
	double x = (double)( rand() % 100000 ) / 100000 * ( this->size - 1 );
	double y = (double)( rand() % 100000 ) / 100000 * ( this->size - 1 );
	double z = TerrainGeometry::getHeightAt( osg::Vec2(x, y) );
	return osg::Vec3(x, y, z);
}

/*
 *        Terrain
 *   +-----------------+
 *   |                 |
 *   |                 |
 *   |                 |
 *   |         ___     |
 *   |        / | \    |
 *   |        | +-|    |
 *   |        \___/    |
 *   |                 |
 *   +-----------------+
 */
osg::Vec3 TerrainGeometry::getRandomPointOnTerrain( osg::Vec2 centre, float radius )
{
	double x = ( (double)( rand() % 2000 ) / 1000 - 1 ) * radius + centre.x();
	double y = ( (double)( rand() % 2000 ) / 1000 - 1 ) * radius + centre.y();
	double z = TerrainGeometry::getHeightAt( osg::Vec2(x, y) );

	return osg::Vec3(x, y, z);
}

/**
 * Accepts coordinates in WORLD SPACE coordinates, and also returns a height in
 * WORLD SPACE height.
 */
double TerrainGeometry::getHeightAt(osg::Vec2 xy)
{
	/*0
	 *
	 * TODO Perform proper interpolation like so:
	 *
	 * TL                     TR
	 *  +----------------+----+
	 *  |                |    |
	 *  |                |    |
	 *  |                |    |
	 *  +----------------+----+
	 *  |              A |    |
	 *  |                |    |
	 *  |                |    |
	 *  |                |    |
	 *  |                |    |
	 *  |                |    |
	 *  +----------------+----+
	 * BL                     BR
	 */

	xy.set( xy.x() / this->scaleFactor, xy.y() / this->scaleFactor );

	// Clamp to terrain size to prevent out of range errors on vector...
	if ( xy.x() > this->numOfPoints - 2 )
		xy.set( this->numOfPoints - 2, xy.y() );
	if ( xy.x() < 0 )
		xy.set( 0, xy.y() );
	if ( xy.y() > this->numOfPoints - 2 )
		xy.set( xy.x(), this->numOfPoints - 2 );
	if ( xy.y() < 0 )
		xy.set( xy.x(), 0 );

	int x = (int)xy.x();
	int y = (int)xy.y();

	double bottomLeft = this->dsm->at( y * this->numOfPoints + x );
	double bottomRight = this->dsm->at( y * this->numOfPoints + x + 1 );
	double topLeft = this->dsm->at( ( y + 1 ) * this->numOfPoints + x );
	double topRight = this->dsm->at( ( y + 1 ) * this->numOfPoints + x + 1);

	// Round down to between zero and one...
	double percentThroughX = xy.x() - (int)(xy.x());
	double percentThroughY = xy.x() - (int)(xy.x());

	double height = ( bottomLeft + bottomRight + topLeft + topRight ) / 4;

	return height * this->scaleFactor;
}


TerrainGeometry::TerrainGeometry( std::vector<double>* dsm, int size )
{
	osg::Vec3Array* vertices = new osg::Vec3Array();
	this->faceNormals = new osg::Vec3Array();
	this->vertexNormals = new osg::Vec3Array();

	this->dsm = dsm;
	this->numOfPoints = sqrt(dsm->size());

	this->size = size;
	this->scaleFactor = (double)( this->size ) / this->numOfPoints;

	this->colours = new osg::Vec4Array();
	for (float i = 0.0f; i < 0.1f; i += 0.005f)
	{
		// 0.0, 0.6, 0.1 -> 0.6, 0.1, 0
		float r = 0 + i * 6.0f;
		float g = 0.6f - i * 5.0f;
		float b = 0.1f - i;
		this->colours->push_back(osg::Vec4(r, g, b, 1.0f));
	}

	for (float i = 0.0f; i < 0.1f; i += 0.005f)
	{
		// 0.6, 0.1, 0 -> 1.0, 1.0, 1.0f
		float r = 0.6f + i * 4;
		float g = 0.1f + i * 9;
		float b = i * 10;
		this->colours->push_back(osg::Vec4(r, g, b, 1.0f));
	}

	this->colours->push_back(osg::Vec4(1, 1, 1, 1));
	this->colours->push_back(osg::Vec4(1, 1, 1, 1));
	this->colours->push_back(osg::Vec4(1, 1, 1, 1));

	for ( int y = 0; y < this->numOfPoints; y ++ )
	{
		for ( int x = 0; x < this->numOfPoints; x ++ )
		{

			double height = dsm->at( y * this->numOfPoints + x );
			if ( height > maxHeight )
			{
				maxHeight = height;
			}

			vertices->push_back( osg::Vec3( x * this->scaleFactor, y * this->scaleFactor, height * this->scaleFactor ) );
			vertexNormals->push_back( osg::Vec3( 0, 0, 1 ) );

			// Face normals exist for a space one row and column smaller than the points.
			if ( x > 0 && y > 0 )
			{
				// And there are two faces per quad.
				faceNormals->push_back( osg::Vec3( 0, 0, 1 ) );
				faceNormals->push_back( osg::Vec3( 0, 0, 1 ) );
			}
			// pointCloud->push_back(y * this->numOfPoints + x);
			// normalIndexArray->push_back(0);
			// x++;
		}
		// std::cout << std::endl;
		// y ++;
		// x = 0;
	}

	for (int y = 0; y < this->numOfPoints - 1; y ++)
	{
		for (int x = 0; x < this->numOfPoints - 1; x ++)
		{
			//Generate two vectors for the face (which create's a plane)
			//Must remember that we're using a right-handed coordinate system
			//here, in order for the outside face to be consistent on each
			//face...

			/*
			 topLeft             topRight
			 (x, y+1)__________ (x+1, y+1)
				   |          /|
				   |         / |
				   | top    /  |
				   | left  /   |
				   |      /    |
				   |     /     |
				   |    /      |
				   |   /       |
				   |  / bottom |
				   | /  right  |
				   |/_________ |
			 (x, y)             (x+n, 1)
			 bottomLeft       bottomRight
			*/


			//diagonal can be used for both tris in the quad...
			osg::Vec3 bottomLeft_topRight =
				vertices->at( ( (y + 1)*this->numOfPoints) + x + 1) - vertices->at(y*this->numOfPoints + x);

			osg::Vec3 bottomLeft_bottomRight =
				vertices->at(y*this->numOfPoints + x + 1) - vertices->at(y*this->numOfPoints + x);

			osg::Vec3 bottomLeft_topLeft =
				vertices->at( (y*this->numOfPoints) + 1 + x) - vertices->at(y*this->numOfPoints + x);
/*

			if (x == 1 && y == 1)
			{
				osg::Vec3 bl = vertices->at(y*this->numOfPoints + x);
				std::cout << "bl: " << "(" << bl.x() << ", " << bl.y() << ", " << bl.z() << ") \n";
				osg::Vec3 br = vertices->at(y*this->numOfPoints + x + 1);
				std::cout << "br: " << "(" << br.x() << ", " << br.y() << ", " << br.z() << ") \n";
				osg::Vec3 tr = vertices->at((y + 1)*this->numOfPoints + x + 1);
				std::cout << "tr: " << "(" << tr.x() << ", " << tr.y() << ", " << tr.z() << ") \n";
				osg::Vec3 tl = vertices->at((y + 1)*this->numOfPoints + x );
				std::cout << "tl: " << "(" << tl.x() << ", " << tl.y() << ", " << tl.z() << ") \n";
				std::cout << "bl->tr: " << "(" << bottomLeft_topRight.x() << ", " << bottomLeft_topRight.y() << ", " << bottomLeft_topRight.z() << ") \n";
				std::cout << "bl->br: " << "(" << bottomLeft_bottomRight.x() << ", " << bottomLeft_bottomRight.y() << ", " << bottomLeft_bottomRight.z() << ") \n";
				std::cout << "bl->br: " << "(" << bottomLeft_topLeft.x() << ", " << bottomLeft_topLeft.y() << ", " << bottomLeft_topLeft.z() << ") \n";

				osg::Vec3 normal_topLeft = bottomLeft_topRight ^ bottomLeft_topLeft;
				std::cout << "bl->tr x bl->tl: " << "(" << normal_topLeft.x() << ", " << normal_topLeft.y() << ", " << normal_topLeft.z() << ") \n";

			}
*/

			osg::Vec3 normal_topLeft = bottomLeft_topLeft ^ bottomLeft_topRight;
			normal_topLeft.normalize();
			faceNormals->at(y * this->numOfPoints + x*2).set(normal_topLeft);
			// std::cout << "(" << normal_topLeft.x() << ", " << normal_topLeft.y() << ", " << normal_topLeft.z() << ") ";

			osg::Vec3 normal_bottomRight = bottomLeft_bottomRight ^ bottomLeft_topRight;
			normal_bottomRight.normalize();
			faceNormals->at(y * this->numOfPoints + x*2+1).set(normal_bottomRight);


			/*
			 *     (1, 0) (1, 1)
			 *        +-----+
			 *        | A  /|
			 *        |   / |
			 * Side 2 |  /  |  (Side 1 is the diagonal).
			 *        | /   |
			 *        |/  B |
			 *        +-----+
			 *     (0, 0) (1, 0)
			 *         Side 3
			 */

			/*osg::Vec3 side1 = vertices->at( (y + 1) * this->numOfPoints + (x + 1) ) - vertices->at(y * this->numOfPoints + x);
			osg::Vec3 side2 = vertices->at( (y + 1) * this->numOfPoints + x ) - vertices->at(y * this->numOfPoints + x);
			osg::Vec3 side3 = vertices->at( y * this->numOfPoints + (x + 1) ) - vertices->at(y * this->numOfPoints + x);

			osg::Vec3 normalA = side1 ^ side2;
			osg::Vec3 normalB = side1 ^ side3;

			faceNormals->insert(faceNormals->begin() + y * this->numOfPoints + x, normalB);
			normalIndexArray->push_back(normalIndexArray->begin() + y * this->numOfPoints + x, normalB);

			faceNormals->insert(faceNormals->begin() + ( y + 1 ) * this->numOfPoints + x, normalA);
			normalIndexArray->push_back(normalIndexArray->begin() + (y + 1) * this->numOfPoints + x, normalA);*/
		}
		// std::cout << std::endl;
	}

	// Calculate Vertex Normals
	for (int y = 0; y < this->numOfPoints; y ++)
	{
		for (int x = 0; x < this->numOfPoints; x ++)
		{
			/*
                ___________
               |    /|    /|
               |   / | t / | tl = topLeft     (x*2-1, y)
               |  /  |  /  | t  = top         (x*2, y)
               | /   | /   | tr = topRight    (x*2+1, y)
               |/ tl |/  tr| br = bottomRight (x*2, y-1)
               |-----/-----| b  = bottom      (x*2-1, y-1)
               | bl /| br /| bl = bottomLeft  (x*2-2, y-1)
               |   / |   / |
               |  /  |  /  | The center is the vertex for which we are
               | / b | /   | looking for the normal in this iteration (x, y)
               |/____|/____|
            */

			int xBottomLeft = (x*2 - 2);
			int yBottomLeft = (y - 1);
			if (xBottomLeft < 0) xBottomLeft += this->numOfPoints - 1;
			if (yBottomLeft < 0) yBottomLeft += this->numOfPoints - 1;

			int xBottom = (x*2 - 1);
			int yBottom = (y - 1);
			if (xBottom < 0) xBottom += this->numOfPoints - 1;
			if (yBottom < 0) yBottom += this->numOfPoints - 1;

			int xBottomRight = (x*2);
			int yBottomRight = (y - 1);
			if (yBottomRight < 0) yBottomRight += this->numOfPoints - 1;

			int xTopRight = (x*2 + 1) % (this->numOfPoints - 1);
			int yTopRight = (y);

			int xTop = (x*2);
			int yTop = (y);

			int xTopLeft = (x*2 - 1);
			int yTopLeft = (y);
			if (xTopLeft < 0) xTopLeft += this->numOfPoints - 1;

			osg::Vec3 normal =
				(faceNormals->at(yTopLeft * this->numOfPoints + xTopLeft) +
				faceNormals->at(yTop * this->numOfPoints + xTop) +
				faceNormals->at(yTopRight * this->numOfPoints + xTopRight) +
				faceNormals->at(yBottomRight * this->numOfPoints + xBottomRight) +
				faceNormals->at(yBottom * this->numOfPoints + xBottom) +
				faceNormals->at(yBottomLeft * this->numOfPoints + xBottomRight)) / 6;

			normal.normalize();

			vertexNormals->at(y * this->numOfPoints + x).set(normal);
       	}
	}
}

void TerrainGeometry::drawImplementation(osg::RenderInfo &renderInfo) const
{
    for (int y = 1; y < this->numOfPoints; y ++)
    {

        glBegin(GL_TRIANGLE_STRIP);
        for (int x = 0; x < this->numOfPoints; x ++)
        {
			double height = this->dsm->at(y * this->numOfPoints + x);
			int colourIndex = (int)(height / this->maxHeight * (this->colours->size() - 1));
			if (colourIndex < 0) colourIndex = 0;
			glColor4d(this->colours->at(colourIndex).x(), this->colours->at(colourIndex).y(), this->colours->at(colourIndex).z(), 1.0f);

			osg::Vec3 normal1 = this->vertexNormals->at(y * this->numOfPoints + x);
			glNormal3d( normal1.x(), normal1.y(), normal1.z());
			glVertex3d( x * this->scaleFactor, y * this->scaleFactor, this->dsm->at( y * this->numOfPoints + x ) * this->scaleFactor );

			height = this->dsm->at((y - 1) * this->numOfPoints + x);
			colourIndex = (int)( height / this->maxHeight * ( this->colours->size() - 1 ) );
			if ( colourIndex < 0 ) colourIndex = 0;
			glColor4d( this->colours->at(colourIndex).x(), this->colours->at(colourIndex).y(), this->colours->at(colourIndex).z(), 1.0f);

			osg::Vec3 normal2 = this->vertexNormals->at((y - 1) * this->numOfPoints + x);
			glNormal3d( normal2.x(), normal2.y(), normal2.z());
			glVertex3d( x * this->scaleFactor, (y - 1) * this->scaleFactor, this->dsm->at( (y - 1) * this->numOfPoints + x) * this->scaleFactor);
        }
        glEnd();

#if 0
        glBegin(GL_LINES);
		glColor4d(1.0f, 1.0f, 0.0f, 1.0f);
        for (int x = 1; x < this->numOfPoints; x ++)
        {
			double height = this->dsm->at(y * this->numOfPoints + x);

			osg::Vec3 start(x, y, height + 0.1);
			osg::Vec3 normal = this->faceNormals->at(y * this->numOfPoints + x*2);
			osg::Vec3 end = start + normal;

			glVertex3d(start.x(), start.y(), start.z());
			glVertex3d(end.x(), end.y(), end.z());
        }
        glEnd();
#endif

    }
}

osg::BoundingBox TerrainGeometry::computeBound() const
{
	osg::BoundingBox box;
	box.set(0, 0,
		-this->maxHeight * this->scaleFactor,
		this->numOfPoints * this->scaleFactor,
		this->numOfPoints * this->scaleFactor,
		this->maxHeight * this->scaleFactor);
	return box;
}

TerrainGeometry::~TerrainGeometry()
{
	// TODO Auto-generated destructor stub
}
