#pragma once
namespace SB::Math
{
	struct OBB
	{
		Vector3 center = Vector3::Zero;
		Vector3 extend = Vector3::One;
		Quaternion rot = Quaternion::Identity;
	};
}