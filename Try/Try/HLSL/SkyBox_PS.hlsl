#include "SkyBox.hlsli"

float4 PS(PS_INPUT input) : SV_Target
{
	return texCube.Sample(sam, input.PosN);
}