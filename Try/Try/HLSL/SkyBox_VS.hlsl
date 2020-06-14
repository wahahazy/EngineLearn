#include "SkyBox.hlsli"

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	// ����z = wʹ��z/w = 1(��պб�����Զƽ��)
	float4 pos = mul(float4(input.PosN, 1.0f), WorldViewProj);
	output.Pos = pos.xyww;
	output.PosN = input.PosN;
	return output;
}