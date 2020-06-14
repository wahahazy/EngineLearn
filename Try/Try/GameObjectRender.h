#pragma once
#include "ReadFile.h"
#include "ConstantBufferTypeInfo.h"

class GameObjectRender
{
public:
	// ʹ��ģ�����(C++11)��������
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	GameObjectRender();
	~GameObjectRender();
	bool InitAll(ComPtr<ID3D11Device> g_pd3dDevice);
	void SetRenderDefault(ComPtr<ID3D11DeviceContext>  g_pImmediateContext); //������Ⱦ���̹�������Ҫ����ɫ���ȶ���
	void XM_CALLCONV UpdateCBChangeView(XMMATRIX matrix);  //������������������ռ���������
	void XM_CALLCONV UpdateCBChangeOnResize(XMMATRIX matrix);
	void XM_CALLCONV UpdateCBChangesEveryFrame(XMMATRIX matrix, Material material);
	void UpdateCBLight(DirectionalLight directionalLight, SpotLight spotLight);
	void SetShadowMapTextureDir(ComPtr<ID3D11ShaderResourceView> shadowMapTexture);
	void SetShadowMapTextureSpot(ComPtr<ID3D11ShaderResourceView> shadowMapTexture);
	void Apply(ComPtr<ID3D11DeviceContext> g_pImmediateContext);

private:
	CBLight                         g_light;
	CBChangesEveryFrame             g_changeEveryFrame;
	CBChangeView                    g_changeView;
	CBChangeOnResize				g_changeOnResize;

	ComPtr<ID3D11Buffer>			g_pCBChangeView;
	ComPtr<ID3D11Buffer>			g_pCBChangeOnResize;
	ComPtr<ID3D11Buffer>			g_pCBChangesEveryFrame;
	ComPtr<ID3D11Buffer>			g_pCBLight;

	ComPtr<ID3D11SamplerState>		g_pSampleStateWrap;
	ComPtr<ID3D11SamplerState>		g_pSampleStateClamp;

	ComPtr<ID3D11VertexShader>		g_pVertexShader;
	ComPtr<ID3D11PixelShader>		g_pPixelShader;
	ComPtr<ID3D11InputLayout>		g_pVertexLayout;

	ComPtr<ID3D11ShaderResourceView>		shadowMapTextureDir;
	ComPtr<ID3D11ShaderResourceView>		shadowMapTextureSpot;

};