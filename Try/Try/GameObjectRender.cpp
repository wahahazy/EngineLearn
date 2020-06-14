#include "GameObjectRender.h"


GameObjectRender::GameObjectRender()
{
	this->g_pSampleStateWrap = nullptr;
	this->g_pSampleStateClamp = nullptr;
	this->g_pVertexShader = nullptr;
	this->g_pPixelShader = nullptr;
	this->g_pVertexLayout = nullptr;

	this->g_pCBChangeView = nullptr;
	this->g_pCBChangeOnResize = nullptr;
	this->g_pCBChangesEveryFrame = nullptr;
	this->g_pCBLight = nullptr;

	this->shadowMapTextureDir = nullptr;
	this->shadowMapTextureSpot = nullptr;

}
GameObjectRender::~GameObjectRender()
{
}

bool GameObjectRender::InitAll(ComPtr<ID3D11Device> g_pd3dDevice)
{

	// Compile the vertex shader
	ComPtr<ID3DBlob> pVSBlob = nullptr;
	HRESULT hr = CreateShaderFromFile(L"HLSL\\GameObject_VS.cso", L"HLSL\\GameObject_VS.hlsl", "VS", "vs_4_0", pVSBlob.GetAddressOf());
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), g_pVertexLayout.GetAddressOf());
	//pVSBlob->Release();
	if (FAILED(hr))
		return false;

	// Compile the pixel shader
	ComPtr<ID3DBlob> pPSBlob = nullptr;
	hr = CreateShaderFromFile(L"HLSL\\GameObject_PS.cso", L"HLSL\\GameObject_PS.hlsl", "PS", "ps_4_0", pPSBlob.GetAddressOf());
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
	bd.ByteWidth = sizeof(CBChangeView);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, g_pCBChangeView.GetAddressOf());
	if (FAILED(hr))
		return false;

	bd.ByteWidth = sizeof(CBChangeOnResize);
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, g_pCBChangeOnResize.GetAddressOf());
	if (FAILED(hr))
		return false;

	bd.ByteWidth = sizeof(CBChangesEveryFrame);
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, g_pCBChangesEveryFrame.GetAddressOf());
	if (FAILED(hr))
		return false;

	bd.ByteWidth = sizeof(CBLight);
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, g_pCBLight.GetAddressOf());
	if (FAILED(hr))
		return false;


	D3D11_SAMPLER_DESC samplerDesc = {};
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = g_pd3dDevice->CreateSamplerState(&samplerDesc, g_pSampleStateWrap.GetAddressOf());
	if (FAILED(hr))
		return false;

	// Create a clamp texture sampler state description.
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	hr = g_pd3dDevice->CreateSamplerState(&samplerDesc, g_pSampleStateClamp.GetAddressOf());
	if (FAILED(hr))
		return false;

	return true;
}

void XM_CALLCONV GameObjectRender::UpdateCBChangeView(XMMATRIX matrix)  //传入的是摄像机到世界空间的世界矩阵
{
	XMVECTOR Position = matrix.r[3];
	XMFLOAT4 pos;
	XMStoreFloat4(&pos, Position);
	g_changeView.gEyePosW = XMFLOAT3(pos.x, pos.y, pos.z);
	g_changeView.mView = XMMatrixTranspose(XMMatrixInverse(nullptr, matrix));
}

void XM_CALLCONV GameObjectRender::UpdateCBChangeOnResize(XMMATRIX matrix)
{
	g_changeOnResize.mProjection = XMMatrixTranspose(matrix);
}

void XM_CALLCONV GameObjectRender::UpdateCBChangesEveryFrame(XMMATRIX matrix, Material material)
{
	g_changeEveryFrame.mWorld = XMMatrixTranspose(matrix);
	g_changeEveryFrame.mWorldInvTranspose = XMMatrixInverse(nullptr, matrix);
	g_changeEveryFrame.mMaterial = material;
}

void GameObjectRender::UpdateCBLight(DirectionalLight directionalLight, SpotLight spotLight)
{
	g_light.gDirectionalLight[0] = directionalLight;
	g_light.gSpotLight[0] = spotLight;
}

void GameObjectRender::SetShadowMapTextureDir(ComPtr<ID3D11ShaderResourceView> shadowMapTexture)
{
	this->shadowMapTextureDir = shadowMapTexture;
}

void GameObjectRender::SetShadowMapTextureSpot(ComPtr<ID3D11ShaderResourceView> shadowMapTexture)
{
	this->shadowMapTextureSpot = shadowMapTexture;
}

void GameObjectRender::SetRenderDefault(ComPtr<ID3D11DeviceContext> g_pImmediateContext)
{
	/**************************/
	//为渲染管线各个阶段绑定好所需资源
	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout.Get());
	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	g_pImmediateContext->VSSetShader(g_pVertexShader.Get(), nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader.Get(), nullptr, 0);
	g_pImmediateContext->PSSetSamplers(0, 1, g_pSampleStateWrap.GetAddressOf());
	g_pImmediateContext->PSSetSamplers(1, 1, g_pSampleStateClamp.GetAddressOf());
	g_pImmediateContext->PSSetShaderResources(2, 1, shadowMapTextureDir.GetAddressOf());
	g_pImmediateContext->PSSetShaderResources(3, 1, shadowMapTextureSpot.GetAddressOf());

}

void GameObjectRender::Apply(ComPtr<ID3D11DeviceContext> g_pImmediateContext)
{
	g_pImmediateContext->VSSetConstantBuffers(0, 1, g_pCBChangeView.GetAddressOf());
	g_pImmediateContext->VSSetConstantBuffers(1, 1, g_pCBChangeOnResize.GetAddressOf());
	g_pImmediateContext->VSSetConstantBuffers(2, 1, g_pCBChangesEveryFrame.GetAddressOf());
	g_pImmediateContext->VSSetConstantBuffers(3, 1, g_pCBLight.GetAddressOf());
	g_pImmediateContext->PSSetConstantBuffers(0, 1, g_pCBChangeView.GetAddressOf());
	g_pImmediateContext->PSSetConstantBuffers(2, 1, g_pCBChangesEveryFrame.GetAddressOf());
	g_pImmediateContext->PSSetConstantBuffers(3, 1, g_pCBLight.GetAddressOf());

	g_pImmediateContext->UpdateSubresource(g_pCBChangeView.Get(), 0, nullptr, &g_changeView, 0, 0);

	g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize.Get(), 0, nullptr, &g_changeOnResize, 0, 0);

	g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame.Get(), 0, nullptr, &g_changeEveryFrame, 0, 0);

	g_pImmediateContext->UpdateSubresource(g_pCBLight.Get(), 0, nullptr, &g_light, 0, 0);



}
