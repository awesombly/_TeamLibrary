#pragma once
#include "MainClass.h"

#include "XObjectManager.h"
#include "uiheader.h"
#include "PlayerController.h"

class GameScene : public MainClass
{
public:
	PlayerController* m_pPlayer = &PlayerController::Get();		// 컨트롤러
	// Character
	AHeroObj*	m_pHero			= nullptr;		
	AHeroObj*	m_pZombie		= nullptr;		
	AHeroObj*	m_pBird			= nullptr;		
	AHeroObj*	m_pChicken		= nullptr;		
	// UI
	JListCtrl*   m_pList		= nullptr;		
	JTextCtrl*	 m_TimerText	= nullptr;
	JCheckCtrl*  m_pCheckBox	= nullptr;
	JSliderCtrl* m_pVolume		= nullptr;
	JSliderCtrl* m_pMouseSense  = nullptr;
public:
	void DrawBoundingBox()	noexcept;

	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

	GameScene() = default;
	virtual ~GameScene() = default;
};