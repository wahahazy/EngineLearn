#pragma once
#include "basic.h"
class Camera
{
public:
	Camera();
	~Camera();
	void XM_CALLCONV SetProjection(XMMATRIX Projection);
	XMMATRIX GetView();
	XMMATRIX GetWorld();   //得到摄像机的世界矩阵
	XMMATRIX GetProjection();
	void XM_CALLCONV UpdateCarWorld(XMMATRIX CarWorld);

protected:
	XMMATRIX Projection;
	XMMATRIX CameraToCarMatrix;  //此矩阵是相机模型到小车的转换矩阵
	XMMATRIX CameraToCarMatrixOri;  //最初的摄像机到小车的转换矩阵
	XMMATRIX CarWorld;  //小车世界矩阵，每次小车的世界矩阵更新的时候，所有相机的这个变量都要更新


};

class FirstCamera :public Camera
{
public:
	FirstCamera();
	~FirstCamera();
	void XM_CALLCONV SetCameraToCarMatrix(XMMATRIX originalMatrix); //初始化转换矩阵
	void UpdateCameraRadian(float radianPhi, float radianTheta);  //更新相机离初始位置的角度

private:
	// 以世界坐标系为基准，当前的旋转角度
	float radianTheta;      //距离原始位置的x与z平面上转动的角度
	float radianThetaMax;
	float radianThetaMin;
	float radianPhi;      //距离原始位置的y与z平面上转动的角度
	float radianPhiMax;  //距离原始位置的y与z平面上最大能转动的角度
	float raidanPhiMin;


};

class SecondCamera :public Camera
{
public:
	SecondCamera();
	~SecondCamera();
	void UpdateDistance(float distance);  //更新相机离小车的距离
	void UpdateCameraRotation(float radianPhi, float radianTheta);  //更新相机离初始位置的角度
	void UpdateCameraToCarMatrix();

private:
	float distance; //相机离小车的距离
	float distanceMax; //相机离小车最大的距离
	float distanceMin; //相机离小车最小的距离

	// 以世界坐标系为基准，当前的旋转角度
	float radianTheta;      //距离原始位置的x与z平面上转动的角度
	float radianPhi;      //距离原始位置的y与z平面上转动的角度
	float radianPhiMax;  //距离原始位置的y与z平面上最大能转动的角度
	float raidanPhiMin;
};


class ThirdCamera :public Camera
{
public:
	ThirdCamera();
	~ThirdCamera();
	void XM_CALLCONV SetCameraToCarMatrix(XMVECTOR cameraPos, XMVECTOR lookAt); //初始化转换矩阵
	void UpdateCameraRotation(float radianPhi, float radianTheta);  //更新相机旋转矩阵
	void UpdateCameraTranslationZ(float translationZ);  //更新相机平移矩阵，前进后退
	void UpdateCameraTranslationX(float translationX);  //更新相机平移矩阵，左右移动

private:
	// 以世界坐标系为基准，当前的旋转角度
	float radianTheta;      //距离原始位置的x与z平面上转动的角度
	float radianPhi;      //距离原始位置的y与z平面上转动的角度

	XMMATRIX  translationMatrix;
	XMVECTOR  lookAtDirection;
	XMVECTOR  RightDirection;
	XMFLOAT3  Position;  //最初摄像头相对小车的位置,因为小车在y方向上跟地面是保持相对静止的，因此可以用此坐标来限制摄像头不能穿越地面
};