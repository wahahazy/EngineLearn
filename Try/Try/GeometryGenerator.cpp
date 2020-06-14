#include "GeometryGenerator.h"

GeometryGenerator::GeometryGenerator()
{

}

GeometryGenerator::~GeometryGenerator()
{

}

MeshData GeometryGenerator::CreatePlane(float length, float centerY, float width, int number, const XMFLOAT4 & color)
{
	MeshData plane;
	plane.vertices.resize(number*number * 4);
	plane.indices.resize(number*number * 2 * 3);

	//左上角的顶点
	float baseZ = length * number / 2;
	float baseX = -width * number / 2;

	int index = 0;
	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			plane.vertices[index].Pos = XMFLOAT3(baseX + j * length, centerY, baseZ - i * width);
			plane.vertices[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			plane.vertices[index].Tex = XMFLOAT2(0.0f, 0.0f);
			index++;

			plane.vertices[index].Pos = XMFLOAT3(baseX + j * length + length, centerY, baseZ - i * width);
			plane.vertices[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			plane.vertices[index].Tex = XMFLOAT2(1.0f, 0.0f);
			index++;


			plane.vertices[index].Pos = XMFLOAT3(baseX + j * length + length, centerY, baseZ - i * width - width);
			plane.vertices[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			plane.vertices[index].Tex = XMFLOAT2(1.0f, 1.0f);
			index++;

			plane.vertices[index].Pos = XMFLOAT3(baseX + j * length, centerY, baseZ - i * width - width);
			plane.vertices[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			plane.vertices[index].Tex = XMFLOAT2(0.0f, 1.0f);
			index++;

		}
	}

	index = 0;
	int baseIndex = 0;
	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			baseIndex = i * number * 4 + j * 4;
			plane.indices[index++] = baseIndex;
			plane.indices[index++] = baseIndex + 1;
			plane.indices[index++] = baseIndex + 3;

			plane.indices[index++] = baseIndex + 3;
			plane.indices[index++] = baseIndex + 1;
			plane.indices[index++] = baseIndex + 2;
		}
	}
	return plane;
}

// ******************
// 设置长方体顶点
//        前
//    1________ 2
//    /|      /|
//   /_|_____/ |
//  0|6|_ _ 3|_|5
//   | /     | /
//   |/______|/
//  7       4
//      
//     人

MeshData GeometryGenerator::CreateCuboid(float length, float height, float width, const XMFLOAT4 & color)
{
	MeshData cuboid;
	cuboid.vertices.resize(24);
	float l2 = length / 2, w2 = width / 2, h2 = height / 2;

	// 顶面
	cuboid.vertices[0].Pos = XMFLOAT3(-l2, h2, -w2);
	cuboid.vertices[1].Pos = XMFLOAT3(-l2, h2, w2);
	cuboid.vertices[2].Pos = XMFLOAT3(l2, h2, w2);
	cuboid.vertices[3].Pos = XMFLOAT3(l2, h2, -w2);
	// 底面
	cuboid.vertices[4].Pos = XMFLOAT3(l2, -h2, -w2);
	cuboid.vertices[5].Pos = XMFLOAT3(l2, -h2, w2);
	cuboid.vertices[6].Pos = XMFLOAT3(-l2, -h2, w2);
	cuboid.vertices[7].Pos = XMFLOAT3(-l2, -h2, -w2);
	// 左面
	cuboid.vertices[8].Pos = XMFLOAT3(-l2, -h2, w2);
	cuboid.vertices[9].Pos = XMFLOAT3(-l2, h2, w2);
	cuboid.vertices[10].Pos = XMFLOAT3(-l2, h2, -w2);
	cuboid.vertices[11].Pos = XMFLOAT3(-l2, -h2, -w2);
	// 右面
	cuboid.vertices[12].Pos = XMFLOAT3(l2, -h2, -w2);
	cuboid.vertices[13].Pos = XMFLOAT3(l2, h2, -w2);
	cuboid.vertices[14].Pos = XMFLOAT3(l2, h2, w2);
	cuboid.vertices[15].Pos = XMFLOAT3(l2, -h2, w2);
	// 前面
	cuboid.vertices[16].Pos = XMFLOAT3(l2, -h2, w2);
	cuboid.vertices[17].Pos = XMFLOAT3(l2, h2, w2);
	cuboid.vertices[18].Pos = XMFLOAT3(-l2, h2, w2);
	cuboid.vertices[19].Pos = XMFLOAT3(-l2, -h2, w2);
	// 后面
	cuboid.vertices[20].Pos = XMFLOAT3(-l2, -h2, -w2);
	cuboid.vertices[21].Pos = XMFLOAT3(-l2, h2, -w2);
	cuboid.vertices[22].Pos = XMFLOAT3(l2, h2, -w2);
	cuboid.vertices[23].Pos = XMFLOAT3(l2, -h2, -w2);

	for (int i = 0; i < 4; ++i)
	{
		cuboid.vertices[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);		// 顶面

		cuboid.vertices[i + 4].Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);	// 底面

		cuboid.vertices[i + 8].Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);	// 左面

		cuboid.vertices[i + 12].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);	// 右面

		cuboid.vertices[i + 16].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);	// 前面

		cuboid.vertices[i + 20].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f); // 后面
	}

	for (int i = 0; i < 6; ++i)
	{
		cuboid.vertices[i * 4].Tex = XMFLOAT2(0.0f, 1.0f);
		cuboid.vertices[i * 4 + 1].Tex = XMFLOAT2(0.0f, 0.0f);
		cuboid.vertices[i * 4 + 2].Tex = XMFLOAT2(1.0f, 0.0f);
		cuboid.vertices[i * 4 + 3].Tex = XMFLOAT2(1.0f, 1.0f);
	}



	cuboid.indices = {
		// 顶面
		0, 1, 2,
		2, 3, 0,

		// 底面
		4, 5, 6,
		6, 7, 4,

		// 左面
		8, 9, 10,
		10, 11, 8,

		// 右面
		12, 13, 14,
		14, 15, 12,

		// 前面
		16, 17, 18,
		18, 19, 16,

		// 后面
		20, 21, 22,
		22, 23, 20
	};

	return cuboid;
}

MeshData GeometryGenerator::CreateCylinder(float radius, float height, int slices, const XMFLOAT4 & color)
{
	MeshData cylinder;
	int index = 0, sideFaceStartIndex;
	cylinder.vertices.resize(4 * slices + 2);
	cylinder.indices.resize(12 * slices);

	float h2 = height / 2;
	float theta = 0.0f;
	float per_theta = XM_2PI / slices;

	//顶端圆面圆心
	cylinder.vertices[index].Pos = XMFLOAT3(0, h2, 0);
	cylinder.vertices[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	cylinder.vertices[index].Tex = XMFLOAT2(0.5f, 0.5f);
	++index;


	//放入顶端圆上各点
	for (int i = 0; i < slices; ++i)
	{
		theta = i * per_theta;
		cylinder.vertices[index].Pos = XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta));
		cylinder.vertices[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		cylinder.vertices[index].Tex = XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f);
		++index;
	}

	//底端圆面圆心
	cylinder.vertices[index].Pos = XMFLOAT3(0, -h2, 0);
	cylinder.vertices[index].Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	cylinder.vertices[index].Tex = XMFLOAT2(0.5f, 0.5f);
	++index;

	//放入底端圆上各点
	for (int i = 0; i < slices; ++i)
	{
		theta = i * per_theta;
		cylinder.vertices[index].Pos = XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta));
		cylinder.vertices[index].Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		cylinder.vertices[index].Tex = XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f);
		++index;
	}
	sideFaceStartIndex = index;

	//放入侧面顶端各点
	for (int i = 0; i < slices; ++i)
	{
		theta = i * per_theta;
		cylinder.vertices[index].Pos = XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta));
		cylinder.vertices[index].Normal = XMFLOAT3(cosf(theta), 0.0f, sinf(theta));
		cylinder.vertices[index].Tex = XMFLOAT2(theta / XM_2PI, 0.0f);
		++index;
	}


	//放入侧面底端各点
	for (int i = 0; i < slices; ++i)
	{
		theta = i * per_theta;
		cylinder.vertices[index].Pos = XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta));
		cylinder.vertices[index].Normal = XMFLOAT3(cosf(theta), 0.0f, sinf(theta));
		cylinder.vertices[index].Tex = XMFLOAT2(theta / XM_2PI, 1.0f);
		++index;
	}


	index = 0;
	//加入顶端圆面的索引
	for (int i = 0; i < slices; ++i)
	{
		cylinder.indices[index++] = i + 1;
		cylinder.indices[index++] = 0;
		cylinder.indices[index++] = (i + 1) % slices + 1;
	}

	//加入底端圆面的索引
	for (int i = 0; i < slices; ++i)
	{
		cylinder.indices[index++] = (i + 1) % slices + slices + 2;
		cylinder.indices[index++] = slices + 1;
		cylinder.indices[index++] = i + slices + 2;
	}

	//加入侧面的索引
	for (int i = 0; i < slices; ++i)
	{
		cylinder.indices[index++] = i + sideFaceStartIndex;
		cylinder.indices[index++] = (i + 1) % slices + sideFaceStartIndex;
		cylinder.indices[index++] = i + slices + sideFaceStartIndex;

		cylinder.indices[index++] = i + slices + sideFaceStartIndex;
		cylinder.indices[index++] = (i + 1) % slices + sideFaceStartIndex;
		cylinder.indices[index++] = (i + 1) % slices + slices + sideFaceStartIndex;
	}

	return cylinder;
}

MeshDataPos GeometryGenerator::CreateSphere(float radius, int levels, int slices, const XMFLOAT4 & color)
{
	MeshDataPos sphere;
	sphere.vertices.resize(2 + (levels - 1) * (slices + 1));
	sphere.indices.resize(6 * (levels - 1) * slices);

	int index = 0;

	float phi = 0.0f, theta = 0.0f;
	float per_phi = XM_PI / levels;
	float per_theta = XM_2PI / slices;
	float x, y, z;

	// 放入顶端点
	sphere.vertices[index].Pos = XMFLOAT3(0.0f, radius, 0.0f);
	index++;

	for (int i = 1; i < levels; ++i)
	{
		phi = per_phi * i;
		// 需要slices + 1个顶点是因为 起点和终点需为同一点，但纹理坐标值不一致
		for (int j = 0; j <= slices; ++j)
		{
			theta = per_theta * j;
			x = radius * sinf(phi) * cosf(theta);
			y = radius * cosf(phi);
			z = radius * sinf(phi) * sinf(theta);
			// 计算出局部坐标、法向量、Tangent向量和纹理坐标
			sphere.vertices[index].Pos = XMFLOAT3(x, y, z);
			index++;
		}
	}

	// 放入底端点

	sphere.vertices[index].Pos = XMFLOAT3(0.0f, -radius, 0.0f);
	index++;

	index = 0;
	// 逐渐放入索引
	if (levels > 1)
	{
		for (int j = 1; j <= slices; ++j)
		{
			sphere.indices[index++] = 0;
			sphere.indices[index++] = j % (slices + 1) + 1;
			sphere.indices[index++] = j;
		}
	}


	for (int i = 1; i < levels - 1; ++i)
	{
		for (int j = 1; j <= slices; ++j)
		{
			sphere.indices[index++] = (i - 1) * (slices + 1) + j;
			sphere.indices[index++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
			sphere.indices[index++] = i * (slices + 1) + j % (slices + 1) + 1;

			sphere.indices[index++] = i * (slices + 1) + j % (slices + 1) + 1;
			sphere.indices[index++] = i * (slices + 1) + j;
			sphere.indices[index++] = (i - 1) * (slices + 1) + j;
		}
	}

	// 逐渐放入索引
	if (levels > 1)
	{
		for (int j = 1; j <= slices; ++j)
		{
			sphere.indices[index++] = (levels - 2) * (slices + 1) + j;
			sphere.indices[index++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
			sphere.indices[index++] = (levels - 1) * (slices + 1) + 1;
		}
	}


	return sphere;
}