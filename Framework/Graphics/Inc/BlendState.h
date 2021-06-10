#pragma once

namespace SB::Graphics
{
	class BlendState
	{
	public:
		enum Mode
		{
			Opaque,
			AlphaBlend,
			AlphaPremultiplied,
			Additive
		};
		BlendState() = default;
		~BlendState();

		static void ClearState();

		BlendState(const BlendState&) = delete;
		BlendState& operator=(const BlendState&) = delete;

		void Initialize(Mode mode);
		void Terminate();
		void Bind() const;
		void Set();

	private:
		ID3D11BlendState* mBlendState = nullptr;
	};
}