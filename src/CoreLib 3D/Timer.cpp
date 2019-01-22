#include "Timer.h"
#include "Input.h"
#include "WriteManager.h"
//#include "ChattingBox.h"
#include "CoreDX.h"

float Timer::SPF = 0.0f;
int	  Timer::FPS = 0;
float Timer::AccumulateTime = 0.0f;
Timer*	Timer::Instance = nullptr;
queue<tuple<void(*)(void*, void*), void*, void*> > Timer::SecondFrameEvent;

Timer::Timer() : m_kDirTick(1.0f / 60), m_ElapseTime(0.0f), m_FrameCnt(0),
				m_CurrentTick({ 0,0 }), m_BeforeTick({ 0,0 }), m_GameSpeed(1.0f)
{
	Instance = this;
	m_DirFrame = 60;
	Init();
}

Timer::~Timer() {}

bool Timer::Init() noexcept
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_CurrentTick);
	QueryPerformanceCounter(&m_BeforeTick);
	return true;
}

// Ÿ�̸�-������ ó��
bool Timer::Frame() noexcept
{
	while (Core::isPlaying)
	{
		std::lock_guard<mutex> lock(m_mutex);
		// �����ӱ��� ���
		QueryPerformanceCounter(&m_CurrentTick);
		m_ElapseTime = (float)(m_CurrentTick.QuadPart - m_BeforeTick.QuadPart) / m_Frequency.QuadPart;

		// �������� ������ ��
		if (m_ElapseTime >= m_kDirTick)
		{
			//if (FPS < m_DirFrame)
			//	m_kDirTick -= SPF * 0.00001f;
			//else if (FPS > m_DirFrame)
			//	m_kDirTick += SPF * 0.00001f;

			m_FrameCnt++;
			AccumulateTime += SPF = m_ElapseTime;
			//SPF *= m_GameSpeed;

			// 1�ʸ��� ������ ����
			static LARGE_INTEGER prevTick = { 0,0 };
			if ((float)(m_CurrentTick.QuadPart - prevTick.QuadPart) / m_Frequency.QuadPart >= 1)
			{
				prevTick.QuadPart = m_CurrentTick.QuadPart;
				FPS = m_FrameCnt;
				m_FrameCnt = 0;
				if (Input::isDebug)
				{
					ErrorMessage("Time : " + to_string(AccumulateTime) + ",   FPS : " + to_string(FPS));
				}

				// �ʴ� �߻� �̺�Ʈ
				while (!SecondFrameEvent.empty())
				{
					auto&[secondEvent, param1, param2] = SecondFrameEvent.front();
					secondEvent(param1, param2);
					SecondFrameEvent.pop();
				}
			}
			m_BeforeTick.QuadPart = m_CurrentTick.QuadPart;

			m_FrameEvent.notify_all();
			std::this_thread::yield();
		}
	}
	{
		std::lock_guard<mutex> lock(m_mutex);
		ErrorMessage(__FUNCTION__ + " -> Timer Exit!"s);
	}
	std::this_thread::sleep_for(chrono::microseconds(100));
	m_FrameEvent.notify_all();
	std::this_thread::yield();
	return false;
}

bool Timer::Render() noexcept
{
	//if (Input::isDebug)
	//{
	//	WriteManager::Get().SetFontSize(5);
	//	WriteManager::Get().DrawTextW({ 10, 10, 400, 100 }, m_InfoStream.str());
	//}
	return true;
}

void Timer::setDirFrame(const int& dirFrame)
{
	m_DirFrame = dirFrame;
	m_kDirTick = 1.0f / m_DirFrame;
}

void Timer::setGameSpeed(const float& speedRate)
{
	m_GameSpeed = speedRate;
}

bool  Timer::Release() noexcept
{
	return true;
}


void Timer::ResetSPF() noexcept
{
	m_CurrentTick.QuadPart = 0;
	m_BeforeTick.QuadPart = 0;
	m_ElapseTime = 0.0f;
	SPF = 0.0f;
	//AccumulateTime = 0.0f;
}