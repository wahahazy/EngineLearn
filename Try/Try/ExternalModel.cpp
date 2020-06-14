#include "ExternalModel.h"

ExternalModel::ExternalModel()
{

}

ExternalModel::~ExternalModel()
{

}

bool ExternalModel::CreateModel(ComPtr<ID3D11Device> g_pd3dDevice, const ObjReader & model)
{
	modelParts.resize(model.objParts.size());

	BoundingBox::CreateFromPoints(boundingBox, XMLoadFloat3(&model.vMin), XMLoadFloat3(&model.vMax));

	for (size_t i = 0; i < model.objParts.size(); ++i)
	{
		auto part = model.objParts[i];
		modelParts[i].SetBuffer(g_pd3dDevice, part);

		// 创建漫射光对应纹理
		auto& strD = part.texStrDiffuse;
		if (strD.size() > 4)
		{
			if (strD.substr(strD.size() - 3, 3) == L"dds")
			{
				ComPtr<ID3D11ShaderResourceView> texture;
				HRESULT hr = CreateDDSTextureFromFile(g_pd3dDevice.Get(), strD.c_str(), nullptr, texture.GetAddressOf());
				if (FAILED(hr))
					return false;
				modelParts[i].SetTexture(texture, texture);

			}
			else
			{
				ComPtr<ID3D11ShaderResourceView> texture;
				HRESULT hr = CreateWICTextureFromFile(g_pd3dDevice.Get(), strD.c_str(), nullptr, texture.GetAddressOf());
				if (FAILED(hr))
					return false;

				modelParts[i].SetTexture(texture, texture);
			}
		}

		part.material.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 10.0f);
		modelParts[i].SetMaterial(part.material);
	}

	return true;
}

void XM_CALLCONV ExternalModel::SetScaleMatrix(XMMATRIX matrix)
{
	for (auto& part : modelParts)
	{
		part.SetScaleMatrix(matrix);
	}
}

void ExternalModel::SetTranslationMatrix(float x, float y, float z)
{
	for (auto& part : modelParts)
	{
		part.UpdateTranslationMatrix(x, y, z);
	}
}

void ExternalModel::SetRotationMatrix(float radian)
{
	for (auto& part : modelParts)
	{
		part.SetRotationMatrixY(radian);
	}
}

BoundingBox ExternalModel::GetLocalBoundingBox()
{
	return boundingBox;
}

void XM_CALLCONV ExternalModel::SetWorldMatrix(XMMATRIX world)
{
	this->worldMatrix = world;
}

void ExternalModel::Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, GameObjectRender &render)
{
	for (auto& part : modelParts)
	{
		part.Draw(g_pImmediateContext, render);
	}
}

void ExternalModel::Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, ShadowMapRender &render)
{
	for (auto& part : modelParts)
	{
		part.Draw(g_pImmediateContext, render);
	}
}