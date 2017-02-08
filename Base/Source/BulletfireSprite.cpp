#include "BulletfireSprite.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "EntityManager.h"
#include "MeshBuilder.h"

BulletfireSprite::BulletfireSprite() :
ownTimer(0.166667f),
stopTimer(0.166667f),
rotationY(0.f)
{
}

BulletfireSprite::~BulletfireSprite()
{
}

void BulletfireSprite::Update(double _dt)
{
	if (!isPressed && ownTimer >= stopTimer)
	{
		ownTimer = stopTimer;
	}
	else if (isPressed && ownTimer >= stopTimer)
	{
		ownTimer = 0.0;
	}
	else
		ownTimer += _dt;
}

void BulletfireSprite::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotationY, 0, 1, 0);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (isPressed && ownTimer >= stopTimer)
		RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

BulletfireSprite* Create::BulletSprite(const std::string meshName, Vector3 pos, Vector3 scale)
{
	BulletfireSprite* fire = new BulletfireSprite();
	fire->SetMesh(MeshBuilder::GetInstance()->GetMesh(meshName));
	fire->SetPosition(pos);
	fire->SetScale(scale);
	EntityManager::GetInstance()->AddEntity(fire, false);

	return fire;
}