#include "Windmill.h"
#include "GenericEntity.h"
#include "SceneGraph\SceneNode.h"
#include "SceneGraph\UpdateTransformation.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include <iostream>

Windmill::Windmill()
{
	parts[0] = Create::Entity("Windmill_botH", Vector3(-315, -10, -315), Vector3(25, 25, 25), false);
	parts[1] = Create::Entity("Windmill_topH", Vector3(0, 5, 0), Vector3(1, 1, 1), false);
	parts[2] = Create::Entity("Windmill_sailH", Vector3(2.3f, 2.f, 0.5f), Vector3(0.8f, 0.8f, 0.8f), false);

	parts[0]->InitLOD("Windmill_botH", "Windmill_botM", "Windmill_botL");
	parts[1]->InitLOD("Windmill_topH", "Windmill_topM", "Windmill_topL");
	parts[2]->InitLOD("Windmill_sailH", "Windmill_sailM", "Windmill_sailL");

	parts[0]->SetCollider(true);
	parts[1]->SetCollider(true);
	parts[2]->SetCollider(true);
	
	parts[0]->SetAABB(Vector3(60, 45, 60), Vector3(-60, 0, -60));
	parts[1]->SetAABB(Vector3(40, 85, 50), Vector3(-40, -65, -45));
	parts[2]->SetAABB(Vector3(15, 70, 70), Vector3(0, -70,-70));
	
	CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(parts[0]);
	CSceneNode* childNode = baseNode->AddChild(parts[1]);
	CSceneNode* grandchildNode = childNode->AddChild(parts[2]);

	CSpatialPartition::GetInstance()->Add(parts[0]);
	CSpatialPartition::GetInstance()->Add(parts[1]);
	CSpatialPartition::GetInstance()->Add(parts[2]);

	cout << "For Debug Purposes: Windmill LOD is loaded. " << endl;
	cout << "Windmill position " << CSceneGraph::GetInstance()->GetNode(parts[2])->GetNodeLocalTransform().GetTranslate() << endl;
}

Windmill::~Windmill()
{

}

void Windmill::Update(double dt)
{
	if (CSceneGraph::GetInstance()->GetNode(parts[2])) {
	CSceneGraph::GetInstance()->GetNode(parts[2])->ApplyRotate(30.f * dt, 1, 0, 0);
	}
}
