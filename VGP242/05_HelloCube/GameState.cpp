#include "GameState.h"
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);
	mCamera.SetPosition({ 0.0f, 0.0f, -5.0f });
	mCamera.SetDirection({ 0.0f, 0.0f, 1.0f });
#pragma region Cube
	//NDC -Normalized Device Coordiante
	mMesh.vertices.emplace_back(VertexPC{ Vector3(0.0f, 0.0f, 0.0f),Vector4(Colors::Blue) });//0
	mMesh.vertices.emplace_back(VertexPC{ Vector3(0.0f, 1.0f, 0.0f),Vector4(Colors::Red) });//1
	mMesh.vertices.emplace_back(VertexPC{ Vector3(1.0f, 0.0f, 0.0f),Vector4(Colors::Green) });//2
	mMesh.vertices.emplace_back(VertexPC{ Vector3(1.0f, 1.0f, 0.0f),Vector4(Colors::Yellow) });//3
	mMesh.vertices.emplace_back(VertexPC{ Vector3(0.0f, 0.0f, -1.0f),Vector4(Colors::Pink) });//4
	mMesh.vertices.emplace_back(VertexPC{ Vector3(0.0f, 1.0f, -1.0f),Vector4(Colors::White) });//5
	mMesh.vertices.emplace_back(VertexPC{ Vector3(1.0f, 0.0f, -1.0f),Vector4(Colors::Black) });//6
	mMesh.vertices.emplace_back(VertexPC{ Vector3(1.0f, 1.0f, -1.0f),Vector4(Colors::Purple) });//7
	//front
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(6);
	//right
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(2);
	//left
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(4);
	//top
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(5);
	//buttom
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(2);
	//back
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(2);
#pragma endregion
	auto device = GraphicsSystem::Get()->GetDevice();
	//Create constant buffer
	mMeshBuffer.Initialize(mMesh.vertices.data(),static_cast<int>(mMesh.vertices.size()), mMesh.indices.data(), static_cast<int>(mMesh.indices.size()));
	mConstantBuffer.Initialize(sizeof(Matrix4));
	//Compile and create shaders
	mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx",VertexPC::Format);
	mPixeShader.Initialize("../../Assets/Shaders/DoTransform.fx");
}

void GameState::Terminate()
{
	mPixeShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
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
		mCamera.Strafe(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::SPACE))
		mCamera.Rise(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}
	mRotation += 0.5f * deltaTime;
}

void GameState::Render()
{
	for (int i = 0; i < 3; i++)
	{
		auto context = GraphicsSystem::Get()->GetContext();
		auto matWorld = Matrix4::RotationY(mRotation*(i+1)*5);
		auto matView = mCamera.GetViewMatrix();
		auto matProj = mCamera.GetPerspectiveMatrix();
		auto matTranspose = Matrix4::Translation(Vector3(i *0.5f, 0.0f, 0.0f));
		auto matWVP = Transpose(matWorld * matView*matProj*matTranspose);
		mConstantBuffer.Update(&matWVP);
		mConstantBuffer.BindVS(0);
		mVertexShader.Bind();
		mPixeShader.Bind();
		mMeshBuffer.Draw();
	}
}
