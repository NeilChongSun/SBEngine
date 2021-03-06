#pragma once

namespace SB::Graphics
{
	class VertexShader
	{
	public:
		void Initialize(const std::filesystem::path& filePath,uint32_t vertexFormat);
		void Initialize(const std::filesystem::path& filePath,const char* shaderName, uint32_t vertexFormat);
		void Terminate();
		void Bind();
	private:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;
	};
}//namespace SB::Graphics