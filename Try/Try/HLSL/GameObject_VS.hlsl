#include "GameObject.hlsli"

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	float4 posW = mul(float4(input.Pos, 1.0f), gWorld);
	output.PosW = posW.xyz;
	output.Pos = mul(posW, gView);
	output.Pos = mul(output.Pos, gProjection);
	output.DirLightProjPos = mul(posW, gDirectionalLight[0].ViewProj);
	output.SpotLightProjPos = mul(posW, gSpotLight[0].ViewProj);
	output.Norm = mul(float4(input.Norm, 1), gWorldInvTranspose).xyz;
	output.Tex = input.Tex;
	return output;
}