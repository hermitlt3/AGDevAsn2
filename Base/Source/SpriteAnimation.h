#include "Mesh.h"
#include <vector>
#include "Vector3.h"

struct Animation
{
	Animation() {}
	void Set(int startFrame, int endFrame, int repeat, float time, bool active)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
		this->animActive = active;
	}
	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
	bool animActive;
};

class SpriteAnimation : public Mesh
{
public:
	SpriteAnimation(const std::string &meshName, int row, int col);
	~SpriteAnimation();
	void Update(double dt);
	virtual void Render();
	int m_row;
	int m_col;
	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	Animation *m_anim;
	Vector3 pos;
	float rotationY;
	Vector3 scale;
	Vector3 playerPos;
};