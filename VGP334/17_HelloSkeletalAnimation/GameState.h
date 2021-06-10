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
	void DrawDepthMap();
	void DrawScene();
	void PostProcess();
private:
	SB::Graphics::Camera mDefaultCamera;
	SB::Graphics::Camera mLightCamera;
	SB::Graphics::Camera* mActiveCamera = nullptr;

	SB::Math::Vector3 mTankPosition;
	SB::Math::Vector3 mTankRotation;

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
		float aoMapWeight = 1.0f;
		float brightness = 1.0f;
		int useShadow = 1.0f;
		float depthBias = 0.0f;
		float padding;
	};

	using TransformBuffer = SB::Graphics::TypedConstantBuffer<TransformData>;
	using LightingBuffer = SB::Graphics::TypedConstantBuffer<SB::Graphics::DirectionalLight>;
	using MaterialBuffer = SB::Graphics::TypedConstantBuffer<SB::Graphics::Material>;
	using SettingsBuffer = SB::Graphics::TypedConstantBuffer<SettingsData>;
	using DepthMapBuffer = SB::Graphics::TypedConstantBuffer<SB::Math::Matrix4>;
	using ShadowBuffer = SB::Graphics::TypedConstantBuffer<SB::Math::Matrix4>;


	TransformBuffer mTransformBuffer;
	LightingBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	TransformData mTransformData;
	SettingsBuffer mSettingsBuffer;
	SettingsData mSettingsData;
	SB::Graphics::DirectionalLight mDirectionLight;
	SB::Graphics::Material mMaterial;

	SB::Graphics::VertexShader mVertexShader;
	SB::Graphics::PixeShader mPixeShader;

	
	SB::Graphics::Mesh mPlaneMesh;
	SB::Graphics::MeshBuffer mPlaneMeshBuffer;

	
	SB::Graphics::Texture mPlaneMap;
	
	SB::Graphics::Sampler mSampler;

	SB::Math::Vector3 mRotation;

	SB::Graphics::RenderTarget mDepthMapRenderTarget;
	SB::Graphics::VertexShader mDepthMapVertexShader;
	SB::Graphics::PixeShader mDepthMapPixelShader;
	DepthMapBuffer mDepthMapBuffer;
	ShadowBuffer mShadowBuffer;


	//Post Processing
	SB::Graphics::RenderTarget mRenderTarget;
	SB::Graphics::MeshPX mScreenQuad;
	SB::Graphics::MeshBuffer mScreenQuadBuffer;
	SB::Graphics::VertexShader mPostProcessVertexShader;
	SB::Graphics::PixeShader mPostProcessPixeShader;

	//Model
	SB::Graphics::Model mModel;
	SB::Math::Vector3 mModelPosition;
	SB::Math::Vector3 mModelRotation;
	float mModelScale;

	std::vector<SB::Math::Matrix4> mBoneMatrices;
	std::vector<SB::Math::Matrix4> mMatrices;
	bool mIsShowSkeleton = false;
};