#include "LightDepth.hlsli"

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(float4(input.Pos, 1.0f), WorldViewProj);
	output.DepthPos = output.Pos;

	return output;
}