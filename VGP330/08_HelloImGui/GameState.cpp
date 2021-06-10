#include "GameState.h"
#include "ImGui/Inc/imgui.h"
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f, 0.0f, -5.0f });
	mCamera.SetDirection({ 0.0f, 0.0f, 1.0f });
}

void GameState::Terminate()
{	
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


	SimpleDraw::AddLine(Vector3::Zero, Vector3::XAxis*20.0f, Colors::Red);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::YAxis*20.0f, Colors::Green);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::ZAxis*20.0f, Colors::Blue);

	

}

void GameState::Render()
{
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();
}