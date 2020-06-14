#pragma once
#include "Camera.h"
#include "ReadFile.h"
#include "GeometryGenerator.h"
#include "SkyBoxRender.h"

class SkyBox
{
public:
	// 使用模板别名(C++11)简化类型名
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	SkyBox();
	~SkyBox();

	void Init(ComPtr<ID3D11Device> g_pd3dDevice, ComPtr<ID3D11DeviceContext> g_pImmediateContext, const std::wstring & cubeMapFileName, float skySphereRadius);
	void Init(ComPtr<ID3D11Device> g_pd3dDevice, ComPtr<ID3D11DeviceContext> g_pImmediateContext, const std::vector<std::wstring> & cubeMapFileName, float skySphereRadius);
	void Draw(ComPtr<ID3D11DeviceContext> g_pImmediateContext, SkyBoxRender skyBoxRender, Camera& camera);
private:
	void SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, float skySphereRadius); //天空盒子的半径无关紧要，因此不单独存储

	ComPtr<ID3D11Buffer> skyBoxVertexBuffer;
	ComPtr<ID3D11Buffer> skyBoxIndexBuffer;
	ComPtr<ID3D11ShaderResourceView> textureCubeSRV;   //天空盒子的纹理视图
	UINT indexNumber;

};