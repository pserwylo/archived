/*
 * DsmReader.h
 *
 *  Created on: 23/05/2009
 *      Author: pete
 */

#ifndef AERIALIMAGEREADER_H_
#define AERIALIMAGEREADER_H_

#include "api/data/DataReader.h"
#include "api/data/AerialImageData.h"

class AerialImageReader : public DataReader<AerialImageData>
{
public:
	AerialImageReader() : DataReader<AerialImageData>() {};
	virtual ~DsmReader() {};

protected:
	virtual bool preValidate( std::string uri );
	virtual bool postValidate( std::string tempPath );
	virtual bool generateData( );

};

#endif /* AERIALIMAGEREADER_H_ */
