#include"Precomplied.h"
#include"Terrain.h"

using namespace SB;

void Terrain::Initialize(uint32_t numRows, uint32_t numCols, float cellSize)
{
	mNumRows = numRows;
	mNumCols = numCols;
	mNumCellsInCol = mNumCols - 1;
	mNumCellsInRow = mNumRows - 1;
	mNumCells = mNumCellsInCol * mNumCellsInRow;
	mCellSize = cellSize;

	mConstantBuffer.Initialize();
	mTerrainVertexShader.Initialize(L"../../Assets/Shaders/Terrain.fx", Graphics::Vertex::Format);
	mTerrainPixelShader.Initialize(L"../../Assets/Shaders/Terrain.fx");

	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mGrassTexture.Initialize("../../Assets/grass.jpg");

	GenerateVertices();
	GenerateIndices();

	mMeshBuffer.Initialize(mMesh, true);
}

void Terrain::Change(uint32_t numRows, uint32_t numCols, float cellSize)
{
	mNumRows = numRows;
	mNumCols = numCols;
	mCellSize = cellSize;
}

void Terrain::Terminate()
{
	mMeshBuffer.Terminate();
	mGrassTexture.Terminate();
	mSampler.Terminate();
	mTerrainPixelShader.Terminate();
	mTerrainVertexShader.Terminate();
	mConstantBuffer.Terminate();
}

void Terrain::LoadHeightmap(const std::filesystem::path & filePath)
{
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "rb");

	fseek(file, 0L, SEEK_END);
	uint32_t fileSize = ftell(file);
	uint32_t dimension = (uint32_t)sqrtf((float)fileSize);
	fseek(file, 0L, SEEK_SET);

	for (uint32_t z = 0; z < mNumRows&&z < dimension; z++)
	{
		for (uint32_t x = 0; x < mNumCols&&x < dimension; x++)
		{
			float height = fgetc(file) / 255.0f*mHeightScale;
			mMesh.vertices[x + ((mNumCols - z - 1)*mNumCols)].position.y = height;
		}
	}
	fclose(file);
	
	mMeshBuffer.Update(mMesh.vertices.data(), mMesh.vertices.size());
}


void Terrain::SetHeightScale(float scale)
{
	mHeightScale = scale;
}

void Terrain::SetDirectionalLight(const Graphics::DirectionalLight & light)
{
	mConstantData.directionalLight = light;
}

void Terrain::Render(const Graphics::Camera & camera)
{
	auto world = Math::Matrix4::Identity;
	auto view = camera.GetViewMatrix();
	auto projeciton = camera.GetPerspectiveMatrix();

	mConstantData.wvp = Math::Transpose(view*projeciton*world);
	mConstantData.viewPosition = camera.GetPosition();
	mConstantBuffer.Update(mConstantData);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mSampler.BindPS();
	mGrassTexture.BindPS(0);
	
	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();

	mMeshBuffer.Draw();
}

void Terrain::DrawEditorUI()
{
}

void Terrain::GenerateVertices()
{
	mMesh.vertices.clear();
	mMesh.vertices.reserve(mNumRows*mNumCols);
	for (uint32_t z = 0; z < mNumRows; z++)
	{
		for (uint32_t x = 0; x < mNumCols; x++)
		{
			auto& vertex = mMesh.vertices.emplace_back();
			vertex.position = { x*mCellSize,0.0f,z*mCellSize };
			vertex.normal = { 0.0f,1.0f,0.0f };
			vertex.tangent = { 1.0f,0.0f,0.0f };
			vertex.texcoord = { (float)x / mNumCols,(float)z / mNumRows };
		}
	}
}

void Terrain::GenerateIndices()
{
	mMesh.indices.reserve(mNumCells * 6);
	uint32_t i = 0;
	for (uint32_t z = 0; z < mNumCellsInCol; z++)
	{
		for (uint32_t x = 0; x < mNumCellsInRow; x++)
		{
			uint32_t lb = x + (z*mNumCols);
			uint32_t lt = x + ((z + 1)*mNumCols);
			uint32_t rb = (x + 1) + (z*mNumCols);
			uint32_t rt = (x + 1) + ((z + 1)*mNumCols);

			mMesh.indices.push_back(lb);
			mMesh.indices.push_back(lt);
			mMesh.indices.push_back(rb);

			mMesh.indices.push_back(lt);
			mMesh.indices.push_back(rt);
			mMesh.indices.push_back(rb);
		}
	}
}
