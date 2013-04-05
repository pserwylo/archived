/*
 * DataReader.h
 *
 *  Created on: 23/05/2009
 *      Author: pete
 */

#ifndef DATAREADER_H_
#define DATAREADER_H_

#include <string>
#include <iostream>

#include <api/data/DataSource.h>

#include <api/UiFactory.h>
#include <api/UiDialogs.h>


template <class T>
class DataReader
{
protected:
	DataReader( DataSource source ) : source( source ) { };
public:
	virtual ~DataReader() {};

public:
	void read();
	inline T* getData() { return this->data; };

protected:
	virtual bool preValidate( std::string uri ) = 0;
	virtual bool postValidate( std::string tempPath ) = 0;
	virtual bool generateData( ) = 0;

private:
	// std::string saveDataToTempDir();

protected:
	DataSource source;

	std::string filename;
	T* data;
	std::string validationError;
};

// TODO: Moved here because of some strange linking issue where
// the .cpp file doesn't seem to be getting compiled.
template <class T>
void DataReader<T>::read()
{

	// Ask the user for the location of the file we want to read.
	// Make sure that we tell them specifically what we want them to choose.
	// gui::openFileDialog( getDescription() );
	std::string uri = UiFactory::getDialogs()->getUri( );
	std::cout << "URI: " << uri << std::endl;

	// The specific subclass of DataReader decides that the user has not supplied what we wanted,
	// so don't bother making the data available for manipulation, because it will just be a
	// waste of resources.
	if ( !this->preValidate( uri ) )
	{
		UiFactory::getDialogs()->displayMessage( this->validationError, "Error", UiDialogs::ERROR );
		return;
	}

	// Make the data available locally, so that we can inspect/manipulate it.
	// std::string tmpFile = tmpnam( NULL );
	// data::getData( uri, tmpFile );

	// The specific subclass of DataReader decides that the user has not supplied what we wanted.
	// if ( !this->postValidate( tmpFile ) )
	// {
	// 	UiFactory::getDialogs()-displayMessage( validationError, "Error", UiDialogs::ERROR );
	// 	return;
	// }

	// Alrighty, we are happy that the data is correct, so we will ask the subclass to turn it into
	// an object the developer can now access.
	if ( !this->generateData() )
	{
		UiFactory::getDialogs()->displayMessage( this->validationError, "Error", UiDialogs::ERROR );
	}
};

#endif /* DATAREADER_H_ */
