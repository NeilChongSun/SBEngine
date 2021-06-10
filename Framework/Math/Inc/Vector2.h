#pragma once

namespace SB::Math {

	struct Vector2
	{
		float x, y;

		constexpr Vector2() :Vector2{ 0.0f } {}
		constexpr Vector2(float f) noexcept :Vector2 {f,f} {}
		constexpr Vector2(float x, float y)noexcept :x{ x }, y{ y }{}

		const static Vector2 Zero;
		const static Vector2 One;
		const static Vector2 XAxis;
		const static Vector2 YAxis;

		constexpr Vector2 operator-() const { return (-x, -y); }
		//Homework: add+ - * /
		constexpr Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
		constexpr Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
		constexpr Vector2 operator*(float f) const { return Vector2(x * f, y * f); }
		constexpr Vector2 operator/(float f) const { return Vector2(x / f, y / f); }
		
		constexpr Vector2& operator+=(const Vector2&v) { x += v.x, y += v.y; return *this; }
		//Homework: add -=  *=  /=
		constexpr Vector2& operator-=(const Vector2&v) { x -= v.x, y -= v.y; return *this; }
		Vector2& operator*=(float f) { x *= f; y *= f; return *this; }
		Vector2& operator/=(float f) { x /= f; y /= f; return *this; }
	};
} //namespcace SB::Math