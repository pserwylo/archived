/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "FernLeaf.h"

#include <osg/LineWidth>
#include <osg/Geode>

using namespace plants;

FernLeaf::FernLeaf( float length, float width )
{
	// Initialise the viewing distances which will trigger different LOD's.
	// Make sure to sync up some-what with the FernSegment viewing distances...
	static const int HIGH = 35;
	static const int MEDIUM = 100;
	static const int LOW = 600;

	// Build the three LOD's.
	osg::ref_ptr<FernLeafHigh> highGeometry = new FernLeafHigh( length, width );
	osg::ref_ptr<FernLeafMedium> mediumGeometry = new FernLeafMedium( length, width );
	osg::ref_ptr<FernLeafLow> lowGeometry = new FernLeafLow( length, width );

	osg::ref_ptr<osg::Geode> high = new osg::Geode();
	osg::ref_ptr<osg::Geode> medium = new osg::Geode();
	osg::ref_ptr<osg::Geode> low = new osg::Geode();

	high->addDrawable( highGeometry.get() );
	medium->addDrawable( mediumGeometry.get() );
	low->addDrawable( lowGeometry.get() );

	this->addChild( low.get(), MEDIUM, LOW );
	this->addChild( medium.get(), HIGH, MEDIUM );
	this->addChild( high.get(), 00.0f, HIGH );
}

FernLeafHigh::FernLeafHigh( float length, float width )
{

	osg::Vec3Array* vertices = new osg::Vec3Array();
	osg::Vec3Array* normals = new osg::Vec3Array();

	/*
	 *      C     D
	 *       *---*
	 *      /| y |\
	 *     | |   | |
	 *     |  | |  |
	 *    /   | |   \
	 *    |   | |   |
	 *    |    |    |
	 *   /  x  |  z  \
	 *   | __--*--__  |
	 *   *-    A    --*
	 * B               E
	 */

	// Implement the shape shown above with a triangle fan
	// starting at A.
	osg::Vec3 a( width/6, 0, 0 );
	osg::Vec3 b( 0, -width/2, length / 15 );
	osg::Vec3 c( width/7, -width/4, length );
	osg::Vec3 d( width/7, width/4, length );
	osg::Vec3 e( 0, width/2, length / 15 );

	vertices->push_back( a );
	vertices->push_back( b );
	vertices->push_back( c );
	vertices->push_back( d );
	vertices->push_back( e );

	osg::Vec3 xNormal = (a - b) ^ (c - b);
	osg::Vec3 yNormal = (d - a) ^ (c - a);
	osg::Vec3 zNormal = (d - e) ^ (a - e);

	osg::Vec3 aNormal = (xNormal + yNormal + zNormal) / 3;
	osg::Vec3 bNormal = xNormal;
	osg::Vec3 cNormal = (xNormal + yNormal) / 2;
	osg::Vec3 dNormal = (yNormal + zNormal) / 2;
	osg::Vec3 eNormal = zNormal;

	normals->push_back( aNormal );
	normals->push_back( bNormal );
	normals->push_back( cNormal );
	normals->push_back( dNormal );
	normals->push_back( eNormal );

	osg::DrawElementsUInt* face = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_FAN, 0 );
	face->push_back( 0 );
	face->push_back( 1 );
	face->push_back( 2 );
	face->push_back( 3 );
	face->push_back( 4 );

	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>* normalIndexArray = new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>();
	normalIndexArray->push_back( 0 );
	normalIndexArray->push_back( 1 );
	normalIndexArray->push_back( 2 );
	normalIndexArray->push_back( 3 );
	normalIndexArray->push_back( 4 );

	osg::Vec4Array* colours = new osg::Vec4Array();
	colours->push_back( osg::Vec4( 0.1f, 0.7f, 0.1f, 1.0f ) );

	this->setVertexArray( vertices );
	this->setNormalArray( normals );
	this->setColorArray( colours );

	this->addPrimitiveSet( face );

	this->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE );
	this->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

	this->setNormalIndices( normalIndexArray );
}

FernLeafMedium::FernLeafMedium( float length, float width )
{

	osg::Vec3Array* vertices = new osg::Vec3Array();
	osg::Vec3Array* normals = new osg::Vec3Array();

	/*
	 *    A     B
	 *     *---*
	 *     |   |
	 *     |   |
	 *      | |
	 *      | |
	 *      | |
	 *       |
	 *       |
	 *       *
	 *       C (tip of leaf)
	 */

	osg::Vec3 a( 0, -width/2, 0 );
	osg::Vec3 b( 0, width/2, 0 );
	osg::Vec3 c( 0, 0, length );

	vertices->push_back( a );
	vertices->push_back( b );
	vertices->push_back( c );

	osg::Vec3 normal = (c - b) ^ (a - b);

	normals->push_back( normal );

	osg::DrawElementsUInt* face = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0 );
	face->push_back( 0 );
	face->push_back( 1 );
	face->push_back( 2 );

	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>* normalIndexArray = new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>();
	normalIndexArray->push_back( 0 );

	osg::Vec4Array* colours = new osg::Vec4Array();
	colours->push_back( osg::Vec4( 0.1f, 0.7f, 0.1f, 1.0f ) );

	this->setVertexArray( vertices );
	this->setNormalArray( normals );
	this->setColorArray( colours );

	this->addPrimitiveSet( face );

	this->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE );
	this->setNormalBinding( osg::Geometry::BIND_PER_PRIMITIVE );

}

FernLeafLow::FernLeafLow( float length, float width )
{
	osg::Vec3Array* vertices = new osg::Vec3Array();
	osg::Vec3Array* normals = new osg::Vec3Array();

	osg::Vec3 a( 0, 0, 0 );
	osg::Vec3 b( 0, 0, length );

	vertices->push_back( a );
	vertices->push_back( b );

	osg::Vec3 normal( 0.0f, 1.0f, 0.0f );

	normals->push_back( normal );

	osg::DrawElementsUInt* face = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0 );
	face->push_back( 0 );
	face->push_back( 1 );

	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>* normalIndexArray = new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>();
	normalIndexArray->push_back( 0 );

	osg::Vec4Array* colours = new osg::Vec4Array();
	colours->push_back( osg::Vec4( 0.1f, 0.7f, 0.1f, 1.0f ) );

	this->setVertexArray( vertices );
	this->setNormalArray( normals );
	this->setColorArray( colours );

	this->addPrimitiveSet( face );

	this->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE );
	this->setNormalBinding( osg::Geometry::BIND_PER_PRIMITIVE );

}
