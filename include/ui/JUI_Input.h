#pragma once
#include "stdui.h"
namespace UI
{
	class JUI_Input : public JSingleton<JUI_Input>
	{
	private:
		friend class JSingleton<JUI_Input>;
	public:
		BYTE m_KeyState[KEYSTATECOUNT];
		DIMOUSESTATE m_MouseState;
	private:
		LPDIRECTINPUT8 m_pDi;
		LPDIRECTINPUTDEVICE8 m_pDidevKey;
		LPDIRECTINPUTDEVICE8 m_pDidevMouse;
		DIDEVICEOBJECTDATA   m_didod[SAMPLE_BUFFER_SIZE];
		BYTE m_KeyBeforeState[KEYSTATECOUNT];
		DIMOUSESTATE m_MouseBeforeState;
		DWORD        m_dwElements;
		HWND         m_hWnd;
		DWORD        m_dwImmediate;
	private:
		bool KeyProcess();
		bool MouseProcess();
		void DeviceAcquire();
		void DeviceUnacquire();

		void PostProcess();
		void SetAcquire(bool bActive);
	public:		
		bool IsKeyDown();
		bool IsKeyDown(DWORD dwKey);
		bool IsKeyUP(DWORD dwKey);
		bool InitDirectInput(HINSTANCE hInst, HWND hWnd, bool keyboard, bool mouse);
		bool Init();
		bool Frame();
		bool Render();
		bool Release();
		bool ResetDevice();

	public:
		JUI_Input();
		virtual ~JUI_Input();
	};
}
//#define I_UIInput JUI_Input::GetInstance()
