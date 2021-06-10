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
	SB::Graphics::MeshPN mMesh;
	SB::Graphics::MeshBuffer mMeshBuffer;

	struct TransformData
	{
		SB::Math::Matrix4 world;
		SB::Math::Matrix4 wvp;
		SB::Math::Vector3 viewPosition;
		float padding;
	};

	using TransformBuffer = SB::Graphics::TypedConstantBuffer<TransformData>;
	using LightingBuffer = SB::Graphics::TypedConstantBuffer<SB::Graphics::DirectionalLight>;
	using MaterialBuffer = SB::Graphics::TypedConstantBuffer<SB::Graphics::Material>;

	TransformBuffer mTransformBuffer;
	LightingBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;

	SB::Graphics::DirectionalLight mDirectionLight;
	SB::Graphics::Material mMaterial;
	SB::Graphics::VertexShader mGouraudShadingVertexShader;
	SB::Graphics::PixeShader mGouraudShadingPixeShader;

	SB::Graphics::VertexShader mPhongShadingVertexShader;
	SB::Graphics::PixeShader mPhongShadingPixeShader;


	SB::Math::Vector3 mRotation;
};