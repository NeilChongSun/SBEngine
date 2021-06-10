#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace SB::Graphics;
using namespace SB::Input;
using namespace SB::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mModelPosition = Vector3{ 10.0f, 3.5f, 10.0f };
	mModelScale = 0.10f;

	mDefaultCamera.SetNearPlane(0.001f);
	mDefaultCamera.SetPosition({ 0.0f, 20.0f, -25.0f });
	mDefaultCamera.SetLookAt({ 0.0f, 10.0f, 0.0f });

	mLightCamera.SetDirection(Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefaultCamera;
	mModel.Initialize("../../Assets/Models/Mutant/Mutant");

	for (auto& bone : mModel.skeleton.bones)
	{
		SB::Math::Matrix4 matrix = bone->toParentTransform;
		Bone* tempBone = bone.get();
		while (tempBone->parent != nullptr)
		{
			tempBone = tempBone->parent;
			matrix = matrix * tempBone->toParentTransform;
		}
		mMatrices.push_back(matrix);
	}
	mBoneMatrices.resize(mMatrices.size());

	mPlaneMesh = MeshBuilder::CreatePlane(150.0f, 16.0f, 16.0f);
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
	mMaterial.power = 10.0f;

	mSettingsData.specularWeight = 1.0f;
	mSettingsData.bumpMapWeight = 0.0f;
	mSettingsData.normalMapWeight = 0.0f;
	mSettingsData.aoMapWeight = 1.0f;
	mSettingsData.brightness = 1.0f;
	mSettingsData.useShadow = 0;
	mSettingsData.depthBias = 0.0f;

	mVertexShader.Initialize("../../Assets/Shaders/StandardShading.fx", Vertex::Format);
	mPixeShader.Initialize("../../Assets/Shaders/StandardShading.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
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
	mModel.Terminate();
	mDepthMapBuffer.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();
	mDepthMapRenderTarget.Terminate();
	mPlaneMap.Terminate();
	mPostProcessPixeShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	mSettingsBuffer.Terminate();
	mPixeShader.Terminate();
	mVertexShader.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}
void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
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
	if (ImGui::CollapsingHeader("Settings"))
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
		ImGui::DragFloat3("Translation##Transform", &mModelPosition.x, 0.5f);
		ImGui::DragFloat3("Rotation##Transform", &mModelRotation.x, 0.01f);
		ImGui::DragFloat("Scale##Transform", &mModelScale, 0.01f);
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		bool lightCamera = mActiveCamera == &mLightCamera;
		if (ImGui::Checkbox("Use Light Camera", &lightCamera))
		{
			mActiveCamera = lightCamera ? &mLightCamera : &mDefaultCamera;
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
	ImGui::Checkbox("Show Skeleton", &mIsShowSkeleton);
	ImGui::End();
}
void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 26.0f;
	const float kTurnSpeed = 0.1f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mDefaultCamera.Walk(kMoveSpeed*deltaTime);

	if (inputSystem->IsKeyDown(KeyCode::S))
		mDefaultCamera.Walk(-kMoveSpeed * deltaTime);

	if (inputSystem->IsKeyDown(KeyCode::SPACE))
		mDefaultCamera.Rise(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		mDefaultCamera.Rise(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mDefaultCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mDefaultCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mDefaultCamera.Strafe(-kMoveSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mDefaultCamera.Strafe(kMoveSpeed * deltaTime);
	}
	mLightCamera.SetPosition(mModelPosition - mLightCamera.GetDirection() * 30.0f);
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
	;
	mDepthMapRenderTarget.BindPS(5);
	//----------------------------------------------------------
	auto matTrans = Matrix4::Translation(mModelPosition);
	auto matRot = Matrix4::RotationX(mModelRotation.x) * Matrix4::RotationY(mModelRotation.y)*Matrix4::RotationZ(mModelRotation.z);
	auto matScale = Matrix4::Scaling(mModelScale);
	auto matWorld = matRot * matTrans*matScale;

	for (int i = 0; i < mMatrices.size(); i++)
	{
		mBoneMatrices[i] = mMatrices[i] * matWorld;
	}
	DrawSkeleton(mModel.skeleton, mBoneMatrices);
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

	if (!mIsShowSkeleton)
		mModel.Draw();
	DrawSkeleton(mModel.skeleton, mBoneMatrices);
	if (mIsShowSkeleton)
		SimpleDraw::Render(mDefaultCamera);

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
	auto matTrans = Matrix4::Translation(mModelPosition);
	auto matRot = Matrix4::RotationX(mModelRotation.x) * Matrix4::RotationY(mModelRotation.y)*Matrix4::RotationZ(mModelRotation.z);
	auto matScale = Matrix4::Scaling(mModelScale);
	auto matWorld = matRot * matTrans*matScale;
	auto wvp = Transpose(matWorld * matViewLight * matProjLight);
	mDepthMapBuffer.Update(wvp);
	mDepthMapBuffer.BindVS(0);
	mModel.Draw();
}