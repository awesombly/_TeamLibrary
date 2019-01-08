#pragma once
#include "MainClass.h"
#include "uiheader.h"

class GameRule : MainClass
{
protected:
	float m_fPlayTime;
	float m_fHideTime;
	bool  m_bSeek;
	bool  m_bGameEnd;
public:
	JTextCtrl*	 m_TimerText;



public:
	float GetPlayTime();
	float GetHideTime();

	void SetPlayTime(float ftime);
	void SetHideTime(float ftime);
	
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

public:
	GameRule();
	virtual ~GameRule();
};

