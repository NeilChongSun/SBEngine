#include "TestStates.h"
using namespace SB::Graphics;
using namespace SB::Input;

void RedState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Red);
}

void RedState::Update(float delataTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		SB::MainApp().ChangeState("BlueState");
	}
}

void BlueState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Blue);
}

void BlueState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		SB::MainApp().ChangeState("RedState");
	}
}
