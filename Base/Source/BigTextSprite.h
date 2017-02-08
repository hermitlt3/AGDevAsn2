#pragma once

#include "EntityBase.h"
#include "Vector3.h"

class BigTextSprite : public EntityBase
{
public:
	BigTextSprite();
	~BigTextSprite();

	virtual void Update(double dt);
	virtual void Render();
	virtual void RenderUI(){};

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

	void SetRotate(float rotate) {
		rotateY = rotate;
	}

	float GetRotate() { return rotateY; }

private:
	float rotateY;
	Mesh* modelMesh;
};

namespace Create
{
	BigTextSprite* BigText(	const std::string meshName, 
							const Vector3 pos, 
							const float rotate, 
							const Vector3 scale);
}