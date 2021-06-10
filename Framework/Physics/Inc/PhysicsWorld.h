#pragma once
#include"Particle.h"
#include"Constraints.h"
namespace SB::Physics
{
	class PhysicsWorld
	{
	public:
		struct Settings
		{
			Math::Vector3 gravity{ 0.0f,-9.8f,0.0f };
			float timeStep = 1.0f / 60.0f;
			float drag = 0.5f;
			int iterations = 1;
		};

		void Initialize(const Settings& settings);

		void Update(float deltaTime);
		void DebugDraw() const;
		
		//For simulation
		void AddParticle(Particle* p);
		void AddConstraint(Constraint* c);
		
		//For environment
		void AddStaticPlane(const Math::Plane& plane);
		void AddStaticOBB(const Math::OBB& obb);
		
		void Clear(bool onlyDynamic=false);

	private:
		void AccumulateForces();
		void Integrate();
		void SatisfyConstraints();

		std::vector<Particle*> mParticles;
		std::vector<Math::Plane> mPlanes;
		std::vector<Constraint*> mConstraints;
		std::vector<Math::OBB> mOBBs;

		Settings mSettings;
		float mTimer = 0.0f;
	};
}