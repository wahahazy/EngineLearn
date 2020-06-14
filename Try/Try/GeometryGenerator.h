#pragma once
#include "basic.h"

struct VertexData
{
	XMFLOAT3 Pos;     //����λ��
	XMFLOAT3 Normal;  //������
	XMFLOAT2 Tex;     //��������
};

struct VertexPos
{
	XMFLOAT3 Pos;     //����λ��
};

struct MeshData
{
	std::vector<VertexData> vertices;  //�����嶥����Ϣ����
	std::vector<WORD> indices;           //������������
};

struct MeshDataPos
{
	std::vector<VertexPos> vertices;  //�����嶥����Ϣ����
	std::vector<WORD> indices;           //������������
};

class GeometryGenerator
{

public:
	GeometryGenerator();
	~GeometryGenerator();

	static MeshData CreatePlane(float length, float centerY, float width, int number = 2, const XMFLOAT4 & color = { 1.0f, 1.0f, 1.0f, 1.0f });  //��������,numberȡ2�ı���
	static MeshData CreateCuboid(float length, float height, float width, const XMFLOAT4 & color = { 1.0f, 1.0f, 1.0f, 1.0f });  //����������,length:x����width:y����height:z����
	static MeshData CreateCylinder(float radius, float height, int slices, const XMFLOAT4 & color = { 1.0f, 1.0f, 1.0f, 1.0f }); //����Բ����
	static MeshDataPos CreateSphere(float radius = 1.0f, int levels = 20, int slices = 20, const XMFLOAT4 & color = { 1.0f, 1.0f, 1.0f, 1.0f });
};
