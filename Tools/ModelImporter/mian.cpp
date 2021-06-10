#include<Graphics/Inc/Graphics.h>
#include<Math/Inc/EngineMath.h>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<cstdio>

using namespace SB::Graphics;

using BoneIndexLookup = std::map<std::string, int>;//Used the lookup name

struct Arguments
{
	const char* inputFileName = nullptr;
	const char* outputFileName = nullptr;
	float scale = 1.0f;
	bool animOnly = false;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	//We need at least 3 arguments.
	if (argc < 3)
		return std::nullopt;

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];

	for (int i = 0; i + 2 < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 's':
				args.scale = static_cast<float>(atof(argv[i + 1]));
				break;
			}
		}
	}
	return args;
}

void PrintUsage()
{
	printf(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage:\n"
		"	ModelImporter.exe [Options] <InputFile> <OutputFile>\n"
		"\n"
		"Options:\n"
		"	-s	Scale applied to the model."
		"\n"
	);
}

inline Color Convert(const aiColor3D& c)
{
	return { c.r,c.g,c.b,1.0f };
}

inline Vector3 Convert(const aiVector3D& v)
{
	return { v.x,v.y,v.z };
}

inline Quaternion Convert(const aiQuaternion& q)
{
	return { q.x,q.y,q.z,q.w };
}

inline Matrix4 Convert(const aiMatrix4x4& m)
{
	Matrix4 mat = *(reinterpret_cast<const Matrix4*>(&m));
	return Transpose(mat);
}

void ExportEmbededTexture(const aiTexture& texture, const Arguments&args, const std::string& fileName)
{
	printf("Extracting embexed texture...\n");

	std::string fullFileName = args.outputFileName;
	fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
	fullFileName += fileName;

	FILE* file = nullptr;
	fopen_s(&file, fullFileName.c_str(), "wb");
	size_t written = fwrite(texture.pcData, 1, texture.mWidth, file);
	ASSERT(written == texture.mWidth, "Error: Failed to extract embedded texture!");
	fclose(file);
}


std::string FindTexture(const aiScene& scene, const aiMaterial& inputMaterial, aiTextureType textureType, const Arguments& args, const char* suffix)
{
	std::filesystem::path textureName;
	const uint32_t textureCount = inputMaterial.GetTextureCount(textureType);
	if (textureCount > 0)
	{
		aiString texturePath;
		if (inputMaterial.GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
		{
			if (auto embededTexture = scene.GetEmbeddedTexture(texturePath.C_Str()); embededTexture != nullptr)
			{
				std::filesystem::path embededFilePaht = texturePath.C_Str();
				std::string fileName = args.inputFileName;
				fileName.erase(fileName.length() - 4);//remove ".fbx" extension
				fileName += suffix;
				fileName += embededFilePaht.extension().u8string();

				ExportEmbededTexture(*embededTexture, args, fileName);

				printf("Exporting embeded texture to %s \n", fileName.c_str());
				textureName = fileName;
			}
			else
			{
				std::filesystem::path filePath = texturePath.C_Str();
				std::string fileName = filePath.filename().u8string();
				printf("Adding texture %s \n", fileName.c_str());
				textureName = fileName;
			}
		}
	}
	//return textureName.filename().u8string().c_str();
	return textureName.u8string().c_str();
}

// Check if inputBone exists in skeleton, if so just return the index.
// Otherwise, add it to the skeleton. The aiBone must have a name!
int TryAddBone(const aiBone* inputBone, Skeleton& skeleton, BoneIndexLookup& boneIndexLookup)
{
	std::string name = inputBone->mName.C_Str();
	ASSERT(!name.empty(), "Error: inputBone has no name!");

	auto iter = boneIndexLookup.find(name);
	if (iter != boneIndexLookup.end())
		return iter->second;

	// Add a new bone in the skeleton for this
	auto& newBone = skeleton.bones.emplace_back(std::make_unique<Bone>());
	newBone->name = std::move(name);
	newBone->index = static_cast<int>(skeleton.bones.size()) - 1;
	newBone->offsetTransform = Convert(inputBone->mOffsetMatrix);

	// Cache the bone index
	boneIndexLookup.emplace(newBone->name, newBone->index);
	return newBone->index;
}

// Recursively walk the aiScene tree and add/link bones to our skeleton as we find them.
Bone* BuildSkeleton(const aiNode& sceneNode, Bone* parent, Skeleton& skeleton, BoneIndexLookup& boneIndexLookup)
{
	Bone* bone = nullptr;

	std::string name = sceneNode.mName.C_Str();
	auto iter = boneIndexLookup.find(name);
	if (iter != boneIndexLookup.end())
	{
		// Bone already exists
		bone = skeleton.bones[iter->second].get();
	}
	else
	{
		// Add a new bone in the skeleton for this (possible need to generate a name for it)
		bone = skeleton.bones.emplace_back(std::make_unique<Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		bone->offsetTransform = Matrix4::Identity;
		if (name.empty())
			bone->name = "NoName" + std::to_string(bone->index);
		else
			bone->name = std::move(name);

		// Cache the bone index
		boneIndexLookup.emplace(bone->name, bone->index);
	}

	// Link to your parent
	bone->parent = parent;
	bone->toParentTransform = Convert(sceneNode.mTransformation);

	// Recurse through your children
	bone->children.reserve(sceneNode.mNumChildren);
	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
	{
		Bone* child = BuildSkeleton(*sceneNode.mChildren[i], bone, skeleton, boneIndexLookup);
		bone->children.push_back(child);
	}
	return bone;
}

void SaveModel(const Arguments& args, const Model& model)
{
	std::filesystem::path fileName;
	fileName = args.outputFileName;
	fileName.replace_extension("model");

	printf("Saving Mode: %s... \n", fileName.u8string().c_str());

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "w");

	const uint32_t meshCount = static_cast<uint32_t>(model.meshData.size());
	fprintf(file, "MeshCount: %d \n", meshCount);

	for (uint32_t i = 0; i < meshCount; i++)
	{
		fprintf_s(file, "MaterialIndex: %d\n", model.meshData[i].materiaIndex);
		MeshIO::Write(file, model.meshData[i].mesh);
	}
	//Save out model.material data as well
	//if diffuseMap is empty string, write <none>
	const uint32_t materialCount = static_cast<uint32_t>(model.materialData.size());
	
	fprintf(file, "MaterialCount: %d \n", materialCount);
	for (uint32_t i = 0; i < materialCount; i++)
	{
		std::string diffuseMapName = model.materialData[i].diffuseMapName;
		
		if(diffuseMapName.empty())
			fprintf_s(file, "DiffuseMapName: <none>\n");
		else
			fprintf_s(file, "DiffuseMapName: %s\n", diffuseMapName.c_str());
		
		MaterialIO::Write(file, model.materialData[i].material);
	}
	fclose(file);
	printf("Save Mode: %s Done \n", fileName.u8string().c_str());
}

void SaveSkeleton(const Arguments& args, const Skeleton& skeleton)
{
	std::filesystem::path path = args.outputFileName;
	path.replace_extension("skeleton");

	printf("Saving skeleton: %s...\n", path.u8string().c_str());

	FILE* file = nullptr;
	fopen_s(&file, path.u8string().c_str(), "w");

	SkeletonIO::Write(file, skeleton);

	fclose(file);
}

void SaveAnimationSet(const Arguments& args, const AnimationSet& animationSet)
{
	std::filesystem::path path= args.outputFileName;
	path.replace_extension("animset");

	printf("Saving animations: %s...\n", path.u8string().c_str());

	FILE* file = nullptr;
	fopen_s(&file, path.u8string().c_str(), "w");

	const uint32_t clipCount = static_cast<uint32_t>(animationSet.clips.size());
	fprintf_s(file, "ClipCount: %d\n", clipCount);
	for (auto& animationClip:animationSet.clips)
	{
		AnimationIO::Write(file, *animationClip);
	}
	fclose(file);
}


int main(int argc, char* argv[])
{
	const auto& argsOpt = ParseArgs(argc, argv);

	if (!argsOpt.has_value())
	{
		PrintUsage();
		return -1;
	}

	const auto& args = argsOpt.value();

	//Create an instance of teh importer class to do the parsing for us
	Assimp::Importer importer;

	//Try to import the model into a scene.
	const aiScene* scene = importer.ReadFile(args.inputFileName, aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);

	if (scene == nullptr)
	{
		printf("Error: %s /n", importer.GetErrorString());
		return -1;
	}
	Model model;
	BoneIndexLookup boneIndexLookup;
	if (scene->HasMeshes())
	{
		printf("Reading mesh data... \n");

		const uint32_t numMeshes = scene->mNumMeshes;
		model.meshData.resize(numMeshes);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			const uint32_t numVertices = inputMesh->mNumVertices;
			const uint32_t numFaces = inputMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			printf("Reading material index...\n");
			model.meshData[meshIndex].materiaIndex = inputMesh->mMaterialIndex;

			printf("Reading vertices... \n");

			std::vector<BoneVertex> vertices;
			vertices.reserve(numVertices);

			const aiVector3D* positions = inputMesh->mVertices;
			const aiVector3D* normals = inputMesh->mNormals;
			const aiVector3D* tangents = inputMesh->mTangents;
			const aiVector3D* texcoord = inputMesh->HasTextureCoords(0) ? inputMesh->mTextureCoords[0] : nullptr;
			for (uint32_t i = 0; i < numVertices; i++)
			{
				auto& vertex = vertices.emplace_back(BoneVertex{});
				vertex.position = Convert(positions[i])*args.scale;
				vertex.normal = Convert(normals[i]);
				vertex.tangent = Convert(tangents[i]);
				vertex.texcoord = texcoord ? Vector2{ texcoord[i].x,texcoord[i].y } : 0.0f;
			}

			printf("Reading indeces...\n");

			std::vector<uint32_t> indices;
			indices.reserve(numIndices);

			const aiFace* faces = inputMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; i++)
			{
				indices.push_back(faces[i].mIndices[0]);
				indices.push_back(faces[i].mIndices[1]);
				indices.push_back(faces[i].mIndices[2]);
			}

			if (inputMesh->HasBones())
			{
				printf("Reading bone weights...\n");
				//Track how weights have we added to each vertex so far
				std::vector<int> numWeights(vertices.size(), 0);

				for (uint32_t meshBoneIndex = 0; meshBoneIndex < inputMesh->mNumBones; meshBoneIndex++)
				{
					aiBone* inputBone = inputMesh->mBones[meshBoneIndex];
					int boneIndex = TryAddBone(inputBone, model.skeleton, boneIndexLookup);		
					
					for (uint32_t weightIndex = 0; weightIndex < inputBone->mNumWeights; weightIndex++)
					{
						const aiVertexWeight& weight = inputBone->mWeights[weightIndex];
						auto& vertex = vertices[weight.mVertexId];
						auto& count = numWeights[weight.mVertexId];

						//Our vertices can hold at least 4 weights
						if (count<4)
						{
							vertex.boneIndices[count] = boneIndex;
							vertex.boneWeight[count] = weight.mWeight;
							++count;
						}
					}
				}
			}

			SkinneMesh mesh;
			mesh.vertices = std::move(vertices);
			mesh.indices = std::move(indices);
			model.meshData[meshIndex].mesh = std::move(mesh);
		}
		//Look for material data.
		if (scene->HasMaterials())
		{
			printf("Reading materials...\n");

			const uint32_t numMaterials = scene->mNumMaterials;
			model.materialData.resize(numMaterials);

			for (uint32_t i = 0; i < numMaterials; i++)
			{
				const aiMaterial* inputMaterial = scene->mMaterials[i];
				aiColor3D ambientColor(0.f, 0.f, 0.f);
				inputMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
				aiColor3D diffuseColor(0.f, 0.f, 0.f);
				inputMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
				aiColor3D specularColor(0.f, 0.f, 0.f);
				inputMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
				ai_real specularPower;
				inputMaterial->Get(AI_MATKEY_SHININESS, specularPower);


				auto& material = model.materialData[i];
				material.material.ambient = Convert(ambientColor);
				material.material.diffuse = Convert(diffuseColor);
				material.material.specular = Convert(specularColor);
				material.material.power = specularPower;
				material.diffuseMapName = FindTexture(*scene, *inputMaterial, aiTextureType_DIFFUSE, args, "_diffuse");
			}
		}
	}
	//Check if we have skeleton information.
	if (!model.skeleton.bones.empty())
	{
		printf("Building skeleton...\n");
		BuildSkeleton(*scene->mRootNode, nullptr, model.skeleton, boneIndexLookup);
	}

	//Look for animation data
	if (scene->HasAnimations())
	{
		printf("Reading animations...\n");

		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; animIndex++)
		{
			const aiAnimation* inputAnim = scene->mAnimations[animIndex];
			auto animClip = std::make_unique<AnimationClip>();

			if (inputAnim->mName.length > 0)
				animClip->name = inputAnim->mName.C_Str();
			else
				animClip->name = "Anim" + std::to_string(animIndex);

			animClip->duration = static_cast<float>(inputAnim->mDuration);
			animClip->ticksPerSecond = static_cast<float>(inputAnim->mTicksPerSecond);

			printf("Reading bone animation for %s ...", animClip->name.c_str()); 

			//Make sue we have the same number of slots for animationClip as the bone in the 
			//skeleton. This allows us to index the animationClip using the index directly
			animClip->boneAnimations.resize(model.skeleton.bones.size());

			for (uint32_t boneAnimIndex = 0; boneAnimIndex < inputAnim->mNumChannels; boneAnimIndex++)
			{
				const aiNodeAnim* inputBoneAnim = inputAnim->mChannels[boneAnimIndex];
				int slotIndex = boneIndexLookup[inputBoneAnim->mNodeName.C_Str()];
				auto& boneAnim = animClip->boneAnimations[slotIndex];
				boneAnim = std::make_unique<Animation>();

				AnimationBuilder builder;
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumPositionKeys; keyIndex++)
				{
					auto& key = inputBoneAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(Convert(key.mValue)* args.scale, static_cast<float>(key.mTime));
				}

				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumRotationKeys; keyIndex++)
				{
					auto& key = inputBoneAnim->mRotationKeys[keyIndex];
					builder.AddRotationKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}

				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumScalingKeys; keyIndex++)
				{
					auto& key = inputBoneAnim->mScalingKeys[keyIndex];
					builder.AddScaleKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}

				*boneAnim = builder.Get();
			}
			//Add the new clip to our animation set
			model.animationSet.clips.emplace_back(std::move(animClip));
		}
	}

	SaveModel(args, model); 
	SaveSkeleton(args, model.skeleton); 
	SaveAnimationSet(args, model.animationSet);

	printf("All done!\n");
	return 0;
}
//2:30:15



