#pragma once
#include "MainClass.h"

#include "XObjectManager.h"
#include "uiheader.h"

class GameScene : public MainClass
{
public:
	PlayerController* m_pPlayer = &PlayerController::Get();		// ��Ʈ�ѷ�

	Collider*	m_pHeightCollider = nullptr;

	AHeroObj*	m_pHero			= nullptr;		// ĳ���� ��ü
	AHeroObj*	m_pZombie		= nullptr;		// 
	AHeroObj*	m_pBird			= nullptr;		// 
	AHeroObj*	m_pChicken		= nullptr;		// 

	JTextCtrl*	 m_TimerText	= nullptr;
	JCheckCtrl*  m_pCheckBox	= nullptr;
	JSliderCtrl* m_pVolume		= nullptr;
	JSliderCtrl* m_pMouseSense  = nullptr;
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

	GameScene() = default;
	virtual ~GameScene() = default;
};