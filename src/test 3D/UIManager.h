#pragma once
#include "ISingleton.h"
#include "uiheader.h"


class UIManager : public ISingleton<UIManager>
{
public:
	// UI
	JEditCtrl*		m_pChat				= nullptr;
	JPanel*			m_pMouseIcon		= nullptr;
	JProgressBar*	m_pHpBar			= nullptr;	
	JProgressBar*	m_pMpBar			= nullptr;
	JProgressBar*	m_pLeftIcon			= nullptr;		
	JProgressBar*	m_pRightIcon		= nullptr;		
	JPanel*			m_pRespawn			= nullptr;
	JProgressBar*	m_pRespawnBar		= nullptr;
	JPanel*			m_pRespawnEffect	= nullptr;
	JPanel*			m_pHitEffect		= nullptr;
	JPanel*			m_pEnemyPanel		= nullptr;		// 
	JProgressBar*	m_pEnemyHP			= nullptr;		// 
	JTextCtrl*		m_pEnemyName		= nullptr;		// 
	JTextCtrl*		m_pEnemyHPText		= nullptr;		// 
	JProgressBar*	m_pExpProgress		= nullptr;		// 
	// 인포창
	JEditCtrl*		m_pInfoName			= nullptr;
	JTextCtrl*		m_pInfoHP			= nullptr;	
	JTextCtrl*		m_pInfoMP			= nullptr;	
	JTextCtrl*		m_pInfoTitle		= nullptr;
	JTextCtrl*		m_pInfoEXP			= nullptr;	
	JTextCtrl*		m_pInfoAttackSpeed	= nullptr;
	JTextCtrl*		m_pInfoMoveSpeed	= nullptr;
	JTextCtrl*		m_pInfoLevel		= nullptr;	
	JTextCtrl*		m_pInfoDamage		= nullptr;	
	JTextCtrl*		m_pInfoArmor		= nullptr;	
	JTextCtrl*		m_pInfoSP			= nullptr;		
	JTextCtrl*		m_pInfoStr			= nullptr;		
	JTextCtrl*		m_pInfoDex			= nullptr;		
	JTextCtrl*		m_pInfoInt			= nullptr;		
	JTextCtrl*		m_pInfoLuk			= nullptr;		
	JButtonCtrl*	m_pInfoStrBtn		= nullptr;	
	JButtonCtrl*	m_pInfoDexBtn		= nullptr;	
	JButtonCtrl*	m_pInfoIntBtn		= nullptr;	
	JButtonCtrl*	m_pInfoLukBtn		= nullptr;	
	// 슬롯, 인벤토리
	JSlot*			m_pSlot1 = nullptr;
	JSlot*			m_pSlot2 = nullptr;
	JSlot*			m_pSlot3 = nullptr;
	///
private:
	friend class ISingleton<UIManager>;
	UIManager() = default;
public:
	virtual ~UIManager() = default;
};

