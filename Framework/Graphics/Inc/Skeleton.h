#pragma once

#include "Bone.h"
#include"AnimationClip.h"

namespace SB::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>> bones;
	};

	void DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices);
	void DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices, const Math::Matrix4& worldMatrix);
	void UpdatePose(Bone* bone, std::vector<Math::Matrix4>& boneMatrices);
	void UpdateAnimPose(Bone* bone, std::vector<Math::Matrix4>& boneMatrices, Math::Matrix4& transform, const AnimationClip& clip, float time);
	Math::Vector3 MatrixToPosition(Math::Matrix4 matrix);
}


