#include "pch.h"
#include "Engine.h"

#include "Renderer.h"

Engine* Engine::CreateEngine(UINT width, UINT height, std::wstring title)
{
	return new Engine(width, height, title);
}

Engine::Engine(UINT width, UINT height, std::wstring title)
	: m_Width(width)
	, m_Height(height)
	, m_Title(title)
	, m_Renderer(nullptr)
{
	m_AspectRatio = static_cast<float>(m_Width) / m_Height;
}

Engine::~Engine()
{

}

void Engine::OnInit()
{
	Log::OnInit();

	m_Renderer = Renderer::CreateRenderer(this);
	m_Renderer->OnInit();
}

void Engine::OnDestroy()
{
	if (m_Renderer)
	{
		m_Renderer->OnDestroy();
		delete m_Renderer;
	}
}

void Engine::OnUpdate()
{
	
}

void Engine::OnRender()
{
	m_Renderer->OnRender();
}

void Engine::OnKeyDown(UINT8 keycode)
{
	if (keycode == VK_ESCAPE)
	{
		::PostQuitMessage(0);
	}
}

void Engine::OnKeyUp(UINT8 keycode)
{

}
