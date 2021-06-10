#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;
using namespace SB::Physics;

namespace
{
	//TODO: Move this function into a new file/name: AnimationUtil and add it to the Graphic Library
	void RunFABRIC(std::vector<Vector3>& points, Vector3 target, int maxIteration, float errorThreshold)
	{
		ASSERT(points.size() >= 2, "Not enougt points to run FABRIK!");

		//Record the origin
		Vector3 origin = points.front();

		//Record the current point distance, we want to matain these valuse when running IK
		std::vector<float> lengths;
		lengths.reserve(points.size() - 1);
		for (size_t i = 0; i + 1 < points.size(); i++)
			lengths.push_back(Magnitude(points[i + 1] - points[i]));

		//Loop until we are close enough, or have used up all iteration
		for (int i = 0; i < maxIteration; i++)
		{
			if (MagnitudeSqr(points.back() - target) < Sqr(errorThreshold))
				break;

			//Run forward pass
			points.back() = target;
			for (int i = (int)points.size() - 2; i >= 0; --i)
			{
				Vector3 direction = Normalize(points[i + 1] - points[i]);
				points[i] = points[i + 1] - (direction*lengths[i]);
			}
			//Run backward pass
			points.front() = origin;
			for (int i = 0; i + 1 < (int)points.size(); i++)
			{
				Vector3 direction = Normalize(points[i + 1] - points[i]);
				points[i + 1] = points[i] + (direction*lengths[i]);
			}
		}
	}
}


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(300.0f);
	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f,2.0f,0.0f });

	const int numBones = 5;
	const float boneLength = 1.0f;

	mPoints.reserve(numBones + 1);
	mPoints.push_back(Vector3::Zero);
	for (int i = 1; i < mPoints.capacity(); i++)
		mPoints.push_back(mPoints.back() + Vector3(0.0f, boneLength, 0.0f));
	mTarget = mPoints.back();
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 10.0f;
	const float kTurnSpeed = 0.1f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::Q))
		mCamera.Rise(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		mCamera.Rise(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	const float speed = 5.0f;
	if (inputSystem->IsKeyDown(KeyCode::UP))
		mTarget.z += speed * deltaTime;

	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		mTarget.z -= speed * deltaTime;

	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		mTarget.x -= speed * deltaTime;

	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		mTarget.x += speed * deltaTime;

	if (inputSystem->IsKeyDown(KeyCode::RSHIFT))
		mTarget.y += speed * deltaTime;

	if (inputSystem->IsKeyDown(KeyCode::RCONTROL))
		mTarget.y -= speed * deltaTime;

	RunFABRIC(mPoints, mTarget, 5, 0.01f);

}

void GameState::DebugUI()
{


}
void GameState::Render()
{
	for (size_t i = 0; i + 1 < mPoints.size(); i++)
	{
		SimpleDraw::AddLine(mPoints[i], mPoints[i + 1], Colors::Green);
		SimpleDraw::AddSphere(mPoints[i], 0.1f, Colors::Cyan);
	}

	SimpleDraw::AddAABB(mTarget, { 0.2f,0.2f,0.2f }, Colors::Red);

	SimpleDraw::AddGroundPlane(30.0f);
	//SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::Render(mCamera);
}
