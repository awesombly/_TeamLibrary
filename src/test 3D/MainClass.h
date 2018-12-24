#pragma once
//#pragma comment( lib, "_CoreLib D3D.lib")
#pragma comment( lib, "ACoreLibTEAM.lib")
#pragma comment( lib, "libppnetwork.lib")

#include "CoreDX.h"
///

#include "SkySphere.h"
#include "SkyBox.h"
#include "MaxImporter.h"
#include "ParticleSystem.h"
#include "Raycast.h"
#include "Light.h"
#include "PlaneUI.h"
#include "RLine.h"
#include "RSphere.h"
#include "RCube.h"
#include "HeightMap.h"
#include "CTransformer.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include "ColliderSphere.h"
#include "PlayerController.h"

#include "AHeroObj.h"
#include "JPanel.h"


// =====================================================================
// 카메라 조작(Shift 누르고)
// {
//		이동		: 마우스 좌클릭, 휠, WASD, QE
//		회전		: 마우스 우클릭, 방향키
//		리셋		: R
//		FOV 조절 : Z, X, C
// }
// 
// F1 : 와이어 프레임
// F2 : 노말값 확인
// F3 : PostEffect Off
// F4 : PostEffect On
//
// 숫자패드 1,2,3 : 씬 전환(IntroScene 외 배치된거 없음)
//
// =====================================================================
// Context, Device 등 DirectX에 관련된건 대부분 DxManager에 있습니다.
// 카메라가 필요하면 ObjectManager::Get().Cameras[ECamera::Main]->m_mat~ 식으로 접근할 수 있습니다.
//
// GameObject를 상속 받으시고 하위 클래스에서 GameObject::Frame(), GameObject::Render() 를 돌리시면 
// Transform에 있는 position, rotation, scale 값에 맞춰 행렬이 갱신됩니다.
// SetParent, CutParent로 부모를 설정 및 해제 합니다.
//
// =====================================================================
// MainClass에서 Core를 상속받고 전체 씬을 관리합니다.
// 각 씬들은 MainClass를 상속받아 관리하기 쉽도록 했습니다.
// 각 씬 별로 해당 씬에 필요한 객체를 정의하고 사용하는 식으로 합니다.
// 각 씬은 MainClass에서 static으로 선언된 변수에 접근할 수 있습니다.(static 아니면 에러뜸)
//
// =====================================================================

enum class ESceneName : char {
	Intro = 0,
	Main,
	Lobby,
};

// 전체 씬과 게임의 흐름을 관리
class MainClass : public Core 
{
public:
	static GameObject* m_pSkyBox;
	static map<ESceneName, MainClass*> m_SceneList;
	static MainClass* m_curScene;

	bool m_isFirstInit = true;
public:
	// 씬 설정
	void SetScene(const ESceneName& sceneName, const bool& useRelease = true) noexcept;
	void SendMovedObject()	noexcept;

	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;
public:
	MainClass() = default;
	~MainClass() = default;
public:
	
	int StartupServer();
	int StartupClient();
};

int ProcessServerPacket();
int ProcessClientPacket();