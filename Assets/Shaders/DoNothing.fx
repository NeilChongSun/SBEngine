//Description: Simple shader that does nothing.

float4 VS(float3 position : POSITION) : SV_POSITION
{
    return float4(position, 1.0f);
}

float4 PS(float4 position : SV_Position) : SV_TARGET
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f); //Yellow
}