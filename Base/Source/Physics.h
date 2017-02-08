#pragma once
#include "Vector3.h"

class Physics
{
	friend class GenericEntity;
	friend class GroundEntity;
	friend class CProjectile;


public:
	~Physics();

	inline Vector3& GetVelocity() { return velocity; }
	inline Vector3& GetAcceleration() { return acceleration; }
	inline Vector3& GetForce() { return force; }
	inline Vector3& GetDirection() { return direction; }
	inline float& GetMass() { return mass; }

	virtual void Update(double dt) = 0;

	inline void SetVelocity(const Vector3& vel) { velocity = vel; }
	inline void SetForce(const Vector3& _force) { force = _force; }
	inline void SetAcceleration(const Vector3& acc) { acceleration = acc; };
	inline void SetDirection(const Vector3& dir) { direction = dir; };
	inline void SetMass(const float& mass) { this->mass = mass; }

protected:

	Vector3 force;
	Vector3 acceleration;
	Vector3 velocity;
	Vector3 direction;

	float mass;
	const float IN_GAME_GRAVITY = -9.8f;

private:
	Physics();
};