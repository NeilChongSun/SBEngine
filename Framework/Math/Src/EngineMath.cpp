#include"Precompiled.h"
#include"EngineMath.h"

using namespace SB::Math;

const Vector3 Vector3::Zero(0.0f);
const Vector3 Vector3::One(1.0f);
const Vector3 Vector3::XAxis(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::YAxis(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ZAxis(0.0f, 0.0f, 1.0f);

const Matrix4 Matrix4::Identity
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

Matrix4 Matrix4::RotationAxis(const Vector3& axis, float rad)
{
	const Vector3 u = Normalize(axis);
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sin(rad);
	const float c = cos(rad);
	Matrix4 m = {
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	};
	return m;
}
Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	return
	{
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	};
}

const Quaternion Quaternion::Zero(0.0f, 0.0f, 0.0f, 0.0f);

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion Quaternion::RotationAxis(const Vector3& axis, float rad)
{
	const float c = cos(rad*0.5f);
	const float s = sin(rad*0.5f);
	const Math::Vector3 a = Math::Normalize(axis);
	return Quaternion(a.x*s, a.y*s, a.z*s, c);
}

Quaternion Quaternion::RotationLook(Vector3 direction, Vector3 up)
{
	//Reference https://answers.unity.com/questions/467614/what-is-the-source-code-of-quaternionlookrotation.html
	Vector3 forward = Normalize(direction);
	Vector3 vector = forward;
	Vector3 vector2 = Normalize(Cross(up, vector));
	Vector3 vector3 = Cross(vector, vector2);
	float m00 = vector2.x;
	float m01 = vector2.y;
	float m02 = vector2.z;
	float m10 = vector3.x;
	float m11 = vector3.y;
	float m12 = vector3.z;
	float m20 = vector.x;
	float m21 = vector.y;
	float m22 = vector.z;

	float num8 = (m00 + m11) + m22;
	Quaternion quaternion;
	if (num8 > 0.0f)
	{
		float num = (float)sqrt(num8 + 1.0f);
		quaternion.w = num * 0.5f;
		num = 0.5f / num;
		quaternion.x = (m12 - m21) * num;
		quaternion.y = (m20 - m02) * num;
		quaternion.z = (m01 - m10) * num;
		return quaternion;
	}
	if ((m00 >= m11) && (m00 >= m22))
	{
		float num7 = (float)sqrt(((1.0f + m00) - m11) - m22);
		float num4 = 0.5f / num7;
		quaternion.x = 0.5f * num7;
		quaternion.y = (m01 + m10) * num4;
		quaternion.z = (m02 + m20) * num4;
		quaternion.w = (m12 - m21) * num4;
		return quaternion;
	}
	if (m11 > m22)
	{
		float num6 = (float)sqrt(((1.0f + m11) - m00) - m22);
		float num3 = 0.5f / num6;
		quaternion.x = (m10 + m01) * num3;
		quaternion.y = 0.5f * num6;
		quaternion.z = (m21 + m12) * num3;
		quaternion.w = (m20 - m02) * num3;
		return quaternion;
	}
	float num5 = (float)sqrt(((1.0f + m22) - m00) - m11);
	float num2 = 0.5f / num5;
	quaternion.x = (m20 + m02) * num2;
	quaternion.y = (m21 + m12) * num2;
	quaternion.z = 0.5f * num5;
	quaternion.w = (m01 - m10) * num2;
	return quaternion;
}
Quaternion Quaternion::RotationFromTo(Vector3 from, Vector3 to)
{
	Vector3 axis = Cross(from, to);
	float rad = acos(Dot(from, to) / (Magnitude(from)*Magnitude(to)));
	return RotationAxis(axis, rad);
}
Quaternion Quaternion::RotationMatrix(const Matrix4& m)
{
	//Reference: https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
	float trace = m._11 + m._22 + m._33;
	Quaternion q;
	if (trace > 0.0f)
	{
		float s = 0.5f / sqrtf(1.0f + trace);
		q = Quaternion
		(
			s*(m._23 - m._32),
			s*(m._31 - m._13),
			s*(m._12 - m._21),
			0.25f / s
		);
	}
	else
	{
		if ((m._11 > m._22) && (m._11 > m._33))
		{
			float s = 0.5f / sqrtf(1.0f + m._11 - m._22 - m._33);
			q = Quaternion
			(
				0.25f*s,
				s*(m._21 + m._12),
				s*(m._32 + m._13),
				s*(m._23 - m._32)
			);
		}
		else if (m._22 > m._33)
		{
			float s = 0.5f / sqrt(1.0f + m._22 - m._11 - m._33);
			q = Quaternion
			(
				s * (m._31 - m._13),
				s * (m._21 + m._12),
				0.25f / s,
				s * (m._32 + m._23)
			);
		}
		else
		{
			float s = 0.5f / sqrt(1.0f + m._33 - m._11 - m._22);
			q = Quaternion
			(
				s * (m._12 - m._21),
				s * (m._31 + m._13),
				s * (m._32 + m._23),
				0.25f / s
			);
		}
	}
	return q;
}

bool SB::Math::Intersect(const Ray& ray, const Plane& plane, float& distance)
{
	const float orgDotN = Dot(ray.origin, plane.n);
	const float dirDotN = Dot(ray.direction, plane.n);

	// Check if ray is parallel to the plane
	if (Abs(dirDotN) < 0.0001f)
	{
		if (Abs(orgDotN - plane.d) < 0.0001f)
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;
	return true;

}

bool SB::Math::IsContained(const Vector3& point, const AABB& aabb)
{
	auto min = aabb.Min();
	auto max = aabb.Max();
	if (point.x<min.x || point.x>max.x || point.y<min.y || point.y>max.y || point.z<min.z || point.z>max.z)
	{
		return false;
	}
	return true;
}

bool SB::Math::IsContained(const Vector3& point, const OBB& obb)
{
	//Compute the world-local matrices
	Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
	Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rot);
	Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
	Matrix4 toWorld = matScale * matRot*matTrans;
	Matrix4 toLocal = Inverse(toWorld);

	//Transform the point into the OBB's local space
	Vector3 localPoint = Transformcoord(point, toLocal);

	//Test against local AABB
	return IsContained(localPoint, AABB{ Vector3::Zero,Vector3::One });
}

bool SB::Math::GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal)
{
	//Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
	Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rot);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	//Transfor the ray into the OBB's local space
	Vector3 org = Transformcoord(ray.origin, matWorldInv);
	Vector3 dir = TransformNormal(ray.direction, matWorldInv);
	Ray localRay{ org,dir };

	Plane planes[] =
	{
		{ {  0.0f,  0.0f, -1.0f}, obb.extend.z },
		{ {  0.0f,  0.0f,  1.0f}, obb.extend.z },
		{ {  0.0f, -1.0f,  0.0f}, obb.extend.y },
		{ {  0.0f,  1.0f,  0.0f}, obb.extend.y },
		{ { -1.0f,  0.0f,  0.0f}, obb.extend.x },
		{ {  1.0f,  0.0f,  0.0f}, obb.extend.x }
	};

	uint32_t numIntersections = 0;
	for (uint32_t i = 0; i < 6; i++)
	{
		const float d = Dot(org, planes[i].n);
		if (d>planes[i].d)
		{
			float distance = 0.0f;
			if (Intersect(localRay,planes[i],distance)&&distance>=0.0f)
			{
				Vector3 pt = org + (dir*distance);
				if (abs(pt.x) <= obb.extend.x  +0.01f&&
					abs(pt.y) <= obb.extend.y + 0.01f&&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = planes[i].n;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections==0)
	{
		return false;
	}

	point = Transformcoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;
}

