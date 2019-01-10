#pragma once
#include "IBasis.h"
#include "uiheader.h"



class GameRule : public IBasis
{
protected:
	float m_fPlayTime;
	float m_fHideTime;
	bool  m_bSeek;
	bool  m_bGameEnd;
public:
	JTextCtrl*	 m_TimerText;


	JPanel*		 m_ZWinPanel;
	JPanel*		 m_GWinPanel;
public:
	float GetPlayTime();
	float GetHideTime();

	void SetPlayTime(float ftime);
	void SetHideTime(float ftime);
	void SetResultPanel(JPanel* pUIRoot);
	
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

public:
	GameRule();
	virtual ~GameRule();
};

