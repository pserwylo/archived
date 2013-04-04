/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "TerrainSaver.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

bool TerrainSaver::save( std::vector<double>* dsm, std::string path )
{
	std::ofstream output;
	output.open( path.c_str(), std::ofstream::out );

	if ( !output.is_open() )
	{
		std::cerr << "Could not open file '" << path << "'" << std::endl;
		return false;
	}
	else
	{
		output << "x, y, z\n";

		int size = sqrt( dsm->size() );
		for ( unsigned int y = 0; y < size; y ++ )
		{
			for ( unsigned int x = 0; x < size; x ++ )
			{
				output << x << "," << y << "," << dsm->at( y * size + x ) << "\n";
			}
		}

		output.close();
		std::cout << "Saved terrain to file '" << path << "'" << std::endl;
		return true;
	}
}
