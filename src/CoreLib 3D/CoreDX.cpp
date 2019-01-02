#include "CoreDX.h"
#include <thread>
#include <mutex>

Timer   Core::m_Timer;
bool	Core::isPlaying = true;

bool Core::GameRun() noexcept
{
	std::thread gameTimer(&Timer::Frame, &m_Timer);
	if (!GameInit())
		return false;
	
	// 쓰레드 가동
	//std::thread gameFrame(&Core::GameFrame, this);
	std::thread gameRender(&Core::GameRender, this);

	// 메인 쓰레드 루프
	//while (MessageProcess());
	while (isPlaying)
	{
		std::unique_lock<mutex> lock(m_Timer.m_mutex);
		m_Timer.m_FrameEvent.wait(lock);

		MessageProcess();
		m_Input.Frame();
		Frame();

		m_Input.MouseUpdate();		// 마우스 홀드, 프리 체크
		m_Timer.m_RenderEvent.notify_all();
		std::this_thread::yield();
	}
	{
		std::unique_lock<mutex> lock(m_Timer.m_mutex);
		ErrorMessage(""s + __FUNCTION__ + " -> Frame Exit!");
		m_Timer.m_RenderEvent.notify_all();
		std::this_thread::yield();
	}

	gameTimer.join();
	//gameFrame.join();
	gameRender.join();

	this_thread::yield();
	this_thread::sleep_for(chrono::seconds(1));
	GameRelease();
	// 모든 쓰레드 종료
	//m_ClientServer.TerminateServer();
	return true;
}

bool Core::GameInit() noexcept
{
	// 난수 초기화
	srand(unsigned int(time(0)));
	InitWELLState();	

	//WSADATA wsa;
	//if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	//	return false;

	if (!m_DxManager.Init()		|| !m_ObjectManager.Init() || 
		!m_SoundManager.Init()	|| !Init())
	{
		ErrorMessage(""s + __FUNCTION__ + ", 초기화 실패!");
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
	//	m_Input.MouseUpdate();		// 마우스 홀드, 프리 체크
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
	return true;
}


bool Core::GameRender() noexcept
{
	while (isPlaying)
	{
		static std::mutex Mutex;
		std::unique_lock<mutex> lock(Mutex);
		m_Timer.m_RenderEvent.wait(lock);

		m_DxManager.PrevRender();
		m_Timer.Render();
		m_Input.Render();
		Render();
		m_DxManager.PostRender();
		//m_Input.MouseUpdate();		// 마우스 홀드, 프리 체크

		//std::this_thread::yield();
	}
	std::unique_lock<mutex> lock(m_Timer.m_mutex);
	ErrorMessage(""s + __FUNCTION__ + " -> Render Exit!");
	return true;
}


bool Core::GameRelease() noexcept
{
	Release();
	m_Timer.Release();
	m_Input.Release();
	//// 접속 종료
	//WSACleanup();
	return true;
}


// 윈도우-인풋 핸들링
void Core::MsgEvent(const MSG& _message) noexcept
{
	m_Input.MsgEvent(_message);
}