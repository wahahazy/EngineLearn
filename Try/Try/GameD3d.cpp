#include "GameD3d.h"
GameD3d::GameD3d(HINSTANCE hInstance, int nCmdShow) : BasicD3d(hInstance, nCmdShow)
{
	this->cameraMode = CameraMode::FirstPerson; //游戏一开始是第一人称
	this->shadowMapDirectionalLight = nullptr;
	this->shadowMapSpotLight = nullptr;
}

GameD3d::~GameD3d()
{

}

bool GameD3d::Init()
{
	if (!BasicD3d::Init())
		return false;

	if (!shadowMapRenderDir.InitAll(g_pd3dDevice.Get(), g_clientWidth, g_clientHeight))
		return false;

	if (!shadowMapRenderSpot.InitAll(g_pd3dDevice.Get(), g_clientWidth, g_clientHeight))
		return false;

	if (!gameObjectRender.InitAll(g_pd3dDevice.Get()))
		return false;

	if (!skyBoxRender.InitAll(g_pd3dDevice.Get()))
		return false;

	if (!InitResource())
		return false;

	// 初始化鼠标，键盘不需要
	mMouse->SetWindow(g_hWnd);
	mMouse->SetMode(DirectX::Mouse::MODE_RELATIVE); //MODE_ABSOLUTE会出现鼠标符号，MODE_RELATIVE则不出现符号

	return true;
}

void GameD3d::InitMaterial()
{
	//地面材质
	materialGround.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	materialGround.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	materialGround.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 10.0f);
}

void GameD3d::InitLight()
{
	// 平行光
	//mDirectionalLight = DirectionalLight();
	mDirectionalLight.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	mDirectionalLight.Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirectionalLight.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 10.0f);
	mDirectionalLight.Direction = XMFLOAT3(0.1f, -0.3f, -0.5f);

	//设定夕阳的位置
	XMFLOAT3 mPosition = { -20.0f,60.0f,100.0f };
	XMVECTOR Eye = XMLoadFloat3(&mPosition);
	XMVECTOR LookAt = XMVector3Normalize(Eye);
	XMVECTOR Tangent = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), LookAt));
	XMVECTOR Up = XMVector3Cross(LookAt, Tangent);
	XMMATRIX View = XMMatrixLookAtLH(Eye, LookAt, Up);

	XMMATRIX Proj = XMMatrixOrthographicLH(90.0f, 90.0f, 5.0f, 200.0f);
	mDirectionalLight.ViewProj = XMMatrixTranspose(View * Proj);


	//
	// 聚光灯
	//mSpotLight = SpotLight();
	mSpotLight.Position = XMFLOAT3(-15.0f, 30.0f, 30.0f);
	mSpotLight.Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	mSpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mSpotLight.WeakFactor = XMFLOAT3(0.4f, 0.0f, 0.0f);
	mSpotLight.Spot = 12.0f;
	mSpotLight.Range = 500.0;

	XMMATRIX ViewOne = XMMatrixLookAtLH(XMVectorSet(-15.0f, 30.0f, 30.0f, 0.0f), XMVectorSet(-15.0f, 0.0f, 30.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	XMMATRIX ProjTwo = XMMatrixPerspectiveFovLH(XM_PIDIV2, g_clientWidth / (FLOAT)g_clientHeight, 1.0f, 200.0f);
	mSpotLight.ViewProj = XMMatrixTranspose(ViewOne * ProjTwo);

	//因为此程序中的灯光是固定不动的，因此在初始化的时候就送入游戏对象的渲染管线中
	gameObjectRender.UpdateCBLight(mDirectionalLight, mSpotLight);
}

bool GameD3d::InitTexture()
{
	//地面纹理，地面将采用两幅纹理混合
	ComPtr<ID3D11ShaderResourceView> textureGround;
	ComPtr<ID3D11ShaderResourceView> textureRoad;
	HRESULT hr = CreateDDSTextureFromFile(g_pd3dDevice.Get(), L"Texture\\grass.dds", nullptr, textureGround.GetAddressOf());
	if (FAILED(hr))
		return false;

	hr = CreateDDSTextureFromFile(g_pd3dDevice.Get(), L"Texture\\bricks.dds", nullptr, textureRoad.GetAddressOf());
	if (FAILED(hr))
		return false;

	ground.SetTexture(textureGround, textureRoad);

	//小车纹理,其实应该分为不一样的
	ComPtr<ID3D11ShaderResourceView> textureCarBody;
	ComPtr<ID3D11ShaderResourceView> textureWheel;
	hr = CreateDDSTextureFromFile(g_pd3dDevice.Get(), L"Texture\\WoodCrate.dds", nullptr, textureCarBody.GetAddressOf());
	if (FAILED(hr))
		return false;
	hr = CreateDDSTextureFromFile(g_pd3dDevice.Get(), L"Texture\\WoodCrate.dds", nullptr, textureWheel.GetAddressOf());
	if (FAILED(hr))
		return false;

	car.SetTexture(textureCarBody, textureWheel);

	return true;
}

bool GameD3d::InitResource()
{
	InitMaterial();
	InitLight();
	if (!InitTexture())
		return false;

	//初始化天空盒子
	skyBox.Init(g_pd3dDevice, g_pImmediateContext, std::vector<std::wstring>{
		L"Texture\\sunset_posX.bmp", L"Texture\\sunset_negX.bmp",
			L"Texture\\sunset_posY.bmp", L"Texture\\sunset_negY.bmp",
			L"Texture\\sunset_posZ.bmp", L"Texture\\sunset_negZ.bmp", }, 1000.0f);

	//XMMatrixShadow方法形成的阴影仅仅投射在一个平面上
	//gameObjectRender.UpdateShadowMatrix(XMMatrixShadow(XMVectorSet(0.0f, 1.0f, 0.0f, -0.01f), XMVectorSet(-0.1f, 0.3f, 0.5f, 0.0f)));////////


	/* 初始化导入的房子模型*/
	objReader.Read(L"Model\\house.mbo", L"Model\\house.obj");
	house.CreateModel(g_pd3dDevice, objReader);

	XMMATRIX S = XMMatrixScaling(0.08f, 0.08f, 0.08f);
	house.SetScaleMatrix(S);
	// 获取房屋包围盒
	BoundingBox houseBox = house.GetLocalBoundingBox();
	houseBox.Transform(houseBox, S);
	houseBox.Center.x -= 30.0f;
	houseBox.Center.z += 10.0f;
	//房子设置在合适的位置
	house.SetRotationMatrix(-XM_PIDIV2);
	house.SetTranslationMatrix(-30.0f, houseBox.Extents.y - houseBox.Center.y, 10.0f);

	/* 初始化内部模型*/
	float gLength = 10, gWidth = 10, gCenterY = 0; //地面的长宽和所处的平面高度
	int gNumber = 150;
	float cLength = 5, cWidth = 6, cHeight = 2;//车身长宽高								  
	float wRadius = 0.4f, wHeight = 0.4f;  //车轮半径、高度

	float carX = -5, carY = cHeight / 2 + 2 * wRadius, carZ = 3; //车子一开始放置在世界中的位置。道路边，靠近房子。
	float firstCameraX = carX, firstCameraY = cHeight + 2 * wRadius + 4, firstCameraZ = carZ - cWidth / 2; //第一人称摄像机一开始放置在世界中的位置，车尾巴且y方向正向偏移一定的距离

	//初始化地面
	ground.SetBuffer(g_pd3dDevice, GeometryGenerator::CreatePlane(gLength, gCenterY, gWidth, gNumber));
	ground.SetMaterial(materialGround);

	//初始化小车
	car.CreateCarModel(cLength, cHeight, cWidth, wRadius, wHeight, g_pd3dDevice);
	car.SetCarPosition(carX, carY, carZ);
	car.SetHouseBoundingPos({ houseBox.Center.x - houseBox.Extents.x + 6,houseBox.Center.x + houseBox.Extents.x ,houseBox.Center.z - houseBox.Extents.z + 2,houseBox.Center.z + houseBox.Extents.z + 3 });



	//初始化第一人称摄像机
	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(firstCameraX, firstCameraY, firstCameraZ, 0.0f);
	XMVECTOR At = XMVectorSet(firstCameraX, firstCameraY, firstCameraZ + 1, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX View = XMMatrixLookAtLH(Eye, At, Up);
	firstCamera.UpdateCarWorld(car.GetWorldMatrix());
	firstCamera.SetCameraToCarMatrix(View);
	gameObjectRender.UpdateCBChangeView(firstCamera.GetWorld());

	// Initialize the projection matrix
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, g_clientWidth / (FLOAT)g_clientHeight, 1.0f, 200.0f);
	gameObjectRender.UpdateCBChangeOnResize(Projection);
	firstCamera.SetProjection(Projection);

	//初始化以小车为观察中心的摄像机
	secondCamera.UpdateCarWorld(car.GetWorldMatrix());
	secondCamera.UpdateCameraToCarMatrix();
	secondCamera.SetProjection(Projection);

	//初始化第三人称摄像机
	Eye = XMVectorSet(firstCameraX, firstCameraY, firstCameraZ - 10, 0.0f);
	At = XMVectorSet(firstCameraX, firstCameraY, firstCameraZ, 0.0f);
	thirdCamera.UpdateCarWorld(car.GetWorldMatrix());
	thirdCamera.SetCameraToCarMatrix(Eye, At);
	thirdCamera.SetProjection(Projection);


	return true;
}

void GameD3d::UpdateData(float dt)
{
	Mouse::State mouseState = mMouse->GetState();
	mMouseTracker.Update(mouseState);
	Keyboard::State keyState = mKeyboard->GetState();
	mKeyboardTracker.Update(keyState);

	//通过对车身进行操作，进而操控整个小车
	float wheelRollRadian = 0;

	if (cameraMode != CameraMode::ThirdPerson)
	{
		if (keyState.IsKeyDown(Keyboard::W))
		{
			//轮子顺时针转
			car.GoAhead(3 * dt);

		}
		if (keyState.IsKeyDown(Keyboard::S))
		{
			//轮子逆时针转
			car.Back(3 * dt);

		}
		if (keyState.IsKeyDown(Keyboard::A))
		{
			car.TurnLeft(dt);
		}
		if (keyState.IsKeyDown(Keyboard::D))
		{
			car.TurnRight(dt);
		}
		car.UpdateWheelParentWorldMatrix();
		firstCamera.UpdateCarWorld(car.GetWorldMatrix());
		secondCamera.UpdateCarWorld(car.GetWorldMatrix());
		thirdCamera.UpdateCarWorld(car.GetWorldMatrix());
	}
	else
	{
		if (keyState.IsKeyDown(Keyboard::W))
		{
			thirdCamera.UpdateCameraTranslationZ(3 * dt);

		}
		if (keyState.IsKeyDown(Keyboard::S))
		{
			thirdCamera.UpdateCameraTranslationZ(-3 * dt);

		}
		if (keyState.IsKeyDown(Keyboard::A))
		{
			thirdCamera.UpdateCameraTranslationX(-3 * dt);
		}
		if (keyState.IsKeyDown(Keyboard::D))
		{
			thirdCamera.UpdateCameraTranslationX(3 * dt);
		}
	}


	if (cameraMode == CameraMode::FirstPerson)
	{
		firstCamera.UpdateCameraRadian(mouseState.y * dt * 1.25f, mouseState.x * dt * 1.25f);
		gameObjectRender.UpdateCBChangeView(firstCamera.GetWorld());
	}
	else if (cameraMode == CameraMode::SecondPerson)
	{
		secondCamera.UpdateCameraRotation(mouseState.y * dt * 1.25f, mouseState.x * dt * 1.25f);
		secondCamera.UpdateDistance(-mouseState.scrollWheelValue / 120 * 1.0f);
		secondCamera.UpdateCameraToCarMatrix();

		gameObjectRender.UpdateCBChangeView(secondCamera.GetWorld());
	}
	else if (cameraMode == CameraMode::ThirdPerson)
	{
		thirdCamera.UpdateCameraRotation(mouseState.y * dt * 1.25f, mouseState.x * dt * 1.25f);

		gameObjectRender.UpdateCBChangeView(thirdCamera.GetWorld());
	}

	// 重置滚轮值
	mMouse->ResetScrollWheelValue();

	// 摄像机模式切换
	if (mKeyboardTracker.IsKeyPressed(Keyboard::D1) && cameraMode != CameraMode::FirstPerson)
	{
		cameraMode = CameraMode::FirstPerson;

		gameObjectRender.UpdateCBChangeView(firstCamera.GetWorld());

		gameObjectRender.UpdateCBChangeOnResize(firstCamera.GetProjection());

	}
	else if ((mKeyboardTracker.IsKeyPressed(Keyboard::D2) && cameraMode != CameraMode::SecondPerson))
	{
		cameraMode = CameraMode::SecondPerson;

		gameObjectRender.UpdateCBChangeView(secondCamera.GetWorld());

		gameObjectRender.UpdateCBChangeOnResize(secondCamera.GetProjection());
	}
	else if ((mKeyboardTracker.IsKeyPressed(Keyboard::D3) && cameraMode != CameraMode::ThirdPerson))
	{
		cameraMode = CameraMode::ThirdPerson;

		gameObjectRender.UpdateCBChangeView(thirdCamera.GetWorld());

		gameObjectRender.UpdateCBChangeOnResize(thirdCamera.GetProjection());
	}
	// 退出程序，这里应向窗口发送销毁信息
	if (keyState.IsKeyDown(Keyboard::Escape))
		SendMessage(this->g_hWnd, WM_DESTROY, 0, 0);
}

void GameD3d::RenderSceneToTexture()
{
	//生成平行光的深度图
	shadowMapRenderDir.SetRenderTarget(g_pImmediateContext);
	shadowMapRenderDir.SetRenderDefault(g_pImmediateContext);
	shadowMapRenderDir.ClearRenderTarget(g_pImmediateContext);
	//将平行光的view和proj矩阵送入渲染管线
	shadowMapRenderDir.UpdateLightViewProj(mDirectionalLight.ViewProj);
	ground.Draw(g_pImmediateContext, shadowMapRenderDir);
	car.Draw(g_pImmediateContext, shadowMapRenderDir);
	house.Draw(g_pImmediateContext, shadowMapRenderDir);
	shadowMapDirectionalLight = shadowMapRenderDir.GetShadowMapTexture();

	//生成聚光灯的深度图
	shadowMapRenderSpot.SetRenderTarget(g_pImmediateContext);
	shadowMapRenderSpot.SetRenderDefault(g_pImmediateContext);
	shadowMapRenderSpot.ClearRenderTarget(g_pImmediateContext);
	//将聚光灯的view和proj矩阵送入渲染管线
	shadowMapRenderSpot.UpdateLightViewProj(mSpotLight.ViewProj);
	ground.Draw(g_pImmediateContext, shadowMapRenderSpot);
	car.Draw(g_pImmediateContext, shadowMapRenderSpot);
	house.Draw(g_pImmediateContext, shadowMapRenderSpot);

	shadowMapSpotLight = shadowMapRenderSpot.GetShadowMapTexture();

}

void GameD3d::RenderScene()
{
	RenderSceneToTexture();//生成shadow map

	SetRenderTarget(); //重置渲染目标

	ClearRenderTarget();

	gameObjectRender.SetRenderDefault(g_pImmediateContext);

	gameObjectRender.SetShadowMapTextureDir(shadowMapDirectionalLight);

	gameObjectRender.SetShadowMapTextureSpot(shadowMapSpotLight);

	ground.Draw(g_pImmediateContext, gameObjectRender);

	car.Draw(g_pImmediateContext, gameObjectRender);

	house.Draw(g_pImmediateContext, gameObjectRender);


	skyBoxRender.SetRenderDefault(g_pImmediateContext);
	if (cameraMode == CameraMode::FirstPerson)
		skyBox.Draw(g_pImmediateContext, skyBoxRender, firstCamera);
	else if (cameraMode == CameraMode::SecondPerson)
		skyBox.Draw(g_pImmediateContext, skyBoxRender, secondCamera);
	else
		skyBox.Draw(g_pImmediateContext, skyBoxRender, thirdCamera);
	g_pSwapChain->Present(0, 0);
}
