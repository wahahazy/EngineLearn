#pragma once
#include "GameObjectRender.h"
#include "GeometryGenerator.h"
#include "ObjReader.h"
#include "ShadowMapRender.h"

// ʹ��ģ�����(C++11)��������
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

	//�������д�����ģ�͵�ʱ��
	bool SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, MeshData geometry);
	bool SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, ObjReader::ObjPart objPart);
	void SetTexture(ComPtr<ID3D11ShaderResourceView> objectTextureRV, ComPtr<ID3D11ShaderResourceView> objectTextureRVTwo);
	void SetMaterial(Material material);  //������Ϸ����Ĳ���

	void Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, GameObjectRender &render);
	void Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, ShadowMapRender &render);

private:
	XMMATRIX                                scaleMatrix;           //��ģ�����ű���
	XMMATRIX                                rotationMatrixZ;	   //��ģ������ڸ�ģ�͵���ת����
	XMMATRIX                                rotationMatrixX;	   //��ģ������ڸ�ģ�͵���ת����
	XMMATRIX                                rotationMatrixY;	   //��ģ������ڸ�ģ�͵���ת����
	XMMATRIX                                translationMatrix;	   //��ģ������ڸ�ģ�͵�ƽ�ƾ���
	XMMATRIX                                parentWorldMatrix;     //��ģ�͵��������

	Material                                material;
	ComPtr<ID3D11ShaderResourceView>		objectTextureRV;
	ComPtr<ID3D11ShaderResourceView>		objectTextureRVTwo;
	ComPtr<ID3D11Buffer>					objectVertexBuffer;
	ComPtr<ID3D11Buffer>					objectIndexBuffer;
	UINT									objectVertexStride;	     // �����ֽڴ�С
	UINT									indexNumber;          //ģ����������
	UINT									vertexNumber;         //ģ�Ͷ�������
	DXGI_FORMAT								indexFormat;




};

