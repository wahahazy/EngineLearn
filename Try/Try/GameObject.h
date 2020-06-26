#pragma once
#include "GameObjectRender.h"
#include "GeometryGenerator.h"
#include "ObjReader.h"
#include "ShadowMapRender.h"

// 使用模板别名(C++11)简化类型名
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class GameObject
{
public:

	GameObject();
	~GameObject();
	XMMATRIX GetWorldMatrix();
	void XM_CALLCONV UpdateParentWorldMatrix(XMMATRIX parentWorldMatrix);
	void UpdateTranslationMatrix(float x, float y, float z);
	void UpdateRotationMatrixZ(float radian);
	void UpdateRotationMatrixX(float radian);
	void UpdateRotationMatrixY(float radian);
	void SetRotationMatrixY(float radian);
	void XM_CALLCONV SetScaleMatrix(XMMATRIX matrix);

	//设置自行创建的模型的时候
	bool SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, MeshData geometry);
	bool SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, ObjReader::ObjPart objPart);
	void SetTexture(ComPtr<ID3D11ShaderResourceView> objectTextureRV, ComPtr<ID3D11ShaderResourceView> objectTextureRVTwo);
	void SetMaterial(Material material);  //设置游戏对象的材质

	void Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, GameObjectRender &render);
	void Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, ShadowMapRender &render);

private:
	XMMATRIX                                scaleMatrix;           //子模型缩放比例
	XMMATRIX                                rotationMatrixZ;	   //子模型相对于父模型的旋转矩阵
	XMMATRIX                                rotationMatrixX;	   //子模型相对于父模型的旋转矩阵
	XMMATRIX                                rotationMatrixY;	   //子模型相对于父模型的旋转矩阵
	XMMATRIX                                translationMatrix;	   //子模型相对于父模型的平移矩阵
	XMMATRIX                                parentWorldMatrix;     //父模型的世界矩阵

	Material                                material;
	ComPtr<ID3D11ShaderResourceView>		objectTextureRV;
	ComPtr<ID3D11ShaderResourceView>		objectTextureRVTwo;
	ComPtr<ID3D11Buffer>					objectVertexBuffer;
	ComPtr<ID3D11Buffer>					objectIndexBuffer;
	UINT									objectVertexStride;	     // 顶点字节大小
	UINT									indexNumber;          //模型索引数量
	UINT									vertexNumber;         //模型顶点数量
	DXGI_FORMAT								indexFormat;




};

