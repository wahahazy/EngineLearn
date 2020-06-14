#include "ShadowMapRender.h"

ShadowMapRender::ShadowMapRender()
{
	this->g_pCBChangesEveryFrame = nullptr;
	this->g_pVertexShader = nullptr;
	this->g_pPixelShader = nullptr;
	this->g_pVertexLayout = nullptr;

	this->shadowMapTexture = nullptr;  //最终得到的阴影映射纹理
	this->g_pRenderTargetView = nullptr;
	this->g_pRenderTargetTexture = nullptr;

	this->g_pDepthStencil = nullptr;
	this->g_pDepthStencilView = nullptr;

}

ShadowMapRender::~ShadowMapRender()
{

}

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
ComPtr<ID3D11ShaderResourceView> ShadowMapRender::GetShadowMapTexture()
{
	return shadowMapTexture;
}

bool ShadowMapRender::InitAll(ComPtr<ID3D11Device> g_pd3dDevice, int textureWidth, int textureHeight)
{
	//新建一个纹理用于保存深度图
	D3D11_TEXTURE2D_DESC textureDesc;
	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	HRESULT result = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, g_pRenderTargetTexture.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = g_pd3dDevice->CreateRenderTargetView(g_pRenderTargetTexture.Get(), &renderTargetViewDesc, g_pRenderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = g_pd3dDevice->CreateShaderResourceView(g_pRenderTargetTexture.Get(), &shaderResourceViewDesc, shadowMapTexture.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = textureWidth;
	descDepth.Height = textureHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	result = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, g_pDepthStencil.GetAddressOf());
	if (FAILED(result))
		return false;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	result = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil.Get(), &descDSV, g_pDepthStencilView.GetAddressOf());
	if (FAILED(result))
		return false;



	// Setup the viewport for rendering.
	ZeroMemory(&g_OutputViewPort, sizeof(g_OutputViewPort));
	g_OutputViewPort.Width = (float)textureWidth;
	g_OutputViewPort.Height = (float)textureHeight;
	g_OutputViewPort.MinDepth = 0.0f;
	g_OutputViewPort.MaxDepth = 1.0f;
	g_OutputViewPort.TopLeftX = 0.0f;
	g_OutputViewPort.TopLeftY = 0.0f;



	//初始化着色器和常量缓冲区
	// Compile the vertex shader
	ComPtr<ID3DBlob> pVSBlob = nullptr;
	result = CreateShaderFromFile(L"HLSL\\LightDepth_VS.cso", L"HLSL\\LightDepth_VS.hlsl", "VS", "vs_4_0", pVSBlob.GetAddressOf());
	if (FAILED(result))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return false;
	}

	// Create the vertex shader
	result = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, g_pVertexShader.GetAddressOf());
	if (FAILED(result))
	{
		//pVSBlob->Release();
		return false;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	result = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	//pVSBlob->Release();
	if (FAILED(result))
		return false;

	// Compile the pixel shader
	ComPtr<ID3DBlob> pPSBlob = nullptr;
	result = CreateShaderFromFile(L"HLSL\\LightDepth_PS.cso", L"HLSL\\LightDepth_PS.hlsl", "PS", "ps_4_0", pPSBlob.GetAddressOf());
	if (FAILED(result))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return false;
	}

	// Create the pixel shader
	result = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, g_pPixelShader.GetAddressOf());
	//pPSBlob->Release();
	if (FAILED(result))
		return false;

	//设置常量缓冲区
	D3D11_BUFFER_DESC bd = {};
	ZeroMemory(&bd, sizeof(bd));
	// Create the constant buffers
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBChangesEveryFrameSkyBox);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	result = g_pd3dDevice->CreateBuffer(&bd, nullptr, g_pCBChangesEveryFrame.GetAddressOf());
	if (FAILED(result))
		return false;


	return true;
}

void ShadowMapRender::SetRenderTarget(ComPtr<ID3D11DeviceContext> g_pImmediateContext)
{
	//设置渲染的目标
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	g_pImmediateContext->OMSetRenderTargets(1, g_pRenderTargetView.GetAddressOf(), g_pDepthStencilView.Get());

	// Set the viewport.
	g_pImmediateContext->RSSetViewports(1, &g_OutputViewPort);
}

void ShadowMapRender::ClearRenderTarget(ComPtr<ID3D11DeviceContext> g_pImmediateContext)
{
	// Just clear the backbuffer
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Black));

	// Clear the depth buffer to 1.0 (max depth)
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void ShadowMapRender::SetRenderDefault(ComPtr<ID3D11DeviceContext>  g_pImmediateContext)
{
	/**************************/
	//为渲染管线各个阶段绑定好所需资源
	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout.Get());
	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	g_pImmediateContext->VSSetShader(g_pVertexShader.Get(), nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader.Get(), nullptr, 0);

}

void XM_CALLCONV ShadowMapRender::UpdateLightViewProj(XMMATRIX ViewProj)
{
	g_LightViewProj = ViewProj;
}

void XM_CALLCONV ShadowMapRender::UpdateCBChangesEveryFrame(XMMATRIX matrix)
{
	g_WorldViewProj = g_LightViewProj * XMMatrixTranspose(matrix);
}

void ShadowMapRender::Apply(ComPtr<ID3D11DeviceContext> g_pImmediateContext)
{
	g_pImmediateContext->VSSetConstantBuffers(0, 1, g_pCBChangesEveryFrame.GetAddressOf());

	CBChangesEveryFrameSkyBox cb;
	cb.worldViewProj = g_WorldViewProj;
	g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame.Get(), 0, nullptr, &cb, 0, 0);
}