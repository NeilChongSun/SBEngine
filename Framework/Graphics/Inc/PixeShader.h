#pragma once
namespace SB::Graphics
{
	class PixeShader
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Initialize(const std::filesystem::path& filePath,const char* shaderName);
		void Terminate();
		void Bind();
	private:
		ID3D11PixelShader* mPixelShader = nullptr;
	};
}//namespace SB::Graphics