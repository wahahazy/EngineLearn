#pragma once
#include "Camera.h"
#include "ReadFile.h"
#include "GeometryGenerator.h"
#include "SkyBoxRender.h"

class SkyBox
{
public:
	// ʹ��ģ�����(C++11)��������
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	SkyBox();
	~SkyBox();

	void Init(ComPtr<ID3D11Device> g_pd3dDevice, ComPtr<ID3D11DeviceContext> g_pImmediateContext, const std::wstring & cubeMapFileName, float skySphereRadius);
	void Init(ComPtr<ID3D11Device> g_pd3dDevice, ComPtr<ID3D11DeviceContext> g_pImmediateContext, const std::vector<std::wstring> & cubeMapFileName, float skySphereRadius);
	void Draw(ComPtr<ID3D11DeviceContext> g_pImmediateContext, SkyBoxRender skyBoxRender, Camera& camera);
private:
	void SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, float skySphereRadius); //��պ��ӵİ뾶�޹ؽ�Ҫ����˲������洢

	ComPtr<ID3D11Buffer> skyBoxVertexBuffer;
	ComPtr<ID3D11Buffer> skyBoxIndexBuffer;
	ComPtr<ID3D11ShaderResourceView> textureCubeSRV;   //��պ��ӵ�������ͼ
	UINT indexNumber;

};