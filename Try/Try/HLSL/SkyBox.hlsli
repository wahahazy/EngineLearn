TextureCube texCube : register(t0);
SamplerState sam : register(s0);

cbuffer cbChangesEveryFrame : register(b0)
{
	matrix WorldViewProj;
}

struct VS_INPUT
{
	float3 PosN : POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 PosN : POSITION;
};