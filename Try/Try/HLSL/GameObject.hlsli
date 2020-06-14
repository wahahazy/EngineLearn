#include "LightTypeInfo.hlsli"

Texture2D gTexDiffuse : register(t0);
Texture2D gTexDiffuseTwo : register(t1);
Texture2D gShadowMapTextureDir : register(t2);
Texture2D gShadowMapTextureSpot : register(t3);

SamplerState SampleTypeWrap  : register(s0);
SamplerState SampleTypeClamp : register(s1);


cbuffer cbChangeView : register(b0)
{
	matrix gView;
	float3 gEyePosW;
};

cbuffer cbChangeOnResize : register(b1)
{
	matrix gProjection;
};

cbuffer cbChangesEveryFrame : register(b2)
{
	matrix gWorld;
	matrix gWorldInvTranspose;
	Material gMaterial;
};

cbuffer cbLight : register(b3)
{
	DirectionalLight gDirectionalLight[1];
	SpotLight gSpotLight[1];
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{

	float4 Pos : SV_POSITION;
	float4 DirLightProjPos: TEXCOORD0;  //������ƽ�й�ռ��е�ͶӰλ��
	float4 SpotLightProjPos: TEXCOORD1;  //�����ھ۹�ƿռ��е�ͶӰλ��
	float3 PosW : POSITION; // �����������е�λ��
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD2;

};
