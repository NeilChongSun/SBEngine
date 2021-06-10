#pragma once
#include<SB/Inc/SB.h>
class Heart : public SB::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
private:
	struct Vertex
	{
		SB::Math::Vector3 position;
		SB::Math::Vector4 color;
	};
	std::vector<Vertex> mVertices;

	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	
};
class House : public SB::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
private:
	struct Vertex
	{
		SB::Math::Vector3 position;
		SB::Math::Vector4 color;
	};
	std::vector<Vertex> mVertices;

	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	
};



class Triforce : public SB::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
private:
	struct Vertex
	{
		SB::Math::Vector3 position;
		SB::Math::Vector4 color;
	};
	std::vector<Vertex> mVertices;

	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
};

