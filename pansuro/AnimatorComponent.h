#pragma once

#include "Core.h"

class Skeleton;
class Animation;

class AnimatorComponent
{
public:
	AnimatorComponent(Skeleton* skel);

	void Bind();
	void Update(float dt);

	float PlayAnimation(Animation* anim, float playRate = 1.0f);

private:
	void ComputeMatrixPalette();

private:
	MatrixPalette m_Palette;

	Skeleton* m_Skeleton;
	Animation* m_Animation;

	float m_AnimPlayRate;
	float m_AnimTime;

	UploadBuffer<MatrixPalette> m_UploadBuffer;
};
