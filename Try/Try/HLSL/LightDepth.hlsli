
cbuffer MatrixBuffer: register(b0)
{
	matrix WorldViewProj;
};



struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 DepthPos:TEXTURE0;
};
