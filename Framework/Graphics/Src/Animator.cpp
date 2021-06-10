#include"Precompiled.h"
#include"Animator.h"

using namespace SB;
using namespace SB::Graphics;

void Animator::Initialize(const Model & model)
{
	mModel = &model;
	mBoneMatrices.resize(mModel->skeleton.bones.size());
	ComputeBindPose();
}

void Animator::Terminate()
{
}

void Animator::ComputeBindPose()
{
	UpdatePose(mModel->skeleton.root, mBoneMatrices);
}

void Animator::PlayAnimation(int index)
{
	ASSERT(index < static_cast<int>(mModel->animationSet.clips.size()), "Animator--Invalid index.");
	mClipIndex = 0;
	mBlendDuration = 0.0f;

	BlendTo(mClipIndex, mBlendDuration);
}

void Animator::BlendTo(int index, float duration)
{
}

void Animator::Update(float deltaTime)
{
	auto& animationClip = mModel->animationSet.clips[mClipIndex];
	mTimer += deltaTime * animationClip->ticksPerSecond;
	if (mTimer>=animationClip->duration)
	{
		mTimer -= animationClip->duration;
	}

	Math::Matrix4 transform;
	UpdateAnimPose(mModel->skeleton.root, mBoneMatrices, transform, *animationClip,mTimer);
}
