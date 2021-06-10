#pragma once

namespace SB::Math {
	struct Quaternion;
	struct Matrix4
	{
		union
		{
			struct
			{
				//row + column subscripts
				float _11, _12, _13, _14;    //_11 _12 _13 _14
				float _21, _22, _23, _24;    //_21 _22 _23 _24
				float _31, _32, _33, _34;    //_31 _32 _33 _34
				float _41, _42, _43, _44;    //_41 _42 _43 _44
			};
			std::array<float, 16> v;
		};
		
		const static Matrix4 Identity;

		Matrix4 operator*(const Matrix4& m) const
		{
			return
			{
				(_11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41), (_11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42), (_11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43), (_11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44),
				(_21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41), (_21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42), (_21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43), (_21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44),
				(_31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41), (_31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42), (_31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43), (_31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44),
				(_41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41), (_41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42), (_41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43), (_41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44)
			};
		}

		Matrix4 operator*(float f) const
		{
			return
			{
				_11 * f, _12 * f, _13 * f, _14 * f,
				_21 * f, _22 * f, _23 * f, _24 * f,
				_31 * f, _32 * f, _33 * f, _34 * f,
				_41 * f, _42 * f, _43 * f, _44 * f
			};
		}


		static Matrix4 Translation(const Vector3& d)
		{
			return 
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				d.x, d.y, d.z, 1.0f
			};
		}

		static Matrix4 RotationAxis(const Vector3 & axis, float rad);
		static Matrix4 RotationQuaternion(const Quaternion& q);
		static Matrix4 RotationX(float rad) { return { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }
		static Matrix4 RotationY(float rad) { return { cosf(rad), 0.0f, -sinf(rad), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sinf(rad), 0.0f, cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }
		static Matrix4 RotationZ(float rad) { return {cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };}
		static Matrix4 Scaling(float s) { return { s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }
		static Matrix4 Scaling(float sx, float sy, float sz) { return {sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };}
		static Matrix4 Scaling(const Vector3& s) { return { s.x, 0.0f, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 0.0f, s.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }


	};
}