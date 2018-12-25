#pragma once
#include "MainClass.h"
#include "JTextCtrl.h"


class LobbyScene : public MainClass
{
public:

public:
	JTextCtrl*	m_toGuest = nullptr;
	JPanel*		m_toHost  = nullptr;
public:
	void InputIP()			noexcept;

	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

	LobbyScene() = default;
	virtual ~LobbyScene() = default;
};