#pragma once
#include "basic.h"

//平行光
struct DirectionalLight
{
	DirectionalLight() { memset(this, 0, sizeof(DirectionalLight)); }
	XMMATRIX ViewProj;
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad; //为保持十六字节，填充用的
};

// 点光
struct PointLight
{
	PointLight() { memset(this, 0, sizeof(PointLight)); }
	XMMATRIX ViewProj;
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;


	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 WeakFactor;
	float Pad;
};

// 聚光灯
struct SpotLight
{
	SpotLight() { memset(this, 0, sizeof(SpotLight)); }
	XMMATRIX ViewProj;
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Direction;
	float Spot;

	XMFLOAT3 WeakFactor;
	float Pad;
};

// 物体表面材质
struct Material
{
	Material() { memset(this, 0, sizeof(Material)); }
	Material(XMFLOAT4 in_ambient, XMFLOAT4 in_diffuse, XMFLOAT4 in_specular)
	{
		Ambient = in_ambient;
		Diffuse = in_diffuse;
		Specular = in_specular;
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = 此材质的光泽度
};
