#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mTankPosition = Vector3{ 10.0f, 3.5f, 10.0f };

	mDefualtCamera.SetNearPlane(0.001f);
	mDefualtCamera.SetPosition({ 0.0f, 20.0f, -25.0f });
	mDefualtCamera.SetLookAt({ 0.0f,0.0f,0.0f });
	
	mLightCamera.SetDirection(Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefualtCamera;

	OBJLoader::Load("../../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(mTankMesh);
	
	mPlaneMesh = MeshBuilder::CreatePlane(50.0f, 16.0f, 16.0f);
	mPlaneMeshBuffer.Initialize(mPlaneMesh);
	
	mTerrain.Initialize(100, 100, 1.0f);
	mTerrain.SetHeightScale(30.0f);
	mTerrain.LoadHeightmap("../../Assets/Heightmaps/heightmap_200x200.raw");
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	
	mDirectionLight.direction = Normalize({ 1.0, -1.0, 1.0 });
	mDirectionLight.ambient = { Colors::White };
	mDirectionLight.diffuse = { Colors::White };
	mDirectionLight.specular = { Colors::White };
	
	mMaterial.ambient = { Colors::White };
	mMaterial.diffuse = { Colors::White };
	mMaterial.specular = { Colors::White };
	mMaterial.power = 40.0f;
	
	mSettingsData.specularWeight = 1.0f;
	mSettingsData.bumpMapWeight = 0.0f;
	mSettingsData.normalMapWeight = 0.0f;
	mSettingsData.aoMapWeight = 1.0f;
	mSettingsData.brightness = 3.5f;
	mSettingsData.useShadow = 1;
	mSettingsData.depthBias = 0.0f;
	
	mVertexShader.Initialize("../../Assets/Shaders/StandardShading.fx", Vertex::Format);
	mPixeShader.Initialize("../../Assets/Shaders/StandardShading.fx");
	
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mDiffuseMap.Initialize("../../Assets/Models/Tank/tank_diffuse.jpg");
	mSpecularMap.Initialize("../../Assets/Models/Tank/tank_specular.jpg");
	mNormalMap.Initialize("../../Assets/Models/Tank/tank_normal.jpg");
	mAOMap.Initialize("../../Assets/Models/Tank/tank_ao.jpg");
	mPlaneMap.Initialize("../../Assets/grass.jpg");

	auto graphicsSystem = GraphicsSystem::Get();
	constexpr uint32_t depthMapSize = 1024;
	
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	mDepthMapVertexShader.Initialize("../../Assets/Shaders/DepthMap.fx", Vertex::Format);
	mDepthMapPixelShader.Initialize("../../Assets/Shaders/DepthMap.fx");
	mDepthMapBuffer.Initialize();
	mShadowBuffer.Initialize();
	
	mRenderTarget.Initialize
	(
		graphicsSystem->GetBackBufferWidth(),
		graphicsSystem->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8
	);

	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessPixeShader.Initialize("../../Assets/Shaders/PostProcessing.fx");

	mAnimation=AnimationBuilder().
		SetTime(0.0f).AddPositionKey(Vector3(10.0f, 3.5f, 10.0f)).AddRotationKey(Quaternion::Identity).AddScaleKey(Vector3::One).
		SetTime(3.0f).AddPositionKey(Vector3(10.0f, 3.5f, 20.0f)).AddRotationKey(Quaternion::Identity).
		SetTime(7.0f).AddPositionKey(Vector3(10.0f, 3.5f, 20.0f)).AddRotationKey(Quaternion::RotationAxis(Vector3::YAxis,Constants::Pi/2)).
		SetTime(11.0f).AddPositionKey(Vector3(30.0f, 3.5f, 20.0f)).
		Get();
}

void GameState::Terminate()
{
	mTerrain.Terminate();
	mDepthMapBuffer.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();
	mDepthMapRenderTarget.Terminate();
	mPlaneMap.Terminate();
	mAOMap.Terminate();
	mPostProcessPixeShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	mSettingsBuffer.Terminate();
	mDiffuseMap.Terminate();
	mSpecularMap.Terminate();
	mNormalMap.Terminate();
	mPixeShader.Terminate();
	mVertexShader.Terminate();
	mDiffuseMap.Terminate();
	mTankMeshBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 26.0f;
	const float kTurnSpeed = 1.5f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mDefualtCamera.Walk(kMoveSpeed*deltaTime);

	if (inputSystem->IsKeyDown(KeyCode::S))
		mDefualtCamera.Walk(-kMoveSpeed * deltaTime);

	if (inputSystem->IsKeyDown(KeyCode::SPACE))
		mDefualtCamera.Rise(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		mDefualtCamera.Rise(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mDefualtCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mDefualtCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mDefualtCamera.Strafe(-kMoveSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mDefualtCamera.Strafe(kMoveSpeed * deltaTime);
	}
	mLightCamera.SetPosition(mTankPosition - mLightCamera.GetDirection() * 30.0f);
	mTime += deltaTime;
}

void GameState::Render()
{
	mDepthMapRenderTarget.BeginRender();
	DrawDepthMap();
	mDepthMapRenderTarget.EndRender();

	mRenderTarget.BeginRender();
	DrawScene();
	mRenderTarget.EndRender();
	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);
	mTerrain.Render(mDefualtCamera);
}

void GameState::DrawScene()
{
	auto matView = mActiveCamera->GetViewMatrix();
	auto matProj = mActiveCamera->GetPerspectiveMatrix();
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	mLightBuffer.Update(mDirectionLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(mSettingsData);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mSampler.BindVS();
	mSampler.BindPS();

	mDiffuseMap.BindPS(0);
	mSpecularMap.BindPS(1);
	mSpecularMap.BindVS(2);
	mNormalMap.BindPS(3);
	mAOMap.BindPS(4);
	mDepthMapRenderTarget.BindPS(5);
	//----------------------------------------------------------
	auto matTrans = Matrix4::Translation(mAnimation.GetPosition(mTime));
	auto matRot = Matrix4::RotationQuaternion(mAnimation.GetRotation(mTime));
	auto matWorld = matRot * matTrans;


	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Update(transformData);
	mTransformBuffer.BindVS(0);

	auto wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowBuffer.Update(wvpLight);
	mShadowBuffer.BindVS(4);

	mVertexShader.Bind();
	mPixeShader.Bind();

	mTankMeshBuffer.Draw();

	matWorld = Matrix4::Identity;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowBuffer.Update(wvpLight);
	mShadowBuffer.BindVS(4);

	mPlaneMap.BindPS(0);

	SettingsData settings;
	settings.specularWeight = 0.0f;
	settings.bumpMapWeight = 0.0f;
	settings.normalMapWeight = 0.0f;
	settings.aoMapWeight = 0.0f;
	settings.useShadow = 1;
	mSettingsBuffer.Update(settings);

	mPlaneMeshBuffer.Draw();
}

void GameState::PostProcess()
{
	mPostProcessVertexShader.Bind();
	mPostProcessPixeShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}

void GameState::DrawDepthMap()
{
	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();
	//----------------------------------------------------------
	auto matTrans = Matrix4::Translation(mAnimation.GetPosition(mTime));
	auto matRot = Matrix4::RotationQuaternion(mAnimation.GetRotation(mTime));
	auto matWorld = matRot * matTrans;
	auto wvp = Transpose(matWorld * matViewLight * matProjLight);
	mDepthMapBuffer.Update(wvp);
	mDepthMapBuffer.BindVS(0);
	mTankMeshBuffer.Draw();
}