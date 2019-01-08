#pragma once
#include "MainClass.h"
#include "uiheader.h"

class GameRule : MainClass
{
public:
	float m_fGameTime;
	float m_fHideTime;
	bool  m_bSeek;
public:
	float GetPlayTime();
	float GetHideTime();
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

public:
	GameRule()= default;
	virtual ~GameRule()= default;
};

