#include "Precompiled.h"
#include"SkeletonIO.h"

using namespace SB;
using namespace SB::Graphics;

namespace
{
	void WriteMatrix(FILE* file, const Math::Matrix4& m)
	{
		for (auto& v : m.v)
			fprintf(file, "%f ", v);
		fprintf(file, "\n");
	}

	void ReadMatrix(FILE* file, Math::Matrix4& m)
	{
		for (uint32_t i = 0; i < 16; i++)
			fscanf_s(file, "%f ", &m.v[i]);
		fscanf_s(file, "\n");
	}
}


void SkeletonIO::Write(FILE* file, const Skeleton& skeleton)
{
	fprintf(file, "BoneCount: %d\n", static_cast<uint32_t>(skeleton.bones.size()));
	for (auto& b : skeleton.bones)
	{
		fprintf(file, "Name: %s\n", b->name.c_str());
		fprintf(file, "Index: %d\n", b->index);
		if (b.get()->parent != nullptr)
			fprintf(file, "ParentIndex: %d\n", b->parent->index);
		else
			fprintf(file, "ParentIndex: -1\n");
		fprintf(file, "ChildCount: %d\n", static_cast<uint32_t>(b->children.size()));
		for (auto& c : b->children)
		{
			fprintf(file, "%d ", c->index);
		}
		fprintf(file, "\n");
		WriteMatrix(file, b->toParentTransform);
		WriteMatrix(file, b->offsetTransform);
	}

}

void SkeletonIO::Read(FILE* file, Skeleton& skeleton)
{
	uint32_t numBones = 0;
	fscanf_s(file, "BoneCount: %d\n", &numBones);
	//skeleton.bones.resize(numBones);
	for (uint32_t i = 0; i < numBones; i++)
	{	
		char boneName[100];
		int index = 0;
		int parentIndex = -1;
		uint32_t numChildren = 0;
		Math::Matrix4 toParentTransform = Math::Matrix4::Identity;
		Math::Matrix4 offsetTransform = Math::Matrix4::Identity;
		
		auto& newBone = skeleton.bones.emplace_back(std::make_unique<Bone>());
		
		fscanf(file, "Name: %s\n", boneName);
		newBone->name = std::move(boneName);
		
		fscanf_s(file, "Index: %d\n", &index);
		newBone->index = index;

		fscanf_s(file, "ParentIndex: %d\n", &parentIndex);
		newBone->parentIndex = parentIndex;
		
		fscanf_s(file, "ChildCount: %d\n", &numChildren);
		newBone->childIndices.resize(numChildren);
		for (uint32_t j = 0; j < numChildren; j++)
		{
			int childIndex = 0;
			fscanf_s(file, "%d ", &childIndex);
			newBone->childIndices[j] = childIndex;
		}
		fscanf_s(file, "\n");

		ReadMatrix(file, toParentTransform);
		newBone->toParentTransform = toParentTransform;
		
		ReadMatrix(file, offsetTransform);
		newBone->offsetTransform = offsetTransform;
	}
}