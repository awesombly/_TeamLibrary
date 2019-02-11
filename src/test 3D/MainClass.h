#pragma once
//#ifdef NDEBUG
//	#ifdef _DEBUG
//		#undef _DEBUG
//	#endif
//#endif
#ifdef _DEBUG
#pragma comment( lib, "ACoreLibTEAM_d.lib")
#pragma comment( lib, "libppnetwork_d.lib")
#pragma comment( lib, "UIToolLib_d.lib")
#else
#pragma comment( lib, "ACoreLibTEAM.lib")
#pragma comment( lib, "libppnetwork.lib")
#pragma comment( lib, "UIToolLib.lib")
#endif
///
#include "CoreDX.h"
///
#include "MaxImporter.h"
#include "SkySphere.h"
#include "ParticleSystem.h"
#include "Raycast.h"
#include "Light.h"
#include "PlaneUI.h"
#include "RLine.h"
#include "RSphere.h"
#include "RCube.h"
#include "CTransformer.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include "HeightMap.h"
///
#include "AHeroObj.h"
#include "AMapMgr.h"
///
//#include "XCamera.h"
#include "XQuadTreeIndex.h"
#include "XMapImporter.h"
///



enum class ESceneName : char {
	Intro = 0,
	Main,
	Lobby,
};

// 전체 씬과 게임의 흐름을 관리
class MainClass : public Core 
{
private:
	static MainClass* m_curScene;
	static map<ESceneName, MainClass*> m_SceneList;
	// 로딩
	static GameObject* m_LoadingImage;
	static GameObject* m_Icon;
	static GameObject* m_Icon2;
	static GameObject* m_Icon3;
	static GameObject* m_Icon4;
	//
	static GameObject*				m_pBullet;
	static list<GameObject*>		m_BulletList;
	static GameObject*				m_pEnemy;
	static list<GameObject*>		m_EnemyList;

	bool					m_isMatching = false;
public:
	static GameObject* m_pSkyBox;

	bool					m_isFirstInit = true;
	static bool				m_isLoading;
	static char				m_loginCheck;
	static MaxImporter*		m_pParser;
	///
	static XMap*			m_pMap;						// 맵
	static XQuadTreeIndex*	m_pMapTree;					// 맵->공간분할, LOD시 수정 예정
public:
	static int m_iMatchingStatus;
	static std::string m_strHostIPv4;
public:
	void MsgEvent(const MSG& _message)	noexcept;
	// 씬 설정
	void SetScene(const ESceneName& sceneName, const bool& useRelease = true) noexcept;
	void LoadSceneInput()	noexcept;
	//void SendMovedObject()	noexcept;

	virtual void StartToHost();
	virtual void StartToGuest();
	void MatchingCheck();

	virtual void LoadUI()				noexcept { return; };
	virtual bool FirstInit()			noexcept;

	virtual bool Init()					noexcept override;
	virtual bool Frame()				noexcept override;
	virtual bool Render()				noexcept override;
	virtual bool Release()				noexcept override;
public:
	MainClass() = default;
	~MainClass() = default;
public:
	int ProcessServerPacket();
	int ProcessClientPacket();
	
	int StartupServer();
	int StartupClient();
	int ConnectMatchingServer();
	int RequestMatch(const short& playerCnt);
	int RequestSignUp(const wchar_t* wcharUsername, const wchar_t* wcharPassword);
	int RequestSignIn(const wchar_t* wcharUsername, const wchar_t* wcharPassword);
	int RequestUpdateUsername(const wchar_t* wcharUsername, const wchar_t* wcharNewUsername);
};