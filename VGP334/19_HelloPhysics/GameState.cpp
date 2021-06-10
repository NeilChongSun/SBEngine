#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;
using namespace SB::Physics;

namespace 
{
	std::random_device sRandomDevice{};
	std::mt19937 sRandmEngine{ sRandomDevice() };

	float RandomFloat(float min, float max)
	{
		return std::uniform_real_distribution<float>{min, max}(sRandmEngine);
	}
}


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(300.0f);
	mCamera.SetPosition({ 0.0f, 10.0f, -30.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	PhysicsWorld::Settings settings;

	settings.gravity = { 0.0f,-9.8f,0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.0f;
	settings.iterations = 1;

	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddStaticPlane({ Vector3::YAxis,0.0f });
	mPhysicsWorld.AddStaticOBB({ {0.0f,2.0f,0.0f},{4.0f,0.5f,5.0f},Quaternion::RotationAxis(Vector3::ZAxis,10.0f*Constants::DegToRad) });
	mPhysicsWorld.AddStaticOBB({ {-3.0f,5.5f,0.0f},{4.0f,0.2f,5.0f},Quaternion::RotationAxis(Vector3::ZAxis,-15.0f*Constants::DegToRad) });

}

void GameState::Terminate()
{
	mPhysicsWorld.Clear();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 20.0f;
	const float kTurnSpeed = 0.2f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::SPACE))
		mCamera.Rise(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		mCamera.Rise(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed * deltaTime);

	mPhysicsWorld.Update(deltaTime);
}

void GameState::DebugUI()
{
	ImGui::Begin("PHYSICS", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Particles",ImVec2(100,30)))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 100; i++)
		{
			auto particle = new Particle();
			particle->SetPosition({ 0.0f,5.0f,0.0f });
			particle->SetVelocity({ RandomFloat(-0.05,0.05f),RandomFloat(0.1f,0.4f),RandomFloat(-0.05f,0.05f) });
			particle->radius = 0.1f;
			particle->bounce = 0.5f;
			mPhysicsWorld.AddParticle(particle);
			
		}
	} 

	if (ImGui::Button("Sticks", ImVec2(100, 30)))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 50; i++)
		{
			auto p1 = new Particle();
			auto p2 = new Particle();
			p1->SetPosition({ 0.5f,5.0f,0.0f });
			p1->SetVelocity({ RandomFloat(-0.05,0.05f),RandomFloat(0.1f,0.4f),RandomFloat(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;

			p2->SetPosition({ -0.5f,5.0f,0.0f });
			p2->SetVelocity({ RandomFloat(-0.05,0.05f),RandomFloat(0.1f,0.4f),RandomFloat(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;

			mPhysicsWorld.AddParticle(p1);
			mPhysicsWorld.AddParticle(p2);
			auto c = new Spring(p1, p2);
			mPhysicsWorld.AddConstraint(c);
		}
	}

	if (ImGui::Button("Tetrahedron", ImVec2(100, 30)))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 20; i++)
		{
			auto p1 = new Particle();
			auto p2 = new Particle();
			auto p3 = new Particle();
			auto p4 = new Particle();
			p1->SetPosition({ -0.5f,5.0f,0.0f });
			p1->SetVelocity({ RandomFloat(-0.05,0.05f),RandomFloat(0.1f,0.4f),RandomFloat(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;

			p2->SetPosition({ 0.0f,5.0f,1.0f });
			p2->SetVelocity({ RandomFloat(-0.05,0.05f),RandomFloat(0.1f,0.4f),RandomFloat(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;

			p3->SetPosition({ 0.5f,5.0f,0.0f });
			p3->SetVelocity({ RandomFloat(-0.05,0.05f),RandomFloat(0.1f,0.4f),RandomFloat(-0.05f,0.05f) });
			p3->radius = 0.1f;
			p3->bounce = 0.3f;

			p4->SetPosition({ 0.0f,6.0f,0.0f });
			p4->SetVelocity({ RandomFloat(-0.05,0.05f),RandomFloat(0.1f,0.4f),RandomFloat(-0.05f,0.05f) });
			p4->radius = 0.1f;
			p4->bounce = 0.3f;

			mPhysicsWorld.AddParticle(p1);
			mPhysicsWorld.AddParticle(p2);
			mPhysicsWorld.AddParticle(p3);
			mPhysicsWorld.AddParticle(p4);
			
			mPhysicsWorld.AddConstraint(new Spring(p1, p2, 1.0f));
			mPhysicsWorld.AddConstraint(new Spring(p2, p3, 1.0f));
			mPhysicsWorld.AddConstraint(new Spring(p3, p1, 1.0f));
			mPhysicsWorld.AddConstraint(new Spring(p1, p4, 1.0f));
			mPhysicsWorld.AddConstraint(new Spring(p2, p4, 1.0f));
			mPhysicsWorld.AddConstraint(new Spring(p3, p4, 1.0f));
		}
	}

	ImGui::End();
}
void GameState::Render()
{
	mPhysicsWorld.DebugDraw();

	SimpleDraw::AddGroundPlane(30.0f);

	SimpleDraw::Render(mCamera);
}
