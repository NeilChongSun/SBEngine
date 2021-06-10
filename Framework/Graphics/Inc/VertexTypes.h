#pragma once
#include"Common.h"
#include"Colors.h"
namespace SB::Graphics
{
	//Vertex element flags
	constexpr uint32_t VE_Position = 0x1 << 0; //0000 0001
	constexpr uint32_t VE_Normal   = 0x1 << 1; //0000 0010
	constexpr uint32_t VE_Tangent  = 0x1 << 2; //0000 0100
	constexpr uint32_t VE_Color    = 0x1 << 3; //0000 1000
	constexpr uint32_t VE_TexCoord = 0x1 << 4; //0001 0000
	constexpr uint32_t VE_BlendIndex = 0x1 << 5; //0010 0000
	constexpr uint32_t VE_BlendWeight = 0x1 << 6; //0100 0000

#define VERTEX_FORMAT(fmt)\
	static constexpr uint32_t Format=fmt

	struct VertexPC
	{
		VERTEX_FORMAT(VE_Position | VE_Color);
		Math::Vector3 position;
		Color color;
	};

	struct VertexPX
	{
		VERTEX_FORMAT(VE_Position | VE_TexCoord);
		SB::Math::Vector3 position;
		float u, v;
	};

	struct VertexPN
	{
		VERTEX_FORMAT(VE_Position | VE_Normal);
		SB::Math::Vector3 position;
		SB::Math::Vector3 normal;
	};
	struct Vertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_TexCoord);
		SB::Math::Vector3 position;
		SB::Math::Vector3 normal;
		SB::Math::Vector3 tangent;
		Math::Vector2 texcoord;
	};
	struct BoneVertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_TexCoord|VE_BlendIndex|VE_BlendWeight);
		SB::Math::Vector3 position;
		SB::Math::Vector3 normal;
		SB::Math::Vector3 tangent;
		Math::Vector2 texcoord;
		int boneIndices[4] = { 0 };
		float boneWeight[4] = { 0.0f };
	};
}//namespace SB::Graphics