// ����ģ�Ͳ��õ���Blinn-Phong����ģ��

// ƽ�й�
struct DirectionalLight
{
	matrix ViewProj;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;  //Ϊ����ʮ���ֽڣ�����õ�
};

// ���
struct PointLight
{
	matrix ViewProj;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;      //���Դ�ķ�Χ

	float3 WeakFactor;       //���˥������
	float Pad;
};

// �۹��
struct SpotLight
{
	matrix ViewProj;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 WeakFactor;
	float Pad;
};

// �������Ĳ���
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;  // w = �˲��ʵĹ����
};



void ComputeDirectionalLight(Material material, DirectionalLight l, float3 normal, float3 toEye, float2 projectTexCoord, float4 LightProjPos, float depthValue, float bias,
	out float4 ambient, out float4 diffuse, out float4 specular)
{
	// ��ʼ�����
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// �����������䷽���෴
	float3 lightVector = normalize(-l.Direction);

	// ��ӻ�����
	ambient = material.Ambient * l.Ambient;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float lightDepthValue = LightProjPos.z / LightProjPos.w;
		lightDepthValue = lightDepthValue - bias;
		if (lightDepthValue < depthValue)
		{
			// �����������������涥�㷨�����ļн�
			float diffuseFactor = dot(lightVector, normal);

			// С��90�Ȳ���������͸߹ⷴ��
			// ���������͸߹ⷴ��
			if (diffuseFactor > 0.0f)
			{
				float3 v = reflect(-lightVector, normal);
				float specularFactor = pow(max(dot(v, toEye), 0.0f), material.Specular.w);

				diffuse = diffuseFactor * material.Diffuse * l.Diffuse;
				specular = specularFactor * material.Specular * l.Specular;
			}
		}
	}
	else
	{
		// �����������������涥�㷨�����ļн�
		float diffuseFactor = dot(lightVector, normal);

		// С��90�Ȳ���������͸߹ⷴ��
		// ���������͸߹ⷴ��
		if (diffuseFactor > 0.0f)
		{
			//float3 h = normalize(toEye+lightVector);
			//float specularFactor = pow(max(dot(h, toEye), 0.0f), material.Specular.w);
			float3 v = reflect(-lightVector, normal);
			float specularFactor = pow(max(dot(v, toEye), 0.0f), material.Specular.w);

			diffuse = diffuseFactor * material.Diffuse * l.Diffuse;
			specular = specularFactor * material.Specular * l.Specular;
		}
	}

}


void ComputePointLight(Material material, PointLight l, float3 pos, float3 normal, float3 toEye, float2 projectTexCoord, float4 LightProjPos, float depthValue, float bias,
	out float4 ambient, out float4 diffuse, out float4 specular)
{
	// ��ʼ�����
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// �ӱ��浽��Դ������
	float3 lightVector = l.Position - pos;

	// ���浽���ߵľ���
	float distance = length(lightVector);

	// ����������ƹ���Range��Χ�⣬���ʾ��û���յ���
	if (distance > l.Range)
		return;

	// ��׼��������
	lightVector /= distance;

	// ���������
	ambient = material.Ambient * l.Ambient;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float lightDepthValue = LightProjPos.z / LightProjPos.w;
		lightDepthValue = lightDepthValue - bias;
		if (lightDepthValue < depthValue)
		{
			// �����������������涥�㷨�����ļн�
			float diffuseFactor = dot(lightVector, normal);

			// С��90�Ȳ���������͸߹ⷴ��
			// ���������͸߹ⷴ��
			if (diffuseFactor > 0.0f)
			{
				float3 h = normalize(toEye + lightVector);
				float specularFactor = pow(max(dot(h, toEye), 0.0f), material.Specular.w);

				diffuse = diffuseFactor * material.Diffuse * l.Diffuse;
				specular = specularFactor * material.Specular * l.Specular;
			}

			// ���˥��
			float weakFactor = 1.0f / dot(l.WeakFactor, float3(1.0f, distance, distance * distance));

			diffuse *= weakFactor;
			specular *= weakFactor;
		}
	}


}


void ComputeSpotLight(Material material, SpotLight l, float3 pos, float3 normal, float3 toEye, float2 projectTexCoord, float4 LightProjPos, float depthValue, float bias,
	out float4 ambient, out float4 diffuse, out float4 specular)
{
	// ��ʼ�����
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// // �ӱ��浽��Դ������
	float3 lightVector = l.Position - pos;

	// ���浽��Դ�ľ���
	float distance = length(lightVector);

	// ����������ƹ���Range��Χ�⣬���ʾ��û���յ���
	if (distance > l.Range)
		return;

	// ��׼��������
	lightVector /= distance;

	// ���������
	ambient = material.Ambient * l.Ambient;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float lightDepthValue = LightProjPos.z / LightProjPos.w;
		lightDepthValue = lightDepthValue - bias;
		if (lightDepthValue < depthValue)
		{
			// �����������������涥�㷨�����ļн�
			float diffuseFactor = dot(lightVector, normal);

			// С��90�Ȳ���������͸߹ⷴ��
			// ���������͸߹ⷴ��
			if (diffuseFactor > 0.0f)
			{
				float3 h = normalize(toEye + lightVector);
				float specularFactor = pow(max(dot(h, toEye), 0.0f), material.Specular.w);

				diffuse = diffuseFactor * material.Diffuse * l.Diffuse;
				specular = specularFactor * material.Specular * l.Specular;
			}
		}

	}

	// ����������
	float spot = pow(max(dot(-lightVector, l.Direction), 0.0f), l.Spot);

	// ���˥��
	float weakFactor = spot / dot(l.WeakFactor, float3(1.0f, distance, distance * distance));

	ambient *= spot;
	diffuse *= weakFactor;
	specular *= weakFactor;
}