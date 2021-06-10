#pragma once
#include<SB/Inc/SB.h>

class GameState : public SB::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
private:
	SB::Graphics::Camera mCamera;
	SB::Graphics::MeshPX mMesh;
	SB::Graphics::MeshBuffer mMeshBuffer;
	SB::Graphics::VertexShader mVertexShader;
	SB::Graphics::PixeShader mPixeShader;
	SB::Graphics::ConstantBuffer mConstantBuffer;
	SB::Graphics::Texture mTexture;
	SB::Graphics::Sampler mSampler;
	float mRotation = 0.0f;
};