/* 
 * This software is the original work of Peter Serwylo. ID: 20266057 
 * This software is submitted in partial fulfillment of the 
 * requirements for the degree of BITS (Honours), Monash University
 */ 

#include "BuildingBlock.h"

#include <osgDB/ReadFile>
#include <osg/Texture2D>

#include <iostream>

osg::ref_ptr<osg::Image> BuildingBlock::wallImage = NULL;
osg::ref_ptr<osg::Image> BuildingBlock::windowImage = NULL;

BuildingBlock::BuildingBlock( osg::Box* box, bool isWindow ) :
	box( box )
{

	osg::Vec3 centre = box->getCenter();
	osg::Vec3 size = box->getHalfLengths();

	// (b = back | f = front)(b = bottom | t = top)(l = left | r = right)
	osg::Vec3 bbl( centre.x() - size.x(), centre.y() - size.y(), centre.z() - size.z() );
	osg::Vec3 bbr( centre.x() + size.x(), centre.y() - size.y(), centre.z() - size.z() );
	osg::Vec3 btl( centre.x() - size.x(), centre.y() + size.y(), centre.z() - size.z() );
	osg::Vec3 btr( centre.x() + size.x(), centre.y() + size.y(), centre.z() - size.z() );
	osg::Vec3 fbl( centre.x() - size.x(), centre.y() - size.y(), centre.z() + size.z() );
	osg::Vec3 fbr( centre.x() + size.x(), centre.y() - size.y(), centre.z() + size.z() );
	osg::Vec3 ftl( centre.x() - size.x(), centre.y() + size.y(), centre.z() + size.z() );
	osg::Vec3 ftr( centre.x() + size.x(), centre.y() + size.y(), centre.z() + size.z() );

	osg::Vec3Array* vertices = new osg::Vec3Array();

	vertices->push_back( bbl ); // 0
	vertices->push_back( bbr ); // 1
	vertices->push_back( btl ); // 2
	vertices->push_back( btr ); // 3
	vertices->push_back( fbl ); // 4
	vertices->push_back( fbr ); // 5
	vertices->push_back( ftl ); // 6
	vertices->push_back( ftr ); // 7

	osg::DrawElementsUInt* faceIndices =
		new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0 );

	/*
	Texture coordinates...
	osg::UByteArray* textureIndices = new osg::UByteArray();

	int bl = 0;
	int br = 1;
	int tr = 2;
	int tl = 2;
	*/

	// left
	faceIndices->push_back( 0 );
	faceIndices->push_back( 2 );
	faceIndices->push_back( 6 );
	faceIndices->push_back( 4 );
	// textureIndices->push_back( bl );
	// textureIndices->push_back( tl );
	// textureIndices->push_back( tr );
	// textureIndices->push_back( br );

	// right
	faceIndices->push_back( 1 );
	faceIndices->push_back( 3 );
	faceIndices->push_back( 7 );
	faceIndices->push_back( 5 );
	// textureIndices->push_back( br );
	// textureIndices->push_back( tr );
	// textureIndices->push_back( tl );
	// textureIndices->push_back( bl );

	// top
	faceIndices->push_back( 2 );
	faceIndices->push_back( 3 );
	faceIndices->push_back( 7 );
	faceIndices->push_back( 6 );
	// textureIndices->push_back( tl );
	// textureIndices->push_back( tr );
	// textureIndices->push_back( bl );
	// textureIndices->push_back( br );

	// bottom
	faceIndices->push_back( 0 );
	faceIndices->push_back( 1 );
	faceIndices->push_back( 5 );
	faceIndices->push_back( 4 );

	// front
	faceIndices->push_back( 4 );
	faceIndices->push_back( 5 );
	faceIndices->push_back( 7 );
	faceIndices->push_back( 6 );

	// back
	faceIndices->push_back( 0 );
	faceIndices->push_back( 1 );
	faceIndices->push_back( 3 );
	faceIndices->push_back( 2 );

	osg::Vec3Array* normals = new osg::Vec3Array();

	osg::Vec3 leftNormal( -1, 0, 0 );
	osg::Vec3 rightNormal( 1, 0, 0 );
	osg::Vec3 topNormal( 0, 1, 0 );
	osg::Vec3 bottomNormal( 0, -1, 0 );
	osg::Vec3 frontNormal( 0, 0, 1 );
	osg::Vec3 backNormal( 0, 0, -1 );

	normals->push_back( leftNormal );
	normals->push_back( rightNormal );
	normals->push_back( topNormal );
	normals->push_back( bottomNormal );
	normals->push_back( frontNormal );
	normals->push_back( backNormal );

	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>* normalIndexArray =
		new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>();
	normalIndexArray->push_back( 0 );
	normalIndexArray->push_back( 1 );
	normalIndexArray->push_back( 2 );
	normalIndexArray->push_back( 3 );
	normalIndexArray->push_back( 4 );
	normalIndexArray->push_back( 4 );

	osg::Vec4Array* colourArray = new osg::Vec4Array();
	for ( float i = 0.5; i < 0.8; i += 0.05 )
	{
		colourArray->push_back( osg::Vec4( i, i, i, 1.0f ) );
	}

	osg::UByteArray* colourIndices = new osg::UByteArray();
	colourIndices->push_back( rand() % ( colourArray->size() - 1 ) );

	this->setVertexArray( vertices );
	this->setNormalArray( normals );
	this->addPrimitiveSet( faceIndices );
	this->setNormalBinding( osg::Geometry::BIND_PER_PRIMITIVE );
	this->setNormalIndices( normalIndexArray );
	this->setColorArray( colourArray );
	this->setColorBinding( osg::Geometry::BIND_OVERALL );
	this->setColorIndices( colourIndices );

	/*
	if ( BuildingBlock::wallImage == NULL )
	{
		BuildingBlock::wallImage = osgDB::readImageFile( "wallTexture.jpg" );
		if ( !BuildingBlock::wallImage )
		{
			std::cerr << "Error: Could not load texture 'wallTexture.jpg'" << std::endl;
		}
	}
	*/
	/*
	if ( BuildingBlock::wallImage )
	{
		osg::Texture2D* wallTexture = new osg::Texture2D();
		wallTexture->setDataVariance(osg::Object::DYNAMIC);
		wallTexture->setImage( BuildingBlock::wallImage );

		this->getOrCreateStateSet()->setTextureAttributeAndModes(
			0,
			wallTexture,
			osg::StateAttribute::ON);

		osg::Vec2Array* textureCoords = new osg::Vec2Array( );

		textureCoords->push_back( osg::Vec2( 0, 0 ) );
		textureCoords->push_back( osg::Vec2( 1, 0 ) );
		textureCoords->push_back( osg::Vec2( 1, 1 ) );
		textureCoords->push_back( osg::Vec2( 0, 1 ) );

		this->setTexCoordArray( 0, textureCoords );
		//this->setTexCoordIndices( 0, textureIndices );
	}
	*/
}

BuildingBlock::~BuildingBlock() {}
