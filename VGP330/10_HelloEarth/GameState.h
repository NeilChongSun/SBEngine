#pragma once
#include<SB/Inc/SB.h>

class GameState : public SB::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;
private:
	SB::Graphics::Camera mCamera;

	struct TransformData
	{
		SB::Math::Matrix4 world;
		SB::Math::Matrix4 wvp;
		SB::Math::Vector3 viewPosition;
		float padding;
	};

	struct SettingsData
	{
		float specularWeight=1.0f;
		float bumpMapWeight=0.0f;
		float normalMapWeight=1.0f;
		float padding;
	};

	using TransformBuffer = SB::Graphics::TypedConstantBuffer<TransformData>;
	using LightingBuffer = SB::Graphics::TypedConstantBuffer<SB::Graphics::DirectionalLight>;
	using MaterialBuffer = SB::Graphics::TypedConstantBuffer<SB::Graphics::Material>;
	using SettingsBuffer = SB::Graphics::TypedConstantBuffer<SettingsData>;
	TransformBuffer mTransformBuffer;
	LightingBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	TransformData mTransformData;
	SettingsBuffer mSettingsBuffer;
	SettingsData mSettingsData;
	SB::Graphics::DirectionalLight mDirectionLight;
	SB::Graphics::Material mMaterial;

	SB::Graphics::VertexShader mEarthVertexShader;
	SB::Graphics::PixeShader mEarthPixeShader;

	SB::Graphics::VertexShader mCloudVertexShader;
	SB::Graphics::PixeShader mCloudPixeShader;

	SB::Graphics::Mesh mMesh;
	SB::Graphics::MeshBuffer mMeshBuffer;

	SB::Graphics::Texture mDiffuseMap;
	SB::Graphics::Texture mSpecularMap;
	SB::Graphics::Texture mDisplacementMap;
	SB::Graphics::Texture mNormalMap;
	SB::Graphics::Texture mCloudMap;
	
	SB::Graphics::BlendState mBlendState;

	SB::Graphics::Sampler mSampler;

	SB::Math::Vector3 mRotation;
	SB::Math::Vector3 mCloudRotation;
};