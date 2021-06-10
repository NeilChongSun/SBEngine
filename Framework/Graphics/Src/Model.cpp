#include"Model.h"
#include"Precompiled.h"
#include"AnimationIO.h"
#include"MeshIO.h"
#include"MaterialIO.h"
#include"SkeletonIO.h"


using namespace SB;
using namespace SB::Graphics;

void ModelLoader::LoadModel(std::filesystem::path fileName, Model& model)
{
	fileName.replace_extension("model");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	//Load model.mesh
	uint32_t numMeshes = 0;
	fscanf_s(file, "MeshCount: %d\n", &numMeshes);

	model.meshData.resize(numMeshes);
	for (uint32_t i = 0; i < numMeshes; i++)
	{
		fscanf_s(file, "MaterialIndex: %d\n", &model.meshData[i].materiaIndex);
		MeshIO::Read(file, model.meshData[i].mesh);
	}

	//Load model.materialData as well
	//if read <none> for texture name, there is no texture data 
	uint32_t numMaterials = 0;
	char diffuseMapName[100];
	fscanf_s(file, "MaterialCount: %d\n", &numMaterials);

	model.materialData.resize(numMaterials);
	for (uint32_t i = 0; i < numMaterials; i++)
	{
		fscanf(file, "DiffuseMapName: %s\n", diffuseMapName);
		//if (diffuseMapName != "<none>")
		if (strcmp(diffuseMapName,"<none>")!=0)
			model.materialData[i].diffuseMapName = diffuseMapName;
		
		MaterialIO::Read(file, model.materialData[i].material);
	}

	fclose(file);

	for (auto& data : model.meshData)
		data.meshBuffer.Initialize(data.mesh);
	
	for (auto& data : model.materialData)
	{
		if (!data.diffuseMapName.empty())
		{
			data.diffuseMap = std::make_unique<Texture>();
			data.diffuseMap->Initialize(diffuseMapName);
		}
	}
}

void ModelLoader::LoadSkeleton(std::filesystem::path fileName, Skeleton & skeleton)
{
	fileName.replace_extension("skeleton");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	SkeletonIO::Read(file, skeleton);
	fclose(file);

	for (auto& bone:skeleton.bones)
	{
		if (bone->parentIndex!=-1)
			bone->parent = skeleton.bones[bone->parentIndex].get();
		else
			skeleton.root = bone.get();
		
		bone->children.reserve(bone->childIndices.size());
		for (auto childIndex : bone->childIndices)
			bone->children.push_back(skeleton.bones[childIndex].get());
	}
}

void ModelLoader::LoadAnimationSet(std::filesystem::path fileName, AnimationSet & animationSet)
{
	fileName.replace_extension("animset");
	FILE* file= nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	int clipCount = 0;
	fscanf(file, "ClipCount: %d\n", &clipCount);
	for (uint32_t clipIndex = 0; clipIndex < clipCount; clipIndex++)
	{
		AnimationClip* clip = animationSet.clips.emplace_back(std::make_unique<AnimationClip>()).get();
		AnimationIO::Read(file, *clip);
	}
	
	fclose(file); 
}

void Model::Initialize(const std::filesystem::path& fileName)
{
	ModelLoader::LoadModel(fileName, *this);
	ModelLoader::LoadSkeleton(fileName, skeleton);
	ModelLoader::LoadAnimationSet(fileName, animationSet);
}

void Model::Terminate()
{
	for (auto& data : meshData)
		data.meshBuffer.Terminate();
	
	for (auto& data : materialData)
		data.diffuseMap->Terminate();

}

void Model::Draw() const
{
	for (size_t i = 0; i < meshData.size(); i++)
	{
		auto& data = meshData[i];
		materialData[data.materiaIndex].diffuseMap->BindPS(0);
		data.meshBuffer.Draw();
	}
}