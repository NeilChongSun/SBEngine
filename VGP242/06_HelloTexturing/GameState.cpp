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
	//front
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, 0.5f, -0.5f),0.0f,0.0f });  //0
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, 0.5f, -0.5f),1.0f,0.0f });   //1
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, -0.5f, -0.5f),0.0f,1.0f }); //2
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, -0.5f, -0.5f),1.0f,1.0f });  //3
	//back
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, 0.5f, 0.5f),1.0f,0.0f });   //4
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, 0.5f, 0.5f),0.0f,0.0f });    //5
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, -0.5f, 0.5f),1.0f,1.0f });  //6
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, -0.5f, 0.5f),0.0f,1.0f });   //7
	//left
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, 0.5f, 0.5f),0.0f,0.0f });//8
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, 0.5f, -0.5f),1.0f,0.0f });//9
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, -0.5f, 0.5f),0.0f,1.0f });//10
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, -0.5f, -0.5f),1.0f,1.0f });//11
	//right
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, 0.5f, -0.5f),0.0f,0.0f });//12
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, 0.5f, 0.5f),1.0f,0.0f });//13
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, -0.5f, -0.5f),0.0f,1.0f });//14
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, -0.5f, 0.5f),1.0f,1.0f });//15
	//top
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, 0.5f, 0.5f),0.0f,0.0f });//16
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, 0.5f, 0.5f),1.0f,0.0f });//17
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, 0.5f, -0.5f),0.0f,1.0f });//18
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, 0.5f, -0.5f),1.0f,1.0f });//19
	//buttom
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, -0.5f, -0.5f),0.0f,0.0f });//20
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, -0.5f, -0.5f),1.0f,0.0f });//21
	mMesh.vertices.emplace_back(VertexPX{ Vector3(-0.5f, -0.5f, 0.5f),0.0f,1.0f });//22
	mMesh.vertices.emplace_back(VertexPX{ Vector3(0.5f, -0.5f, 0.5f),1.0f,1.0f });//23
	//front
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(3);
	//right
	mMesh.indices.push_back(12);
	mMesh.indices.push_back(13);
	mMesh.indices.push_back(14);
	mMesh.indices.push_back(14);
	mMesh.indices.push_back(13);
	mMesh.indices.push_back(15);
	//left
	mMesh.indices.push_back(8);
	mMesh.indices.push_back(9);
	mMesh.indices.push_back(10);
	mMesh.indices.push_back(10);
	mMesh.indices.push_back(9);
	mMesh.indices.push_back(11);
	//top
	mMesh.indices.push_back(16);
	mMesh.indices.push_back(17);
	mMesh.indices.push_back(18);
	mMesh.indices.push_back(18);
	mMesh.indices.push_back(17);
	mMesh.indices.push_back(19);
	//buttom
	mMesh.indices.push_back(20);
	mMesh.indices.push_back(21);
	mMesh.indices.push_back(22);
	mMesh.indices.push_back(22);
	mMesh.indices.push_back(21);
	mMesh.indices.push_back(23);
	//back
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(7);

#pragma endregion
	mTexture.Initialize("../../Assets/link.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	//Create constant buffer
	mMeshBuffer.Initialize(mMesh.vertices.data(),static_cast<int>(mMesh.vertices.size()), mMesh.indices.data(),static_cast<int>(mMesh.indices.size()));
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
	mTexture.Terminate();
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
		mCamera.Yaw(-kTurnSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Yaw(kTurnSpeed*deltaTime);		
	if (inputSystem->IsKeyDown(KeyCode::SPACE))
		mCamera.Rise(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}
	mRotation += -1.0f * deltaTime;
}

void GameState::Render()
{
	for (int i = 0; i < 1; i++)
	{
		auto context = GraphicsSystem::Get()->GetContext();
		auto matWorld = Matrix4::RotationY(mRotation);
		auto matView = mCamera.GetViewMatrix();
		auto matProj = mCamera.GetPerspectiveMatrix();
		auto matTranspose = Matrix4::Translation(Vector3(i *0.5f, 0.0f, 0.0f));
		auto matWVP = Transpose(matWorld * matView*matProj*matTranspose);
		mConstantBuffer.Update(&matWVP);
		mConstantBuffer.BindVS();
		mVertexShader.Bind();
		mPixeShader.Bind();
		mMeshBuffer.Draw();
		mTexture.BindPS(0);
		mSampler.BindPS();
	}
}
