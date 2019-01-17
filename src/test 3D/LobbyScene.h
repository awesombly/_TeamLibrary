#pragma once
#include "MainClass.h"
#include "uiheader.h"
#include "PlayerController.h"

class LobbyScene : public MainClass
{
public:
	bool	  m_isStart		= false;
	bool	  m_isMatching = false;
	AHeroObj* m_pBackHero	= nullptr;
	AHeroObj* m_pBackGuard  = nullptr;
	AHeroObj* m_pBackZombie = nullptr;
	///
	//JPanel*		m_toHost		= nullptr;
	//JPanel*		m_toGuest		= nullptr;
	JTextCtrl*	m_toGuestIP		= nullptr;
	JPanel*		m_toGuestPanel	= nullptr;
	JPanel*		m_pStartEffect	= nullptr;
public:
	void SelectCharacter(const PlayerController::ECharacter& eCharacter) noexcept;
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