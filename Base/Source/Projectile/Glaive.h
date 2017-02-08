#pragma once
#include "Projectile.h"
#include "EntityBase.h"
#include "Vector3.h"
#include "Collider/Collider.h"
#include "../GroundEntity.h"

class Mesh;
class CPlayerInfo;

class CGlaive : public CProjectile
{
protected:
	
public:
	CGlaive();
	CGlaive(Mesh* _modelmesh);
	~CGlaive();

	void SetLength(const float m_fLength);
	float GetLength(void) const;
	void CalculateAngles(void);
	
	void Update(double dt = 0.0333f);

	void Render(void);

private:
	float m_fLength;
	float angle_x, angle_y, angle_z;

};

namespace Create
{
	CGlaive* Glaive(const std::string& _meshName, const Vector3& _position, const Vector3& _direction, const float m_fLength, 
		const float m_fLifetime, const float m_fSpeed, CPlayerInfo* _source = NULL);
}