#pragma once
#include "MainClass.h"
#include "PlayerController.h"
#include "GameMap.h"
#include "uiheader.h"


class GameScene : public MainClass
{
public:
	enum class EGameState {
		PreWait, Wait, WaveInit, Spawn, End,
	};

	EGameState m_eState = EGameState::PreWait;
	PlayerController* m_pPlayer = &PlayerController::Get();		// ��Ʈ�ѷ�
	float* m_pFrameCount = nullptr;
	
	int   m_spawnCount = 0;
	// UI
	JEditCtrl*	 m_pChat = nullptr;
	JTextCtrl*	 m_TimerText	= nullptr;
	JCheckCtrl*  m_pCheckBox	= nullptr;

	wstring		 m_chatMessage;
	///
	GameMap&	 m_MapObjects = GameMap::Get();
public:
	void DrawBoundingBox()		noexcept;
	bool CheatMessage()			noexcept;
	void HostFrame()			noexcept;

	void LoadUI()				noexcept override;
	bool FirstInit()			noexcept override;

	virtual bool Init()			noexcept override;
	virtual bool Frame()		noexcept override;
	virtual bool Render()		noexcept override;
	virtual bool Release()		noexcept override;

	GameScene() = default;
	virtual ~GameScene() = default;
};