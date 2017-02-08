#include "SpriteAnimation.h"
#include "GL\glew.h"
#include "Vertex.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col)
: Mesh(meshName)
, m_row(row)
, m_col(col)
, m_currentTime(0)
, m_currentFrame(0)
, m_playCount(0)
, pos(0, 0, 0)
, rotationY(0)
, scale(0, 0, 0)
, playerPos(0, 0, 0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
		delete m_anim;
}

void SpriteAnimation::Update(double dt)
{
	rotationY = Math::RadianToDegree(atan2((playerPos - pos).x, (playerPos - pos).z));

	if (m_anim->animActive == true) // If animation happens
	{
		// Get Animation current time, m_currentTime.
		m_currentTime += (float)dt;
		//Animation current time increments with dt

		// Find int numFrame.
		int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);
		// Get float frameTime.
		float frameTime = m_anim->animTime / numFrame;
		//FrameTime = animTime / numFrame
		m_currentFrame = Math::Min(m_anim->endFrame, m_anim->startFrame + (int)(m_currentTime / frameTime));
		// Get Animation current frame, m_currentFrame.
		
		// Check if m_currentTime is greater than or equal animTime
		if (m_currentTime >= m_anim->animTime)
		{
			m_anim->ended = true;
			if (m_anim->repeatCount == 0)
			{
				m_anim->animActive = false;
				m_currentTime = 0.0;
				//m_currentFrame = m_anim->startFrame;
			}
			if (m_anim->repeatCount == 1)
			{
				m_currentTime = 0.0;
				m_currentFrame = m_anim->startFrame;
			}
		}
		/*Set animation end flag, ended to true
			Check if repeatCount == 0
			Set animActive = false
			reset m_currentTime
			set m_currentFrame = startFrame
			Check if repeatCount == 1
			reset m_currentTime
			set m_currentFrame = startFrame*/
	}
}

void SpriteAnimation::Render()
{
	// Insert codes. Hint: same as in Mesh.cpp.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position)+sizeof(Color)));
	//if(textureID > 0)
	//{
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position)+sizeof(Color)+sizeof(Vector3)));
	//}


	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (mode == DRAW_LINES)
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
	else if (mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 *sizeof(GLuint)));
	else
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 *sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//if(textureID > 0)
	//{
	glDisableVertexAttribArray(3);
	//}
}