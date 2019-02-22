#pragma once
#include "ISingleton.h"
#include "uiheader.h"


class UIManager : public ISingleton<UIManager>
{
public:
	// 로딩화면
	JPanel*			m_pUIRoot = nullptr;
	JTextCtrl*		m_pObjectText = nullptr;
	JTextCtrl*		m_pPercent = nullptr;
	JProgressBar*	m_pProgress = nullptr;
	// UI
	JTextCtrl*		m_TimerText			= nullptr;
	JPanel*			m_pStatePanel		= nullptr;
	JPanel*			m_FightPanel		= nullptr;
	JEditCtrl*		m_pChat				= nullptr;
	JPanel*			m_pMouseIcon		= nullptr;
	JPanel*			m_pRespawnEffect	= nullptr;
	JPanel*			m_pHitEffect		= nullptr;
	JPanel*			m_pGreenEffect		= nullptr;
	// 프로그래스
	JPanel*			m_pRespawn			= nullptr;
	JProgressBar*	m_pRespawnBar		= nullptr;
	JProgressBar*	m_pHpBar			= nullptr;	
	JProgressBar*	m_pMpBar			= nullptr;
	JProgressBar*	m_pLeftIcon			= nullptr;		
	JProgressBar*	m_pRightIcon		= nullptr;		

	JPanel*			m_pEnemyPanel		= nullptr;
	JProgressBar*	m_pEnemyHP			= nullptr;
	JTextCtrl*		m_pEnemyName		= nullptr;
	JTextCtrl*		m_pEnemyHPText		= nullptr;
	JProgressBar*	m_pExpProgress		= nullptr;
	// 옵션
	JPanel*			m_pOptionPanel		= nullptr;
	// 인포창
	JPanel*			m_pInfoPanel		= nullptr;
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
	// 인벤, 슬롯
	JInventory*		m_pInvenSlot			= nullptr;
	JPanel*			m_pInvenPanel			= nullptr;
	JTextCtrl*		m_pInvenMoney			= nullptr;
	JPanel*			m_pInvenWeapon			= nullptr;
	JPanel*			m_pInvenArmor			= nullptr;
	JPanel*			m_pInvenAcce1			= nullptr;
	JPanel*			m_pInvenAcce2			= nullptr;
	JTextCtrl*		m_pInvenInfoName		= nullptr;
	JTextCtrl*		m_pInvenInfoEffect		= nullptr;
	JTextCtrl*		m_pInvenInfoReinforce	= nullptr;

	JSlot*			m_pSlot1				= nullptr;
	JSlot*			m_pSlot2				= nullptr;
	JSlot*			m_pSlot3				= nullptr;
	// 타워
	JPanel*			m_pTowerPanel			= nullptr;
	JTextCtrl*		m_pTowerCurLevel		= nullptr;
	JTextCtrl*		m_pTowerCurAtkDamage	= nullptr;
	JTextCtrl*		m_pTowerCurAtkSpeed		= nullptr;
	JTextCtrl*		m_pTowerText1			= nullptr;

	JTextCtrl*		m_pTowerNextLevel		= nullptr;
	JTextCtrl*		m_pTowerNextAtkDamage	= nullptr;
	JTextCtrl*		m_pTowerNextAtkSpeed	= nullptr;
	JTextCtrl*		m_pTowerText2			= nullptr;
	JTextCtrl*		m_pTowerUpgrade			= nullptr;
	// 상점
	JPanel*			m_pShopPanel			= nullptr;
	JTextCtrl*		m_pShopItem0			= nullptr;
	JTextCtrl*		m_pShopItem1			= nullptr;
	JTextCtrl*		m_pShopItem2			= nullptr;
	JTextCtrl*		m_pShopItem3			= nullptr;
	JTextCtrl*		m_pShopItem4			= nullptr;
	JTextCtrl*		m_pShopItem5			= nullptr;
	JTextCtrl*		m_pShopItem6			= nullptr;
	JTextCtrl*		m_pShopItem7			= nullptr;
	// 대장간
	JPanel*			m_pSmithyPanel			= nullptr;
	JTextCtrl*		m_pSmithyBtnWeapon		= nullptr;
	JTextCtrl*		m_pSmithyBtnArmor		= nullptr;
	JTextCtrl*		m_pSmithyBtnAcce1		= nullptr;
	JTextCtrl*		m_pSmithyBtnAcce2		= nullptr;

	JTextCtrl*		m_pSmithyInfo1Weapon	= nullptr;
	JTextCtrl*		m_pSmithyInfo2Weapon	= nullptr;
	JTextCtrl*		m_pSmithyInfo1Armor		= nullptr;
	JTextCtrl*		m_pSmithyInfo2Armor		= nullptr;
	JTextCtrl*		m_pSmithyInfo1Acce1		= nullptr;
	JTextCtrl*		m_pSmithyInfo2Acce1		= nullptr;
	JTextCtrl*		m_pSmithyInfo1Acce2		= nullptr;
	JTextCtrl*		m_pSmithyInfo2Acce2		= nullptr;
	///
	JPanel*			m_pHelpTextPanel		= nullptr;
	JImageCtrl*		m_pXPush				= nullptr;
public:
	void			AddSlotItem(const wstring_view& itemName);
private:
	friend class ISingleton<UIManager>;
	UIManager() = default;
public:
	virtual ~UIManager() = default;
};

