#include "Barrel.h"
#include "GenericEntity.h"

Barrel::Barrel(Vector3 pos, Vector3 scale)
{
	barrel = Create::Entity("Barrel_H", pos, scale);
	barrel->InitLOD("Barrel_H", "Barrel_M", "Barrel_L");
	barrel->SetAABB(Vector3(scale.x / 3, scale.y , scale.z / 3), Vector3(- scale.x / 3, 0, - scale.z / 3));
	barrel->SetCollider(true);
}

Barrel::Barrel(Vector3 pos, float scale)
{
	barrel = Create::Entity("Barrel_H", pos, Vector3(scale, scale, scale));
	barrel->InitLOD("Barrel_H", "Barrel_M", "Barrel_L");
	barrel->SetAABB(Vector3(scale / 3, scale, scale / 3), Vector3(-scale / 3, 0, -scale / 3));
	barrel->SetCollider(true);
}

Barrel::~Barrel()
{

}