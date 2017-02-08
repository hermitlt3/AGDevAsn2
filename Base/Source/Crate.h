#ifndef STATIC_CRATE_H
#define STATIC_CRATE_H
#include "Vector3.h"
class GenericEntity;

class Crate
{
public:
	Crate() {};
	Crate(Vector3 pos, Vector3 scale);
	Crate(Vector3 pos, float scale);
	~Crate();

private:
	GenericEntity* crate;
};

#endif