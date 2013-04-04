/*
 * Model.h
 *
 *  Created on: 25/05/2009
 *      Author: pete
 */

#ifndef MODEL_H_
#define MODEL_H_

class DsmData;

class Model
{
public:
	static Model& getInstance() { return Model::singleton; };

public:
	Model(){};
	virtual ~Model(){};

public:
	DsmData* dsm;

private:
	static Model singleton;

};

Model Model::singleton;

#endif /* MODEL_H_ */
