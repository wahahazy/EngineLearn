#include "Camera.h"

Camera::Camera()
{
	this->Projection = XMMatrixIdentity();
	this->CameraToCarMatrix = XMMatrixIdentity();
	this->CameraToCarMatrixOri = XMMatrixIdentity();
	this->CarWorld = XMMatrixIdentity();
}

Camera::~Camera()
{

}

void XM_CALLCONV Camera::SetProjection(XMMATRIX Projection)
{
	this->Projection = Projection;
}

XMMATRIX Camera::GetView()
{
	return XMMatrixInverse(nullptr, GetWorld());
}

XMMATRIX Camera::GetWorld()
{
	return CameraToCarMatrix * CarWorld;
}

XMMATRIX Camera::GetProjection()
{
	return Projection;
}



void XM_CALLCONV Camera::UpdateCarWorld(XMMATRIX CarWorld)
{
	this->CarWorld = CarWorld;
}


FirstCamera::FirstCamera()
{
	this->radianTheta = 0;
	this->radianThetaMax = XM_PIDIV4;
	this->radianThetaMin = -XM_PIDIV4;
	this->radianPhi = 0;
	this->radianPhiMax = XM_PI / 8;
	this->raidanPhiMin = -XM_PIDIV4;
}

FirstCamera::~FirstCamera()
{

}

void XM_CALLCONV FirstCamera::SetCameraToCarMatrix(XMMATRIX originalMatrix)
{
	this->CameraToCarMatrix = XMMatrixInverse(nullptr, originalMatrix)* XMMatrixInverse(nullptr, CarWorld);
	this->CameraToCarMatrixOri = this->CameraToCarMatrix;
}

void FirstCamera::UpdateCameraRadian(float Phi, float Theta)
{

	radianPhi += Phi;
	// 将真实的上下视野角度Phi限制在[最大向上角度45度，最大向下角度22.5度]
	if (radianPhi < raidanPhiMin)
		radianPhi = raidanPhiMin;
	else if (radianPhi > radianPhiMax)
		radianPhi = radianPhiMax;

	radianTheta += Theta;
	// 将真实的左右视野角度Theta限制在[左右45度]之间 
	if (radianTheta < radianThetaMin)
		radianTheta = radianThetaMin;
	else if (radianTheta > radianThetaMax)
		radianTheta = radianThetaMax;

	CameraToCarMatrix = XMMatrixRotationY(radianTheta)*XMMatrixRotationX(radianPhi)*CameraToCarMatrixOri;
}

SecondCamera::SecondCamera()
{
	this->distanceMax = 30;
	this->distanceMin = 10;
	this->distance = (this->distanceMin + this->distanceMax) / 2;

	this->radianTheta = -XM_PIDIV2;
	this->radianPhi = XM_PIDIV4;
	this->radianPhiMax = XM_PIDIV2;
	this->raidanPhiMin = XM_PI / 8;
}

SecondCamera::~SecondCamera()
{

}

void SecondCamera::UpdateDistance(float dis)
{
	distance += dis;
	if (distance < distanceMin)
		distance = distanceMin;
	else if (distance > distanceMax)
		distance = distanceMax;
}

void SecondCamera::UpdateCameraRotation(float Phi, float Theta)
{

	radianPhi += Phi;
	// 将真实的上下视野角度Phi限制在(0到67.5度)之间
	if (radianPhi < raidanPhiMin)
		radianPhi = raidanPhiMin;
	else if (radianPhi > radianPhiMax)
		radianPhi = radianPhiMax;

	radianTheta = XMScalarModAngle(radianTheta + Theta);
}

void SecondCamera::UpdateCameraToCarMatrix()
{
	float x = distance * sinf(radianPhi) * cosf(radianTheta);
	float z = distance * sinf(radianPhi) * sinf(radianTheta);
	float y = distance * cosf(radianPhi);
	XMFLOAT3 mPosition = { x, y, z };
	XMVECTOR Eye = XMLoadFloat3(&mPosition);
	XMVECTOR LookAt = XMVector3Normalize(Eye);
	XMVECTOR Tangent = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), LookAt));
	XMVECTOR Up = XMVector3Cross(LookAt, Tangent);
	XMMATRIX View = XMMatrixLookAtLH(Eye, LookAt, Up);

	CameraToCarMatrix = XMMatrixInverse(nullptr, View);
}


ThirdCamera::ThirdCamera()
{
	this->radianTheta = 0;
	this->radianPhi = 0;

	translationMatrix = XMMatrixIdentity();
}

ThirdCamera::~ThirdCamera()
{

}

void XM_CALLCONV ThirdCamera::SetCameraToCarMatrix(XMVECTOR cameraPos, XMVECTOR lookAt)
{
	XMStoreFloat3(&Position, cameraPos);
	lookAtDirection = XMVector3Normalize(lookAt - cameraPos);
	RightDirection = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), lookAtDirection));
	XMVECTOR Up = XMVector3Cross(lookAtDirection, RightDirection);
	XMMATRIX View = XMMatrixLookAtLH(cameraPos, lookAt, Up);

	this->CameraToCarMatrix = XMMatrixInverse(nullptr, View)* XMMatrixInverse(nullptr, CarWorld);
	this->CameraToCarMatrixOri = this->CameraToCarMatrix;

}

void ThirdCamera::UpdateCameraRotation(float Phi, float Theta)
{

	radianPhi = XMScalarModAngle(radianPhi + Phi);

	radianTheta = XMScalarModAngle(radianTheta + Theta);

	CameraToCarMatrix = XMMatrixRotationY(radianTheta)*XMMatrixRotationX(radianPhi)*CameraToCarMatrixOri*translationMatrix;
}


void ThirdCamera::UpdateCameraTranslationZ(float translationZ)
{
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, XMVector3Transform(lookAtDirection, XMMatrixRotationY(radianTheta)*XMMatrixRotationX(radianPhi)));
	XMFLOAT4 translation;
	XMStoreFloat4(&translation, translationMatrix.r[3]);
	if (Position.y + translation.y + dir.y*translationZ > 2)
	{
		translationMatrix *= XMMatrixTranslation(dir.x*translationZ, dir.y*translationZ, dir.z*translationZ);
		CameraToCarMatrix = XMMatrixRotationY(radianTheta)*XMMatrixRotationX(radianPhi)*CameraToCarMatrixOri*translationMatrix;
	}

}

void ThirdCamera::UpdateCameraTranslationX(float translationX)
{
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, XMVector3Transform(RightDirection, XMMatrixRotationY(radianTheta)*XMMatrixRotationX(radianPhi)));

	XMFLOAT4 translation;
	XMStoreFloat4(&translation, translationMatrix.r[3]);
	if (Position.y + translation.y + dir.y*translationX > 2)
	{
		translationMatrix *= XMMatrixTranslation(dir.x*translationX, dir.y*translationX, dir.z*translationX);

		CameraToCarMatrix = XMMatrixRotationY(radianTheta)*XMMatrixRotationX(radianPhi)*CameraToCarMatrixOri*translationMatrix;
	}
}