#pragma once

#include "Vector3.h"

class Mesh;
class GroundEntity;
class GenericEntity;
class CSteve
{
public:
	CSteve(void);
	virtual ~CSteve();

	void Init(Vector3 pos);

	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	bool GetIsDead() { return isDead; }

	void SetTarget(Vector3 target) { this->target = target; }
	
	void SetWayPoints(Vector3 a, Vector3 b, Vector3 c, Vector3 d) {
		wayPoints[0] = a;
		wayPoints[1] = b;
		wayPoints[2] = c;
		wayPoints[3] = d;
	}
	void Kill();

private:
	GroundEntity* m_pTerrain;
	GenericEntity* bodyParts[6];

	Vector3 wayPoints[4];
	Vector3 target;

	double m_dSpeed;
	bool isDead;
	int waypointIndex;
	int currIndex;
};
