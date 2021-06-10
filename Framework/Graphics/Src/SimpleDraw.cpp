#include"Precompiled.h"
#include"SimpleDraw.h"
#include"MeshBuffer.h"
#include"PixeShader.h"
#include"ConstantBuffer.h"
#include"VertexShader.h"
using namespace SB;
using namespace SB::Graphics;
namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount)
		{
			mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
			mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
			mConstantBuffer.Initialize(sizeof(Math::Matrix4));
			mMeshBuffer.Initialize<VertexPC>(nullptr, maxVertexCount, true);
			mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
			mVertexCount = 0;
			mMaxVertexCount = maxVertexCount;
		}

		void Terminate()
		{
			mMeshBuffer.Terminate();
			mConstantBuffer.Terminate();
			mPixelShader.Terminate();
			mVertexShader.Terminate();
		}

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
		{
			if (mVertexCount + 2 <= mMaxVertexCount)
			{
				mLineVertices[mVertexCount++] = VertexPC{ v0, color };
				mLineVertices[mVertexCount++] = VertexPC{ v1, color };
			}
		}

		void Render(const Camera& camera)
		{
			auto matView = camera.GetViewMatrix();
			auto matProj = camera.GetPerspectiveMatrix();
			auto transform = Math::Transpose(matView * matProj);
			mConstantBuffer.Update(&transform);
			mConstantBuffer.BindVS();

			mVertexShader.Bind();
			mPixelShader.Bind();

			mMeshBuffer.Update(mLineVertices.get(), mVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Draw();

			mVertexCount = 0;
		}

	private:
		VertexShader mVertexShader;
		PixeShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices;
		uint32_t mVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color& color)
{
	const float minX = center.x - extend.x;
	const float minY = center.y - extend.y;
	const float minZ = center.z - extend.z;
	const float maxX = center.x + extend.x;
	const float maxY = center.y + extend.y;
	const float maxZ = center.z + extend.z;

	sInstance->AddLine({ minX, minY, minZ }, { minX, minY, maxZ }, color);
	sInstance->AddLine({ minX, minY, maxZ }, { maxX, minY, maxZ }, color);
	sInstance->AddLine({ maxX, minY, maxZ }, { maxX, minY, minZ }, color);
	sInstance->AddLine({ maxX, minY, minZ }, { minX, minY, minZ }, color);

	sInstance->AddLine({ minX, minY, minZ }, { minX, maxY, minZ }, color);
	sInstance->AddLine({ minX, minY, maxZ }, { minX, maxY, maxZ }, color);
	sInstance->AddLine({ maxX, minY, maxZ }, { maxX, maxY, maxZ }, color);
	sInstance->AddLine({ maxX, minY, minZ }, { maxX, maxY, minZ }, color);

	sInstance->AddLine({ minX, maxY, minZ }, { minX, maxY, maxZ }, color);
	sInstance->AddLine({ minX, maxY, maxZ }, { maxX, maxY, maxZ }, color);
	sInstance->AddLine({ maxX, maxY, maxZ }, { maxX, maxY, minZ }, color);
	sInstance->AddLine({ maxX, maxY, minZ }, { minX, maxY, minZ }, color);
}

void SimpleDraw::AddOBB(const Math::OBB& obb, const Color& color)
{
	Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
	Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rot);
	Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
	Math::Matrix4 toWorld = matScale * matRot*matTrans;

	Math::Vector3 points[] =
	{
		//Front quad
		Math::Vector3(-1.0f,-1.0f,-1.0f),
		Math::Vector3(-1.0f,1.0f,-1.0f),
		Math::Vector3(+1.0f,1.0f,-1.0f),
		Math::Vector3(+1.0f,-1.0f,-1.0f),
		//Back quad
		Math::Vector3(-1.0f,-1.0f,1.0f),
		Math::Vector3(-1.0f,1.0f,1.0f),
		Math::Vector3(1.0f,1.0f,1.0f),
		Math::Vector3(1.0f,-1.0f,1.0f),
	};

	for (auto& p : points)
		p = Math::Transformcoord(p, toWorld);

	sInstance->AddLine(points[0], points[1], color);
	sInstance->AddLine(points[1], points[2], color);
	sInstance->AddLine(points[2], points[3], color);
	sInstance->AddLine(points[3], points[0], color);

	sInstance->AddLine(points[0], points[4], color);
	sInstance->AddLine(points[1], points[5], color);
	sInstance->AddLine(points[2], points[6], color);
	sInstance->AddLine(points[3], points[7], color);
	
	sInstance->AddLine(points[4], points[5], color);
	sInstance->AddLine(points[5], points[6], color);
	sInstance->AddLine(points[6], points[7], color);
	sInstance->AddLine(points[7], points[4], color);
}

void SimpleDraw::AddCone(const Math::Vector3& base, const Math::Vector3& direction, float radius, const Color& color)
{
	constexpr int sectors = 8;
	auto side = Math::Normalize(Math::Cross(base, direction)) * radius;
	auto angle = 0.0f;
	auto angleStep = Math::Constants::TwoPi / sectors;

	for (int i = 0; i < sectors; ++i)
	{
		auto matRot0 = Math::Matrix4::RotationAxis(direction, angle);
		auto matRot1 = Math::Matrix4::RotationAxis(direction, angle + angleStep);
		auto v0 = Math::TransformNormal(side, matRot0);
		auto v1 = Math::TransformNormal(side, matRot1);
		sInstance->AddLine(base + v0, base + direction, color);
		sInstance->AddLine(base + v0, base + v1, color);
		angle += angleStep;
	}
}

void SimpleDraw::AddCylinder(const Math::Vector3& base, const Math::Vector3& direction, float radius, const Color& color)
{
	constexpr int sectors = 8;
	auto side = Math::Normalize(Math::Cross(base, direction)) * radius;
	auto angle = 0.0f;
	auto angleStep = Math::Constants::TwoPi / sectors;

	for (int i = 0; i < sectors; ++i)
	{
		auto matRot0 = Math::Matrix4::RotationAxis(direction, angle);
		auto matRot1 = Math::Matrix4::RotationAxis(direction, angle + angleStep);
		auto v0 = Math::TransformNormal(side, matRot0);
		auto v1 = Math::TransformNormal(side, matRot1);
		sInstance->AddLine(base + v0, base + direction + v0, color);
		sInstance->AddLine(base + v0, base + v1, color);
		sInstance->AddLine(base + direction + v0, base + direction + v1, color);
		angle += angleStep;
	}
}

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, int rings, int slices)
{
	const float thetaStep = Math::Constants::Pi / (rings - 1);
	const float phiStep = Math::Constants::TwoPi / slices;

	std::vector<Math::Vector3> vertices;

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
			Math::Vector3 arm{ x * radius, y * radius, z * radius };
			vertices.emplace_back(center + arm);
		}
	}

	for (int i = 0; i + 1 < rings; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			sInstance->AddLine(
				vertices[(j + 0) + ((i + 0) * (slices + 1))],
				vertices[(j + 0) + ((i + 1) * (slices + 1))],
				color);
			sInstance->AddLine(
				vertices[(j + 0) + ((i + 0) * (slices + 1))],
				vertices[(j + 1) + ((i + 0) * (slices + 1))],
				color);
		}
	}
}

void SimpleDraw::AddTransform(const Math::Matrix4& transform)
{
	auto r = Math::GetRight(transform);
	auto u = Math::GetUp(transform);
	auto l = Math::GetLook(transform);
	auto p = Math::GetTranslation(transform);
	sInstance->AddLine(p, p + r, Colors::Red);
	sInstance->AddLine(p, p + u, Colors::Green);
	sInstance->AddLine(p, p + l, Colors::Blue);
}

void SimpleDraw::AddBone(const Math::Matrix4& transform)
{
	auto r = Math::GetRight(transform);
	auto u = Math::GetUp(transform);
	auto l = Math::GetLook(transform);
	auto p = Math::GetTranslation(transform);
	AddSphere(p, 0.025f, Colors::BlueViolet, 5, 6);
	sInstance->AddLine(p, p + r * 0.1f, Colors::Red);
	sInstance->AddLine(p, p + u * 0.1f, Colors::Green);
	sInstance->AddLine(p, p + l * 0.1f, Colors::Blue);
}

void SimpleDraw::AddGroundPlane(float size, const Color& color)
{
	const float halfSize = size * 0.5f;
	for (float i = -halfSize; i <= halfSize; i += 1.0f)
	{
		sInstance->AddLine({ i, 0.0f, -halfSize }, { i, 0.0f, halfSize }, color);
		sInstance->AddLine({ -halfSize, 0.0f, i }, { halfSize, 0.0f, i }, color);
	}
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}