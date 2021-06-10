#pragma once

#include "Animation.h"

namespace SB::Graphics
{
	class AnimationClip
	{
	public:
		bool GetTransform(float time, uint32_t boneIndex, Math::Matrix4& transform) const;

		std::string name;
		float duration = 0.0f;
		float ticksPerSecond = 0.0f;
		Animations boneAnimations;
	};
}