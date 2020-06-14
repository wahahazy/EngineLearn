#include "SkyBox.h"

SkyBox::SkyBox()
{
	this->skyBoxVertexBuffer = nullptr;
	this->skyBoxIndexBuffer = nullptr;
	this->textureCubeSRV = nullptr;
	this->indexNumber = 0;
}

SkyBox::~SkyBox()
{

}

void SkyBox::Init(ComPtr<ID3D11Device> g_pd3dDevice, ComPtr<ID3D11DeviceContext> g_pImmediateContext, const std::wstring & cubeMapFileName, float skySphereRadius)
{
	//读取文件得到cubemap贴图
	HRESULT hr = (CreateWICTexture2DCubeFromFile(g_pd3dDevice.Get(), g_pImmediateContext.Get(), cubeMapFileName, nullptr, &textureCubeSRV, false));
	if (FAILED(hr))
		return;

	SetBuffer(g_pd3dDevice, skySphereRadius);
}

void SkyBox::Init(ComPtr<ID3D11Device> g_pd3dDevice, ComPtr<ID3D11DeviceContext> g_pImmediateContext, const std::vector<std::wstring> & cubeMapFileName, float skySphereRadius)
{
	HRESULT hr = (CreateWICTexture2DCubeFromFile(g_pd3dDevice.Get(), g_pImmediateContext.Get(), cubeMapFileName, nullptr, &textureCubeSRV, false));
	if (FAILED(hr))
		return;
	SetBuffer(g_pd3dDevice, skySphereRadius);
}

void SkyBox::Draw(ComPtr<ID3D11DeviceContext> g_pImmediateContext, SkyBoxRender skyBoxRender, Camera& camera)
{
	UINT strides = sizeof(XMFLOAT3);
	UINT offsets = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, skyBoxVertexBuffer.GetAddressOf(), &strides, &offsets);
	g_pImmediateContext->IASetIndexBuffer(skyBoxIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//从世界矩阵中取出平移变换的数值
	XMMATRIX CameraWorld = camera.GetWorld();
	XMVECTOR Position = CameraWorld.r[3];
	XMFLOAT4 pos;
	XMStoreFloat4(&pos, Position);

	//旋转天空球，以便将夕阳放置到合适的位置,用于之后设置平行光的来源
	skyBoxRender.UpdateCBChangesEveryFrame(XMMatrixRotationY(XM_PI + XM_PI / 5)*XMMatrixTranslation(pos.x, pos.y, pos.z)*camera.GetView()*camera.GetProjection());
	skyBoxRender.Apply(g_pImmediateContext.Get());

	// 设置SRV
	g_pImmediateContext->PSSetShaderResources(0, 1, textureCubeSRV.GetAddressOf());

	g_pImmediateContext->DrawIndexed(indexNumber, 0, 0);
}

void SkyBox::SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, float skySphereRadius)
{

	auto sphere = GeometryGenerator::CreateSphere(skySphereRadius);

	// 顶点缓冲区创建
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * (UINT)sphere.vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = sphere.vertices.data();

	HRESULT hr = g_pd3dDevice->CreateBuffer(&vbd, &InitData, skyBoxVertexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		return;

	// 索引缓冲区创建
	indexNumber = (UINT)sphere.indices.size();

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(WORD) * indexNumber;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	InitData.pSysMem = sphere.indices.data();

	hr = g_pd3dDevice->CreateBuffer(&ibd, &InitData, skyBoxIndexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		return;
}