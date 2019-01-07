#include "CoreDX.h"
#include <thread>
#include <mutex>

Timer   Core::m_Timer;
bool	Core::isPlaying = true;

bool Core::GameRun() noexcept
{
	if (!GameInit())
		return false;
	
	// ������ ����
	std::thread gameTimer(&Timer::Frame, &m_Timer);
	//std::thread gameFrame(&Core::GameFrame, this);
	//std::thread gameRender(&Core::GameRender, this);

	// ���� ������ ����
	//while (MessageProcess());

	while (isPlaying)
	{
		std::unique_lock<mutex> lock(m_Timer.m_mutex);
		m_Timer.m_FrameEvent.wait(lock);
		GameFrame();
	}
	//{
	//	std::unique_lock<mutex> lock(m_Timer.m_mutex);
	//	ErrorMessage(""s + __FUNCTION__ + " -> Frame Exit!");
	//	std::this_thread::yield();
	//}

	gameTimer.join();
	//gameFrame.join();
	//gameRender.join();

	this_thread::sleep_for(chrono::milliseconds(100));
	this_thread::yield();
	GameRelease();
	this_thread::sleep_for(chrono::milliseconds(100));
	this_thread::yield();
	// ��� ������ ����
	//m_ClientServer.TerminateServer();
	return true;
}

bool Core::GameInit() noexcept
{
	// ���� �ʱ�ȭ
	srand(unsigned int(time(0)));
	InitWELLState();	

	//WSADATA wsa;
	//if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	//	return false;

	if (!m_DxManager.Init()		|| !m_ObjectManager.Init() || 
		!m_SoundManager.Init()	|| !Init())
	{
		ErrorMessage(""s + __FUNCTION__ + ", �ʱ�ȭ ����!");
		return false;
	}
	return true;
}

bool Core::GameFrame() noexcept 
{
	//while (isPlaying)
	//{
	//	std::unique_lock<mutex> lock(m_Timer.m_mutex);
	//	m_Timer.m_FrameEvent.wait(lock);

	//	MessageProcess();
	//	m_Input.MouseUpdate();		// ���콺 Ȧ��, ���� üũ
	//	m_Input.Frame();
	//	Frame();

	//	m_Timer.m_RenderEvent.notify_all();
	//	std::this_thread::yield();
	//}
	//std::unique_lock<mutex> lock(m_Timer.m_mutex);
	//ErrorMessage(""s + __FUNCTION__ + " -> Frame Exit!");
	//std::this_thread::sleep_for(chrono::milliseconds(100));
	//m_Timer.m_RenderEvent.notify_all();
	//std::this_thread::yield();

	//while (isPlaying)
	//{
	//	std::unique_lock<mutex> lock(m_Timer.m_mutex);
	//	m_Timer.m_FrameEvent.wait(lock);

		// Frame
		MessageProcess();
		m_Input.Frame();
		Frame();
		m_Input.MouseUpdate();		// ���콺 Ȧ��, ���� üũ
		// Render
		m_DxManager.PrevRender();
		m_Timer.Render();
		m_Input.Render();
		Render();
		m_DxManager.PostRender();
//		std::this_thread::yield();
//	}
//	{
//		std::unique_lock<mutex> lock(m_Timer.m_mutex);
//		ErrorMessage(""s + __FUNCTION__ + " -> Frame Exit!");
//		std::this_thread::yield();
//	}
	return true;
}


bool Core::GameRender() noexcept
{
	//while (isPlaying)
	//{
	//	static std::mutex Mutex;
	//	std::unique_lock<mutex> lock(Mutex);
	//	m_Timer.m_RenderEvent.wait(lock);
	//
	//	m_DxManager.PrevRender();
	//	m_Timer.Render();
	//	m_Input.Render();
	//	Render();
	//	m_DxManager.PostRender();
	//	//m_Input.MouseUpdate();		// ���콺 Ȧ��, ���� üũ
	//
	//	//std::this_thread::yield();
	//}
	//std::unique_lock<mutex> lock(m_Timer.m_mutex);
	//ErrorMessage(""s + __FUNCTION__ + " -> Render Exit!");
	return true;
}


bool Core::GameRelease() noexcept
{
	Release();
	m_Timer.Release();
	m_Input.Release();
	//// ���� ����
	//WSACleanup();
	return true;
}


// ������-��ǲ �ڵ鸵
void Core::MsgEvent(const MSG& _message) noexcept
{
	m_Input.MsgEvent(_message);
}