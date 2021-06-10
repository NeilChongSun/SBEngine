// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//
// This file was generated by exporting HLSL from Visual Studio's default "Phong" material, and then modified to handle no texture scenarios, multiple lights, and work with FL 9.x
// <Visual Studio install folder>\Common7\IDE\Extensions\Microsoft\VsGraphics\Assets\Effects\Phong.dgsl 
//

Texture2D Texture1 : register( t0 );

SamplerState TexSampler : register( s0 );

cbuffer MaterialVars : register (b0)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialEmissive;
    float MaterialSpecularPower;
};

cbuffer LightVars : register (b1)
{
    float4 AmbientLight;
    float4 LightColor[4];
    float4 LightAttenuation[4];
    float3 LightDirection[4];
    float LightSpecularIntensity[4];
    uint IsPointLight[4];
    uint ActiveLights;
}

cbuffer ObjectVars : register(b2)
{
    float4x4 LocalToWorld4x4;
    float4x4 LocalToProjected4x4;
    float4x4 WorldToLocal4x4;
    float4x4 WorldToView4x4;
    float4x4 UVTransform4x4;
    float3 EyePosition;
};

cbuffer MiscVars : register(b3)
{
    float ViewportWidth;
    float ViewportHeight;
    float Time;
};

struct V2P
{
    float4 pos : SV_POSITION;
    float4 diffuse : COLOR;
    float2 uv : TEXCOORD0;
    float3 worldNorm : TEXCOORD1;
    float3 worldPos : TEXCOORD2;
    float3 toEye : TEXCOORD3;
    float4 tangent : TEXCOORD4;
    float3 normal : TEXCOORD5;
};

struct P2F
{
    float4 fragment : SV_Target;
};

//
// lambert lighting function
//
float3 LambertLighting(
    float3 lightNormal,
    float3 surfaceNormal,
    float3 lightColor,
    float3 pixelColor
    )
{
    // compute amount of contribution per light
    float diffuseAmount = saturate(dot(lightNormal, surfaceNormal));
    float3 diffuse = diffuseAmount * lightColor * pixelColor;
    return diffuse;
}

//
// specular contribution function
//
float3 SpecularContribution(
    float3 toEye,
    float3 lightNormal,
    float3 surfaceNormal,
    float3 materialSpecularColor,
    float materialSpecularPower,
    float lightSpecularIntensity,
    float3 lightColor
    )
{
    // compute specular contribution
    float3 vHalf = normalize(lightNormal + toEye);
    float specularAmount = saturate(dot(surfaceNormal, vHalf));
    specularAmount = pow(specularAmount, max(materialSpecularPower,0.0001f)) * lightSpecularIntensity;
    float3 specular = materialSpecularColor * lightColor * specularAmount;
    
    return specular;
}

//
// combines a float3 RGB value with an alpha value into a float4
//
float4 CombineRGBWithAlpha(float3 rgb, float a) 
{ 
    return float4(rgb.r, rgb.g, rgb.b, a); 
}

P2F main(V2P pixel)
{
    P2F result;

    float3 worldNormal = normalize(pixel.worldNorm);
    float3 toEyeVector = normalize(pixel.toEye);

    float3 local1 = MaterialAmbient.rgb * AmbientLight.rgb;
    float3 local4 = 0;
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        local1 += LambertLighting(LightDirection[i], worldNormal, LightColor[i].rgb, pixel.diffuse.rgb);
        local4 += SpecularContribution(toEyeVector, LightDirection[i], worldNormal, MaterialSpecular.rgb, MaterialSpecularPower, LightSpecularIntensity[i], LightColor[i].rgb);
    }

    local1 = saturate(local1);
    result.fragment = CombineRGBWithAlpha(local1 + local4, pixel.diffuse.a);

    return result;
}

P2F mainTk(V2P pixel)
{
    P2F result;

    float3 worldNormal = normalize(pixel.worldNorm);
    float3 toEyeVector = normalize(pixel.toEye);

    float3 local1 = MaterialAmbient.rgb * AmbientLight.rgb;
    float3 local4 = 0;
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        local1 += LambertLighting(LightDirection[i], worldNormal, LightColor[i].rgb, pixel.diffuse.rgb);
        local4 += SpecularContribution(toEyeVector, LightDirection[i], worldNormal, MaterialSpecular.rgb, MaterialSpecularPower, LightSpecularIntensity[i], LightColor[i].rgb);
    }

    local1 = saturate(local1);
    result.fragment = CombineRGBWithAlpha(local1 + local4, pixel.diffuse.a);

    if (result.fragment.a == 0.0f) discard;

    return result;
}

P2F mainTx(V2P pixel)
{
    P2F result;

    float3 worldNormal = normalize(pixel.worldNorm);
    float3 toEyeVector = normalize(pixel.toEye);

    float3 local1 = MaterialAmbient.rgb * AmbientLight.rgb;
    float3 local4 = 0;
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        local1 += LambertLighting(LightDirection[i], worldNormal, LightColor[i].rgb, pixel.diffuse.rgb);
        local4 += SpecularContribution(toEyeVector, LightDirection[i], worldNormal, MaterialSpecular.rgb, MaterialSpecularPower, LightSpecularIntensity[i], LightColor[i].rgb);
    }

    local1 = saturate(local1);
    float3 local5 = mad(local1, Texture1.Sample(TexSampler, pixel.uv).rgb, local4);
    float local6 = Texture1.Sample(TexSampler, pixel.uv).a * pixel.diffuse.a;
    result.fragment = CombineRGBWithAlpha(local5, local6);

    return result;
}

P2F mainTxTk(V2P pixel)
{
    P2F result;

    float3 worldNormal = normalize(pixel.worldNorm);
    float3 toEyeVector = normalize(pixel.toEye);

    float3 local1 = MaterialAmbient.rgb * AmbientLight.rgb;
    float3 local4 = 0;
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        local1 += LambertLighting(LightDirection[i], worldNormal, LightColor[i].rgb, pixel.diffuse.rgb);
        local4 += SpecularContribution(toEyeVector, LightDirection[i], worldNormal, MaterialSpecular.rgb, MaterialSpecularPower, LightSpecularIntensity[i], LightColor[i].rgb);
    }

    local1 = saturate(local1);
    float3 local5 = mad(local1, Texture1.Sample(TexSampler, pixel.uv).rgb, local4);
    float local6 = Texture1.Sample(TexSampler, pixel.uv).a * pixel.diffuse.a;
    result.fragment = CombineRGBWithAlpha(local5, local6);

    if (result.fragment.a == 0.0f) discard;

    return result;
}
