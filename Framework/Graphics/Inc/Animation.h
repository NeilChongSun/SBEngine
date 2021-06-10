#pragma once
#include"Keyframe.h"
namespace SB::Graphics
{
	class Animation
	{
	public:
		Math::Vector3 GetPosition(float time)const;
		Math::Quaternion GetRotation(float time)const;
		Math::Vector3 GetScale(float time) const;
		//std::tuple<Math::Vector3, Math::Quaternion, Math::Vector3> GetTransform(float time)const;
		Math::Matrix4 GetTransform(float time)const;
		void SetLooping(bool looping) { mLooping = looping; }
		bool IsLooping()const { return mLooping; }
		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
	private:
		bool mLooping = false;
	};

	using Animations = std::vector<std::unique_ptr<Animation>>;
}