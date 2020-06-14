#include "GameObject.hlsli"

float4 PS(PS_INPUT input) : SV_Target
{
	// ��ǰ���вü���alphaֵС��0.1�����ر��ü�
	float4 texColor = gTexDiffuse.Sample(SampleTypeWrap, input.Tex);
	clip(texColor.a - 0.1f);

	float4 texColorTwo = gTexDiffuseTwo.Sample(SampleTypeWrap, input.Tex);
	clip(texColorTwo.a - 0.1f);


	// ��׼��������
	input.Norm = normalize(input.Norm);

	// ����ָ���۾�������
	float3 toEyeW = normalize(gEyePosW - input.PosW);

	// ��ʼ��Ϊ0 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float2 projectTexCoordDir, projectTexCoordSpot;
	float bias = 0.001f ,depthValueDir,depthValueSpot;



	//ƽ�й�
	projectTexCoordDir.x = input.DirLightProjPos.x / input.DirLightProjPos.w / 2.0f + 0.5f;
	projectTexCoordDir.y = -input.DirLightProjPos.y / input.DirLightProjPos.w / 2.0f + 0.5f;
	depthValueDir = gShadowMapTextureDir.Sample(SampleTypeClamp, projectTexCoordDir).r;
	ComputeDirectionalLight(gMaterial, gDirectionalLight[0], input.Norm, toEyeW, projectTexCoordDir, input.DirLightProjPos, depthValueDir, bias, A, D, S);
	ambient += A;
	diffuse += D;
	specular += S;



	//�۹��
	projectTexCoordSpot.x = input.SpotLightProjPos.x / input.SpotLightProjPos.w / 2.0f + 0.5f;
	projectTexCoordSpot.y = -input.SpotLightProjPos.y / input.SpotLightProjPos.w / 2.0f + 0.5f;
	depthValueSpot = gShadowMapTextureSpot.Sample(SampleTypeClamp, projectTexCoordSpot).r;
	ComputeSpotLight(gMaterial, gSpotLight[0], input.PosW, input.Norm, toEyeW, projectTexCoordSpot, input.SpotLightProjPos, depthValueSpot, bias, A, D, S);
	ambient += A;
	diffuse += D;
	specular += S;



	ambient = saturate(ambient);
	diffuse = saturate(diffuse);
	specular = saturate(specular);

	if (input.PosW.x <= 10 && input.PosW.x >= -10 && input.PosW.y <= 30 && input.PosW.y >= -30)
	{
		float4 finalColor = saturate(texColorTwo * (saturate(ambient + diffuse)) + specular);
		finalColor.a = texColorTwo.a * gMaterial.Diffuse.a;
		return finalColor;
	}
	else
	{
		float4 finalColor = saturate(texColor * (saturate(ambient + diffuse)) + specular);
		finalColor.a = texColor.a * gMaterial.Diffuse.a;
		return finalColor;
	}
}