/*
 * This software is the original work of Peter Serwylo. ID: 20266057
 * This software is submitted in partial fulfillment of the
 * requirements for the degree of BITS (Honours), Monash University
 */

#include "TerrainGenerator.h"
#include "assert.h"

#include <stdlib.h>
#include <time.h>

#include <osg/Vec3>

TerrainGenerator::TerrainGenerator() :
	isGenerated(false)
{
	srand(time(NULL));
}

TerrainGenerator::~TerrainGenerator()
{}


void TerrainGenerator::setSize(int n)
{
	this->size = n;
	this->realSize = n + 1;
	this->maxHeight = n / 2;
}

void TerrainGenerator::setSmoothness(float smoothness)
{
	this->randomness = 1.0f - smoothness;
}

std::vector<double>* TerrainGenerator::getDsm()
{
	assert(this->isGenerated);
	return this->dsm;
}

void TerrainGenerator::generate()
{
	// Initialise the empty terrain...
	// Note that the size of the height-map is actually one more than the size
	// supplied by *this->size*.
	// This is because there are *this->size* QUADS, and *this->size + 1* VERTICES.

	this->dsm = new std::vector<double>();
	for (int i = 0; i <= this->size; i ++)
	{
		// Fill each row with zero height...
		for (int j = 0; j <= this->size; j ++)
		{
			this->dsm->push_back(0);
		}
	}
	this->midpointStep(this->size + 1, this->maxHeight, FORCE_UP);
	this->isGenerated = true;
}

void TerrainGenerator::midpointStep(int steps, int displacement, int force)
{
	//mid point = average of corners...
	for (unsigned int i = 0; i < this->realSize / steps; i++)
	{
		int y = (steps / 2 + i * steps) % this->realSize;

		for (unsigned int j = 0; j < this->realSize / steps; j++)
		{
			int x = (steps/2 + j*steps) % this->realSize;

			int xTopLeft = (x - steps/2) % this->realSize;
			int yTopLeft = (y - steps/2) % this->realSize;

			int xTopRight = (x + steps/2) % this->realSize;
			int yTopRight = (y - steps/2) % this->realSize;

			int xBottomRight = (x + steps/2) % this->realSize;
			int yBottomRight = (y + steps/2) % this->realSize;

			int xBottomLeft = (x - steps/2) % this->realSize;
			int yBottomLeft = (y + steps/2) % this->realSize;

			double randomAmount = rand()%(displacement*2) - displacement;

			//Means it is the first step, and therefore we can reasonably guess that
			//we want it to be a certain shape... Here we force it up a certain amount...
			if (force == TerrainGenerator::FORCE_UP)
			{
				randomAmount = rand() % (this->maxHeight/2) + this->maxHeight/2;
			}
			else if (force == TerrainGenerator::FORCE_DOWN)
			{
				randomAmount = - abs(randomAmount);
			}

			//Now calculate the average of all midpoints and add the random number, to
			//get the height value for this point.
			(*(this->dsm))[y * this->realSize + x] =
				(this->dsm->at(yTopLeft * this->realSize + xTopLeft) +
				 this->dsm->at(yTopRight * this->realSize + xTopRight) +
				 this->dsm->at(yBottomRight * this->realSize + xBottomRight) +
				 this->dsm->at(yBottomLeft * this->realSize + xBottomLeft))/4 + randomAmount;
		}
	}

	//Once all the midpoints have been calculated, it is legal to go and then
	//calculate the diamond steps for each new point just added.
	for (unsigned int i = 0; i <= this->realSize / steps; i++)
	{
		int y = (steps / 2 + i * steps) % this->realSize;
		for (unsigned int j = 0; j <= this->realSize / steps; j++)
		{
			int x = (steps / 2 + j * steps) % this->realSize;
			this->preDiamondStep(x, y, steps);
		}
	}

	if (steps >= 4)
	{
		displacement *= pow(2, -this->randomness);
		if (displacement <= 0)
		{
			displacement = 1;
		}
		this->midpointStep(steps / 2, displacement);
	}
}

void TerrainGenerator::preDiamondStep(int x, int y, int steps)
{
	int xTop = x;
	int yTop = (y + steps / 2) % (this->realSize);
	this->diamondStep(xTop, yTop, steps);

	int xRight = (x + steps / 2) % (this->realSize);
	int yRight = y;
	this->diamondStep(xRight, yRight, steps);

	int xBottom = x;
	int yBottom = (y - steps / 2) % this->realSize;
	if (yBottom < 0) yBottom += this->realSize-1;
	this->diamondStep(xBottom, yBottom, steps);

	int xLeft = (x - steps / 2) % this->realSize;
	int yLeft = y;
	if (xLeft < 0) xLeft += this->realSize-1;
	this->diamondStep(xLeft, yLeft, steps);

	//std::cout << "D(" << x << ", " << y << ") = (" << xTop << ", " << yTop << ") "
	//  "(" << xRight << ", " << yRight << ") (" << xBottom << ", " << yBottom << ") "
	//  "(" << xLeft << ", " << yLeft << ")\n" << std::endl;
}

void TerrainGenerator::diamondStep(unsigned int x, unsigned int y, int steps)
{
	if (x > 0 && y > 0 && x < (this->realSize - 1) && (y < this->realSize - 1))
	{
		int xTop = x;
		int yTop = (y + steps/2) % (this->realSize-1);

		int xRight = (x + steps/2) % (this->realSize-1);
		int yRight = y;

		int xBottom = x;
		int yBottom = (y - steps/2);
		if (yBottom < 0) yBottom += this->realSize-1;

		int xLeft = (x - steps/2) % (this->realSize-1);
		int yLeft = y;
		if (xLeft < 0) xLeft += this->realSize-1;

		//std::cout << "A(" << x << ", " << y << ") = (" << xTop << ", " << yTop << ") "
		//  "(" << xRight << ", " << yRight << ") (" << xBottom << ", " << yBottom << ") "
		//  "(" << xLeft << ", " << yLeft << ")" << std::endl;

		(*(this->dsm))[y * this->realSize + x] =
			(this->dsm->at(yTop * this->realSize + xTop) +
			this->dsm->at(yRight * this->realSize + xRight) +
			this->dsm->at(yBottom * this->realSize + xBottom) +
			this->dsm->at(yLeft * this->realSize + xLeft)) / 4;
	}
}
