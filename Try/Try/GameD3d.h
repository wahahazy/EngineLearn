#pragma once
#include "BasicD3d.h"
#include "Car.h"
#include "Camera.h"
#include "SkyBox.h"
#include "ExternalModel.h"
#include "ShadowMapRender.h"

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
	void RenderSceneToTexture(); //����shadow map
	void RenderScene();
	bool InitResource();
	void InitMaterial();  //��ʼ������
	void InitLight();     //��ʼ����
	bool InitTexture();   //��ʼ������

	ObjReader objReader;				// ģ�Ͷ�ȡ����

	//��Ⱦ����
	ShadowMapRender shadowMapRenderDir;    //��Ⱦ����ƽ�й����ͼ����Ⱦ����
	ShadowMapRender shadowMapRenderSpot;    //��Ⱦ���ɾ۹�����ͼ����Ⱦ����

	GameObjectRender gameObjectRender;	//��Ϸ�������Ⱦ����
	SkyBoxRender skyBoxRender;			//��պ��ӵ���Ⱦ����

	//ģ��
	Car car;
	GameObject ground;					//����
	ExternalModel house;
	SkyBox skyBox;

	//����
	Material materialGround;

	//�����
	FirstCamera firstCamera;			//��һ�˳������
	SecondCamera secondCamera;			//��С��Ϊ�۲����ĵ������
	ThirdCamera thirdCamera;			//�����˳������
	CameraMode cameraMode;				//�����ģʽ

	//��Դ����
	DirectionalLight mDirectionalLight;			// ƽ�й�
	SpotLight mSpotLight;						// �۹��

	//�������ͼ
	ComPtr<ID3D11ShaderResourceView>	shadowMapDirectionalLight;
	ComPtr<ID3D11ShaderResourceView>	shadowMapSpotLight;

};