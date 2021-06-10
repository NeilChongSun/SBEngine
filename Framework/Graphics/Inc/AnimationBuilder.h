#pragma once
#include"Animation.h"
namespace SB::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& AddPositionKey(Math::Vector3 position, float time);
		AnimationBuilder& AddRotationKey(Math::Quaternion rotation, float time);
		AnimationBuilder& AddScaleKey(Math::Vector3 scale, float time);

		AnimationBuilder& AddPositionKey(Math::Vector3 position);
		AnimationBuilder& AddRotationKey(Math::Quaternion rotation);
		AnimationBuilder& AddScaleKey(Math::Vector3 scale);

		AnimationBuilder& SetTime(float time);
		AnimationBuilder& AddTime(float time);

		Animation Get();

	private:
		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mTime = 0.0f;
		
	};
}