#include"MeshBuilder.h"
using namespace SB::Graphics;
using namespace SB::Math;
using namespace SB::Math::Constants;
MeshPX MeshBuilder::CreatePlanePX(float width, float height)
{
	MeshPX mMesh;
	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			mMesh.vertices.push_back(VertexPX{ Math::Vector3(x,-y,0.0f), static_cast<float>(x / width), static_cast<float>(y / height) });
		}
	}

	int index = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mMesh.indices.push_back(0 + index);
			mMesh.indices.push_back(1 + index);
			mMesh.indices.push_back(width + 2 + index);

			mMesh.indices.push_back(0 + index);
			mMesh.indices.push_back(width + 2 + index);
			mMesh.indices.push_back(width + 1 + index);
			index++;
		}
		index++;
	}
	return mMesh;
}

MeshPX MeshBuilder::CreateCylinderPX(float height, float r, int rings)
{
	MeshPX mMesh;
	for (int y = 0; y <= height; y++)
	{
		for (float theta = 0; theta <= TwoPi; theta += TwoPi / rings)
		{
			mMesh.vertices.push_back(VertexPX{ Math::Vector3(cosf(theta)*r,-y,sinf(theta)*r), static_cast<float>(theta / TwoPi), static_cast<float>(y / height) });
		}
	}

	int index = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < rings; x++)
		{
			mMesh.indices.push_back(0 + index);
			mMesh.indices.push_back(1 + index);
			mMesh.indices.push_back(rings + 2 + index);

			mMesh.indices.push_back(0 + index);
			mMesh.indices.push_back(rings + 2 + index);
			mMesh.indices.push_back(rings + 1 + index);

			index++;
		}
		index++;
	}
	return mMesh;
}
MeshPX MeshBuilder::CreateSpherePX(int slice, float radius, int ring, bool isInside)
{
	std::vector<VertexPX> vertices;
	std::vector<uint32_t> indices;
	float area = Constants::TwoPi * radius * static_cast<float>(ring);

	float phiInc = Constants::Pi / static_cast<float>(ring);
	float thetaInc = Constants::TwoPi / static_cast<float>(slice);

	for (int i = 0; i <= ring; ++i)
	{
		for (int j = 0; j <= slice; ++j)
		{
			float phi = i * phiInc;
			float theta = j * thetaInc;
			vertices.emplace_back(VertexPX{ Vector3{
				sinf(theta) * radius*sinf(phi),
				cosf(phi) * radius,
				-cosf(theta) * radius *sinf(phi)},
				 static_cast<float>(theta / Constants::TwoPi), 
				 static_cast<float>(phi / Constants::Pi) });
		}
	}

	if (!isInside)
	{
		for (size_t y = 0; y < ring; ++y)
		{
			for (size_t x = 0; x < slice; ++x)
			{
				auto base = x + (y * slice + y);
				indices.push_back(base);
				indices.push_back(base + slice + 2);
				indices.push_back(base + slice + 1);

				indices.push_back(base);
				indices.push_back(base + 1);
				indices.push_back(base + slice + 2);
			}
		}
	}
	else
	{
		for (size_t y = 0; y < ring; ++y)
		{
			for (size_t x = 0; x < slice; ++x)
			{
				auto base = x + (y * slice + y);
				indices.push_back(base);
				indices.push_back(base + slice + 1);
				indices.push_back(base + slice + 2);

				indices.push_back(base);
				indices.push_back(base + slice + 2);
				indices.push_back(base + 1);
			}
		}
	}

	MeshPX mesh;
	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);

	return mesh;
}

MeshPN MeshBuilder::CreateSpherePN(int slices, float r, int rings)
{
	MeshPN mMesh;
	for (float phi = 0; phi < TwoPi; phi += TwoPi / rings)
	{
		for (float theta = 0; theta <= TwoPi; theta += TwoPi / slices)
		{
			mMesh.vertices.push_back(VertexPN{ Math::Vector3(cosf(theta)*r*sinf(phi),cosf(phi)*r,sinf(theta)*r*sinf(phi)),Math::Normalize(Math::Vector3(cosf(theta)*r*sinf(phi),cosf(phi)*r,sinf(theta)*r*sinf(phi))) });
		}
	}

	int index = 0;
	for (int y = 0; y < rings; y++)
	{
		for (int x = 0; x < slices; x++)
		{
			mMesh.indices.push_back(0 + index);
			mMesh.indices.push_back(1 + index);
			mMesh.indices.push_back(slices + 2 + index);

			mMesh.indices.push_back(0 + index);
			mMesh.indices.push_back(slices + 2 + index);
			mMesh.indices.push_back(slices + 1 + index);

			index++;
		}
		index++;
	}
	return mMesh;
}

Mesh MeshBuilder::CreateSphere(int rings, int slices)
{
	Mesh mesh;

	const float radius = 1.0f;
	const float thetaStep = Math::Constants::Pi / (rings - 1);
	const float phiStep = Math::Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (int i = 0; i < rings; ++i)
	{
		for (int j = 0; j <= slices; ++j)
		{
			float theta = thetaStep * i;
			float phi = phiStep * j;
			float r = sinf(theta);
			float x = cosf(phi) * r;
			float z = sinf(phi) * r;
			float y = -cosf(theta);

			Math::Vector3 normal = { x, y, z };
			Math::Vector3 tangent = Math::Normalize({ -z, 0.0f, x });
			Math::Vector2 uv = { j * uStep, 1.0f - i * vStep };

			mesh.vertices.emplace_back(
				Vertex{
					normal * radius,
					normal,
					tangent,
					uv
				});
		}
	}

	for (int i = 0; i < rings; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			mesh.indices.push_back((j + 0) + ((i + 0) * (slices + 1)));
			mesh.indices.push_back((j + 0) + ((i + 1) * (slices + 1)));
			mesh.indices.push_back((j + 1) + ((i + 1) * (slices + 1)));

			mesh.indices.push_back((j + 0) + ((i + 0) * (slices + 1)));
			mesh.indices.push_back((j + 1) + ((i + 1) * (slices + 1)));
			mesh.indices.push_back((j + 1) + ((i + 0) * (slices + 1)));
		}
	}
	return mesh;
}


Mesh MeshBuilder::CreateSphere(int slices, float r, int rings)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	float phiInc = Math::Constants::Pi / static_cast<float>(rings);
	float thetaInc = Math::Constants::TwoPi / static_cast<float>(slices);
	for (int i = 0; i <= rings; ++i)
	{
		float phi = i * phiInc;
		for (int j = 0; j <= slices; ++j)
		{
			float theta = j * thetaInc;
			Math::Vector3 pos = Math::Vector3{ sinf(theta) * r*sinf(phi),cosf(phi) * r,-cosf(theta) * r *sinf(phi) };
			vertices.emplace_back
			(
				Vertex{ pos,
				Math::Normalize(pos),
				Math::Normalize(Math::Vector3{-pos.z,0.0f,pos.x }),
				Math::Vector2(static_cast<float>(theta / TwoPi), static_cast<float>(phi / Pi)) });
		}
	}

	for (size_t y = 0; y < rings; ++y)
	{
		for (size_t x = 0; x < slices; ++x)
		{
			auto base = x + (y * slices + y);
			indices.push_back(base);
			indices.push_back(base + slices + 2);
			indices.push_back(base + slices + 1);

			indices.push_back(base);
			indices.push_back(base + 1);
			indices.push_back(base + slices + 2);
		}
	}

	Mesh mesh;
	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);

	return mesh;
}

MeshPX MeshBuilder::CreateNDCQuad()
{
	std::vector<VertexPX> vertices;
	std::vector<uint32_t> indices;
	VertexPX topLeft = { Math::Vector3(-1.0f,1.0f,0.0f),0.0f,0.0f };
	VertexPX topRight = { Math::Vector3(1.0f,1.0f,0.0f),1.0f,0.0f };
	VertexPX bottomLeft = { Math::Vector3(-1.0f,-1.0f,0.0f),0.0f,1.0f };
	VertexPX bottomRight = { Math::Vector3(1.0f,-1.0f,0.0f),1.0f,1.0f };

	vertices.push_back(topLeft);
	vertices.push_back(topRight);
	vertices.push_back(bottomLeft);
	vertices.push_back(bottomRight);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	MeshPX mesh;
	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);

	return mesh;
}

Mesh MeshBuilder::CreatePlane(float size, int row, int column)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	float xStep = size / static_cast<float>(row - 1);
	float zStep = size / static_cast<float>(column - 1);

	float uStep = 1.0f / static_cast<float>(row - 1);
	float vStep = 1.0f / static_cast<float>(column - 1);

	for (int z = 0; z <= column; ++z)
	{
		for (int x = 0; x <= row; ++x)
		{
			float xx = x * xStep;
			float zz = z * zStep;
			Vector3 position = Vector3{ zz, 0.0f, xx };
			Vector3 normal = Vector3::YAxis;
			Vector3 tangent = Vector3::XAxis;

			vertices.emplace_back(Vertex{
				position,
				normal,
				tangent,
				Vector2{ static_cast<float>(x * uStep), static_cast<float>(z * vStep)} });


			if ((x != row) && (z != column))
			{
				auto base = x + (z * row + z);
				indices.push_back(base);
				indices.push_back(base + row + 2);
				indices.push_back(base + row + 1);

				indices.push_back(base);
				indices.push_back(base + 1);
				indices.push_back(base + row + 2);
			}
		}
	}

	Mesh mesh;
	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);

	return mesh;

}

