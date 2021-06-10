#include "GameState.h"
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetNearPlane(0.001f);
	mCamera.SetPosition({ 0.0f, 0.0f, -15.0f });
	mCamera.SetDirection({ 0.0f, 0.0f, 1.0f });

#pragma endregion
	//auto mesh = MeshBuilder::CreatePlanePX(3, 3);
	//auto mesh = MeshBuilder::CreateCylinderPX(3, 1, 24);
	auto mesh = MeshBuilder::CreateSpherePX(24, 2, 24);


	mTextures[0].Initialize("../../Assets/earth.jpg");
	mTextures[1].Initialize("../../Assets/moon.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	//Create constant buffer
	mMeshBuffer.Initialize(mesh.vertices.data(), static_cast<int>(mesh.vertices.size()), mesh.indices.data(), static_cast<int>(mesh.indices.size()));
	mConstantBuffer.Initialize(sizeof(Matrix4));
	//Compile and create shaders
	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixeShader.Initialize("../../Assets/Shaders/DoTexturing.fx");
}

void GameState::Terminate()
{
	mPixeShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	mSampler.Terminate();
	mTextures[0].Terminate();
	mTextures[1].Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 8.0f;
	const float kTurnSpeed = 0.8f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Yaw(-kTurnSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Yaw(kTurnSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::SPACE))
		mCamera.Rise(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		mCamera.Rise(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON) && inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON) && inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(kMoveSpeed * deltaTime);
	}

	mSelfRotation[0] += -1.0f*deltaTime;
	mSelfRotation[1] += 0.5f*deltaTime;
	mRevolveRotation[0] += 0.0f;
	mRevolveRotation[1] += 0.3f*deltaTime;
}

void GameState::Render()
{
	float sca[2];
	sca[0] = 2.0f;
	sca[1] = 0.5f;

	auto context = GraphicsSystem::Get()->GetContext();
	mConstantBuffer.BindVS();
	mVertexShader.Bind();
	mPixeShader.Bind();
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	mSampler.BindPS();
	for (int i = 0; i < 2; i++)
	{
		mTextures[i].BindPS(0);
		auto scale = Matrix4::Scaling(sca[i]);

		auto selfRotation = Matrix4::RotationY(mSelfRotation[i]);
		auto revolveRotation = Matrix4::RotationY(mRevolveRotation[i]);

		auto matTranspose = Matrix4::Translation(Vector3(i *10.0f, 0.0f, 0.0f));
		auto matWVP = Transpose(matWorld *scale*selfRotation*matTranspose* revolveRotation *matView*matProj);
		mConstantBuffer.Update(&matWVP);
		mMeshBuffer.Draw();
	}
}
