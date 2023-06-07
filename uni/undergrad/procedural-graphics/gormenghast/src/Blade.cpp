/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include <iostream>
#include <stdlib.h>
#include "Blade.h"

using namespace grass;

/**
 * Choose a random colour and find out the bounding box of the blade
 * so that OSG can perform culling correctly.
 */
Blade::Blade(osg::Vec3Array* points, osg::Vec3 location, int detail) :
	points(points),
	location(location),
	widthBase(0.01),
	detail(detail)
{
	float r = (float)(rand() % 20 + 10) / 255;
	float g = (float)(rand() % 40 + 90) / 255;
	float b = (float)(rand() % 10 ) / 255;

	this->colour.set( r, g, b );

	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;
	osg::Vec3 max;
	for (osg::Vec3Array::iterator it = points->begin(); it != points->end(); it ++)
	{
		osg::Vec3 point = *it;
		float x = location.x() + point.x();
		float y = location.y() + point.y();
		float z = location.z() + point.z();
		if (it == points->begin())
		{
			minX = maxX = x;
			minY = maxY = y;
			minZ = maxZ = z;
		}
		else
		{
			if (x < minX) minX = x;
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;

			if (x > maxX) maxX = x;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
	}

	this->bounds.set(minX, minY, minZ, maxX, maxY, maxZ);

	// calculate which direction the blade is extruded to.
	// I will take an estimate as the vector perpendicular to both AB and AD.

	/*         C         D
	 *           *-----*
	 *          /       \
 	 *         /         * E
	 *        /
	 *     B *
	 *       |
	 *       |
	 *     A *
	 */

	osg::Vec3 AC = this->points->at(1) - this->points->at( this->points->size() / 2 );
	osg::Vec3 AE = *(this->points->end()) - *(this->points->begin());

	this->extrudeDirection = AC ^ AE;
	this->extrudeDirection.normalize();

}

Blade::~Blade() {}

void Blade::drawImplementation(osg::RenderInfo &renderInfo) const
{
 	switch(this->detail)
	{
	case DETAIL_LOW:
		this->drawLowQuality();
		break;

	case DETAIL_MEDIUM:
		this->drawMediumQuality();
		break;

	case DETAIL_HIGH:
	default:
		this->drawHighQuality();
		break;
	}
}

osg::BoundingSphere Blade::computeBound() const
{
	return this->bounds;
}

/**
 * Only draw three vertices of the blade.
 * Kinda like unrolling the loop from medium quality, but excluding most vertices.
 */
void Blade::drawLowQuality() const
{

	glLineWidth( 1 );
	glColor4f( this->colour.x(), this->colour.y(), this->colour.z(), 1.0f );
	glPushMatrix();
		glTranslatef( this->location.x(), this->location.y(), this->location.z() );
		glBegin( GL_LINE_STRIP );

		int pointIndex = 0;
		osg::Vec3 point = this->points->at(pointIndex);
		osg::Vec3 forward = this->points->at(pointIndex + 1) - point;
		osg::Vec3 normal = forward ^ this->extrudeDirection;
		glNormal3f( normal.x(), normal.y(), normal.z() );
		glVertex3f( point.x(), point.y(), point.z() );

		pointIndex = this->points->size() / 2;
		point = this->points->at(pointIndex);
		forward = this->points->at(pointIndex + 1) - point;
		normal = forward ^ this->extrudeDirection;
		glNormal3f( normal.x(), normal.y(), normal.z() );
		glVertex3f( point.x(), point.y(), point.z() );

		pointIndex = this->points->size() - 1;
		point = this->points->at(pointIndex);
		forward = point - this->points->at(pointIndex - 1);
		normal = forward ^ this->extrudeDirection;
		normal.normalize();
		glNormal3f( normal.x(), normal.y(), normal.z() );
		glVertex3f( point.x(), point.y(), point.z() );

		glEnd( );
	glPopMatrix();

}

/**
 * Trace a GL_LINE_STRIP through the particle trail.
 */
void Blade::drawMediumQuality() const
{
	glLineWidth( 1 );
	glColor4f( this->colour.x(), this->colour.y(), this->colour.z(), 1.0f );
	glPushMatrix();
		glTranslatef( this->location.x(), this->location.y(), this->location.z() );
		glBegin( GL_LINE_STRIP );

			for (unsigned int i = 0; i < this->points->size(); i ++)
			{
				osg::Vec3 point = this->points->at(i);

				osg::Vec3 forward = (i != this->points->size() - 1) ? (this->points->at(i + 1) - point) : point - this->points->at(i - 1);
				osg::Vec3 normal = forward ^ this->extrudeDirection;
				normal.normalize();

				glNormal3f( normal.x(), normal.y(), normal.z() );
				glVertex3f( point.x(), point.y(), point.z() );
			}

		glEnd( );
	glPopMatrix();

}

void Blade::drawHighQuality() const
{

	glLineWidth( 1 );
	glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
	glPushMatrix();
		glTranslatef( this->location.x(), this->location.y(), this->location.z() );

		/*
		 * With wider grass, it is looks quite nice to have a seam down the middle
		 * of the blade. On the thiner blades it dominates a little too much.
		glBegin( GL_LINE_STRIP );

			for (unsigned int i = 0; i < this->points->size(); i ++)
			{
				osg::Vec3 point = this->points->at(i);

				osg::Vec3 forward = (i != this->points->size() - 1) ? (this->points->at(i + 1) - point) : point - this->points->at(i - 1);
				osg::Vec3 normal = forward ^ this->extrudeDirection;

				glNormal3f( normal.x(), normal.y(), normal.z() );
				glVertex3f( point.x(), point.y(), point.z() );
			}

		glEnd( );
		*/

		glColor4f( this->colour.x(), this->colour.y(), this->colour.z(), 1.0f );
		glLineWidth( 1 );
		glBegin( GL_TRIANGLE_STRIP );

			for (unsigned int i = 0; i < this->points->size(); i ++)
			{
				osg::Vec3 point = this->points->at(i);
				double width = this->widthBase - ((double)i / this->points->size()) * this->widthBase;

				osg::Vec3 left = point + this->extrudeDirection * width / 2;
				osg::Vec3 right = point - this->extrudeDirection * width / 2;

				osg::Vec3 forward = (i != this->points->size() - 1) ? (this->points->at(i + 1) - point) : point - this->points->at(i - 1);
				osg::Vec3 normal = (forward ^ left);
				normal.normalize();

				glNormal3f( normal.x(), normal.y(), normal.z() );
				glVertex3f( left.x(), left.y(), left.z() );
				glVertex3f( right.x(), right.y(), right.z() );
			}

		glEnd( );
	glPopMatrix();

}
