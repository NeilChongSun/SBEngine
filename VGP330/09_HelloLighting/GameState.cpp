#include "GameState.h"
#include "ImGui/Inc/imgui.h"
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f, 0.0f, -15.0f });
	mCamera.SetDirection({ 0.0f, 0.0f, 1.0f });


	//Create constant buffer
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionLight.direction = Normalize({ 1.0, -1.0, 1.0 });
	mDirectionLight.ambient = { 0.4f };
	mDirectionLight.diffuse = { 0.2f };
	mDirectionLight.specular = { 0.1f };

	mMaterial.ambient = { Colors::Yellow };
	mMaterial.diffuse = { Colors::LimeGreen};
	mMaterial.specular = { 0.2f,0.7f,0.6f,1.0f };
	mMaterial.power = 10.0f;

	mMesh = MeshBuilder::CreateSpherePN(24, 2.0f, 24);
	mMeshBuffer.Initialize(mMesh);
	//Compile and create shaders

	mGouraudShadingVertexShader.Initialize("../../Assets/Shaders/DoGouraudShading.fx", VertexPN::Format);
	mGouraudShadingPixeShader.Initialize("../../Assets/Shaders/DoGouraudShading.fx");

	mPhongShadingVertexShader.Initialize("../../Assets/Shaders/DoPhongShading.fx", VertexPN::Format);
	mPhongShadingPixeShader.Initialize("../../Assets/Shaders/DoPhongShading.fx");
}

void GameState::Terminate()
{
	mGouraudShadingPixeShader.Terminate();
	mGouraudShadingVertexShader.Terminate();
	mPhongShadingVertexShader.Terminate();
	mPhongShadingPixeShader.Terminate();
	mMeshBuffer.Terminate();
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();

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

}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();
	
	mLightBuffer.Update(mDirectionLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);


	TransformData transformData;
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	{
		auto matTrans = Matrix4::Translation({ -3.0f,0.0f,0.0f });
		auto matRot = Matrix4::RotationX(mRotation.x)*Matrix4::RotationY(mRotation.y);
		auto matWorld = matRot * matTrans;

		transformData.world = Transpose(matWorld);
		transformData.wvp = Transpose(matWorld*matView*matProj);
		transformData.viewPosition = mCamera.GetPosition();

		mTransformBuffer.Update(transformData);
		mTransformBuffer.BindVS(0);

		mGouraudShadingVertexShader.Bind();
		mGouraudShadingPixeShader.Bind();
		mMeshBuffer.Draw();
	}

	{
		auto matTrans = Matrix4::Translation({ 3.0f,0.0f,0.0f });
		auto matRot = Matrix4::RotationX(mRotation.x)*Matrix4::RotationY(mRotation.y);
		auto matWorld = matRot * matTrans;

		transformData.world = Transpose(matWorld);
		transformData.wvp = Transpose(matWorld*matView*matProj);
		transformData.viewPosition = mCamera.GetPosition();

		mTransformBuffer.Update(transformData);
		mTransformBuffer.BindVS(0);


		mPhongShadingVertexShader.Bind();
		mPhongShadingPixeShader.Bind();

		mMeshBuffer.Draw();
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat( "Direction X##Light", &mDirectionLight.direction.x,0.01f,-1.0f,1.0f );
		directionChanged |= ImGui::DragFloat( "Direction Y##Light", &mDirectionLight.direction.y,0.01f,-1.0f,1.0f );
		directionChanged |= ImGui::DragFloat( "Direction X##Light", &mDirectionLight.direction.z,0.01f,-1.0f,1.0f );
		if (directionChanged)
		{
			mDirectionLight.direction = Normalize(mDirectionLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
		ImGui::DragFloat("Power##Material", &mMaterial.power,1.0f,1.0f,100.0f);
	}
	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragFloat3("RotationX##Transform", &mRotation.x, 0.01f);
	}
	ImGui::End();

}

