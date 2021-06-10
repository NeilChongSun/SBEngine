#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;

namespace
{
	void SimplerDrawCamera(const Camera& camera)
	{
		auto defaultMatView = camera.GetViewMatrix();
		Vector3 cameraPosition = camera.GetPosition();
		Vector3 cameraRight = { defaultMatView._11, defaultMatView._21, defaultMatView._31 };
		Vector3 cameraUp = { defaultMatView._12, defaultMatView._22, defaultMatView._32 };
		Vector3 cameraLook = { defaultMatView._13, defaultMatView._23, defaultMatView._33 };
		SimpleDraw::AddSphere(cameraPosition, 0.1f, Colors::White, 6, 8);
		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraRight, Colors::Red);
		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraUp, Colors::Green);
		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraLook, Colors::Blue);
	}
}


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
}

void GameState::Terminate()
{
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
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

	if (inputSystem->IsKeyDown(KeyCode::W))
		mActiveCamera->Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mActiveCamera->Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mActiveCamera->Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mActiveCamera->Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mActiveCamera->Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mActiveCamera->Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::UP))
		mTankRotation.x += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		mTankRotation.x -= deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		mTankRotation.y += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		mTankRotation.y -= deltaTime;


	mLightCamera.SetDirection(mDirectionLight.direction);
	//mLightCamera.SetPosition(mLightCamera.GetDirection() * -50.0f);

	mTankPositions.clear();

	const int count = 5;
	const float offsetX = (count - 1) * mTankSpacing * -0.5f;
	const float offsetZ = (count - 1) * mTankSpacing * -0.5f;
	for (int z = 0; z < count; ++z)
	{
		for (int x = 0; x < count; ++x)
		{
			float posX = (x * mTankSpacing) + offsetX;
			float posZ = (z * mTankSpacing) + offsetZ;
			mTankPositions.push_back({ posX, 3.5f, posZ });
		}
	}

	mViewFrustumVertices =
	{
		// Near plane
		{ -1.0f, -1.0f, 0.0f },
		{ -1.0f,  1.0f, 0.0f },
		{  1.0f,  1.0f, 0.0f },
		{  1.0f, -1.0f, 0.0f },

		// Far plane
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f,  1.0f, 1.0f },
		{  1.0f,  1.0f, 1.0f },
		{  1.0f, -1.0f, 1.0f },
	};
	auto defaultMatView = mDefualtCamera.GetViewMatrix();
	auto defaultMatProj = mDefualtCamera.GetPerspectiveMatrix();
	auto invViewProj = Inverse(defaultMatView * defaultMatProj);
	for (auto& vertex : mViewFrustumVertices)
	{
		vertex = Transformcoord(vertex, invViewProj);
	}

	auto lightLook = mLightCamera.GetDirection();
	auto lightSide = Normalize(Cross(Vector3::YAxis, lightLook));
	auto lightUp = Normalize(Cross(lightLook, lightSide));
	float minX = FLT_MAX, maxX = -FLT_MAX;
	float minY = FLT_MAX, maxY = -FLT_MAX;
	float minZ = FLT_MAX, maxZ = -FLT_MAX;
	for (auto& vertex : mViewFrustumVertices)
	{
		float projectX = Dot(lightSide, vertex);
		minX = Min(minX, projectX);
		maxX = Max(maxX, projectX);
		float projectY = Dot(lightUp, vertex);
		minY = Min(minY, projectY);
		maxY = Max(maxY, projectY);
		float projectZ = Dot(lightLook, vertex);
		minZ = Min(minZ, projectZ);
		maxZ = Max(maxZ, projectZ);
	}
	mLightCamera.SetPosition(
		lightSide * (minX + maxX) * 0.5f +
		lightUp * (minY + maxY) * 0.5f +
		lightLook * (minZ + maxZ) * 0.5f
	);
	mLightCamera.SetNearPlane(minZ - 300.0f);
	mLightCamera.SetFarPlane(maxZ);
	//mLightProjectMatrix = mLightCamera.Getwhat(maxX - minX, maxY - minY);

	auto v0 = lightSide * minX + lightUp * minY + lightLook * minZ;
	auto v1 = lightSide * minX + lightUp * maxY + lightLook * minZ;
	auto v2 = lightSide * maxX + lightUp * maxY + lightLook * minZ;
	auto v3 = lightSide * maxX + lightUp * minY + lightLook * minZ;
	auto v4 = lightSide * minX + lightUp * minY + lightLook * maxZ;
	auto v5 = lightSide * minX + lightUp * maxY + lightLook * maxZ;
	auto v6 = lightSide * maxX + lightUp * maxY + lightLook * maxZ;
	auto v7 = lightSide * maxX + lightUp * minY + lightLook * maxZ;

	SimpleDraw::AddLine(v0, v1, Colors::Yellow);
	SimpleDraw::AddLine(v1, v2, Colors::Yellow);
	SimpleDraw::AddLine(v2, v3, Colors::Yellow);
	SimpleDraw::AddLine(v3, v0, Colors::Yellow);

	SimpleDraw::AddLine(v0, v4, Colors::Red);
	SimpleDraw::AddLine(v1, v5, Colors::Red);
	SimpleDraw::AddLine(v2, v6, Colors::Red);
	SimpleDraw::AddLine(v3, v7, Colors::Red);

	SimpleDraw::AddLine(v4, v5, Colors::Red);
	SimpleDraw::AddLine(v5, v6, Colors::Red);
	SimpleDraw::AddLine(v6, v7, Colors::Red);
	SimpleDraw::AddLine(v7, v4, Colors::Red);

	SimplerDrawCamera(mLightCamera);
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
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light"), ImGuiTreeNodeFlags_DefaultOpen)
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
	if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static bool specularMap = mSettingsData.specularWeight > 0.0f;
		static bool normalMap = mSettingsData.normalMapWeight > 0.0f;
		static bool aoMap = mSettingsData.aoMapWeight > 0.0f;
		static bool useShadow = mSettingsData.useShadow == 1;
		ImGui::SliderFloat("Displacement", &mSettingsData.bumpMapWeight, 0.0f, 1.0f);
		if (ImGui::Checkbox("Specular Map", &specularMap))
		{
			mSettingsData.specularWeight = specularMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Normal Map", &normalMap))
		{
			mSettingsData.normalMapWeight = normalMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Ambient Occlusion Map", &aoMap))
		{
			mSettingsData.aoMapWeight = aoMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Use Shadow", &useShadow))
		{
			mSettingsData.useShadow = useShadow ? 1 : 0;
		}
		ImGui::SliderFloat("Depth Bias", &mSettingsData.depthBias, 0.0f, 0.01f, "%.4f");
		ImGui::SliderFloat("Brightness", &mSettingsData.brightness, 1.0f, 10.0f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Translation##Transform", &mTankPosition.x, 0.01f);
		ImGui::DragFloat3("Rotation##Transform", &mTankRotation.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//ImGui::DragFloat3("Translation##Transform", &mTankPosition.x, 0.01f);
		ImGui::DragFloat3("Rotation##Transform", &mTankRotation.x, 0.01f);
		ImGui::DragFloat("Spacing", &mTankSpacing);
	}

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool lightCamera = mActiveCamera == &mLightCamera;
		if (ImGui::Checkbox("Use Light Camera", &lightCamera))
		{
			mActiveCamera = lightCamera ? &mLightCamera : &mDefualtCamera;
		}

		ImGui::Image(
			mDepthMapRenderTarget.GetShaderResourceView(),
			{ 150.0f, 150.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}
	ImGui::End();
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

	auto matTrans = Matrix4::Translation(mTankPosition);
	auto matRot = Matrix4::RotationX(mTankRotation.x) * Matrix4::RotationY(mTankRotation.y);
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

	SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[1], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[2], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[3], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[0], Colors::White);

	SimpleDraw::AddLine(mViewFrustumVertices[0], mViewFrustumVertices[4], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[1], mViewFrustumVertices[5], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[2], mViewFrustumVertices[6], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[3], mViewFrustumVertices[7], Colors::White);

	SimpleDraw::AddLine(mViewFrustumVertices[4], mViewFrustumVertices[5], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[5], mViewFrustumVertices[6], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[6], mViewFrustumVertices[7], Colors::White);
	SimpleDraw::AddLine(mViewFrustumVertices[7], mViewFrustumVertices[4], Colors::White);

	SimplerDrawCamera(mDefualtCamera);

	SimpleDraw::Render(*mActiveCamera);


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

	auto matTrans = Matrix4::Translation(mTankPosition);
	auto matRot = Matrix4::RotationX(mTankRotation.x) * Matrix4::RotationY(mTankRotation.y)*Matrix4::RotationZ(mTankRotation.z);
	auto matWorld = matRot * matTrans;
	auto wvp = Transpose(matWorld * matViewLight * matProjLight);
	mDepthMapBuffer.Update(wvp);
	mDepthMapBuffer.BindVS(0);
	mTankMeshBuffer.Draw();
}