#pragma once
#include "LightTypeInfo.h"

struct CBChangesEveryFrameSkyBox
{
	CBChangesEveryFrameSkyBox() { memset(this, 0, sizeof(CBChangesEveryFrameSkyBox)); }
	XMMATRIX worldViewProj;
};

struct CBChangeView
{
	CBChangeView() { memset(this, 0, sizeof(CBChangeView)); }
	XMMATRIX mView;
	XMFLOAT3 gEyePosW;
};

struct CBChangeOnResize
{
	CBChangeOnResize() { memset(this, 0, sizeof(CBChangeOnResize)); }
	XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
	CBChangesEveryFrame() { memset(this, 0, sizeof(CBChangesEveryFrame)); }
	XMMATRIX mWorld;
	XMMATRIX mWorldInvTranspose;
	Material mMaterial;
};

struct CBLight
{
	CBLight() { memset(this, 0, sizeof(CBLight)); }
	DirectionalLight gDirectionalLight[1];
	SpotLight gSpotLight[1];
};
