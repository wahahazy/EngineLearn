#pragma once
#include "basic.h"

//ƽ�й�
struct DirectionalLight
{
	DirectionalLight() { memset(this, 0, sizeof(DirectionalLight)); }
	XMMATRIX ViewProj;
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad; //Ϊ����ʮ���ֽڣ�����õ�
};

// ���
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

// �۹��
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

// ����������
struct Material
{
	Material() { memset(this, 0, sizeof(Material)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = �˲��ʵĹ����

	const char* m_texture_path = "";
	const char* m_name = "";
};
