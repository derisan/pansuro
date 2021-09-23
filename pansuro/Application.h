#pragma once

#include "Base.h"

class Engine;

class Application
{
public:
	static int Run(Engine* engine, HINSTANCE hInstance, int nCmdShow);
	static HWND GetHwnd() { return m_Hwnd; }

protected:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HWND m_Hwnd;
};

