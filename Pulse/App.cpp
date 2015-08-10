#include <windows.h>

#include <vector>
#include <streambuf>
#include <iostream>

#include <d2d1_1.h>
#include <d3d11_1.h>

#include "Pulsar.h"
#include "Home.h"
#include "Enemy.h"
#include "Game.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")

Game *game;
LARGE_INTEGER start;
LARGE_INTEGER frequency;
ID2D1Factory1 *pID2D1Factory = NULL;

class outbuf : public std::streambuf {
public:
	outbuf() {
		setp(0, 0);
	}

	virtual int_type overflow(int_type c = traits_type::eof()) {
		return fputc(c, stdout) == EOF ? traits_type::eof() : c;
	}
};


LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		delete game;
		game = new Game(start, frequency, pID2D1Factory);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}	
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	)
{
	
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg = {0};
	HRESULT hr;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"WindowClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		L"WindowClass",
		L"Pulse",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ShowWindow(hwnd, nCmdShow);

	hr = CoInitialize(NULL);

	/*
		Creating the 3D Device
	*/
	ID3D11Device *pD3D11Device = NULL;
	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		NULL, 0,
		D3D11_SDK_VERSION,
		&pD3D11Device,
		NULL,
		NULL
		);

	/*
		Creating the Swap Chain
	*/
	IDXGIDevice * pDXGIDevice;
	hr = pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

	IDXGIFactory2 * pIDXGIFactory;
	hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void **)&pIDXGIFactory);

	DXGI_SWAP_CHAIN_DESC1 props = {};
	props.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	props.SampleDesc.Count = 1;
	props.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	props.BufferCount = 2;

	IDXGISwapChain1 *pDXGISwapChain = NULL;
	hr = pIDXGIFactory->CreateSwapChainForHwnd(
		pD3D11Device,
		hwnd,
		&props,
		NULL,
		NULL,
		&pDXGISwapChain);

	/*
		Creating the Direct2D Device Context
	*/

	//ID2D1Factory1 *pID2D1Factory = NULL;
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, 
		__uuidof(ID2D1Factory1), 
		&options, 
		reinterpret_cast<void **>(&pID2D1Factory));

	ID2D1Device *pID2D1Device = NULL;
	hr = pID2D1Factory->CreateDevice(
		pDXGIDevice,
		&pID2D1Device);

	ID2D1DeviceContext *pID2D1DeviceContext = NULL;
	hr = pID2D1Device->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&pID2D1DeviceContext);

	/*
		Connecting the Device Context and Swap Chain
	*/

	IDXGISurface *pIDXGISurface = NULL;
	hr = pDXGISwapChain->GetBuffer(
		0,
		IID_PPV_ARGS(&pIDXGISurface));

	FLOAT dpiX, dpiY;
	pID2D1Factory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM, 
				D2D1_ALPHA_MODE_IGNORE), 
			dpiX, 
			dpiY);

	ID2D1Bitmap1 *pID2D1Bitmap = NULL;
	hr = pID2D1DeviceContext->CreateBitmapFromDxgiSurface(
		pIDXGISurface,
		&bitmapProperties, 
		&pID2D1Bitmap);

	pID2D1DeviceContext->SetTarget(pID2D1Bitmap);

	//// create the console
	//FILE *pFile;
	//if (AllocConsole()) {
	//	freopen_s(&pFile, "CONOUT$", "w", stdout);
	//	SetConsoleTitle(L"Debug Console");
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	//}

	//// set std::cout to use my custom streambuf
	//outbuf ob;
	//std::streambuf *sb = std::cout.rdbuf(&ob);

	/*
		Message Pump
	*/

	QueryPerformanceCounter(&start);
	QueryPerformanceFrequency(&frequency);

	game = new Game(
		start, 
		frequency,
		pID2D1Factory);

	while (TRUE)
	{
		// Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			// Run game code here
			// ...
			// ...

			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);

			game->Update(now);

			pID2D1DeviceContext->BeginDraw();

			game->Render(pID2D1DeviceContext);

			pID2D1DeviceContext->EndDraw();

			pDXGISwapChain->Present(1, 0);

			if (game->EnemyCount() == 0) {
				game = new Game(
					start,
					frequency,
					pID2D1Factory);
			}

		}
	}

	return 0;
}