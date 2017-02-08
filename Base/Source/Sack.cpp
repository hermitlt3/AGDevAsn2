#include "Sack.h"
#include "GenericEntity.h"

Sack::Sack(Vector3 pos, Vector3 scale)
{
	sack = Create::Entity("Sack_H", pos, scale);
	sack->InitLOD("Sack_H", "Sack_M", "Sack_L");
	sack->SetAABB(Vector3(scale.x / 3, scale.y / 1.2f, scale.z / 3.5f), Vector3(-scale.x / 3, 0, -scale.z / 3));
	sack->SetCollider(true);
}

Sack::Sack(Vector3 pos, float scale)
{
	sack = Create::Entity("Sack_H", pos, Vector3(scale, scale, scale));
	sack->InitLOD("Sack_H", "Sack_M", "Sack_L");
	sack->SetAABB(Vector3(scale / 3, scale / 1.2f, scale / 3.5f), Vector3(-scale / 3, 0, -scale / 3));
	sack->SetCollider(true);
}

Sack::~Sack()
{

}