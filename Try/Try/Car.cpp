#include "Car.h"

Car::Car()
{
	this->wSlices = 64;    //车轮圆切割的块数，默认为64块
	this->parentLeftRightRadian = 0;
	this->parentLeftRightRadianMax = XM_PI / 3;  //车轮最大的左右转角为60度
	this->totalRadian = 0;
}

Car::~Car()
{

}

void Car::CreateCarModel(float cLength, float cHeight, float cWidth, float wRadius, float wHeight, ComPtr<ID3D11Device> g_pd3dDevice)
{
	this->cLength = cLength;
	this->cHeight = cHeight;
	this->cWidth = cWidth;

	this->wRadius = wRadius;
	this->wHeight = wHeight;


	Material material;
	material.Ambient = XMFLOAT4(0.588f, 0.588f, 0.588f, 1.0f);
	material.Diffuse = XMFLOAT4(0.588f, 0.588f, 0.588f, 1.0f);
	material.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 10.0f);

	carBody.SetBuffer(g_pd3dDevice, GeometryGenerator::CreateCuboid(cLength, cHeight, cWidth));
	carBody.SetMaterial(material);

	for (int i = 0; i < 4; i++)
	{
		wheel[i].SetBuffer(g_pd3dDevice, GeometryGenerator::CreateCylinder(wRadius, wHeight, wSlices));
		wheel[i].SetMaterial(material);
	}
	//设置车轮一开始相对于小车的位置和转向
	wheel[0].UpdateRotationMatrixZ(XM_PIDIV2);
	wheel[0].UpdateTranslationMatrix((wHeight - cLength) / 2, -cHeight / 2 - wRadius, cWidth / 2 - 1 - wRadius);

	wheel[1].UpdateRotationMatrixZ(XM_PIDIV2);
	wheel[1].UpdateTranslationMatrix((cLength - wHeight) / 2, -cHeight / 2 - wRadius, cWidth / 2 - 1 - wRadius);

	wheel[2].UpdateRotationMatrixZ(XM_PIDIV2);
	wheel[2].UpdateTranslationMatrix((wHeight - cLength) / 2, -cHeight / 2 - wRadius, 1 + wRadius - cWidth / 2);

	wheel[3].UpdateRotationMatrixZ(XM_PIDIV2);
	wheel[3].UpdateTranslationMatrix((cLength - wHeight) / 2, -cHeight / 2 - wRadius, 1 + wRadius - cWidth / 2);
}


void Car::SetHouseBoundingPos(XMFLOAT4 pos)
{
	//设置车子不能碰到房子的边界位置
	houseBoundingPos.x = pos.x - cLength / 2;
	houseBoundingPos.y = pos.y + cLength / 2;

	houseBoundingPos.z = pos.z - cWidth / 2;
	houseBoundingPos.w = pos.w + cWidth / 2;
}

void Car::SetTexture(ComPtr<ID3D11ShaderResourceView> carBodyTexture, ComPtr<ID3D11ShaderResourceView> wheelTexture)
{
	carBody.SetTexture(carBodyTexture, carBodyTexture);
	for (int i = 0; i < 4; i++)
	{
		wheel[i].SetTexture(wheelTexture, wheelTexture);
	}
}

XMMATRIX Car::GetWorldMatrix()
{
	return carBody.GetWorldMatrix();
}

void Car::SetCarPosition(float x, float y, float z)
{
	carBody.UpdateTranslationMatrix(x, y, z);
	UpdateWheelParentWorldMatrix();
}

void Car::UpdateWheelParentWorldMatrix()
{
	XMMATRIX parentWorldMatrix = carBody.GetWorldMatrix();
	for (int i = 0; i < 4; i++)
	{
		wheel[i].UpdateParentWorldMatrix(parentWorldMatrix);
	}
}

void Car::GoAhead(float radian)
{
	/*判断行驶后的小车不能穿透房子*/
	XMVECTOR Position = GetWorldMatrix().r[3];
	XMFLOAT4 pos;
	XMStoreFloat4(&pos, Position);
	float translationX = sinf(GetTotalRadian())*radian;
	float translationZ = cosf(GetTotalRadian())*radian;
	pos.x += translationX;
	pos.z += translationZ;
	if (pos.x > houseBoundingPos.x&&pos.x<houseBoundingPos.y&&pos.z>houseBoundingPos.z&&pos.z < houseBoundingPos.w)
		return;

	float wheelRollRadian = radian / wRadius;

	for (int i = 0; i < 4; i++)
	{
		wheel[i].UpdateRotationMatrixX(wheelRollRadian);
	}
	UpdateParentRotation(radian);
	carBody.UpdateTranslationMatrix(translationX, 0, translationZ);
}

void Car::Back(float radian)
{
	/*判断行驶后的小车不能穿透房子*/
	XMVECTOR Position = GetWorldMatrix().r[3];
	XMFLOAT4 pos;
	XMStoreFloat4(&pos, Position);
	float translationX = -sinf(GetTotalRadian())*radian;
	float translationZ = -cosf(GetTotalRadian())*radian;
	pos.x += translationX;
	pos.z += translationZ;
	if (pos.x > houseBoundingPos.x&&pos.x<houseBoundingPos.y&&pos.z>houseBoundingPos.z&&pos.z < houseBoundingPos.w)
		return;

	float wheelRollRadian = radian / wRadius;

	for (int i = 0; i < 4; i++)
	{
		wheel[i].UpdateRotationMatrixX(-wheelRollRadian);
	}
	UpdateParentRotation(-radian);
	carBody.UpdateTranslationMatrix(translationX, 0, translationZ);
}

void Car::TurnLeft(float radian)
{
	UpdateParentLeftRightRadian(-radian);

	for (int i = 0; i < 2; i++)
	{
		wheel[i].SetRotationMatrixY(parentLeftRightRadian);
	}
}

void Car::TurnRight(float radian)
{
	UpdateParentLeftRightRadian(radian);

	for (int i = 0; i < 2; i++)
	{
		wheel[i].SetRotationMatrixY(parentLeftRightRadian);
	}
}

void Car::UpdateParentLeftRightRadian(float radian)
{
	parentLeftRightRadian += radian;
	if (parentLeftRightRadian > parentLeftRightRadianMax)
		parentLeftRightRadian = parentLeftRightRadianMax;
	if (parentLeftRightRadian < -parentLeftRightRadianMax)
		parentLeftRightRadian = -parentLeftRightRadianMax;
}

void Car::UpdateParentRotation(float dt)
{
	if (parentLeftRightRadian <= 0.2&&parentLeftRightRadian >= -0.2)
		return;
	float radian = 0.0005f*parentLeftRightRadian;

	//前进
	if (dt > 0)
	{
		if (parentLeftRightRadian < 0)
		{
			//左转的情况下
			carBody.UpdateRotationMatrixY(radian);
			totalRadian = totalRadian + radian;
			if (totalRadian > XM_2PI)
				totalRadian -= XM_2PI;

		}
		else
		{
			carBody.UpdateRotationMatrixY(radian);
			totalRadian = totalRadian + radian;
			if (totalRadian < -XM_2PI)
				totalRadian += XM_2PI;
		}
	}
	else
	{
		if (parentLeftRightRadian < 0)
		{
			//左转的情况下
			carBody.UpdateRotationMatrixY(-radian);
			totalRadian = totalRadian - radian;
			if (totalRadian < -XM_2PI)
				totalRadian += XM_2PI;
		}
		else
		{
			carBody.UpdateRotationMatrixY(-radian);
			totalRadian = totalRadian - radian;
			if (totalRadian > XM_2PI)
				totalRadian -= XM_2PI;
		}
	}

}

float Car::GetParentLeftRightRadian()
{
	return parentLeftRightRadian;
}

float Car::GetTotalRadian()
{
	return totalRadian;
}

void Car::Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, GameObjectRender &render)
{
	carBody.Draw(g_pImmediateContext, render);

	for (int i = 0; i < 4; i++)
	{
		wheel[i].Draw(g_pImmediateContext, render);
	}

}

void Car::Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, ShadowMapRender &render)
{
	carBody.Draw(g_pImmediateContext, render);

	for (int i = 0; i < 4; i++)
	{
		wheel[i].Draw(g_pImmediateContext, render);
	}
}