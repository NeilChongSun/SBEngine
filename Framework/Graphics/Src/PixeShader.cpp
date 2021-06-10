#include"Precompiled.h"
#include"PixeShader.h"
#include"D3DUtil.h"
using namespace SB;
using namespace SB::Graphics;




void PixeShader::Initialize(const std::filesystem::path& filePath,const char* shaderName)
{
	DWORD shaderFlages = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		filePath.wstring().c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		shaderName, "ps_5_0",
		shaderFlages, 0, &shaderBlob, &errorBlob);
	if (errorBlob&&errorBlob->GetBufferPointer())

		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to Compile pixel shader.");
	auto device = GetDevice();
	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void PixeShader::Initialize(const std::filesystem::path& filePath)
{
	Initialize(filePath, "PS");
}

void SB::Graphics::PixeShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void SB::Graphics::PixeShader::Bind()
{
	auto context = GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}
