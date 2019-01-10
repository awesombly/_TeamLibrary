#include "GameRule.h"
#include "WriteManager.h"


bool GameRule::Init() noexcept
{


	return true;
}



bool GameRule::Frame()	noexcept
{

	if (m_bSeek == true)
	{
		//WriteManager::Get().SetFontColor(D2D1::ColorF::Blue);
		m_TimerText->m_Text = to_wstring(GetHideTime() - Timer::AccumulateTime).substr(0, 5);
	}
	else
	{
		//WriteManager::Get().SetFontColor(D2D1::ColorF::Red);
		m_TimerText->m_Text = to_wstring(GetPlayTime() - Timer::AccumulateTime).substr(0, 5);
	}

	if (GetHideTime() - Timer::AccumulateTime <=0)
	{
		m_bSeek = false; 

		m_GWinPanel->m_bRender = true;
		m_ZWinPanel->m_bRender = false;
	}

	if (GetPlayTime() - Timer::AccumulateTime <= 0)
	{
		m_bGameEnd = true;

		m_GWinPanel->m_bRender = false;
		m_ZWinPanel->m_bRender = true;
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
	return true;
}


void GameRule::SetResultPanel(JPanel* pUIRoot)
{
	m_GWinPanel = (JPanel*)pUIRoot->find_child(L"GuardWin");
	m_ZWinPanel = (JPanel*)pUIRoot->find_child(L"ZombieWin");
}


float GameRule::GetPlayTime() 
{
	return m_fPlayTime;
}
float GameRule::GetHideTime() {
	return m_fHideTime;
}


void GameRule::SetPlayTime(float ftime)
{
	m_fPlayTime = ftime;
}
void GameRule::SetHideTime(float ftime)
{
	m_fHideTime = ftime;
}


GameRule::GameRule()
{
	m_bSeek = true;
	m_bGameEnd = false;
	m_fPlayTime = 180.0f;
	m_fHideTime = 20.0f;
}


GameRule::~GameRule()
{
}
