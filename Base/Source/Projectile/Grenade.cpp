#include "Grenade.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "MyMath.h"
#include "../SpatialPartition/SpatialPartition.h"
#include "../SceneGraph/SceneGraph.h"
#include "../AnimationHelper.h"
#include "../AudioManager.h"

#include <iostream>
using namespace std;

CGrenade::CGrenade(void)
	: CProjectile(NULL)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{

}

CGrenade::CGrenade(Mesh* _modelMesh)
	: CProjectile(_modelMesh)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{

}

CGrenade::~CGrenade(void)
{
	m_pTerrain = NULL; // Don't delete this as the terrain is deleted in CPlayerInfo
	modelMesh = NULL;
	theSource = NULL;
}

// Update the status of this projectile
void CGrenade::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	m_fLifetime -= (float)dt;
	if (m_fLifetime < 0.0f)
	{
		Create::SAnimation("GrenadeExplode", 0, 14, 0, 1.f, true, position + Vector3(0, 15, 0), Vector3(30,30,30), GetSource()->GetPos());
		vec3df pos(GetSource()->GetPos().x - position.x, GetSource()->GetPos().y - position.y, GetSource()->GetPos().z - position.z);
		pos = pos.normalize();
		AudioManager::GetInstance()->Sound_Engine->play3D("Music/Boom.mp3", pos, false);
		SetStatus(false);
		SetIsDone(true);	// This method informs EntityManager to remove this instance
		if (position.x >(CSpatialPartition::GetInstance()->GetxSize() >> 1) ||
			position.x < -(CSpatialPartition::GetInstance()->GetxSize() >> 1) ||
			position.z >(CSpatialPartition::GetInstance()->GetzSize() >> 1) ||
			position.z < -(CSpatialPartition::GetInstance()->GetzSize() >> 1))
		{
			return;
		}
		// Check the SpatialPartition to destroy nearby objects
		vector<EntityBase*> ExportList = CSpatialPartition::GetInstance()->GetObjects(position, 1.0f);
		for (int i = 0; i < ExportList.size(); ++i)
		{
			// Remove from Scene Graph
			if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
			{
				cout << "*** This Entity removed ***" << endl;
			}
			else
			{
				ExportList[i]->SetIsDone(true);
			}
		}
		return;
	}
	Vector3 horizontalForce;
	// W = F * m
	force.y = IN_GAME_GRAVITY * mass;

	//v = u + at
	acceleration = force * (1.f / mass);
	velocity += acceleration * dt;
	if (position.y + velocity.y * dt < m_pTerrain->GetTerrainHeight(position) - 10.0f)
	{
		velocity.y = -velocity.y;
		velocity *= 0.3f;
		vec3df pos(GetSource()->GetPos().x - position.x, GetSource()->GetPos().y - position.y, GetSource()->GetPos().z - position.z);
		pos = pos.normalize() * 5.f;
		//vec3df pos(0, 0, 0);
		if (velocity.Length() > 0.2f)
			AudioManager::GetInstance()->Sound_Engine->play3D("Music/GrenadeBounce.mp3", pos, false);
	}
	position += velocity * dt;
	force.SetZero();

	// Check if the Grenade is already on the ground
	if (position.y < m_pTerrain->GetTerrainHeight(position) - 10.0f)
	{
		position.y = m_pTerrain->GetTerrainHeight(position) - 10.0f;
		m_fSpeed = 0.0f;
		return;
	}
}

// Set the terrain for the player info
void CGrenade::SetTerrain(GroundEntity* m_pTerrain)
{
	this->m_pTerrain = m_pTerrain;
}

// Create a projectile and add it into EntityManager
CGrenade* Create::Grenade(	const std::string& _meshName,
							const Vector3& _position, 
							const Vector3& _direction, 
							const float m_fLifetime, 
							const float m_fSpeed,
							CPlayerInfo* _source)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CGrenade* result = new CGrenade(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetSource(_source);
	result->SetTerrain(_source->GetTerrain());
	result->SetType(GRENADE);
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}