#pragma once

#include"Mesh.h"
#include"MeshBuffer.h"
#include"Texture.h"
#include"Material.h"
#include"Skeleton.h"
#include"AnimationSet.h"

namespace SB::Graphics
{
	class Model;
	class ModelLoader
	{
	public:
		static void LoadModel(std::filesystem::path fileNmae, Model& model);
		static void LoadSkeleton(std::filesystem::path fileNmae, Skeleton& skeleton);
		static void LoadAnimationSet(std::filesystem::path fileName, AnimationSet& animationSet);
	};
	class Model
	{
	public:
		void Initialize(const std::filesystem::path& fileName);
		void Terminate();

		void Draw() const;

	public:
		struct MeshData
		{
			SkinneMesh mesh;
			uint32_t materiaIndex = 0;
			MeshBuffer meshBuffer;
		};

		struct MaterialData
		{
			Material material;
			std::string diffuseMapName;
			std::unique_ptr<Texture> diffuseMap;
		};

		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
		Skeleton skeleton;
		AnimationSet animationSet;
	};
}