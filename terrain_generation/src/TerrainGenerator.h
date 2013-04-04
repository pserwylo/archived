/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#ifndef TERRAINGENERATOR_H_
#define TERRAINGENERATOR_H_


#include <vector>

class TerrainGenerator
{
public:
	TerrainGenerator();
	virtual ~TerrainGenerator();

	// Size *must* be 2^something
	void setSize(int n);
	void setSmoothness(float smoothness);

	void generate();

	std::vector<double>* getDsm();

public:
	enum
	{
		NO_FORCE,
		FORCE_UP,
		FORCE_DOWN
	};


private:
	// Calculated based on the width of the terrain...
	int maxHeight;

	void diamondStep(unsigned int x, unsigned int y, int steps);
	void preDiamondStep(int x, int y, int steps);
	void midpointStep(int steps, int displacement, int force = TerrainGenerator::NO_FORCE);

private:
	std::vector<double>* dsm;
	int size;
	unsigned int realSize;
	float randomness;
	bool isGenerated;
};

#endif /* TERRAINGENERATOR_H_ */
