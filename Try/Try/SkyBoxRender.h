#pragma once
#include "ReadFile.h"
#include "ConstantBufferTypeInfo.h"
class SkyBoxRender
{
public:
	// 使用模板别名(C++11)简化类型名
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	SkyBoxRender();
	~SkyBoxRender();

	bool InitAll(ComPtr<ID3D11Device> g_pd3dDevice);
	void SetRenderDefault(ComPtr<ID3D11DeviceContext>  g_pImmediateContext); //设置渲染过程过程中需要的着色器等东西
	void XM_CALLCONV UpdateCBChangesEveryFrame(XMMATRIX matrix);
	void Apply(ComPtr<ID3D11DeviceContext> g_pImmediateContext);

private:
	XMMATRIX								g_WorldViewProj;
	ComPtr<ID3D11Buffer>					g_pCBChangesEveryFrame;
	ComPtr<ID3D11SamplerState>				g_pSamplerLinear;
	ComPtr<ID3D11VertexShader>				g_pVertexShader;
	ComPtr<ID3D11PixelShader>				g_pPixelShader;
	ComPtr<ID3D11InputLayout>				g_pVertexLayout;
	ComPtr<ID3D11RasterizerState>           g_pRSNoCull;  //无背面剔除
	ComPtr<ID3D11DepthStencilState>         g_pDSSLessEqual;  //允许深度为1的像素绘制
};