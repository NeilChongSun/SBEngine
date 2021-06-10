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
	SB::Physics::PhysicsWorld mPhysicsWorld;
	
};