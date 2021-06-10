#pragma once
#include"Mesh.h"
namespace SB::Graphics
{
	class MeshBuffer
	{
	public:
		enum class Topology
		{
			Lines,
			Trangles
		};
		template<class MeshType>
		void Initialize(const MeshType& mesh,bool dynamic=false)
		{
			Initialize(
				mesh.vertices.data(),
				static_cast<int>(mesh.vertices.size()),
				mesh.indices.data(),
				static_cast<int>(mesh.indices.size()),
				dynamic
			);
		}
		template<class VertexType>
		void Initialize(const VertexType* vertices,int vertexCount, const uint32_t* indices,int indexCount,bool dynamic=false)
		{
			InitializeInternal(vertices,sizeof(VertexType),vertexCount,indices,indexCount,dynamic);
		}

		template<class VertexType>
		void Initialize(const VertexType* vertices, int vertexCount, bool dynamic=false)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, nullptr, 0, dynamic);
		}
		void Terminate();

		void SetTopology(Topology topology);
		void Update(const void* vertexData, uint32_t numVertices);
		void Draw() const;

	private:
		void InitializeInternal(const void* vertices, int vertexSize, int vertexCount, const uint32_t* indices, int indexCount, bool dynamic);
		
		ID3D11Buffer* mIndexBuffer = nullptr;
		ID3D11Buffer* mVertexBuffer = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		int mVertexSize = 0;
		int mVertexCount = 0;
		int mIndexCount=0;
	};
}//namespace SB::Graphics
