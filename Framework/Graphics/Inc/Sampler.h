#pragma once

namespace SB::Graphics {

class Sampler
{
public:
	enum class Filter { Point, Linear, Anisotropic };
	enum class AddressMode { Border, Clamp, Mirror, Wrap };

	void Initialize(Filter filter, AddressMode addressMode);
	void Terminate();

	void BindVS() const;
	void BindPS() const;

private:
	ID3D11SamplerState* mSampler = nullptr;
};

} // namespace PCEngine::Graphics