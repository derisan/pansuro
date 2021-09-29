#include "pch.h"
#include "Input.h"

#include "Application.h"

Input* Input::Instance()
{
	static Input instance;
	return &instance;
}

bool Input::IsButtonHold(KeyType key)
{
	return m_KeyStates[static_cast<UINT8>(key)] == KeyState::Hold;
}

bool Input::IsButtonDown(KeyType key)
{
	return m_KeyStates[static_cast<UINT8>(key)] == KeyState::Down;
}

bool Input::IsButtonUp(KeyType key)
{
	return m_KeyStates[static_cast<UINT8>(key)] == KeyState::Up;
}

Input::Input()
{
	
}

void Input::Init()
{
	m_Hwnd = Application::GetHwnd();
	m_KeyStates.resize(256, KeyState::None);
}

void Input::Update()
{
	HWND hwnd = ::GetActiveWindow();
	if (m_Hwnd != hwnd)
	{
		for (UINT key = 0; key < kKeyCount; key++)
			m_KeyStates[key] = KeyState::None;

		return;
	}

	BYTE asciiKeys[kKeyCount] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (UINT key = 0; key < kKeyCount; key++)
	{
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = m_KeyStates[key];

			if (state == KeyState::Hold|| state == KeyState::Down)
				state = KeyState::Hold;
			else
				state = KeyState::Down;
		}
		else
		{
			KeyState& state = m_KeyStates[key];

			if (state == KeyState::Hold|| state == KeyState::Down)
				state = KeyState::Up;
			else
				state = KeyState::None;
		}
	}
}
