#include "GameObject.h"

GameObject::GameObject()
{
	this->scaleMatrix = XMMatrixIdentity(); //单位矩阵
	this->rotationMatrixZ = XMMatrixIdentity(); //单位矩阵
	this->rotationMatrixX = XMMatrixIdentity(); //单位矩阵
	this->rotationMatrixY = XMMatrixIdentity(); //单位矩阵
	this->translationMatrix = XMMatrixIdentity(); //单位矩阵
	this->parentWorldMatrix = XMMatrixIdentity(); //单位矩阵
	this->objectTextureRV = nullptr;
	this->objectVertexBuffer = nullptr;
	this->objectIndexBuffer = nullptr;
	this->indexFormat = DXGI_FORMAT_R16_UINT;

}

GameObject::~GameObject()
{

}


XMMATRIX GameObject::GetWorldMatrix()
{
	return scaleMatrix * rotationMatrixZ*rotationMatrixX* rotationMatrixY * translationMatrix * parentWorldMatrix;
}

void XM_CALLCONV GameObject::UpdateParentWorldMatrix(XMMATRIX parentWorldMatrix)
{
	this->parentWorldMatrix = parentWorldMatrix;
}

void GameObject::UpdateTranslationMatrix(float x, float y, float z)
{
	this->translationMatrix *= XMMatrixTranslation(x, y, z);
}

void GameObject::UpdateRotationMatrixZ(float radian)
{
	this->rotationMatrixZ *= XMMatrixRotationZ(radian);
}

void GameObject::UpdateRotationMatrixX(float radian)
{
	this->rotationMatrixX *= XMMatrixRotationX(radian);
}

void GameObject::UpdateRotationMatrixY(float radian)
{
	this->rotationMatrixY *= XMMatrixRotationY(radian);
}

void GameObject::SetRotationMatrixY(float radian)
{
	this->rotationMatrixY = XMMatrixRotationY(radian);
}

void XM_CALLCONV GameObject::SetScaleMatrix(XMMATRIX matrix)
{
	this->scaleMatrix = matrix;
}

bool GameObject::SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, MeshData geometry)
{
	objectVertexStride = sizeof(VertexData);
	vertexNumber = (UINT)geometry.vertices.size();
	// Create vertex buffer
	D3D11_BUFFER_DESC bd = {};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = objectVertexStride * vertexNumber;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = geometry.vertices.data();
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, objectVertexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		return false;


	// Create index buffer
	indexNumber = (UINT)geometry.indices.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * indexNumber;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = geometry.indices.data();
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, objectIndexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		return false;
	return true;
}

bool GameObject::SetBuffer(ComPtr<ID3D11Device> g_pd3dDevice, ObjReader::ObjPart part)
{
	objectVertexStride = (UINT)sizeof(VertexData);
	vertexNumber = (UINT)part.vertices.size();
	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = vertexNumber * objectVertexStride;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = part.vertices.data();

	HRESULT hr = g_pd3dDevice->CreateBuffer(&vbd, &InitData, objectVertexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		return false;
	// 设置索引缓冲区描述
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	if (vertexNumber > 65535)
	{
		indexNumber = (UINT)part.indices32.size();
		indexFormat = DXGI_FORMAT_R32_UINT;
		ibd.ByteWidth = indexNumber * (UINT)sizeof(DWORD);
		InitData.pSysMem = part.indices32.data();

	}
	else
	{
		indexNumber = (UINT)part.indices16.size();
		indexFormat = DXGI_FORMAT_R16_UINT;
		ibd.ByteWidth = indexNumber * (UINT)sizeof(WORD);
		InitData.pSysMem = part.indices16.data();
	}
	// 新建索引缓冲区
	hr = g_pd3dDevice->CreateBuffer(&ibd, &InitData, objectIndexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		return false;

	return true;
}

void GameObject::SetTexture(ComPtr<ID3D11ShaderResourceView> objectTextureRV, ComPtr<ID3D11ShaderResourceView> objectTextureRVTwo)
{
	this->objectTextureRV = objectTextureRV;
	this->objectTextureRVTwo = objectTextureRVTwo;
}

void GameObject::SetMaterial(Material material)  //设置游戏对象的材质
{
	this->material = material;
}

void GameObject::Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, GameObjectRender &render)
{
	//因为每个模型的顶点缓冲区和索引缓冲区是不一样的，因此在渲染过程中context中的缓冲区内容可能会一直发生变化，因此在绘制的时候设置。
	UINT stride = objectVertexStride;
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, objectVertexBuffer.GetAddressOf(), &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(objectIndexBuffer.Get(), indexFormat, 0);
	//更新常量缓冲区

	render.UpdateCBChangesEveryFrame(GetWorldMatrix(), material);
	render.Apply(g_pImmediateContext);

	//设置纹理
	g_pImmediateContext->PSSetShaderResources(0, 1, objectTextureRV.GetAddressOf());
	g_pImmediateContext->PSSetShaderResources(1, 1, objectTextureRVTwo.GetAddressOf());

	//绘制模型
	g_pImmediateContext->DrawIndexed(indexNumber, 0, 0);

}

void GameObject::Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, ShadowMapRender &render)
{
	//因为每个模型的顶点缓冲区和索引缓冲区是不一样的，因此在渲染过程中context中的缓冲区内容可能会一直发生变化，因此在绘制的时候设置。
	UINT stride = objectVertexStride;
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, objectVertexBuffer.GetAddressOf(), &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(objectIndexBuffer.Get(), indexFormat, 0);
	//更新常量缓冲区

	render.UpdateCBChangesEveryFrame(GetWorldMatrix());
	render.Apply(g_pImmediateContext);


	//绘制模型
	g_pImmediateContext->DrawIndexed(indexNumber, 0, 0);

}
