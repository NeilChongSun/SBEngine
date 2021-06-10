#include"Precompiled.h"
#include"MeshIO.h"

using namespace SB::Graphics;

void MeshIO::Write(FILE * file, const SkinneMesh & mesh)
{
	fprintf(file, "VertexCount: %d\n", static_cast<uint32_t>(mesh.vertices.size()));
	for (auto& v : mesh.vertices)
	{
		fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
			v.position.x, v.position.y, v.position.z,
			v.normal.x, v.normal.y, v.normal.z,
			v.tangent.x, v.tangent.y, v.tangent.z,
			v.texcoord.x, v.texcoord.y,
			v.boneIndices[0], v.boneIndices[1], v.boneIndices[2], v.boneIndices[3],
			v.boneWeight[0], v.boneWeight[1], v.boneWeight[2], v.boneWeight[3]);
	}

	fprintf(file, "IndexCount: %d\n", static_cast<uint32_t>(mesh.indices.size()));
	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	{
		fprintf(file, "%i %i %i\n", mesh.indices[i], mesh.indices[i + 1], mesh.indices[i + 2]);
	}
}

void MeshIO::Read(FILE * file, SkinneMesh & mesh)
{
	uint32_t numVertices = 0;

	fscanf_s(file, "VertexCount: %d\n", &numVertices);
	mesh.vertices.resize(numVertices);
	for (size_t i = 0; i < numVertices; i++)
	{
		fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
			&mesh.vertices[i].position.x, &mesh.vertices[i].position.y, &mesh.vertices[i].position.z,
			&mesh.vertices[i].normal.x, &mesh.vertices[i].normal.y, &mesh.vertices[i].normal.z,
			&mesh.vertices[i].tangent.x, &mesh.vertices[i].tangent.y, &mesh.vertices[i].tangent.z,
			&mesh.vertices[i].texcoord.x, &mesh.vertices[i].texcoord.y,
			&mesh.vertices[i].boneIndices[0], &mesh.vertices[i].boneIndices[1], &mesh.vertices[i].boneIndices[2], &mesh.vertices[i].boneIndices[3],
			&mesh.vertices[i].boneWeight[0], &mesh.vertices[i].boneWeight[1], &mesh.vertices[i].boneWeight[2], &mesh.vertices[i].boneWeight[3]);
	}

	uint32_t numIndices = 0;
	fscanf_s(file, "IndexCount: %d\n", &numIndices);
	mesh.indices.resize(numIndices);
	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	{
		uint32_t a, b, c;
		fscanf_s(file, "%d %d %d\n", &a, &b, &c);
		mesh.indices[i] = a;
		mesh.indices[i + 1] = b;
		mesh.indices[i + 2] = c;
	}
}
