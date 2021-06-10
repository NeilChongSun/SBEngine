#include"Precompiled.h"
#include"AnimationIO.h"

using namespace SB;
using namespace SB::Graphics;

void AnimationIO::Write(FILE * file, const Animation & animation)
{
}

void AnimationIO::Read(FILE * file, Animation & animation)
{
}

void AnimationIO::Write(FILE * file, const AnimationClip & animationClip)
{
	//Note that animationClip has the same number of animation as there are bones.
	//This is so that we can use the bone index to index for an animation directly.
	//If animationClip.boneAnimation[i] is null, you need to write out[empty], so when you 
	//load the animation clip,you know to skip those slots
	fprintf(file, "Name: %s\n", animationClip.name.c_str());
	fprintf(file, "Duration: %f\n", animationClip.duration);
	fprintf(file, "TicksPerSecond: %f\n", animationClip.ticksPerSecond);
	fprintf(file, "AnimationCount: %d\n", static_cast<uint32_t>(animationClip.boneAnimations.size()));
	for (auto& animation : animationClip.boneAnimations)
	{
		if (animation.get() == NULL)
			fprintf(file, "[empty]\n");
		else
		{
			fprintf(file, "[Animation]\n");
			fprintf(file, "PositionKeys: %d\n", animation->mPositionKeys.size());
			for (auto& positionKey : animation->mPositionKeys)
			{
				fprintf(file, "%f %f %f %f\n", positionKey.time, positionKey.key.x, positionKey.key.y, positionKey.key.z);
			}
			fprintf(file, "RotationKeys: %d\n", animation->mRotationKeys.size());
			for (auto& rotationKey : animation->mRotationKeys)
			{
				fprintf(file, "%f %f %f %f %f\n", rotationKey.time, rotationKey.key.x, rotationKey.key.y, rotationKey.key.z, rotationKey.key.w);
			}
			fprintf(file, "ScaleKeys: %d\n", animation->mScaleKeys.size());
			for (auto& scaleKey : animation->mScaleKeys)
			{
				fprintf(file, "%f %f %f %f\n", scaleKey.time, scaleKey.key.x, scaleKey.key.y, scaleKey.key.z);
			}
		}
	}
}

void AnimationIO::Read(FILE * file, AnimationClip & animationClip)
{
	char name[128];
	fscanf(file, "Name: %s\n", name);
	animationClip.name = name;

	fscanf(file, "Duration: %f\n", &animationClip.duration);
	fscanf(file, "TicksPerSecond: %f\n", &animationClip.ticksPerSecond);
	int animationCount = 0;
	fscanf(file, "AnimationCount: %d\n", &animationCount);
	//animationClip.boneAnimations.resize(animationCount);
	for (uint32_t animationIndex = 0; animationIndex < animationCount; animationIndex++)
	{
		char slot[128];
		fscanf(file, "%s\n", slot);
		
		auto& animation = animationClip.boneAnimations.emplace_back(std::make_unique<Animation>());
		
		if (strcmp(slot, "[empty]") == 0)
			animationClip.boneAnimations[animationIndex] =NULL;
		else
		{
			int positionKeyCount = 0;
			fscanf(file, "PositionKeys: %d\n", &positionKeyCount);
			animation->mPositionKeys.resize(positionKeyCount);
			for (uint32_t positionKeyIndex = 0; positionKeyIndex < positionKeyCount; positionKeyIndex++)
			{
				auto& positionKey = animation->mPositionKeys[positionKeyIndex];
				fscanf(file, "%f %f %f %f\n", &positionKey.time, &positionKey.key.x, &positionKey.key.y, &positionKey.key.z);
			}

			int rotationKeyCount = 0;
			fscanf(file, "RotationKeys: %d\n", &rotationKeyCount);
			animation->mRotationKeys.resize(rotationKeyCount);
			for (uint32_t rotationKeyIndex = 0; rotationKeyIndex < rotationKeyCount; rotationKeyIndex++)
			{
				auto& rotationKey = animation->mRotationKeys[rotationKeyIndex];
				fscanf(file, "%f %f %f %f %f\n", &rotationKey.time, &rotationKey.key.x, &rotationKey.key.y, &rotationKey.key.z, &rotationKey.key.w);
			}

			int scaleKeyCount = 0;
			fscanf(file, "ScaleKeys: %d\n", &scaleKeyCount);
			animation->mScaleKeys.resize(scaleKeyCount);
			for (uint32_t scaleKeyIndex = 0; scaleKeyIndex < scaleKeyCount; scaleKeyIndex++)
			{
				auto& scaleKey = animation->mScaleKeys[scaleKeyIndex];
				fscanf(file, "%f %f %f %f\n", &scaleKey.time, &scaleKey.key.x, &scaleKey.key.y, &scaleKey.key.z);
			}
		}
	}
}
