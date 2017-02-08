#ifndef STATIC_BARREL_H
#define STATIC_BARREL_H
#include "Vector3.h"
class GenericEntity;

class Barrel
{
public:
	Barrel() {};
	Barrel(Vector3 pos, Vector3 scale);
	Barrel(Vector3 pos, float scale);
	~Barrel();

private:
	GenericEntity* barrel;
};

#endif