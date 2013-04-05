/*
 * DsmReader.h
 *
 *  Created on: 23/05/2009
 *      Author: pete
 */

#ifndef DSMREADER_H_
#define DSMREADER_H_

#include "api/data/DataReader.h"
#include "api/data/DsmData.h"

class DsmReader : public DataReader<DsmData>
{
public:
	DsmReader( DataSource source ) : DataReader<DsmData>( source ) {};
	virtual ~DsmReader() {};

protected:
	virtual bool preValidate( std::string uri );
	virtual bool postValidate( std::string tempPath );
	virtual bool generateData( );

};

#endif /* DSMREADER_H_ */
