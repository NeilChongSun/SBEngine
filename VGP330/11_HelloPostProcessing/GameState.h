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
	//PostProcessing
	void DrawScene();
	void PostProcess();
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

	struct BlurData
	{
		float textureWidth;
		float blurScale = 1.0f;
		float padding[2];
	};

	using TransformBuffer = SB::Graphics::TypedConstantBuffer<TransformData>;
	using LightingBuffer = SB::Graphics::TypedConstantBuffer<SB::Graphics::DirectionalLight>;
	using MaterialBuffer = SB::Graphics::TypedConstantBuffer<SB::Graphics::Material>;
	using SettingsBuffer = SB::Graphics::TypedConstantBuffer<SettingsData>;
	using BlurBuffer = SB::Graphics::TypedConstantBuffer<BlurData>;
	TransformBuffer mTransformBuffer;
	LightingBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	TransformData mTransformData;
	SettingsBuffer mSettingsBuffer;
	SettingsData mSettingsData;
	BlurBuffer mBlurBuffer;
	BlurData mBlurData;

	SB::Graphics::ConstantBuffer mGalaxyConstantBuffer;


	SB::Graphics::DirectionalLight mDirectionLight;
	SB::Graphics::Material mMaterial;

	SB::Graphics::VertexShader mEarthVertexShader;
	SB::Graphics::PixeShader mEarthPixeShader;

	SB::Graphics::VertexShader mCloudVertexShader;
	SB::Graphics::PixeShader mCloudPixeShader;

	SB::Graphics::VertexShader mGalaxyVertexShader;
	SB::Graphics::PixeShader mGalaxyPixelShader;

	SB::Graphics::Mesh mMesh;
	SB::Graphics::MeshBuffer mMeshBuffer;

	SB::Graphics::MeshPX mGalaxyMesh;
	SB::Graphics::MeshBuffer mGalaxyMeshBuffer;

	SB::Graphics::Texture mDiffuseMap;
	SB::Graphics::Texture mSpecularMap;
	SB::Graphics::Texture mDisplacementMap;
	SB::Graphics::Texture mNormalMap;
	SB::Graphics::Texture mCloudMap;
	SB::Graphics::Texture mGalaxyMap;
	
	SB::Graphics::BlendState mBlendState;

	SB::Graphics::Sampler mSampler;

	SB::Math::Vector3 mRotation;
	SB::Math::Vector3 mCloudRotation;

	//Post Processing
	SB::Graphics::RenderTarget mRenderTarget;
	SB::Graphics::MeshPX mScreenQuad;
	SB::Graphics::MeshBuffer mScreenQuadBuffer;
	SB::Graphics::VertexShader mPostProcessVertexShader;
	SB::Graphics::PixeShader mPostProcessPixeShader;
	SB::Graphics::PixeShader mBlurEffectPixelShader;
	SB::Graphics::PixeShader mPixelEffectPixelShader;

	bool isBlur = false;
	bool isPixel = false;
};