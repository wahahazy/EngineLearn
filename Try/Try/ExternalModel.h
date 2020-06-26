#pragma once
#include "GameObject.h"

class ExternalModel
{
public:

	ExternalModel();
	~ExternalModel();

	bool CreateModel(ComPtr<ID3D11Device> g_pd3dDevice, const ObjReader & model);
	void XM_CALLCONV SetScaleMatrix(XMMATRIX matrix);
	void SetTranslationMatrix(float x, float y, float z);
	void SetRotationMatrix(float radian);

	BoundingBox GetLocalBoundingBox();

	void XM_CALLCONV SetWorldMatrix(XMMATRIX world);

	void Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, GameObjectRender &render);
	void Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, ShadowMapRender &render);

private:
	XMMATRIX   worldMatrix;
	std::vector<GameObject> modelParts;
	BoundingBox boundingBox;


};