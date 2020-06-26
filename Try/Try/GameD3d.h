#pragma once
#include "BasicD3d.h"
#include "Car.h"
#include "Camera.h"
#include "SkyBox.h"
#include "ExternalModel.h"
#include "ShadowMapRender.h"
#include "ObjModel.h"

class GameD3d : public BasicD3d
{
public:
	enum class CameraMode { FirstPerson, SecondPerson, ThirdPerson };

public:
	GameD3d(HINSTANCE hInstance, int nCmdShow);
	~GameD3d();
	bool Init();

private:
	void UpdateData(float dt);
	void RenderSceneToTexture(); //生成shadow map
	void RenderScene();
	bool InitResource();
	void InitMaterial();  //初始化材质
	void InitLight();     //初始化光
	bool InitTexture();   //初始化纹理

	ObjReader objReader;				// 模型读取对象
	obj::ObjReader obj_reader;

	//渲染管线
	ShadowMapRender shadowMapRenderDir;    //渲染生成平行光深度图的渲染管线
	ShadowMapRender shadowMapRenderSpot;    //渲染生成聚光灯深度图的渲染管线

	GameObjectRender gameObjectRender;	//游戏对象的渲染管线
	SkyBoxRender skyBoxRender;			//天空盒子的渲染管线

	//模型
	Car car;
	GameObject ground;					//地面
	ExternalModel house;
	SkyBox skyBox;
	obj::ObjModel m_fox;

	//材质
	Material materialGround;

	//摄像机
	FirstCamera firstCamera;			//第一人称摄像机
	SecondCamera secondCamera;			//以小车为观察中心的摄像机
	ThirdCamera thirdCamera;			//第三人称摄像机
	CameraMode cameraMode;				//摄像机模式

	//光源类型
	DirectionalLight mDirectionalLight;			// 平行光
	SpotLight mSpotLight;						// 聚光灯

	//光照深度图
	ComPtr<ID3D11ShaderResourceView>	shadowMapDirectionalLight;
	ComPtr<ID3D11ShaderResourceView>	shadowMapSpotLight;

};