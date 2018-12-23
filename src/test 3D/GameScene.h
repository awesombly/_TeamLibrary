#pragma once
#include "MainClass.h"

class GameScene : public MainClass
{
public:
	PlayerController* m_pPlayer = nullptr;		// ��Ʈ�ѷ�

	AHeroObj*	m_pHero			= nullptr;		// ĳ���� ��ü
	AHeroObj*	m_pZombi		= nullptr;		// 
	AHeroObj*	m_pBird			= nullptr;		// 
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

	GameScene() = default;
	virtual ~GameScene() = default;
};