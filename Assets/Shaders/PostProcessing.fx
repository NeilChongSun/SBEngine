static float Pixels[13] =
{
   -6,
   -5,
   -4,
   -3,
   -2,
   -1,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
};

static const float BlurWeights[13] =
{
   0.002216f,
   0.008764f,
   0.026995f,
   0.064759f,
   0.120985f,
   0.176033f,
   0.199471f,
   0.176033f,
   0.120985f,
   0.064759f,
   0.026995f,
   0.008764f,
   0.002216f,
};

cbuffer BlurBuffer : register(b0)
{
	float textureWidth;
	float blurScale;
}


Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);
struct VSInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};


VSOutput VS(VSInput input)
{
	VSOutput output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return textureMap.Sample(textureSampler, input.texCoord);
}

float4 PSBlur(VSOutput input) : SV_Target
{
	float pixelWidth = 1.0f / textureWidth;
	float4 color = {0.0f, 0.0f, 0.0f, 1.0f};

	float2 blur;
	blur.y = input.texCoord.y;

	[unroll(13)]
	for (int i = 0; i < 13; i++)
	{
		blur.x = input.texCoord.x + Pixels[i] * pixelWidth;
		color += textureMap.Sample(textureSampler, blur.xy) * BlurWeights[i];
	}

	return color;
}

float4 PSPixel(VSOutput input) : SV_Target
{
	float2 uv = input.texCoord;
	float u = (int)(uv.x * 100) / 100.0f;
	float v = (int)(uv.y * 60) / 60.0f;
	float4 color = textureMap.Sample(textureSampler, float2(u,v));
	return color;
}

