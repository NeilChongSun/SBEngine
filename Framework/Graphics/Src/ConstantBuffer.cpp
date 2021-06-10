#include"Precompiled.h"
#include"ConstantBUffer.h"
#include"D3DUtil.h"
using namespace SB;
using namespace SB::Graphics;
using namespace SB::Math;

void SB::Graphics::ConstantBuffer::Initialize(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = bufferSize;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	auto device = GetDevice();
	HRESULT hr = device->CreateBuffer(&constantBufferDesc, nullptr, &mBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

}

void SB::Graphics::ConstantBuffer::Terminate()
{
	SafeRelease(mBuffer);
}

void SB::Graphics::ConstantBuffer::BindVS(uint32_t slot) const
{
	GetContext()->VSSetConstantBuffers(slot, 1, &mBuffer);
}

void SB::Graphics::ConstantBuffer::BindPS(uint32_t slot) const
{
	GetContext()->PSSetConstantBuffers(slot, 1, &mBuffer);
}

void SB::Graphics::ConstantBuffer::Update(const void * data) const
{
	GetContext()->UpdateSubresource(mBuffer, 0, nullptr, data, 0, 0);
}
