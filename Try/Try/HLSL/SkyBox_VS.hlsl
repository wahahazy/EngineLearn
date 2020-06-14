#include "SkyBox.hlsli"

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	// 设置z = w使得z/w = 1(天空盒保持在远平面)
	float4 pos = mul(float4(input.PosN, 1.0f), WorldViewProj);
	output.Pos = pos.xyww;
	output.PosN = input.PosN;
	return output;
}