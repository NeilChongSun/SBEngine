#pragma once
#include"Model.h"
namespace SB::Graphics
{
	class Animator
	{
	public:
		void Initialize(const Model& model);
		void Terminate();

		void ComputeBindPose();
		void PlayAnimation(int index);
		void BlendTo(int index, float duration);

		void Update(float deltaTime);

		auto& GetBoneMatrices()const { return mBoneMatrices; }

		float GetTimer()const { return mTimer; }

	private:
		const Model* mModel;
		float mTimer = 0.0f;
		int mClipIndex = 0;

		float mBlendTimer = 0.0f;
		float mBlendWeight = 0.0f;
		float mBlendTime = 0.0f;
		float mBlendDuration = 0.0f;
		int mBlendIndex = 0;

		std::vector<SB::Math::Matrix4> mBoneMatrices;
	};
}