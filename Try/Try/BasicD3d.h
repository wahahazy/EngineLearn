#pragma once
#include "basic.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "GameTimer.h"


class BasicD3d
{
public:
	BasicD3d(HINSTANCE hInstance, int nCmdShow);    // �ڹ��캯���ĳ�ʼ���б����úó�ʼ����
	~BasicD3d();

	virtual bool Init();  //��ʼ�����ں�����
	int Run();           //��Ϣѭ�����ͳ�����Ⱦ״̬
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


protected:
	bool InitWindow();
	bool InitDevice();
	void SetRenderTarget();
	void ClearRenderTarget();
	virtual void UpdateData(float dt) = 0;   // ������Ҫʵ�ָ÷��������ÿһ֡�ĸ���
	virtual void RenderScene() = 0;             // ������Ҫʵ�ָ÷��������ÿһ֡�Ļ���


	int                     nCmdShow;
	HINSTANCE               g_hInst;
	HWND                    g_hWnd;
	D3D_DRIVER_TYPE         g_driverType;
	D3D_FEATURE_LEVEL       g_featureLevel;
	D3D11_VIEWPORT			vp;

	// ʹ��ģ�����(C++11)��������
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D11Device>           g_pd3dDevice;
	ComPtr<ID3D11Device1>          g_pd3dDevice1;
	ComPtr<ID3D11DeviceContext>    g_pImmediateContext;
	ComPtr<ID3D11DeviceContext1>   g_pImmediateContext1;
	ComPtr<IDXGISwapChain>         g_pSwapChain;
	ComPtr<IDXGISwapChain1>        g_pSwapChain1;
	ComPtr<ID3D11RenderTargetView> g_pRenderTargetView;
	ComPtr<ID3D11Texture2D>        g_pDepthStencil;
	ComPtr<ID3D11DepthStencilView> g_pDepthStencilView;
	int g_clientWidth;                                   // �ӿڿ��
	int g_clientHeight;                                  // �ӿڸ߶�

	std::unique_ptr<Mouse> mMouse;						// ���
	Mouse::ButtonStateTracker mMouseTracker;			// ���״̬׷����
	std::unique_ptr<Keyboard> mKeyboard;				// ����
	Keyboard::KeyboardStateTracker mKeyboardTracker;	// ����״̬׷����

	GameTimer mTimer;           // ��ʱ��
	bool      mGamePaused;       // Ӧ���Ƿ���ͣ
};
