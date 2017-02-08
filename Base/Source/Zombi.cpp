#include "Zombi.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GenericEntity.h"
#include "GroundEntity.h"
#include "SceneGraph/SceneNode.h"
#include "SceneGraph/SceneGraph.h"
#include "AudioManager.h"

CZombie::CZombie() :
m_pTerrain(NULL),
legless(false),
isDead(false),
m_dSpeed(5.0),
waypointIndex(1),
currIndex(0),
target(Vector3(0, -1000, 0))
{
}

CZombie::~CZombie()
{
}

void CZombie::Init(Vector3 pos)
{
	bodyParts[0] = Create::Entity("ZHead", Vector3(0, 2.5f, 0), Vector3(1.f, 1.f, 1.f), false);
	bodyParts[1] = Create::Entity("ZHand", Vector3(1, 1, -1.6f), Vector3(1.f, 1.f, 1.f), false);
	bodyParts[2] = Create::Entity("ZHand", Vector3(1, 1, 1.6f), Vector3(1.f, 1.f, 1.f), false);
	bodyParts[3] = Create::Entity("ZTorso", Vector3(pos.x, pos.y, pos.z), Vector3(1.5f, 1.5f, 1.5f), false);
	bodyParts[4] = Create::Entity("ZLeg", Vector3(0, -3, -0.5f), Vector3(1.f, 1.f, 1.f), false);
	bodyParts[5] = Create::Entity("ZLeg", Vector3(0, -3, 0.5f), Vector3(1.f, 1.f, 1.f), false);

	for (int i = 0; i < 6; ++i) {
		bodyParts[i]->SetCollider(true);
	}

	bodyParts[0]->SetAABB(Vector3(1.5f, 1.5f, 1.5f), Vector3(-1.5f, -1.5f, -1.5f));
	bodyParts[1]->SetAABB(Vector3(1.5f, 1.f, 1.f), Vector3(-1.5f, -1.f, -1.f));
	bodyParts[2]->SetAABB(Vector3(1.5f, 1.f, 1.f), Vector3(-1.5f, -1.f, -1.f));
	bodyParts[3]->SetAABB(Vector3(1.f, 2.f, 1.6f), Vector3(-1.f, -2.f, -1.6f));
	bodyParts[4]->SetAABB(Vector3(1.f, 2.f, 1.f), Vector3(-1.f, -2.f, -1.f));
	bodyParts[5]->SetAABB(Vector3(1.f, 2.f, 0.8f), Vector3(-1.f, -2.f, -0.8f));

	CSceneNode* bodyNode = CSceneGraph::GetInstance()->AddNode(bodyParts[3]);
	CSceneNode* headNode = bodyNode->AddChild(bodyParts[0]);
	CSceneNode* lhandNode = bodyNode->AddChild(bodyParts[1]);
	lhandNode->ApplyRotate(90.f, 0, 1, 0);
	CSceneNode* rhandNode = bodyNode->AddChild(bodyParts[2]);
	rhandNode->ApplyRotate(90.f, 0, 1, 0);
	CSceneNode* llegNode = bodyNode->AddChild(bodyParts[4]);
	CSceneNode* rlegNode = bodyNode->AddChild(bodyParts[5]);

	for (int i = 0; i < 6; ++i) {
		CSpatialPartition::GetInstance()->Add(bodyParts[i]);
	}
}

// Set the terrain for the player info
void CZombie::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;
	}
}

// Get the terrain for the player info
GroundEntity* CZombie::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void CZombie::Update(double dt)
{
	Vector3 p;
	if (!CSceneGraph::GetInstance()->GetNode(bodyParts[4]) &&
		!CSceneGraph::GetInstance()->GetNode(bodyParts[5]))
	{
		if (legless == false)
		{
			if (CSceneGraph::GetInstance()->GetNode(bodyParts[3]))
				CSceneGraph::GetInstance()->GetNode(bodyParts[3])->ApplyTranslate(0, -3, 0);
			legless = true;
		}
	}
	

	if (CSceneGraph::GetInstance()->GetNode(bodyParts[3])) {
		p = CSceneGraph::GetInstance()->GetNode(bodyParts[3])->GetNodeLocalTransform().GetTranslate();
		if (target.y != -1000.f) {
			Vector3 movingtowards;
			movingtowards = (target - CSceneGraph::GetInstance()->GetNode(bodyParts[3])->GetNodeLocalTransform().GetTranslate()).Normalized();

			if (CSceneGraph::GetInstance()->GetNode(bodyParts[3]))
				CSceneGraph::GetInstance()->GetNode(bodyParts[3])->ApplyTranslate(dt * movingtowards.x * m_dSpeed, 0, 0);
		}
		else
		{
			if (CSceneGraph::GetInstance()->GetNode(bodyParts[3])->GetNodeLocalTransform().GetTranslate().x <
				wayPoints[waypointIndex].x + 1.f &&
				CSceneGraph::GetInstance()->GetNode(bodyParts[3])->GetNodeLocalTransform().GetTranslate().x >
				wayPoints[waypointIndex].x - 1.f &&
				CSceneGraph::GetInstance()->GetNode(bodyParts[3])->GetNodeLocalTransform().GetTranslate().z <
				wayPoints[waypointIndex].z + 1.f &&
				CSceneGraph::GetInstance()->GetNode(bodyParts[3])->GetNodeLocalTransform().GetTranslate().z >
				wayPoints[waypointIndex].z - 1.f)
			{
				if (waypointIndex == 3)
					waypointIndex = 0;
				else
					waypointIndex++;

				if (currIndex == 3)
					currIndex = 0;
				else
					currIndex++;
			}
			Vector3 movingtowards;
			movingtowards = (wayPoints[waypointIndex] - wayPoints[currIndex]).Normalized();

			if (CSceneGraph::GetInstance()->GetNode(bodyParts[3]))
				CSceneGraph::GetInstance()->GetNode(bodyParts[3])->ApplyTranslate(dt * movingtowards.x * m_dSpeed, 0, dt * movingtowards.z * m_dSpeed);
		}
	}
	else
	{
		if (!isDead)
		{
			vec3df pa(p.x, p.y, p.z);
			pa = pa.normalize() * 3.f;
			AudioManager::GetInstance()->Sound_Engine->play3D("Music/ZombieDie.mp3", pa, false);
		}
		isDead = true;
	}
}

void CZombie::Kill()
{
	 CSceneGraph::GetInstance()->DeleteNode(bodyParts[3]);
}