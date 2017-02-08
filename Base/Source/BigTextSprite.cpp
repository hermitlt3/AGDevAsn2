#include "BigTextSprite.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "EntityManager.h"
#include "MeshBuilder.h"

BigTextSprite::BigTextSprite()
{

}

BigTextSprite::~BigTextSprite()
{

}

void BigTextSprite::Update(double dt)
{

}

void BigTextSprite::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotateY, 0, 1, 0);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

namespace Create
{
	BigTextSprite* BigText(const std::string meshName,
		const Vector3 pos,
		const float rotate,
		const Vector3 scale)
	{
		BigTextSprite* bigtext = new BigTextSprite();
		bigtext->SetMesh(MeshBuilder::GetInstance()->GetMesh(meshName));
		bigtext->SetPosition(pos);
		bigtext->SetRotate(rotate);
		bigtext->SetScale(scale);
		EntityManager::GetInstance()->AddEntity(bigtext, false);
		return bigtext;
	}
}