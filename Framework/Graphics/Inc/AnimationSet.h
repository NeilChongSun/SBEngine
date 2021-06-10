#pragma once

#include"AnimationClip.h"

namespace SB::Graphics
{
	struct AnimationSet
	{
		std::vector<std::unique_ptr<AnimationClip>> clips;
	};
}