#pragma once

namespace SB::Math {

	struct Vector4
	{
		float r, g, b, w;

		constexpr Vector4() :Vector4{ 0.0f } {}
		constexpr Vector4(float f) noexcept :Vector4{ f,f,f,f } {}
		constexpr Vector4(float r, float g, float b, float w)noexcept :r{ r }, g{ g }, b{ b }, w{ w }{}

		const static Vector4 Zero;
		const static Vector4 One;
		const static Vector4 R;
		const static Vector4 G;
		const static Vector4 B;
		const static Vector4 W;
	};
} //namespcace SB::Math#pragma once
