#include "Window.h"
#include "ObjectManager.h"


// static �ʱ�ȭ
RECT        Window::m_clientRect{0,0,0,0};
HWND		Window::m_hWnd = 0;			
HINSTANCE	Window::m_hInstance = 0;	
//MyVector2	Window::DrawPoint{ 0, 0 };
Window*		Window::Instance = nullptr;

bool Window::MessageProcess()
{
	while (1)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			// �޼��� �ؼ�
			TranslateMessage(&message);
			// �޼���->���ν��� ����
			DispatchMessage(&message);

			if (message.message == WM_QUIT)
				break;
			MsgEvent(message);
		}
		else
		{
			return true;
		}
	}
	return false;
}

// �ü���κ��� �޼����� �޾� ó���ϴ� �Լ�.
// �޼����� �޼��� ť�� ����ǰ�, �ʿ�� �ش� �����찡 �����´�.
// CALLBACK : ����ڰ� ȣ������ �ʰ� �ü������ ȣ���ϴ� �Լ�
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, 
								 WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
	{
		// �ּ�ȭ �޼����� �ƴҽ�
		if (wParam != SIZE_MINIMIZED)
		{
			// ����, ���� ����Ʈ�� ���� ���� ũ�� �Ѿ��
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			//static once_flag bResize;
			//call_once(bResize, [&]() {
				DxManager::Get().ResizeDevice(width, height);
			//});

			//Window::ResizeWindow({ 0, 0, width, height });
			//auto pEvent = [](void* width, void* height)
			//{
			//	DxManager::Get().ResizeDevice(*(UINT*)width, *(UINT*)height);
			//};
			//ObjectManager::PostFrameEvent.emplace(pEvent, &width, &height);
			//DxManager::Get().ResizeDevice(width, height);
		}
	}	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	} break;
	case WM_DESTROY:
	{
		// ���� �޼����� ������ ���ν����� ���
		PostQuitMessage(0);
	} break;
	default:
		// �ȹ��� �޼������� �ü���� �Ѱ� �˾Ƽ� ó���ϰ� �Ѵ�
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ������ �ʱ� ����, ����
// CHAR : ��Ƽ����Ʈ, WCHAR : �����ڵ�, TCHAR : �˾Ƽ�����
bool Window::SetWindow(const HINSTANCE& hInstance, const int& nCmdShow, const TCHAR* title) noexcept
{
	m_hInstance = hInstance;

	// 1. ������ Ŭ���� ��ü ���(�ü��)
	WNDCLASSEX winClass;
	// ������ Ŭ���� �̸�, ũ��, ��Ÿ��
	winClass.lpszClassName = L"MyWindow";
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	// ������ ���ν���, �ν��Ͻ�
	winClass.lpfnWndProc = WndProc;
	winClass.hInstance = m_hInstance;
	// �޴�, 
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	// ������, Ŀ��, ��� �ε�
	winClass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	winClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	winClass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClassEx(&winClass))		// ��� ó��
		return false;
	
	AdjustWindowRectEx(&m_winRect, WS_POPUP, false, WS_EX_APPWINDOW);
	m_winRect.right -= m_winRect.left;
	m_winRect.bottom -= m_winRect.top;
	m_winRect.left += m_clientRect.left;
	m_winRect.top += m_clientRect.top;
	

	// 2. ��ϵ� Ŭ���� ��ü ����� ������ �����ϰ� �ڵ鿡 ����
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"MyWindow", title,
							WS_OVERLAPPED,
							m_winRect.left, m_winRect.top,
							m_winRect.right, m_winRect.bottom,
							NULL, NULL, m_hInstance, NULL);
	if (m_hWnd == NULL)
		return false;

	// ������ ���
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);			// ȭ�� ����->WM_PAINT ȣ��

	GetClientRect(m_hWnd, &m_clientRect);

	return true;
}

void Window::SetClientRect(const int& x, const int& y, const int& width, const int& height) noexcept
{
	m_winRect = { 0, 0, width, height };
	m_clientRect = { x, y, width, height };
};

void Window::ResizeWindow(const MyRect& rect) noexcept
{
	rect.RECTset(&Instance->m_winRect);
	rect.RECTset(&Instance->m_clientRect);
	AdjustWindowRectEx(&Instance->m_winRect, WS_OVERLAPPED, false, WS_EX_APPWINDOW);
	Instance->m_winRect.right -= Instance->m_winRect.left;
	Instance->m_winRect.bottom -= Instance->m_winRect.top;

	SetWindowPos(m_hWnd, NULL, 0, 0, Instance->m_winRect.right, Instance->m_winRect.bottom, SWP_SHOWWINDOW);
	GetClientRect(m_hWnd, &m_clientRect);
};

RECT& Window::getClientRect()
{
	return m_clientRect;
}

RECT& Window::GetWinRect()
{
	return Instance->m_winRect;
}