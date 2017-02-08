#ifndef STATIC_SACK_H
#define STATIC_SACK_H
#include "Vector3.h"
class GenericEntity;

class Sack
{
public:
	Sack() {};
	Sack(Vector3 pos, Vector3 scale);
	Sack(Vector3 pos, float scale);
	~Sack();

private:
	GenericEntity* sack;
};

#endif