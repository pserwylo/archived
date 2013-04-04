/*
 * DsmData.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef AERIALIMAGEDATA_H_
#define AERIALIMAGEDATA_H_

#include <api/data/Data.h>

#include <osg/Array>
#include <osg/BoundingBox>
#include <string>

#define PLABX_USE_OPENCV

#ifdef PLABX_USE_OPENCV
// #include <opencv...>
struct IplImage { };
#endif
#ifdef PLABX_USE_CIMG
// #include <cimg stuff...>
#endif


class AerialImageData: public Data
{
public:
	AerialImageData( std::string name );
	virtual ~AerialImageData();

// If the developer using this library chooses, they can have either of these two image
// librarie's available. (Technically, they can have both, but would result in twice as much
// memory, and also out of sync images when filters are applied to one and not the other, so
// this will be prevented...
#ifdef PLABX_USE_OPENCV

public:
	IplImage* getCvImage();

private:
	IplImage* cvImage;


// TODO: Swap to "else if def"
#endif
#ifdef PLABX_USE_CIMG

public:
	Cimg* getCimgImage();

private:
	Cimg* cimgImage;


#endif


public:
	friend class AerialImageReader;

};

#endif /* AERIALIMAGERYDATA_H_ */
