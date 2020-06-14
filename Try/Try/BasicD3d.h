#pragma once
#include "basic.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "GameTimer.h"


class BasicD3d
{
public:
	BasicD3d(HINSTANCE hInstance, int nCmdShow);    // 在构造函数的初始化列表设置好初始参数
	~BasicD3d();

	virtual bool Init();  //初始化窗口和驱动
	int Run();           //消息循环检测和持续渲染状态
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


protected:
	bool InitWindow();
	bool InitDevice();
	void SetRenderTarget();
	void ClearRenderTarget();
	virtual void UpdateData(float dt) = 0;   // 子类需要实现该方法，完成每一帧的更新
	virtual void RenderScene() = 0;             // 子类需要实现该方法，完成每一帧的绘制


	int                     nCmdShow;
	HINSTANCE               g_hInst;
	HWND                    g_hWnd;
	D3D_DRIVER_TYPE         g_driverType;
	D3D_FEATURE_LEVEL       g_featureLevel;
	D3D11_VIEWPORT			vp;

	// 使用模板别名(C++11)简化类型名
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
	int g_clientWidth;                                   // 视口宽度
	int g_clientHeight;                                  // 视口高度

	std::unique_ptr<Mouse> mMouse;						// 鼠标
	Mouse::ButtonStateTracker mMouseTracker;			// 鼠标状态追踪器
	std::unique_ptr<Keyboard> mKeyboard;				// 键盘
	Keyboard::KeyboardStateTracker mKeyboardTracker;	// 键盘状态追踪器

	GameTimer mTimer;           // 计时器
	bool      mGamePaused;       // 应用是否暂停
};
