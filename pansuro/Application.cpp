#include "pch.h"
#include "Application.h"

#include "Engine.h"

HWND Application::m_Hwnd = nullptr;

int Application::Run(Engine* engine, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"app";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, static_cast<LONG>(engine->GetWidth()), static_cast<LONG>(engine->GetHeight()) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	m_Hwnd = CreateWindow(
		windowClass.lpszClassName,
		engine->GetTitle().c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		engine);
	
	AllocConsole();

	engine->Init();

	ShowWindow(m_Hwnd, nCmdShow);

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	engine->Shutdown();

	return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK Application::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Engine* engine = reinterpret_cast<Engine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CREATE:
	{
		LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
	}
	return 0;

	case WM_PAINT:
		if (engine)
		{
			engine->Update();
			engine->Render();
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

