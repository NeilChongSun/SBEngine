#pragma once
#include"Mesh.h"
namespace SB::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshPX CreatePlanePX(float width, float height);
		static MeshPX CreateCylinderPX(float height, float r,int rings);
		static MeshPX CreateSpherePX(int slices, float r, int rings,bool isInside);	
		static MeshPN CreateCylinderPN(float height, float r,int rings);
		static MeshPN CreateSpherePN(int slices, float r, int rings);
		static Mesh CreateSphere(int slices, float r, int rings);
		static Mesh CreateSphere(int rings, int slices);
		static MeshPX CreateNDCQuad();
		static Mesh CreatePlane(float size, int rows = 16, int columns = 16);
	};
}