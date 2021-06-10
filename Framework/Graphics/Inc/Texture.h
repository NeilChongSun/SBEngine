#pragma once

namespace SB::Graphics {

class Texture
{
public:
	//Texture(const Texture&) = delete;
	//Texture& operator=(const Texture&) = delete;
	void Initialize(const std::filesystem::path& fileName);
	void Terminate();

	void BindPS(uint32_t slot) const;
	void BindVS(uint32_t slot) const;

private:
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
};

} // namespace PCEngine::Graphics