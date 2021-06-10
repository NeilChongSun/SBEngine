#include "Precompiled.h"
#include "Skeleton.h"

#include "SimpleDraw.h"

using namespace SB;
using namespace SB::Graphics;

void SB::Graphics::DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices)
{
	// Homework
	//Use skeleton so you know what the parent child order is
	//But, use boneMatrices(which is the muitipled out matrices) to get the position
	//Draw lines to connect the bones

	for (auto &bone : skeleton.bones)
	{
		for (auto& child : bone->children)
		{
			auto posA = MatrixToPosition(boneMatrices[bone->index]);
			auto posB = MatrixToPosition(boneMatrices[child->index]);
			SimpleDraw::AddLine(posA, posB, Colors::Red);
		}
		SimpleDraw::AddSphere(MatrixToPosition(boneMatrices[bone->index]), 0.1, Colors::Green, 8, 8);
	}
}

void SB::Graphics::DrawSkeleton(const Skeleton & skeleton, const std::vector<Math::Matrix4>& boneMatrices, const Math::Matrix4 & worldMatrix)
{
	std::vector<Math::Matrix4> boneWorldMatrices;
	boneWorldMatrices.resize(boneMatrices.size());
	for (int i = 0; i < boneMatrices.size(); i++)
	{
		boneWorldMatrices[i] = boneMatrices[i] * worldMatrix;
	}
	for (auto &bone : skeleton.bones)
	{
		for (auto& child : bone->children)
		{
			auto posA = MatrixToPosition(boneWorldMatrices[bone->index]);
			auto posB = MatrixToPosition(boneWorldMatrices[child->index]);
			SimpleDraw::AddLine(posA, posB, Colors::Red);
		}
		SimpleDraw::AddSphere(MatrixToPosition(boneWorldMatrices[bone->index]), 0.1, Colors::Green, 8, 8);
	}

}

void SB::Graphics::UpdatePose(Bone * bone, std::vector<Math::Matrix4>& boneMatrices)
{
	if (bone == nullptr)
		return;

	if (bone->parent == nullptr)
		boneMatrices[bone->index] = Math::Matrix4::Identity;
	else
		boneMatrices[bone->index] = bone->toParentTransform*boneMatrices[bone->parent->index];

	for (auto& child : bone->children)
	{
		UpdatePose(child, boneMatrices);
	}
}

void SB::Graphics::UpdateAnimPose(Bone * bone, std::vector<Math::Matrix4>& boneMatrices, Math::Matrix4& transform, const AnimationClip & clip, float time)
{
	if (bone == nullptr)
		return;

	if (bone->parent == nullptr)
		boneMatrices[bone->index] = Math::Matrix4::Identity;
	else
	{
		if (clip.GetTransform(time, bone->index, transform))
		{
			boneMatrices[bone->index] = transform * boneMatrices[bone->parent->index];
		}
		else
		{
			boneMatrices[bone->index] = bone->toParentTransform * boneMatrices[bone->parent->index];
		}
	}

	for (auto& child : bone->children)
	{
		UpdateAnimPose(child, boneMatrices, transform, clip, time);
	}
}

Math::Vector3 SB::Graphics::MatrixToPosition(Math::Matrix4 matrix)
{
	return { matrix._41,matrix._42,matrix._43 };
}
