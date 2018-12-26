#pragma once
#include "MainClass.h"

#include "XQuadTreeIndex.h"
//#include "XCamera.h"
#include "XMapImporter.h"
#include "XObjectManager.h"

class GameScene : public MainClass
{
public:
	PlayerController* m_pPlayer = &PlayerController::Get();		// ��Ʈ�ѷ�

	AHeroObj*	m_pHero			= nullptr;		// ĳ���� ��ü
	AHeroObj*	m_pZombi		= nullptr;		// 
	AHeroObj*	m_pBird			= nullptr;		// 
	AHeroObj*	m_pChicken		= nullptr;		// 


	//XCamera			m_Camera;					// �������� ī�޶�
	XMap*			m_pMap;						// ��
	XQuadTreeIndex*	m_pMapTree;					// ��->��������, LOD�� ���� ����
	XMapImporter	m_Importer;					// ���̺� ������ �δ�
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

	GameScene() = default;
	virtual ~GameScene() = default;
};