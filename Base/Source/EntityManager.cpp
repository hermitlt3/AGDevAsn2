#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile/Laser.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Update();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Update();

	// Check for Collision amongst entities with collider properties
	CheckForCollision(_dt);

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if(CSceneGraph::GetInstance()->GetNode(*it) == nullptr)
			(*it)->Render();
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Render();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition)
{
	entityList.push_back(_newEntity);

	// Add to the Spatial Partition
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);

		// Remove from SceneNode too
		if (CSceneGraph::GetInstance()->DeleteNode(_existingEntity)==false)
		{
			cout << "EntityManager::RemoveEntity: Unable to remove this entity from Scene Graph" << endl;
		}
		else
		{
			// Add to the Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;	
	}
	// Return false if not found
	return false;
}

// Mark an entity for deletion
bool EntityManager::MarkForDeletion(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}
	// Return false if not found
	return false;
}

// Set a handle to the Spatial Partition to this class
void EntityManager::SetSpatialPartition(CSpatialPartition* theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

// Constructor
EntityManager::EntityManager()
	: theSpatialPartition(NULL)
{
}

// Destructor
EntityManager::~EntityManager()
{
	// Drop the Spatial Partition instance
	CSpatialPartition::GetInstance()->DropInstance();

	// Clear out the Scene Graph
	CSceneGraph::GetInstance()->Destroy();
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);
	Vector3 thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB;
	// Get the minAABB and maxAABB for each entity
	if (CSceneGraph::GetInstance()->GetNode(ThisEntity) == nullptr)
	{
		thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
		thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	}
	else
	{
		thisMinAABB = CSceneGraph::GetInstance()->GetNode(ThisEntity)->GetNodeLocalTransform().GetTranslate() + thisCollider->GetMinAABB();
		thisMaxAABB = CSceneGraph::GetInstance()->GetNode(ThisEntity)->GetNodeLocalTransform().GetTranslate() + thisCollider->GetMaxAABB();
	}
	if (CSceneGraph::GetInstance()->GetNode(ThatEntity) == nullptr)
	{
		thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
		thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();
	}
	else
	{
		thatMinAABB = CSceneGraph::GetInstance()->GetNode(ThatEntity)->GetNodeLocalTransform().GetTranslate() + thatCollider->GetMinAABB();
		thatMaxAABB = CSceneGraph::GetInstance()->GetNode(ThatEntity)->GetNodeLocalTransform().GetTranslate() + thatCollider->GetMaxAABB();

	}
	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (CSceneGraph::GetInstance()->GetNode(ThisEntity) == nullptr &&
		CSceneGraph::GetInstance()->GetNode(ThatEntity) == nullptr)
	{
		if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
			DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
		{
			return true;
		}
	}
	else if (CSceneGraph::GetInstance()->GetNode(ThisEntity) != nullptr &&
		CSceneGraph::GetInstance()->GetNode(ThatEntity) == nullptr)
	{
		if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
			DistanceSquaredBetween(CSceneGraph::GetInstance()->GetNode(ThisEntity)->GetNodeLocalTransform().GetTranslate(), ThatEntity->GetPosition()) * 2.0)
		{
			return true;
		}
	}
	else if (CSceneGraph::GetInstance()->GetNode(ThisEntity) == nullptr &&
		CSceneGraph::GetInstance()->GetNode(ThatEntity) != nullptr)
	{
		if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
			DistanceSquaredBetween(ThisEntity->GetPosition(), CSceneGraph::GetInstance()->GetNode(ThatEntity)->GetNodeLocalTransform().GetTranslate()) * 2.0)
		{
			return true;
		}
	}
	else if (CSceneGraph::GetInstance()->GetNode(ThisEntity) != nullptr &&
		CSceneGraph::GetInstance()->GetNode(ThatEntity) != nullptr)
	{
		if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
			DistanceSquaredBetween(CSceneGraph::GetInstance()->GetNode(ThisEntity)->GetNodeLocalTransform().GetTranslate(), CSceneGraph::GetInstance()->GetNode(ThatEntity)->GetNodeLocalTransform().GetTranslate()) * 2.0)
		{
			return true;
		}
	}
	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	Vector3 thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB;
	// Get the minAABB and maxAABB for each entity
	if (CSceneGraph::GetInstance()->GetNode(ThisEntity) == nullptr)
	{
		thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
		thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	}
	else
	{
		thisMinAABB = CSceneGraph::GetInstance()->GetNode(ThisEntity)->GetNodeLocalTransform().GetTranslate() + thisCollider->GetMinAABB();
		thisMaxAABB = CSceneGraph::GetInstance()->GetNode(ThisEntity)->GetNodeLocalTransform().GetTranslate() + thisCollider->GetMaxAABB();
	}
	if (CSceneGraph::GetInstance()->GetNode(ThatEntity) == nullptr)
	{
		thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
		thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();
	}
	else
	{
		thatMinAABB = CSceneGraph::GetInstance()->GetNode(ThatEntity)->GetNodeLocalTransform().GetTranslate() + thatCollider->GetMinAABB();
		thatMaxAABB = CSceneGraph::GetInstance()->GetNode(ThatEntity)->GetNodeLocalTransform().GetTranslate() + thatCollider->GetMaxAABB();

	}
	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check where a line segment between two positions intersects a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f) 
		return false;
	if (fDst1 == fDst2) 
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

// Check two positions are within a box region
bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

// Check for intersection between a line segment and a plane
bool EntityManager::CheckLineSegmentPlane(	Vector3 line_start, Vector3 line_end, 
											Vector3 minAABB, Vector3 maxAABB,
											Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(double dt)
{
	/*for (std::list<EntityBase*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		if ((*it)->GetIsLaser())
		{
			CLaser* Laser = dynamic_cast<CLaser*>(*it);
			if (Laser->GetPosition().x > (CSpatialPartition::GetInstance()->GetxSize() >> 1) ||
				Laser->GetPosition().x < -(CSpatialPartition::GetInstance()->GetxSize() >> 1) ||
				Laser->GetPosition().z >(CSpatialPartition::GetInstance()->GetzSize() >> 1) ||
				Laser->GetPosition().z < -(CSpatialPartition::GetInstance()->GetzSize() >> 1))
				continue;
			vector<EntityBase*> temp = CSpatialPartition::GetInstance()->GetObjects(Laser->GetPosition(), 0);
			vector<EntityBase*> getLost;
			std::list<EntityBase*> myList(temp.begin(), temp.end());
			std::list<EntityBase*>::iterator it2 = myList.begin();
			while (it2 != myList.end())
			{
				if (*it == *it2)
				{
					++it2;
					continue;
				}
				if ((*it2)->HasCollider())
				{
					Vector3 thatMinAABB, thatMaxAABB;
					CCollider *it2Collider = dynamic_cast<CCollider*>(*it2);
					if (CSceneGraph::GetInstance()->GetNode(*it2) != nullptr)
					{
						CSceneNode* tempNode = CSceneGraph::GetInstance()->GetNode(*it2);
						thatMinAABB = tempNode->GetNodeLocalTransform().GetTranslate() + it2Collider->GetMinAABB();
						thatMaxAABB = tempNode->GetNodeLocalTransform().GetTranslate() + it2Collider->GetMaxAABB();
					}
					else
					{
						thatMinAABB = (*it2)->GetPosition() + it2Collider->GetMinAABB();
						thatMaxAABB = (*it2)->GetPosition() + it2Collider->GetMaxAABB();
					}
					if (CheckLineSegmentPlane(Laser->GetPosition(),
						Laser->GetPosition() - Laser->GetDirection() * Laser->GetLength(),
						thatMinAABB, thatMaxAABB,
						Vector3(0, 0, 0)) == true)
					{
						Laser->SetIsDone(true);
						if (CSceneGraph::GetInstance()->GetNode(*it2) == nullptr)
						{
							getLost.push_back(*it2);
						}
						else
						{
							CSceneGraph::GetInstance()->DeleteNode(*it2);
							getLost.push_back(*it2);
						}
					}
				}
				++it2;
			}
			for (std::vector<EntityBase*>::iterator st = getLost.begin(); st != getLost.end(); ++st)
			{
				(*st)->SetIsDone(true);
			}
			getLost.clear();
		}
	}
	*/

	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		// Check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			// Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;
				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPosition = Vector3(0, 0, 0);

					// Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMaxAABB;
					Vector3 thatMinAABB;
					if (CSceneGraph::GetInstance()->GetNode(*colliderThat) != nullptr)
					{
						CSceneNode* tempNode = CSceneGraph::GetInstance()->GetNode(*colliderThat);
						thatMinAABB = tempNode->GetNodeLocalTransform().GetTranslate() + thatCollider->GetMinAABB();
						thatMaxAABB = tempNode->GetNodeLocalTransform().GetTranslate() + thatCollider->GetMaxAABB();
					}
					else
					{
						thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
						thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();
					}

					if (CheckLineSegmentPlane(thisEntity->GetPosition(),
						thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
						thatMinAABB, thatMaxAABB,
						hitPosition) == true)
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);


						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
						{
							cout << "*** This Entity removed ***" << endl;
						}
						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
						{
							cout << "*** That Entity removed ***" << endl;
						}
					}
				}
			}
		}
	}
	/*else 
		if ((*it)->HasCollider())
		{
			vector<EntityBase*> temp;
			if (CSceneGraph::GetInstance()->GetNode(*it) == nullptr)
			{
				if ((*it)->GetPosition().x > (CSpatialPartition::GetInstance()->GetxSize() >> 1) ||
					(*it)->GetPosition().x < -(CSpatialPartition::GetInstance()->GetxSize() >> 1) ||
					(*it)->GetPosition().z >(CSpatialPartition::GetInstance()->GetzSize() >> 1) ||
					(*it)->GetPosition().z < -(CSpatialPartition::GetInstance()->GetzSize() >> 1))
					continue;
				temp = CSpatialPartition::GetInstance()->GetObjects((*it)->GetPosition(), 0);
			}
			else
			{
				if (CSceneGraph::GetInstance()->GetNode(*it)->GetNodeLocalTransform().GetTranslate().x >(CSpatialPartition::GetInstance()->GetxSize() >> 1) ||
					CSceneGraph::GetInstance()->GetNode(*it)->GetNodeLocalTransform().GetTranslate().x < -(CSpatialPartition::GetInstance()->GetxSize() >> 1) ||
					CSceneGraph::GetInstance()->GetNode(*it)->GetNodeLocalTransform().GetTranslate().z >(CSpatialPartition::GetInstance()->GetzSize() >> 1) ||
					CSceneGraph::GetInstance()->GetNode(*it)->GetNodeLocalTransform().GetTranslate().z < -(CSpatialPartition::GetInstance()->GetzSize() >> 1))
					continue;
				temp = CSpatialPartition::GetInstance()->GetObjects(CSceneGraph::GetInstance()->GetNode(*it)->GetNodeLocalTransform().GetTranslate(), 0);
			}
			std::list<EntityBase*> myList(temp.begin(), temp.end());
			for (std::list<EntityBase*>::iterator it2 = myList.begin(); it2 != myList.end();)
			{
				if ((*it2) == (*it))
				{
					++it2;
					continue;
				}
				if (it2 == myList.end())
					cout << "fucking bug" << endl;
				if ((*it2)->HasCollider())
				{
					if (CheckAABBCollision(*it, *it2))
					{
						CProjectile* p = dynamic_cast<CProjectile*>(*it);
						if (p)
						{
							if (p->GetType() != GRENADE)
							{
								if (CSceneGraph::GetInstance()->DeleteNode(*it2))
								{
									cout << "Node destroyed\n";
									it2 = myList.erase(it2);
									cout << CSpatialPartition::GetInstance()->GetObjects((*it2)->GetPosition(), 0).size() << "\t" << myList.size() << endl;
								}
								else
								{
									(*it2)->SetIsDone(true);
									it2 = myList.erase(it2);
								}
								(*it)->SetIsDone(true);
							}
						}
						else  ++it2;
					}
					else  ++it2;
				}
				else  ++it2;
			}
		}*/
	return false;
}
