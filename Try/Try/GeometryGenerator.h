#pragma once
#include "basic.h"

struct VertexData
{
	XMFLOAT3 Pos;     //����λ��
	XMFLOAT3 Normal;  //������
	XMFLOAT2 Tex;     //��������
	VertexData(XMFLOAT3 in_pos, XMFLOAT3 in_normal, XMFLOAT2 in_tex)
	{
		Pos = in_pos;
		Normal = in_normal;
		Tex = in_tex;
	}

	VertexData(XMFLOAT3 in_pos, XMFLOAT3 in_normal)
	{
		Pos = in_pos;
		Normal = in_normal;
		Tex = XMFLOAT2(0,0);
	}

	VertexData(XMFLOAT3 in_pos, XMFLOAT2 in_tex)
	{
		Pos = in_pos;
		Normal = XMFLOAT3(0,0,0);
		Tex = in_tex;
	}

	VertexData(XMFLOAT3 in_pos)
	{
		Pos = in_pos;
		Normal = XMFLOAT3(0, 0, 0);
		Tex = XMFLOAT2(0, 0);
	}
	VertexData()
	{
		Pos = XMFLOAT3(0, 0, 0);
		Normal = XMFLOAT3(0, 0, 0);
		Tex = XMFLOAT2(0, 0);
	}

};

struct VertexPos
{
	XMFLOAT3 Pos;     //����λ��
};

struct MeshData
{
	bool m_use_position = true;
	bool m_use_normal = true;
	bool m_use_texcoord = true;
	std::vector<VertexData> vertices;	 //�����嶥����Ϣ����
	std::vector<WORD> indices;           //������������

	void VertexDataUseState(bool in_use_position, bool in_use_normal, bool in_use_texcoord);
	void AddVertexData(VertexData in_vertex_data);
};

struct MeshDataPos
{
	std::vector<VertexPos> vertices;	//�����嶥����Ϣ����
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
