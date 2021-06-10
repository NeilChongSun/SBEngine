#include"Precompiled.h"
#include"AnimationBuilder.h"

using namespace SB::Graphics;

AnimationBuilder & SB::Graphics::AnimationBuilder::AddPositionKey(Math::Vector3 position, float time)
{
	mPositionKeys.emplace_back(position, time);
	SetTime(time);
	return *this;
}

AnimationBuilder & SB::Graphics::AnimationBuilder::AddRotationKey(Math::Quaternion rotation, float time)
{
	mRotationKeys.emplace_back(rotation, time);
	SetTime(time);
	return *this;
}

AnimationBuilder & SB::Graphics::AnimationBuilder::AddScaleKey(Math::Vector3 scale, float time)
{
	mScaleKeys.emplace_back(scale, time);
	SetTime(time);
	return *this;
}

AnimationBuilder & SB::Graphics::AnimationBuilder::AddPositionKey(Math::Vector3 position)
{
	mPositionKeys.emplace_back(position, mTime);
	return *this;
}

AnimationBuilder & SB::Graphics::AnimationBuilder::AddRotationKey(Math::Quaternion rotation)
{
	mRotationKeys.emplace_back(rotation, mTime);
	return *this;
}

AnimationBuilder & SB::Graphics::AnimationBuilder::AddScaleKey(Math::Vector3 scale)
{
	mScaleKeys.emplace_back(scale, mTime);
	return *this;
}

AnimationBuilder & SB::Graphics::AnimationBuilder::SetTime(float time)
{
	mTime = time;
	return *this;
}

AnimationBuilder & SB::Graphics::AnimationBuilder::AddTime(float time)
{
	mTime += time;
	return *this;
}

Animation SB::Graphics::AnimationBuilder::Get()
{
	Animation animation;
	animation.mPositionKeys = std::move(mPositionKeys);
	animation.mRotationKeys = std::move(mRotationKeys);
	animation.mScaleKeys = std::move(mScaleKeys);
	return animation;
}
