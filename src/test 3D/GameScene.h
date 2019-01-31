#pragma once
#include "MainClass.h"
#include "PlayerController.h"

#include "XObjectManager.h"
#include "uiheader.h"

#include "GameRule.h"

class GameScene : public MainClass
{
public:
	enum class EGameState {
		Wait, GameStart, WaveInit, Spawn
	};
	EGameState m_eState = EGameState::Wait;
	PlayerController* m_pPlayer = &PlayerController::Get();		// 컨트롤러
	float m_frameCount = 0.0f;
	UINT  m_waveCount = 0;
	int   m_spawnCount = 0;
	// UI
	JEditCtrl*	 m_pChat = nullptr;
	JTextCtrl*	 m_TimerText	= nullptr;
	JCheckCtrl*  m_pCheckBox	= nullptr;
	JSliderCtrl* m_pVolume		= nullptr;
	JSliderCtrl* m_pMouseSense  = nullptr;
	wstring		 m_chatMessage;
	///
	GameRule	m_Rule;
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