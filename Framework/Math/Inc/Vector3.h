#pragma once

namespace SB::Math {

	struct Vector3
	{
		float x, y, z;

		constexpr Vector3() :Vector3{ 0.0f } {}
		constexpr Vector3(float f) noexcept :Vector3{ f,f,f } {}
		constexpr Vector3(float x, float y, float z)noexcept :x{ x }, y{ y }, z{ z }{}

		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;

		constexpr Vector3 operator-() const { return (-x, -y, -z); }
		//Homework: add+ - * /
		constexpr Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
		constexpr Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
		constexpr Vector3 operator*(float f) const { return Vector3(x * f, y * f, z * f); }
		constexpr Vector3 operator/(float f) const { return Vector3(x / f, y / f, z / f); }
		
		constexpr Vector3& operator+=(const Vector3&v) { x += v.x, y += v.y, z += v.z; return *this; }
		//Homework: add -=  *=  /=
		constexpr Vector3& operator-=(const Vector3&v) { x -= v.x, y -= v.y, z -= v.z; return *this; }
		Vector3& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }
		Vector3& operator/=(float f) { x /= f; y /= f; z /= f; return *this; }
	};
} //namespcace SB::Math