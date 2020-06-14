#pragma once
#include "ReadFile.h"
#include "ConstantBufferTypeInfo.h"
class ShadowMapRender
{
public:
	// 使用模板别名(C++11)简化类型名
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ShadowMapRender();
	~ShadowMapRender();
	ComPtr<ID3D11ShaderResourceView> GetShadowMapTexture();
	bool InitAll(ComPtr<ID3D11Device> g_pd3dDevice, int textureWidth, int textureHeight);
	void SetRenderTarget(ComPtr<ID3D11DeviceContext> g_pImmediateContext);
	void ClearRenderTarget(ComPtr<ID3D11DeviceContext> g_pImmediateContext);
	void SetRenderDefault(ComPtr<ID3D11DeviceContext>  g_pImmediateContext); //设置渲染过程过程中需要的着色器等东西
	void XM_CALLCONV UpdateLightViewProj(XMMATRIX ViewProj);
	void XM_CALLCONV UpdateCBChangesEveryFrame(XMMATRIX matrix);
	void Apply(ComPtr<ID3D11DeviceContext> g_pImmediateContext);

private:
	XMMATRIX								g_LightViewProj;
	XMMATRIX								g_WorldViewProj;
	ComPtr<ID3D11Buffer>					g_pCBChangesEveryFrame;
	ComPtr<ID3D11VertexShader>				g_pVertexShader;
	ComPtr<ID3D11PixelShader>				g_pPixelShader;
	ComPtr<ID3D11InputLayout>				g_pVertexLayout;


	ComPtr<ID3D11ShaderResourceView>	shadowMapTexture;  //最终得到的阴影映射纹理
	ComPtr<ID3D11RenderTargetView>		g_pRenderTargetView;
	ComPtr<ID3D11Texture2D>				g_pRenderTargetTexture;
	ComPtr<ID3D11Texture2D>				g_pDepthStencil;
	ComPtr<ID3D11DepthStencilView>		g_pDepthStencilView;
	D3D11_VIEWPORT						g_OutputViewPort;	// 输出所用的视口


};
