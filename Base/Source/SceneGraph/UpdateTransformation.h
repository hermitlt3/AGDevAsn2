#pragma once
#include "Mtx44.h"

class CUpdateTransformation
{
protected:
	int curSteps, deltaSteps, minSteps, maxSteps;
	Mtx44 Update_Mtx;

public:
	CUpdateTransformation();
	virtual ~CUpdateTransformation();

	// Reset the transformation matrix to identity matrix
	void Reset(void);
	// Update the steps
	void Update(void);

	// Apply a translation to the Update Transformation Matrix
	void ApplyUpdate(const float dx, const float dy, const float dz);
	// Apply a rotation to the Update Transformation Matrix
	void ApplyUpdate(const float angle, const float rx, const float ry, const float rz);

	// Set the minSteps and maxSteps
	void SetSteps(const int minSteps, const int maxSteps);
	// Get the minSteps and maxSteps
	void GetSteps(int& minSteps, int& maxSteps);

	// Get the Update_Mtx
	Mtx44 GetUpdateTransformation(void);
};

