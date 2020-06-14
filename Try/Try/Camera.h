#pragma once
#include "basic.h"
class Camera
{
public:
	Camera();
	~Camera();
	void XM_CALLCONV SetProjection(XMMATRIX Projection);
	XMMATRIX GetView();
	XMMATRIX GetWorld();   //�õ���������������
	XMMATRIX GetProjection();
	void XM_CALLCONV UpdateCarWorld(XMMATRIX CarWorld);

protected:
	XMMATRIX Projection;
	XMMATRIX CameraToCarMatrix;  //�˾��������ģ�͵�С����ת������
	XMMATRIX CameraToCarMatrixOri;  //������������С����ת������
	XMMATRIX CarWorld;  //С���������ÿ��С�������������µ�ʱ��������������������Ҫ����


};

class FirstCamera :public Camera
{
public:
	FirstCamera();
	~FirstCamera();
	void XM_CALLCONV SetCameraToCarMatrix(XMMATRIX originalMatrix); //��ʼ��ת������
	void UpdateCameraRadian(float radianPhi, float radianTheta);  //����������ʼλ�õĽǶ�

private:
	// ����������ϵΪ��׼����ǰ����ת�Ƕ�
	float radianTheta;      //����ԭʼλ�õ�x��zƽ����ת���ĽǶ�
	float radianThetaMax;
	float radianThetaMin;
	float radianPhi;      //����ԭʼλ�õ�y��zƽ����ת���ĽǶ�
	float radianPhiMax;  //����ԭʼλ�õ�y��zƽ���������ת���ĽǶ�
	float raidanPhiMin;


};

class SecondCamera :public Camera
{
public:
	SecondCamera();
	~SecondCamera();
	void UpdateDistance(float distance);  //���������С���ľ���
	void UpdateCameraRotation(float radianPhi, float radianTheta);  //����������ʼλ�õĽǶ�
	void UpdateCameraToCarMatrix();

private:
	float distance; //�����С���ľ���
	float distanceMax; //�����С�����ľ���
	float distanceMin; //�����С����С�ľ���

	// ����������ϵΪ��׼����ǰ����ת�Ƕ�
	float radianTheta;      //����ԭʼλ�õ�x��zƽ����ת���ĽǶ�
	float radianPhi;      //����ԭʼλ�õ�y��zƽ����ת���ĽǶ�
	float radianPhiMax;  //����ԭʼλ�õ�y��zƽ���������ת���ĽǶ�
	float raidanPhiMin;
};


class ThirdCamera :public Camera
{
public:
	ThirdCamera();
	~ThirdCamera();
	void XM_CALLCONV SetCameraToCarMatrix(XMVECTOR cameraPos, XMVECTOR lookAt); //��ʼ��ת������
	void UpdateCameraRotation(float radianPhi, float radianTheta);  //���������ת����
	void UpdateCameraTranslationZ(float translationZ);  //�������ƽ�ƾ���ǰ������
	void UpdateCameraTranslationX(float translationX);  //�������ƽ�ƾ��������ƶ�

private:
	// ����������ϵΪ��׼����ǰ����ת�Ƕ�
	float radianTheta;      //����ԭʼλ�õ�x��zƽ����ת���ĽǶ�
	float radianPhi;      //����ԭʼλ�õ�y��zƽ����ת���ĽǶ�

	XMMATRIX  translationMatrix;
	XMVECTOR  lookAtDirection;
	XMVECTOR  RightDirection;
	XMFLOAT3  Position;  //�������ͷ���С����λ��,��ΪС����y�����ϸ������Ǳ�����Ծ�ֹ�ģ���˿����ô���������������ͷ���ܴ�Խ����
};