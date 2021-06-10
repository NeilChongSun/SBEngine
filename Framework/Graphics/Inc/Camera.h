#pragma once
namespace SB::Graphics {
	//This camera assumes YAxis as up direction. You cannot look straight up nor straight down
	class Camera
	{
	public:
		void SetPosition(const Math::Vector3& position);
		void SetDirection(const Math::Vector3& direction);
		void SetLookAt(const Math::Vector3& target);
		void SetAspectRatio(float ratio);

		//3 degrees of freedom for translateion
		void Walk(float distance);
		void Strafe(float distance);
		void Rise(float distance);

		//2 degrees of freedom for retation
		void Pitch(float radian);
		void Yaw(float radian);

		void SetFov(float fov);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		const Math::Vector3& GetPosition() const { return mPosition; }
		const Math::Vector3& GetDirection() const { return mDirection; }

		Math::Matrix4 GetViewMatrix() const;
		Math::Matrix4 GetPerspectiveMatrix() const;

	private:
		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::ZAxis;

		float mFov = 60.0f*Math::Constants::DegToRad;
		float mNearPlane = 1.0f;
		float mFarPlane = 10000.0f;
		float mAspectRatio = 0.0f;

	};

}