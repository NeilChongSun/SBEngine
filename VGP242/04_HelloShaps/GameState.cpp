#include "GameState.h"
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;
//Heart--------------------------------------------------------------
void Heart::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	//NDC -Normalized Device Coordiante
	float halfHigh = sqrt(3.0f) / 2 * 0.8;
	//mid
	mVertices.emplace_back(Vertex{ Vector3(-0.2f, halfHigh, 0.0f),Vector4(Colors::LightGray) });
	mVertices.emplace_back(Vertex{ Vector3(0.2f, halfHigh, 0.0f),Vector4(Colors::LightGray) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, halfHigh / 2, 0.0f),Vector4(Colors::LightGray) });
	//left
	mVertices.emplace_back(Vertex{ Vector3(-1.0f, halfHigh, 0.0f),Vector4(Colors::LightGray) });
	mVertices.emplace_back(Vertex{ Vector3(-0.6f, halfHigh, 0.0f),Vector4(Colors::LightGray) });
	mVertices.emplace_back(Vertex{ Vector3(-0.8f, halfHigh / 2, 0.0f),Vector4(Colors::LightGray) });

	//right
	mVertices.emplace_back(Vertex{ Vector3(0.6f, halfHigh, 0.0f),Vector4(Colors::LightGray) });
	mVertices.emplace_back(Vertex{ Vector3(1.0f, halfHigh, 0.0f),Vector4(Colors::LightGray) });
	mVertices.emplace_back(Vertex{ Vector3(0.8f, halfHigh / 2, 0.0f),Vector4(Colors::LightGray) });
	//trangle
	mVertices.emplace_back(Vertex{ Vector3(0.0f, -halfHigh, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(-1.0f, halfHigh, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(1.0f, halfHigh, 0.0f),Vector4(Colors::Red) });

	auto device = GraphicsSystem::Get()->GetDevice();

	//Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = mVertices.size() * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	//Compile and create vertex shader
	DWORD shaderFlages = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlages, 0, &shaderBlob, &errorBlob);
	if (errorBlob&&errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({
		"POSITION",0,
		DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,0 });

	vertexLayout.push_back({
		"COLOR",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,0 });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failee to create vertex layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlages, 0, &shaderBlob, &errorBlob);
	if (errorBlob&&errorBlob->GetBufferPointer())

		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failee to Compile pixel shader.");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void Heart::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void Heart::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
		SB::MainApp().ChangeState("Heart");
	if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
		SB::MainApp().ChangeState("House");
	if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
		SB::MainApp().ChangeState("Triforce");
}

void Heart::Render()
{

	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->Draw(mVertices.size(), 0);
}
//House
void House::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	//NDC -Normalized Device Coordiante
	//proff1
	mVertices.emplace_back(Vertex{ Vector3(-0.3f, 0.0f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(-0.2f, 0.2f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(-0.1f, 0.0f, 0.0f),Vector4(Colors::Red) });

	mVertices.emplace_back(Vertex{ Vector3(-0.2f, 0.2f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, 0.2f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(-0.1f, 0.0f, 0.0f),Vector4(Colors::Red) });

	mVertices.emplace_back(Vertex{ Vector3(-0.1f, 0.0f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, 0.2f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.1f, 0.0f, 0.0f),Vector4(Colors::Red) });

	mVertices.emplace_back(Vertex{ Vector3(0.0f, 0.2f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.2f, 0.2f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.1f, 0.0f, 0.0f),Vector4(Colors::Red) });
	//proff2
	mVertices.emplace_back(Vertex{ Vector3(0.1f, 0.0f, 0.0f),Vector4(Colors::Gray) });
	mVertices.emplace_back(Vertex{ Vector3(0.2f, 0.2f, 0.0f),Vector4(Colors::Gray) });
	mVertices.emplace_back(Vertex{ Vector3(0.3f, 0.0f, 0.0f),Vector4(Colors::Gray) });
	//wall1
	mVertices.emplace_back(Vertex{ Vector3(-0.3f, -0.3f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(-0.3f, 0.0f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, -0.3f, 0.0f),Vector4(Colors::Yellow) });

	mVertices.emplace_back(Vertex{ Vector3(0.0f, -0.3f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(-0.3f, 0.0f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, 0.0f, 0.0f),Vector4(Colors::Yellow) });

	mVertices.emplace_back(Vertex{ Vector3(0.0f, 0.0f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(0.1f, 0.0f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(0.1f, -0.3f, 0.0f),Vector4(Colors::Yellow) });

	mVertices.emplace_back(Vertex{ Vector3(0.0f, -0.15f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, 0.0f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(0.05f, -0.15f, 0.0f),Vector4(Colors::Yellow) });

	mVertices.emplace_back(Vertex{ Vector3(0.05f, -0.3f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(0.05f, -0.15f, 0.0f),Vector4(Colors::Yellow) });
	mVertices.emplace_back(Vertex{ Vector3(0.1f, -0.3f, 0.0f),Vector4(Colors::Yellow) });
	//wall2
	mVertices.emplace_back(Vertex{ Vector3(0.3f, -0.3f, 0.0f),Vector4(Colors::YellowGreen) });
	mVertices.emplace_back(Vertex{ Vector3(0.1f, 0.0f, 0.0f),Vector4(Colors::YellowGreen) });
	mVertices.emplace_back(Vertex{ Vector3(0.3f, 0.0f, 0.0f),Vector4(Colors::YellowGreen) });

	mVertices.emplace_back(Vertex{ Vector3(0.1f, -0.3f, 0.0f),Vector4(Colors::YellowGreen) });
	mVertices.emplace_back(Vertex{ Vector3(0.1f, 0.0f, 0.0f),Vector4(Colors::YellowGreen) });
	mVertices.emplace_back(Vertex{ Vector3(0.3f, -0.3f, 0.0f),Vector4(Colors::YellowGreen) });
	//door
	mVertices.emplace_back(Vertex{ Vector3(0.0f, -0.3f, 0.0f),Vector4(Colors::Brown) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, -0.15f, 0.0f),Vector4(Colors::Brown) });
	mVertices.emplace_back(Vertex{ Vector3(0.05f, -0.3f, 0.0f),Vector4(Colors::Brown) });

	mVertices.emplace_back(Vertex{ Vector3(0.0f, -0.15f, 0.0f),Vector4(Colors::Brown) });
	mVertices.emplace_back(Vertex{ Vector3(0.05f, -0.15f, 0.0f),Vector4(Colors::Brown) });
	mVertices.emplace_back(Vertex{ Vector3(0.05f, -0.3f, 0.0f),Vector4(Colors::Brown) });






	auto device = GraphicsSystem::Get()->GetDevice();

	//Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = mVertices.size() * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	//Compile and create vertex shader
	DWORD shaderFlages = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlages, 0, &shaderBlob, &errorBlob);
	if (errorBlob&&errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({
		"POSITION",0,
		DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,0 });

	vertexLayout.push_back({
		"COLOR",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,0 });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failee to create vertex layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlages, 0, &shaderBlob, &errorBlob);
	if (errorBlob&&errorBlob->GetBufferPointer())

		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failee to Compile pixel shader.");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void House::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void House::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
		SB::MainApp().ChangeState("Heart");
	if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
		SB::MainApp().ChangeState("House");
	if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
		SB::MainApp().ChangeState("Triforce");
}

void House::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->Draw(mVertices.size(), 0);
}
//Triforce--------------------------------------------------------------
void Triforce::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	//NDC -Normalized Device Coordiante
	//top
	mVertices.emplace_back(Vertex{ Vector3(0.0f, sqrt(0.75f) / 2, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.25f, 0.0f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(-0.25f, 0.0f, 0.0f),Vector4(Colors::Red) });
	//left
	mVertices.emplace_back(Vertex{ Vector3(-0.25f, 0.0f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, -sqrt(0.75f) / 2, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(-0.5f, -sqrt(0.75f) / 2, 0.0f),Vector4(Colors::Red) });
	//right
	mVertices.emplace_back(Vertex{ Vector3(0.25f, 0.0f, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.5f, -sqrt(0.75f) / 2, 0.0f),Vector4(Colors::Red) });
	mVertices.emplace_back(Vertex{ Vector3(0.0f, -sqrt(0.75f) / 2, 0.0f),Vector4(Colors::Red) });

	auto device = GraphicsSystem::Get()->GetDevice();
	//Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = mVertices.size() * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
	//Compile and create vertex shader
	DWORD shaderFlages = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlages, 0, &shaderBlob, &errorBlob);
	if (errorBlob&&errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({
		"POSITION",0,
		DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,0 });

	vertexLayout.push_back({
		"COLOR",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,0 });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failee to create vertex layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlages, 0, &shaderBlob, &errorBlob);
	if (errorBlob&&errorBlob->GetBufferPointer())

		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failee to Compile pixel shader.");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void Triforce::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void Triforce::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
		SB::MainApp().ChangeState("Heart");
	if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
		SB::MainApp().ChangeState("House");
	if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
		SB::MainApp().ChangeState("Triforce");
}

void Triforce::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->Draw(mVertices.size(), 0);
}
