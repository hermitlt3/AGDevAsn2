#include "Crate.h"
#include "GenericEntity.h"

Crate::Crate(Vector3 pos, Vector3 scale)
{
	crate = Create::Entity("Crate_H", pos, scale);
	crate->InitLOD("Crate_H", "Crate_M", "Crate_L");
	crate->SetAABB(Vector3(scale.x / 3, scale.y / 1.2f, scale.z / 3), Vector3(-scale.x / 3, 0, -scale.z / 3));
	crate->SetCollider(true);
}

Crate::Crate(Vector3 pos, float scale)
{
	crate = Create::Entity("Crate_H", pos, Vector3(scale, scale, scale));
	crate->InitLOD("Crate_H", "Crate_M", "Crate_L");
	crate->SetAABB(Vector3(scale / 3, scale / 1.2f, scale / 3), Vector3(-scale / 3, 0, -scale / 3));
	crate->SetCollider(true);
}

Crate::~Crate()
{

}