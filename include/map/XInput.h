#pragma once
#include"XStd.h"

//enum KeyState { KEY_FREE, KEY_PUSH, KEY_HOLD, KEY_UP };
//
//class XInput :public XBase, public XSingleton<XInput>
//{
//public:
//	friend class XSingleton<XInput>;
//public:
//	DWORD		 m_dwKeyState[256];
//	DWORD		 KeyCheck(DWORD key);
//public:
//	virtual bool Init();
//	virtual bool Frame();
//	virtual bool Render();
//	virtual bool Release();
//private:
//	XInput();
//public:
//	virtual ~XInput();
//};


#define KEYSTATECOUNT		256

struct XBASIS_INPUT_MAP
{
	bool bUpKey;
	bool bDownKey;
	bool bLeftKey;
	bool bRightKey;

	bool bWKey;
	bool bSKey;
	bool bAKey;
	bool bDKey;
	bool bQKey;
	bool bEKey;
	bool bZKey;
	bool bCKey;

	bool bLeftClick;
	bool bRightClick;
	bool bMiddleClick;

	bool bLeftHold;
	bool bRightHold;
	bool bMiddleHold;

	bool bExit;
	bool bSpace; // 카메라의 이동가속도를 증가시킨다.

	int  iMouseValue[3];

	bool bFullScreen;
	bool bChangeFillMode;
	bool bChangePrimitive;
	bool bChangeCullMode;
	bool bChangeCameraType;
	bool bDebugRender;
};

enum KeyState {
	KEY_FREE = 0,
	KEY_UP = 1,
	KEY_PUSH = 2,
	KEY_HOLD = 3,
};

class XInput : public XSingleton<XInput>
{
private:
	friend class XSingleton<XInput>;
public:
	HWND m_hWnd;
	HINSTANCE m_hinstance;
	//  인풋 객체
	LPDIRECTINPUT8		m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKeyDevice; // 키보드
	LPDIRECTINPUTDEVICE8  m_pMouseDevice;// 마우스
	BYTE				  m_KeyState[KEYSTATECOUNT];
	BYTE				  m_KeyStateOld[KEYSTATECOUNT];
	DIMOUSESTATE		  m_DIMouseState;
	std::vector<wstring>  m_strList;
	TCHAR		m_csBuffer[MAX_PATH];
	BYTE		m_MouseState[3];
	BYTE		m_BeforeMouseState[3];
	POINT		m_MousePos;
public:
	void		Set(HWND hWnd, HINSTANCE hInstance);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	void		MsgEvent(MSG msg);
	BYTE		KeyCheck(BYTE dwKey);
	bool		InitDirectInput();
	void		DeviceAcquire();
	void		DeviceUnacquire();
public:
	XInput(void);
	virtual ~XInput(void);
};

#define I_Input XSingleton<XInput>::Get()
