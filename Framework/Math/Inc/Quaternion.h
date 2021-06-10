#pragma once

namespace SB::Math {
	struct Matrix4;

	struct Quaternion
	{
		float x, y, z, w;

		constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		constexpr Quaternion(float x, float y, float z, float w) noexcept :x(x), y(y), z(z), w(w) {}
		const static Quaternion Zero;
		const static Quaternion Identity;

		static Quaternion RotationAxis(const Vector3& axis, float rad);
		static Quaternion RotationMatrix(const Matrix4& m);
		static Quaternion RotationLook(Vector3 direction, Vector3 up = Vector3::YAxis);
		static Quaternion RotationFromTo(Vector3 form, Vector3 to);

		constexpr Quaternion operator-() const
		{
			return { -x,-y,-z,-w };
		}

		constexpr Quaternion operator+(const Quaternion& q)const
		{
			return{ x + q.x,y + q.y,z + q.z,w + q.w };
		}

		constexpr Quaternion operator-(const Quaternion& q)const
		{
			return{ x - q.x,y - q.y,z - q.z,w - q.w };
		}

		constexpr Quaternion operator*(float f)const
		{
			return{ x*f,y*f,z*f,w*f };
		}

		constexpr Quaternion operator*(const Quaternion& q)const
		{
			return
			{
				(w*q.x + x * q.w + y * q.z - z * q.y),
				(w*q.y - x * q.z + y * q.w + z * q.x),
				(w*q.z + x * q.y - y * q.x + z * q.w),
				(w*q.w - x * q.x - y * q.y - z * q.z)
			};
		}

		constexpr Quaternion operator/(float f) const
		{
			return { x / f,y / f,z / f,w / f };
		}
	};
}