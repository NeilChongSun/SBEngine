#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.001f);
	mCamera.SetPosition({ 0.0f, 0.0f, -5.0f });
	mCamera.SetDirection({ 0.0f, 0.0f, 1.0f });
	mMesh = MeshBuilder::CreateSphere(256,256);
	mMeshBuffer.Initialize(mMesh);

	mDiffuseMap.Initialize("../../Assets/earth.jpg");
	mSpecularMap.Initialize("../../Assets/earth_spec.jpg");
	mDisplacementMap.Initialize("../../Assets/earth_bump.jpg");
	mNormalMap.Initialize("../../Assets/earth_normal.jpg");
	mCloudMap.Initialize("../../Assets/earth_clouds.jpg");
	//Create constant buffer
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mBlurBuffer.Initialize();

	mDirectionLight.direction = Normalize({ 1.0, -1.0, 1.0 });
	mDirectionLight.ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	mDirectionLight.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };

	mMaterial.ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	mMaterial.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mMaterial.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.power = 40.0f;

	mBlurData.textureWidth = 400.0f;

	//Compile and create shaders
	mEarthVertexShader.Initialize("../../Assets/Shaders/Earth.fx", "VSEarth", Vertex::Format);
	mEarthPixeShader.Initialize("../../Assets/Shaders/Earth.fx", "PSEarth");

	mCloudVertexShader.Initialize("../../Assets/Shaders/Earth.fx", "VSCloud", Vertex::Format);
	mCloudPixeShader.Initialize("../../Assets/Shaders/Earth.fx", "PSCloud");
	
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);

	mBlendState.Initialize(BlendState::Mode::Additive);

	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize
	(
		graphicsSystem->GetBackBufferWidth(),
		graphicsSystem->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8
	);

	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessPixeShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PS");
	mBlurEffectPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSBlur");
	mPixelEffectPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSPixel");
}

void GameState::Terminate()
{
	mPostProcessPixeShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	mSettingsBuffer.Terminate();
	mDiffuseMap.Terminate();
	mSpecularMap.Terminate();
	mDisplacementMap.Terminate();
	mNormalMap.Terminate();
	mCloudMap.Terminate();
	mCloudVertexShader.Terminate();
	mCloudPixeShader.Terminate();
	mEarthPixeShader.Terminate();
	mEarthVertexShader.Terminate();
	mDiffuseMap.Terminate();
	mMeshBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mBlendState.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 8.0f;
	const float kTurnSpeed = 0.8f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Yaw(-kTurnSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Yaw(kTurnSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::SPACE))
		mCamera.Rise(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		mCamera.Rise(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON) && inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON) && inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(kMoveSpeed * deltaTime);
	}

	mCloudRotation.y += 0.05*deltaTime;
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
	DrawScene();
	mRenderTarget.EndRender();

	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);

}

void GameState::DebugUI()
{
	ImGui::Begin("Setting");
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionLight.direction.x, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionLight.direction.y, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionLight.direction.z, 0.01f, -1.0f, 1.0f);
		if (directionChanged)
		{
			mDirectionLight.direction = Normalize(mDirectionLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}

	if (ImGui::CollapsingHeader("Setting"))
	{
		static bool normalMap = true;
		static bool specularMap = true;
		ImGui::SliderFloat("Displacement", &mSettingsData.bumpMapWeight, 0.0f, 1.0f);
		if (ImGui::Checkbox("Specular Map", &specularMap))
		{
			mSettingsData.specularWeight = specularMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Normal Map", &normalMap))
		{
			mSettingsData.normalMapWeight = normalMap ? 1.0f : 0.0f;
		}
	}

	if (ImGui::CollapsingHeader("EarthTransform"))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Post Processing"))
	{
		ImGui::Checkbox("Blur", &isBlur);
		ImGui::Checkbox("Pixel", &isPixel);
	}

	ImGui::End();
}

void GameState::DrawScene()
{
	mLightBuffer.Update(mDirectionLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	auto matTrans = Matrix4::Translation({ 0.0f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x)*Matrix4::RotationY(mRotation.y)*Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * matTrans;
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mTransformData.world = Transpose(matWorld);
	mTransformData.wvp = Transpose(matWorld*matView*matProj);
	mTransformData.viewPosition = mCamera.GetPosition();

	mTransformBuffer.Update(mTransformData);
	mTransformBuffer.BindVS(0);

	mSettingsBuffer.Update(mSettingsData);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mEarthVertexShader.Bind();
	mEarthPixeShader.Bind();

	mDiffuseMap.BindPS(0);
	mSpecularMap.BindPS(1);
	mDisplacementMap.BindVS(2);
	mNormalMap.BindPS(3);

	mSampler.BindVS();
	mSampler.BindPS();

	mMeshBuffer.Draw();

	//-----
	mCloudVertexShader.Bind();
	mCloudPixeShader.Bind();
	auto matScaling = Matrix4::Scaling(1.01f);
	auto matCloudRot = Matrix4::RotationX(mCloudRotation.x)*Matrix4::RotationY(mCloudRotation.y)*Matrix4::RotationZ(mCloudRotation.z);
	mTransformData.wvp = Transpose(matWorld * matScaling *matCloudRot * matView * matProj);
	mTransformBuffer.Update(mTransformData);
	mTransformBuffer.BindVS(0);
	mSettingsBuffer.Update(mSettingsData);
	mSettingsBuffer.BindPS(4);
	mCloudMap.BindPS(5);
	mBlendState.Bind();
	mMeshBuffer.Draw();
	BlendState::ClearState();
}

void GameState::PostProcess()
{
	mPostProcessVertexShader.Bind();

	if (isBlur)
	{
		mBlurBuffer.Update(mBlurData);
		mBlurBuffer.BindPS(0);
		mBlurEffectPixelShader.Bind();
		mBlurEffectPixelShader.Bind();
		mBlurEffectPixelShader.Bind();
		mBlurEffectPixelShader.Bind();
		mBlurEffectPixelShader.Bind();
	}
	else if (isPixel)
	{
		mPixelEffectPixelShader.Bind();
	}
	else
	{
		mPostProcessPixeShader.Bind();
	}


	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}

