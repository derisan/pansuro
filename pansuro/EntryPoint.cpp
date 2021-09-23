#include "pch.h"
#include "Engine.h"
#include "Application.h"


_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	Engine* engine = Engine::CreateEngine(1280, 720, L"My App");
	return Application::Run(engine, hInstance, nCmdShow);
}
