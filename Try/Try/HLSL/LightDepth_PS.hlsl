#include "LightDepth.hlsli"

float4 PS(PS_INPUT input) : SV_Target
{
	float depthValue = input.DepthPos.z / input.DepthPos.w;
	return float4(depthValue, depthValue, depthValue,1.0f);  //只需要深度值
}