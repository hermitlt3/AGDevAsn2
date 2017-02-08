#pragma once
#include "EntityBase.h"
#include <string>

class Mesh;
class BulletfireSprite : public EntityBase
{
public:
	BulletfireSprite();
	virtual ~BulletfireSprite();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI() {};

	virtual bool HasCollider(void) const {
		return false;
	};
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value) {
	}
	// Set the flag, bLaser
	virtual void SetIsLaser(const bool bLaser) {
	}
	// Get the flag, bLaser
	virtual bool GetIsLaser(void) const {
		return false;
	}

	void SetMesh(Mesh* _mesh) { modelMesh = _mesh; }
	virtual Mesh* GetMesh(void) { return modelMesh; };

	double ownTimer;
	double stopTimer;
	bool isPressed;
	float rotationY;

private:
	Mesh* modelMesh;
	
};

namespace Create
{
	BulletfireSprite* BulletSprite(const std::string meshName, Vector3 pos, Vector3 scale);
}