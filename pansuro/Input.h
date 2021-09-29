#pragma once

#include "Core.h"

#define INPUT Input::Instance()

enum class KeyType
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',

	LBUTTON = VK_LBUTTON,
	RBUTTON = VK_RBUTTON,
};

enum class KeyState
{
	None,
	Hold,
	Down,
	Up
};


class Input
{
public:
	static Input* Instance();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	void Init();
	void Update();

	bool IsButtonHold(KeyType key);
	bool IsButtonDown(KeyType key);
	bool IsButtonUp(KeyType key);

private:
	Input();

private:
	static const int kKeyCount = 256;

	std::vector<KeyState> m_KeyStates;
	HWND m_Hwnd;
};

