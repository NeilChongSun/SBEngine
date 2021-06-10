//cbuffer TransformBuffer : register(b0)
//{
//	matrix World;
//	matrix WVP;
//	float3 ViewPosition;
//}
//cbuffer LightBuffer : register(b1)
//{
//	float3 LightDirection;
//	float4 LightAmbient;
//	float4 LightDiffuse;
//	float4 LightSpecular;
//}
//cbuffer MaterialBuffer : register(b2)
//{
//	float4 MaterialAmbient;
//	float4 MaterialDiffuse;
//	float4 MaterialSpecular;
//	float MaterialPower;
//}
//cbuffer SettingsBuffer : register(b3)
//{
//	float specularMapWeight : packoffset(c0.x);
//	float bumpMapWeight : packoffset(c0.y);
//	float normalMapWeight : packoffset(c0.z);
//}
//Texture2D diffuseMap : register(t0);
//Texture2D specularMap : register(t1);
//Texture2D displacementMap : register(t2);
//Texture2D normalMap : register(t3);
//Texture2D nightMap : register(t4);
//Texture2D cloudMap : register(t5);
//SamplerState textureSampler : register(s0);
//struct VSInput
//{
//	float3 position : POSITION;
//	float3 normal : NORMAL;
//	float3 tangent : TANGENT;
//	float2 texCoord : TEXCOORD;
//};
//struct VSOutput
//{
//	float4 position : SV_POSITION;
//	float3 worldNormal : NORMAL;
//	float3 worldTangent : TEXCOORD0;
//	float3 dirToLight: TEXCOORD1;
//	float3 dirToView: TEXCOORD2;
//	float2 texCoord : TEXCOORD3;
//};
//struct VSCloudOutput
//{
//	float4 position : SV_POSITION;
//	float2 texCoord : TEXCOORD;
//};
//VSOutput VSEarth(VSInput input)
//{
//	VSOutput output;
//	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
//	float3 localPosition = input.position + (input.normal * displacement * bumpMapWeight);
//	float3 worldPosition = mul(float4(localPosition, 1.0f), World).xyz;
//	float3 worldNormal = mul(input.normal, (float3x3)World);
//	float3 worldTangent = mul(input.tangent, (float3x3)World);
//	output.position = mul(float4(localPosition, 1.0f), WVP);
//	output.worldNormal = worldNormal;
//	output.worldTangent = worldTangent;
//	output.dirToLight = -LightDirection;
//	output.dirToView = normalize(ViewPosition - worldPosition);
//	output.texCoord = input.texCoord;
//	return output;
//}
//float4 PSEarth(VSOutput input) : SV_Target
//{
//	float3 worldNormal = normalize(input.worldNormal);
//	float3 worldTangent = normalize(input.worldTangent);
//	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));
//	float3 dirToLight = normalize(input.dirToLight);
//	float3 dirToView = normalize(input.dirToView);
//	float3 normal = worldNormal;
//	if (normalMapWeight != 0.0f)
//	{
//		float3x3 TBNW = { worldTangent, worldBinormal, worldNormal };
//		float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
//		float3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
//		normal = mul(normalSampled, TBNW);
//	}
//	float4 ambient = LightAmbient * MaterialAmbient;
//	float diffuseIntensity = saturate(dot(dirToLight, normal));
//	float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;
//	float3 halfAngle = normalize(dirToLight + dirToView);
//	float specularBase = saturate(dot(halfAngle, normal));
//	float specularIntensity = pow(specularBase, MaterialPower);
//	float4 specular = specularIntensity * LightSpecular * MaterialSpecular;
//	float4 dayColor = diffuseMap.Sample(textureSampler, input.texCoord);
//	float4 nightColor = nightMap.Sample(textureSampler, input.texCoord);
//	float4 textureColor = lerp(dayColor, nightColor, dot(LightDirection, worldNormal));
//	float specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;
//	float4 color = (ambient + diffuse) * textureColor + specular * (specularMapWeight != 0.0f ? specularFactor : 1.0f);
//	return color;
//}
//VSCloudOutput VSCloud(VSInput input)
//{
//	VSCloudOutput output;
//	//--------------------
//	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
//	float3 localPosition = input.position + (input.normal * displacement * bumpMapWeight);
//	output.position = mul(float4(localPosition, 1.0f), WVP);
//	output.texCoord = input.texCoord;
//	return output;
//}
//float4 PSCloud(VSCloudOutput input) : SV_Target
//{
//	return cloudMap.Sample(textureSampler, input.texCoord);
//}


// Description: Custom shader for render the Earth.

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP;
	float3 ViewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float MaterialPower;
}

cbuffer SettingsBuffer : register(b3)
{
	float specularMapWeight : packoffset(c0.x);
	float bumpMapWeight : packoffset(c0.y);
	float normalMapWeight : packoffset(c0.z);
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D nightLightMap : register(t4);
Texture2D cloudMap : register(t5);
SamplerState textureSampler : register(s0);

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TEXCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord	: TEXCOORD3;
};

struct VSCloudOutput
{
	float4 position : SV_Position;
	float3 normal : NORMAL;
	float3 dirToLight : TEXCOORD1;
	float2 texCoord	: TEXCOORD3;
};

VSOutput VSEarth(VSInput input)
{
	VSOutput output;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 localPosition = input.position + (input.normal * displacement * bumpMapWeight);
	float3 worldPosition = mul(float4(localPosition, 1.0f), World).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)World);
	float3 worldTangent = mul(input.tangent, (float3x3)World);

	output.position = mul(float4(localPosition, 1.0f), WVP);
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;

	return output;
}

float4 PSEarth(VSOutput input) : SV_Target
{
	// Renormalize normals from vertex shader
	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 normal = worldNormal;
	if (normalMapWeight != 0.0f)
	{
		float3x3 TBNW = { worldTangent, worldBinormal, worldNormal };
		float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
		float3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
		normal = mul(normalSampled, TBNW);
	}

	float4 ambient = LightAmbient * MaterialAmbient;

	float diffuseRaw = dot(dirToLight, normal);
	float diffuseIntensity = saturate(diffuseRaw);
	float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	float3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = saturate(dot(halfAngle, normal));
	float specularIntensity = pow(specularBase, MaterialPower);
	float4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;
	float4 nightColor = nightLightMap.Sample(textureSampler, input.texCoord);

	float nightIntensity = saturate(-diffuseRaw);
	float4 color =
		(ambient + diffuse) * textureColor +
		nightIntensity * nightColor +
		specular * (specularMapWeight != 0.0f ? specularFactor : 1.0f);
	return color;
}

VSCloudOutput VSCloud(VSInput input)
{
	VSCloudOutput output;
	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	//float3 localPosition = input.position + (input.normal * 0.15f);
	float3 localPosition = input.position + (input.normal * 1 * bumpMapWeight*0.6);

	output.position = mul(float4(localPosition, 1.0f), WVP);
	output.normal = mul(input.normal, (float3x3)World);;
	output.dirToLight = -LightDirection;
	output.texCoord = input.texCoord;

	return output;
}

float4 PSCloud(VSCloudOutput input) : SV_Target
{
	// Renormalize normals from vertex shader
	float3 normal = normalize(input.normal);
	float3 dirToLight = normalize(input.dirToLight);

	float diffuseIntensity = saturate(dot(dirToLight, normal));
	float4 diffuse = diffuseIntensity * LightDiffuse;

	return diffuse * cloudMap.Sample(textureSampler, input.texCoord);
}