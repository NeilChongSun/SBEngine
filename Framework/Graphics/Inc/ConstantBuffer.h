#pragma once

namespace SB::Graphics
{
	class ConstantBuffer
	{
	public:
		void Initialize(uint32_t bufferSize);
		void Terminate();
		void Update(const void* data) const ;
		void BindVS(uint32_t slot=0) const;
		void BindPS(uint32_t slot=0) const;
	private:
		ID3D11Buffer* mBuffer = nullptr;
	};

	template<class T>
	class TypedConstantBuffer : public ConstantBuffer
	{
	public:
		using DataType = T;
		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0,
				"[TypedConstantBuffer] data type is must be 16 byte to aligned.");
			ConstantBuffer::Initialize(sizeof(DataType));
		}

		void Initialize(const DataType& data)
		{
			ConatantBuffer::Initialize(sizeof(DataType), &data);
		}

		void Update(const DataType& data) const
		{
			ConstantBuffer::Update(&data);
		}
	};



}//namespace SB::Graphics
