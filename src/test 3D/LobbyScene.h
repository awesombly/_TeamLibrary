#pragma once
#include "MainClass.h"
#include "AHeroObj.h"
#include "uiheader.h"
#include "PlayerController.h"


class LobbyScene : public MainClass
{
public:
	bool	  m_isStart		= false;
	AHeroObj* m_pBackHero	= nullptr;
	AHeroObj* m_pBackGuard  = nullptr;
	AHeroObj* m_pBackArcher = nullptr;
	AHeroObj* m_pBackMage   = nullptr;

	///
	JTextCtrl*	m_toGuestIP		= nullptr;
	JPanel*		m_toGuestPanel	= nullptr;
	JPanel*		m_pStartEffect	= nullptr;
	JPanel*		m_pIsMatching = nullptr;
public:
	void SelectCharacter(const bool& toRight) noexcept;
	void LoadUI()				noexcept override;
	bool FirstInit()			noexcept override;

	void StartToHost()					 override;
	void StartToGuest()					 override;

	virtual bool Init()			noexcept override;
	virtual bool Frame()		noexcept override;
	virtual bool Render()		noexcept override;
	virtual bool Release()		noexcept override;

	LobbyScene() = default;
	virtual ~LobbyScene() = default;
};