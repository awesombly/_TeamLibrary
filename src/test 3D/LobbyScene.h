#pragma once
#include "MainClass.h"
#include "uiheader.h"


class LobbyScene : public MainClass
{
public:
	//JPanel*		m_toHost		= nullptr;
	JTextCtrl*	m_toGuestIP		= nullptr;
	//JPanel*		m_toGuest		= nullptr;
	JPanel*		m_toGuestPanel	= nullptr;
public:
	void LoadUI()				noexcept override;
	bool FirstInit()			noexcept override;

	virtual bool Init()			noexcept override;
	virtual bool Frame()		noexcept override;
	virtual bool Render()		noexcept override;
	virtual bool Release()		noexcept override;

	LobbyScene() = default;
	virtual ~LobbyScene() = default;
};