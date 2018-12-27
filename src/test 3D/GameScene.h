#pragma once
#include "MainClass.h"

#include "XQuadTreeIndex.h"
//#include "XCamera.h"
#include "XMapImporter.h"
#include "XObjectManager.h"
#include "uiheader.h"

class GameScene : public MainClass
{
public:
	PlayerController* m_pPlayer = &PlayerController::Get();		// 컨트롤러

	Collider*	m_pHeightCollider = nullptr;

	AHeroObj*	m_pHero			= nullptr;		// 캐릭터 본체
	AHeroObj*	m_pZombie		= nullptr;		// 
	AHeroObj*	m_pBird			= nullptr;		// 
	AHeroObj*	m_pChicken		= nullptr;		// 

	JTextCtrl*	 m_TimerText	= nullptr;
	JCheckCtrl*  m_pCheckBox	= nullptr;
	JSliderCtrl* m_pVolume		= nullptr;
	JSliderCtrl* m_pMouseSense  = nullptr;

	//XCamera			m_Camera;					// 프러스텀 카메라
	XMap*			m_pMap;						// 맵
	XQuadTreeIndex*	m_pMapTree;					// 맵->공간분할, LOD시 수정 예정
	XMapImporter	m_Importer;					// 세이브 데이터 로더
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

	GameScene() = default;
	virtual ~GameScene() = default;
};