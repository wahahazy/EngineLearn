#pragma once
#include "basic.h"

struct VertexData
{
	XMFLOAT3 Pos;     //顶点位置
	XMFLOAT3 Normal;  //法向量
	XMFLOAT2 Tex;     //纹理坐标
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
};

struct VertexPos
{
	XMFLOAT3 Pos;     //顶点位置
};

struct MeshData
{
	std::vector<VertexData> vertices;	 //几何体顶点信息集合
	std::vector<WORD> indices;           //三角形索引集
};

struct MeshDataPos
{
	std::vector<VertexPos> vertices;  //几何体顶点信息集合
	std::vector<WORD> indices;           //三角形索引集
};

class GeometryGenerator
{

public:
	GeometryGenerator();
	~GeometryGenerator();

	static MeshData CreatePlane(float length, float centerY, float width, int number = 2, const XMFLOAT4 & color = { 1.0f, 1.0f, 1.0f, 1.0f });  //创建地形,number取2的倍数
	static MeshData CreateCuboid(float length, float height, float width, const XMFLOAT4 & color = { 1.0f, 1.0f, 1.0f, 1.0f });  //创建长方体,length:x方向，width:y方向，height:z方向
	static MeshData CreateCylinder(float radius, float height, int slices, const XMFLOAT4 & color = { 1.0f, 1.0f, 1.0f, 1.0f }); //创建圆柱体
	static MeshDataPos CreateSphere(float radius = 1.0f, int levels = 20, int slices = 20, const XMFLOAT4 & color = { 1.0f, 1.0f, 1.0f, 1.0f });
};
