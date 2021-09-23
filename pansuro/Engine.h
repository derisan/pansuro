#pragma once

#include "Base.h"

class Renderer;

class Engine
{
public:
	static Engine* CreateEngine(UINT width, UINT height, std::wstring title);

	void OnInit();
	void OnDestroy();
	void OnUpdate();
	void OnRender();
	void OnKeyDown(UINT8 keycode);
	void OnKeyUp(UINT8 keycode);

	UINT GetWidth() const { return m_Width; }
	UINT GetHeight() const { return m_Height; }
	float GetAspectRatio() const { return m_AspectRatio; }
	const std::wstring& GetTitle() const { return m_Title; }

	~Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

private:
	Engine(UINT width, UINT height, std::wstring title);

private:
	UINT m_Width;
	UINT m_Height;
	float m_AspectRatio;
	std::wstring m_Title;

	Renderer* m_Renderer;
};
