#include "GameRule.h"
#include "WriteManager.h"
#include "SoundManager.h"



bool GameRule::Init() noexcept
{
	return true;
}

bool GameRule::Frame()	noexcept
{
	if (m_bPlayStart == false)
	{
		m_TimerText->m_Text = to_wstring(GetReadyTime() - Timer::AccumulateTime).substr(0, 5);
		m_FightPanel->m_bRender=true;
	}
	else
	{
		m_TimerText->m_Text = to_wstring(GetPlayTime() - Timer::AccumulateTime).substr(0, 5);
	}




	//시작시 스타트 사운드 재생
	if (GetReadyTime() - Timer::AccumulateTime <=0)
	{
		if (m_bPlayStart != true) {
			m_bPlayStart = true;
			SoundManager::Get().Play("SE_game_time_start.mp3");
		}
	}

	//게임종료시 사운드 재생
	if (GetPlayTime() - Timer::AccumulateTime <= 0)
	{
		m_TimeOverPanel->m_bRender = true;

		if (m_bPlayEnd != true) {
			m_bPlayEnd = true;
			SoundManager::Get().Play("SE_game_time_end.mp3");
			m_pStatePanel->m_bRender = true;
			m_pStatePanel->PreEvent.first = nullptr;
		}
		if(Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
			m_pStatePanel->m_bRender = false;
		m_TimerText->m_Text = '0';
	}

	return true;
}

bool GameRule::Render()	noexcept
{

	return true;
}

bool GameRule::Release()	noexcept
{
	m_TimerText->Release();
	delete m_GWinPanel;
	delete m_ZWinPanel;
	delete m_FightPanel;
	delete m_TimeOverPanel;
	return true;
}


void GameRule::SetResultPanel(JPanel* pUIRoot)
{
	m_GWinPanel = (JPanel*)pUIRoot->find_child(L"GuardWin");
	m_ZWinPanel = (JPanel*)pUIRoot->find_child(L"ZombieWin");
	m_pStatePanel = (JPanel*)pUIRoot->find_child(L"State_Panel");
	m_FightPanel = (JPanel*)pUIRoot->find_child(L"fight_panel");
	m_TimeOverPanel = (JPanel*)pUIRoot->find_child(L"TimeOver");
}


float GameRule::GetPlayTime() 
{
	return m_fPlayTime;
}

float GameRule::GetReadyTime() 
{
	return m_fReadyTime;
}

void GameRule::SetPlayTime(float ftime)
{
	m_fPlayTime = ftime;
}

void GameRule::SetReadyTime(float ftime)
{
	m_fReadyTime = ftime;
}


GameRule::GameRule()
{
	m_bPlayEnd = false;
	m_bPlayStart = false;

	m_fPlayTime = 20.0f;
	m_fReadyTime = 6;
}


GameRule::~GameRule()
{
}
