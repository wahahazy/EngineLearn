#pragma once
#include "GameObject.h"
class Car
{
public:
	// ʹ��ģ�����(C++11)��������
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	Car();
	~Car();
	void CreateCarModel(float cLength, float cHeight, float cWidth, float wRadius, float wHeight, ComPtr<ID3D11Device> g_pd3dDevice);
	void SetTexture(ComPtr<ID3D11ShaderResourceView> carBodyTexture, ComPtr<ID3D11ShaderResourceView> wheelTexture);
	XMMATRIX GetWorldMatrix();
	void SetCarPosition(float x, float y, float z);//����С���������еĳ�ʼλ��
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
	float	parentLeftRightRadian;   //��ģ�ͼ���Ҫת�ĽǶ�
	float	parentLeftRightRadianMax;  //������ת�����Ƕ�
	float	totalRadian;              //��ģ����ת�ܽǶ�

	float	cLength;  //����
	float	cHeight;  //�����
	float	cWidth;   //�����
	float	wRadius;  //���ְ뾶
	float	wHeight;  //���ָ߶�
	int		wSlices;    //����Բ�и�Ŀ�����Ĭ��Ϊ64��

	GameObject carBody;				// ����
	GameObject wheel[4];            //0,1Ϊǰ�֣�2,3Ϊ����

	XMFLOAT4 houseBoundingPos;  //��˳��ΪposXMin,posXMax,posZMin,posZMax

};