// 光照模型采用的是Blinn-Phong光照模型

// 平行光
struct DirectionalLight
{
	matrix ViewProj;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;  //为保持十六字节，填充用的
};

// 点光
struct PointLight
{
	matrix ViewProj;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;      //点光源的范围

	float3 WeakFactor;       //光的衰弱因子
	float Pad;
};

// 聚光灯
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

// 物体表面的材质
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;  // w = 此材质的光泽度
};



void ComputeDirectionalLight(Material material, DirectionalLight l, float3 normal, float3 toEye, float2 projectTexCoord, float4 LightProjPos, float depthValue, float bias,
	out float4 ambient, out float4 diffuse, out float4 specular)
{
	// 初始化输出
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 光向量与照射方向相反
	float3 lightVector = normalize(-l.Direction);

	// 添加环境光
	ambient = material.Ambient * l.Ambient;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float lightDepthValue = LightProjPos.z / LightProjPos.w;
		lightDepthValue = lightDepthValue - bias;
		if (lightDepthValue < depthValue)
		{
			// 计算光向量与物体表面顶点法向量的夹角
			float diffuseFactor = dot(lightVector, normal);

			// 小于90度才有漫反射和高光反射
			// 添加漫反射和高光反射
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
		// 计算光向量与物体表面顶点法向量的夹角
		float diffuseFactor = dot(lightVector, normal);

		// 小于90度才有漫反射和高光反射
		// 添加漫反射和高光反射
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
	// 初始化输出
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 从表面到光源的向量
	float3 lightVector = l.Position - pos;

	// 表面到光线的距离
	float distance = length(lightVector);

	// 如果顶点距离灯光在Range范围外，则表示光没有照到它
	if (distance > l.Range)
		return;

	// 标准化光向量
	lightVector /= distance;

	// 环境光计算
	ambient = material.Ambient * l.Ambient;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float lightDepthValue = LightProjPos.z / LightProjPos.w;
		lightDepthValue = lightDepthValue - bias;
		if (lightDepthValue < depthValue)
		{
			// 计算光向量与物体表面顶点法向量的夹角
			float diffuseFactor = dot(lightVector, normal);

			// 小于90度才有漫反射和高光反射
			// 添加漫反射和高光反射
			if (diffuseFactor > 0.0f)
			{
				float3 h = normalize(toEye + lightVector);
				float specularFactor = pow(max(dot(h, toEye), 0.0f), material.Specular.w);

				diffuse = diffuseFactor * material.Diffuse * l.Diffuse;
				specular = specularFactor * material.Specular * l.Specular;
			}

			// 光的衰弱
			float weakFactor = 1.0f / dot(l.WeakFactor, float3(1.0f, distance, distance * distance));

			diffuse *= weakFactor;
			specular *= weakFactor;
		}
	}


}


void ComputeSpotLight(Material material, SpotLight l, float3 pos, float3 normal, float3 toEye, float2 projectTexCoord, float4 LightProjPos, float depthValue, float bias,
	out float4 ambient, out float4 diffuse, out float4 specular)
{
	// 初始化输出
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// // 从表面到光源的向量
	float3 lightVector = l.Position - pos;

	// 表面到光源的距离
	float distance = length(lightVector);

	// 如果顶点距离灯光在Range范围外，则表示光没有照到它
	if (distance > l.Range)
		return;

	// 标准化光向量
	lightVector /= distance;

	// 环境光计算
	ambient = material.Ambient * l.Ambient;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float lightDepthValue = LightProjPos.z / LightProjPos.w;
		lightDepthValue = lightDepthValue - bias;
		if (lightDepthValue < depthValue)
		{
			// 计算光向量与物体表面顶点法向量的夹角
			float diffuseFactor = dot(lightVector, normal);

			// 小于90度才有漫反射和高光反射
			// 添加漫反射和高光反射
			if (diffuseFactor > 0.0f)
			{
				float3 h = normalize(toEye + lightVector);
				float specularFactor = pow(max(dot(h, toEye), 0.0f), material.Specular.w);

				diffuse = diffuseFactor * material.Diffuse * l.Diffuse;
				specular = specularFactor * material.Specular * l.Specular;
			}
		}

	}

	// 计算汇聚因子
	float spot = pow(max(dot(-lightVector, l.Direction), 0.0f), l.Spot);

	// 光的衰弱
	float weakFactor = spot / dot(l.WeakFactor, float3(1.0f, distance, distance * distance));

	ambient *= spot;
	diffuse *= weakFactor;
	specular *= weakFactor;
}