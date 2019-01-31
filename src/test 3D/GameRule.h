#pragma once
#include "IBasis.h"
#include "uiheader.h"



class GameRule : public IBasis
{
protected:
	float m_fReadyTime, m_fPlayTime;
	bool  m_bPlayEnd, m_bPlayStart;

	//bool  m_bSPlay, m_bSPlayOver;
public:
	JTextCtrl*	 m_TimerText;

	JPanel*		 m_pEndingUI;
	//JPanel*		 m_ZWinPanel;
	//JPanel*		 m_GWinPanel;

	JPanel*		 m_FightPanel;
	//JPanel*		 m_TimeOverPanel;

	JPanel*		 m_pStatePanel = nullptr;
public:
	float GetPlayTime();
	float GetReadyTime();

	void SetPlayTime(float ftime);
	void SetReadyTime(float ftime);
	void SetResultPanel(JPanel* pUIRoot);
	
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

public:
	GameRule();
	virtual ~GameRule();
};

