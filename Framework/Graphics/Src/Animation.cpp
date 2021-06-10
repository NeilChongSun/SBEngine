#include"Precompiled.h"
#include"Animation.h"
using namespace SB;
using namespace SB::Graphics;

Math::Vector3 SB::Graphics::Animation::GetPosition(float time) const
{
	float t;
	if (time < mPositionKeys.back().time)
	{

		for (int i = 0; i < mPositionKeys.size(); i++)
		{
			if (time >= mPositionKeys[i].time&&time<mPositionKeys[i+1].time)
			{
				t = (time - mPositionKeys[i].time) / (mPositionKeys[i + 1].time - mPositionKeys[i].time);
				return Lerp(mPositionKeys[i].key, mPositionKeys[i + 1].key, t);
			}
			else
			{
				continue;
			}
		}
	}
	else if (time >= mPositionKeys.back().time&&!mLooping)
	{
		return mPositionKeys.back().key;
	}
}

Math::Quaternion SB::Graphics::Animation::GetRotation(float time) const
{
	float t;
	if (time < mRotationKeys.back().time)
	{

		for (int i = 0; i < mRotationKeys.size(); i++)
		{
			if (time >= mRotationKeys[i].time&&time < mRotationKeys[i + 1].time)
			{
				t = (time - mRotationKeys[i].time) / (mRotationKeys[i + 1].time - mRotationKeys[i].time);
				return Slerp(mRotationKeys[i].key, mRotationKeys[i + 1].key, t);
			}
			else
			{
				continue;
			}
		}
	}
	else if (time >= mRotationKeys.back().time && !mLooping)
	{
		return mRotationKeys.back().key;
	}
}

Math::Vector3 SB::Graphics::Animation::GetScale(float time) const
{
	float t;
	if (time < mScaleKeys.back().time)
	{

		for (int i = 0; i < mScaleKeys.size(); i++)
		{
			if (time >= mScaleKeys[i].time&&time < mScaleKeys[i + 1].time)
			{
				t = (time - mScaleKeys[i].time) / (mScaleKeys[i + 1].time - mScaleKeys[i].time);
				Vector3 scal;
				scal= Lerp(mScaleKeys[i].key, mScaleKeys[i + 1].key, t);
				return scal;
			}
			else
			{
				continue;
			}
		}
	}
	else if (time >= mScaleKeys.back().time && !mLooping)
	{
		return mScaleKeys.back().key;
	}
}

Math::Matrix4 SB::Graphics::Animation::GetTransform(float time) const
{
	auto position = GetPosition(time);
	auto rotation = GetRotation(time);
	auto scale = GetScale(time);
	
	auto matPos = Matrix4::Translation(position);
	auto matRot = Matrix4::RotationQuaternion(rotation);
	auto matSca = Matrix4::Scaling(scale);
		
	return  matSca*matRot*matPos;
}

//std::tuple<Math::Vector3, Math::Quaternion, Math::Vector3> SB::Graphics::Animation::GetTransform(float time) const
//{
//	auto position = GetPosition(time);
//	auto rotation = GetRotation(time);
//	auto scale = GetScale(time);
//
//	auto matPos = Matrix4::Translation(position);
//	auto matRot = Matrix4::RotationQuaternion(rotation);
//	auto matSca = Matrix4::Scaling(scale);
//
//
//	return  matRot*matPos*matSca;
//}

