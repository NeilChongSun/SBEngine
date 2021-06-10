#pragma once
#include<SB/Inc/SB.h>

class RedState : public SB::AppState
{
public:
	void Initialize() override;
	void Update(float delataTime) override;

};

class BlueState : public SB::AppState 
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

