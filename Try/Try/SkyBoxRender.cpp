#include "SkyBoxRender.h"

SkyBoxRender::SkyBoxRender()
{
	this->g_pSamplerLinear = nullptr;
	this->g_pVertexShader = nullptr;
	this->g_pPixelShader = nullptr;
	this->g_pVertexLayout = nullptr;

	this->g_pCBChangesEveryFrame = nullptr;
	this->g_WorldViewProj = XMMatrixIdentity(); //单位矩阵

	this->g_pRSNoCull = nullptr;
	this->g_pDSSLessEqual = nullptr;
}

SkyBoxRender::~SkyBoxRender()
{
}

bool SkyBoxRender::InitAll(ComPtr<ID3D11Device> g_pd3dDevice)
{
	// Compile the vertex shader
	ComPtr<ID3DBlob> pVSBlob = nullptr;
	HRESULT hr = CreateShaderFromFile(L"HLSL\\SkyBox_VS.cso", L"HLSL\\SkyBox_VS.hlsl", "VS", "vs_4_0", pVSBlob.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return false;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, g_pVertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		//pVSBlob->Release();
		return false;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	//pVSBlob->Release();
	if (FAILED(hr))
		return false;

	// Compile the pixel shader
	ComPtr<ID3DBlob> pPSBlob = nullptr;
	hr = CreateShaderFromFile(L"HLSL\\SkyBox_PS.cso", L"HLSL\\SkyBox_PS.hlsl", "PS", "ps_4_0", pPSBlob.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return false;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, g_pPixelShader.GetAddressOf());
	//pPSBlob->Release();
	if (FAILED(hr))
		return false;

	//设置常量缓冲区
	D3D11_BUFFER_DESC bd = {};
	ZeroMemory(&bd, sizeof(bd));
	// Create the constant buffers
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBChangesEveryFrameSkyBox);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, g_pCBChangesEveryFrame.GetAddressOf());
	if (FAILED(hr))
		return false;

	// Create the sample state

	// 线性过滤模式
	D3D11_SAMPLER_DESC sampDesc = {};
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, g_pSamplerLinear.GetAddressOf());
	if (FAILED(hr))
		return false;

	// 无背面剔除模式
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	hr = g_pd3dDevice->CreateRasterizerState(&rasterizerDesc, g_pRSNoCull.GetAddressOf());
	if (FAILED(hr))
		return false;

	// 初始化深度/模板状态
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	// 允许使用深度值一致的像素进行替换的深度/模板状态
	// 该状态用于绘制天空盒，因为深度值为1.0时默认无法通过深度测试
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = false;
	hr = g_pd3dDevice->CreateDepthStencilState(&dsDesc, g_pDSSLessEqual.GetAddressOf());
	if (FAILED(hr))
		return false;

	return true;

}

void SkyBoxRender::SetRenderDefault(ComPtr<ID3D11DeviceContext>  g_pImmediateContext)
{
	/**************************/
	//为渲染管线各个阶段绑定好所需资源
	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout.Get());
	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pImmediateContext->VSSetShader(g_pVertexShader.Get(), nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader.Get(), nullptr, 0);
	g_pImmediateContext->RSSetState(g_pRSNoCull.Get());
	g_pImmediateContext->PSSetSamplers(0, 1, g_pSamplerLinear.GetAddressOf());
	g_pImmediateContext->OMSetDepthStencilState(g_pDSSLessEqual.Get(), 0);
	g_pImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void XM_CALLCONV SkyBoxRender::UpdateCBChangesEveryFrame(XMMATRIX matrix)
{
	g_WorldViewProj = XMMatrixTranspose(matrix);
}

void SkyBoxRender::Apply(ComPtr<ID3D11DeviceContext> g_pImmediateContext)
{
	g_pImmediateContext->VSSetConstantBuffers(0, 1, g_pCBChangesEveryFrame.GetAddressOf());

	CBChangesEveryFrameSkyBox cb;
	cb.worldViewProj = g_WorldViewProj;
	g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame.Get(), 0, nullptr, &cb, 0, 0);

}