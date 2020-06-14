#pragma once
#include "GameObject.h"
class Car
{
public:
	// 使用模板别名(C++11)简化类型名
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	Car();
	~Car();
	void CreateCarModel(float cLength, float cHeight, float cWidth, float wRadius, float wHeight, ComPtr<ID3D11Device> g_pd3dDevice);
	void SetTexture(ComPtr<ID3D11ShaderResourceView> carBodyTexture, ComPtr<ID3D11ShaderResourceView> wheelTexture);
	XMMATRIX GetWorldMatrix();
	void SetCarPosition(float x, float y, float z);//设置小车在世界中的初始位置
	void UpdateWheelParentWorldMatrix();
	void Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, GameObjectRender &render);
	void Draw(ComPtr<ID3D11DeviceContext>  g_pImmediateContext, ShadowMapRender &render);

	void GoAhead(float radian);
	void Back(float radian);
	void TurnLeft(float radian);
	void TurnRight(float radian);


	void UpdateParentLeftRightRadian(float radian);
	void UpdateParentRotation(float radian);
	float GetParentLeftRightRadian();
	float GetTotalRadian();

	void SetHouseBoundingPos(XMFLOAT4 pos);

private:
	float	parentLeftRightRadian;   //父模型即将要转的角度
	float	parentLeftRightRadianMax;  //可以旋转的最大角度
	float	totalRadian;              //父模型旋转总角度

	float	cLength;  //车身长
	float	cHeight;  //车身高
	float	cWidth;   //车身宽
	float	wRadius;  //车轮半径
	float	wHeight;  //车轮高度
	int		wSlices;    //车轮圆切割的块数，默认为64块

	GameObject carBody;				// 车身
	GameObject wheel[4];            //0,1为前轮，2,3为后轮

	XMFLOAT4 houseBoundingPos;  //按顺序为posXMin,posXMax,posZMin,posZMax

};