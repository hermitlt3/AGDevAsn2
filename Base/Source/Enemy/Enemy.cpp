#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CEnemy::CEnemy()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f,0.0f,0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(Vector3 pos, Vector3 tar)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(pos.x, pos.y, pos.z);
	target.Set(tar.x, tar.y, tar.z);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 1.0;

	// Initialise the LOD meshes
	InitLOD("Dummy", "Dummy_Mid", "Dummy_Low");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 8, 1), Vector3(-1, -1, -1));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(00.0f, 0.0f, 0.0f);
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 1.0;

	// Initialise the LOD meshes
	InitLOD("Dummy", "Dummy", "Dummy");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 8, 1), Vector3(-1, -1, -1));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void CEnemy::Update(double dt)
{
	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed;
	//cout << position << "..." << viewVector << endl;

	// Constrain the position
	Constrain();

	// Update the target
	/*if (position.z > 400.0f)
		target.z = position.z * -1;
	else if (position.z < -400.0f)
		target.z = position.z * -1;*/
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}