/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef TERRAINSAVER_H_
#define TERRAINSAVER_H_

#include <string>
#include <vector>

/**
 * Ignore this, I just use this to dump a DSM to the filesystem.
 * Actually realised once I had the terrain generator going that
 * I could use the generated data for another side project.
 */
class TerrainSaver
{
public:
	static bool save( std::vector<double>*, std::string path );
};

#endif /* TERRAINSAVER_H_ */
