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

#include "CoreDX.h"
///
#include "MaxImporter.h"
#include "SkySphere.h"
#include "SkyBox.h"
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

#include "AHeroObj.h"
#include "JPanel.h"

//#include "XCamera.h"
#include "XQuadTreeIndex.h"
#include "XMapImporter.h"


// =====================================================================
// ī�޶� ����(Shift ������)
// {
//		�̵�		: ���콺 ��Ŭ��, ��, WASD, QE
//		ȸ��		: ���콺 ��Ŭ��, ����Ű
//		����		: R
//		FOV ���� : Z, X, C
// }
// 
// F1 : ���̾� ������
// F2 : �븻�� Ȯ��
// F3 : PostEffect Off
// F4 : PostEffect On
//
// �����е� 1,2,3 : �� ��ȯ(IntroScene �� ��ġ�Ȱ� ����)
//
// =====================================================================
// Context, Device �� DirectX�� ���õȰ� ��κ� DxManager�� �ֽ��ϴ�.
// ī�޶� �ʿ��ϸ� ObjectManager::Get().Cameras[ECamera::Main]->m_mat~ ������ ������ �� �ֽ��ϴ�.
//
// GameObject�� ��� �����ð� ���� Ŭ�������� GameObject::Frame(), GameObject::Render() �� �����ø� 
// Transform�� �ִ� position, rotation, scale ���� ���� ����� ���ŵ˴ϴ�.
// SetParent, CutParent�� �θ� ���� �� ���� �մϴ�.
//
// =====================================================================
// MainClass���� Core�� ��ӹް� ��ü ���� �����մϴ�.
// �� ������ MainClass�� ��ӹ޾� �����ϱ� ������ �߽��ϴ�.
// �� �� ���� �ش� ���� �ʿ��� ��ü�� �����ϰ� ����ϴ� ������ �մϴ�.
// �� ���� MainClass���� static���� ����� ������ ������ �� �ֽ��ϴ�.(static �ƴϸ� ������)
//
// =====================================================================

enum class ESceneName : char {
	Intro = 0,
	Main,
	Lobby,
};

// ��ü ���� ������ �帧�� ����
class MainClass : public Core 
{
private:
	static MainClass* m_curScene;
	// �ε�
	static GameObject* m_LoadingImage;
	static GameObject* m_Icon;
	static GameObject* m_Icon2;
	static GameObject* m_Icon3;
	static GameObject* m_Icon4;
public:
	//static MaxImporter* m_pParser;
	static GameObject* m_pSkyBox;
	static map<ESceneName, MainClass*> m_SceneList;

	static bool m_isLoading;
	bool		m_isFirstInit = true;
	///
	static XMap*			m_pMap;						// ��
	static XQuadTreeIndex*	m_pMapTree;					// ��->��������, LOD�� ���� ����
	static XMapImporter		m_Importer;					// ���̺� ������ �δ�
public:
	// �� ����
	void SetScene(const ESceneName& sceneName, const bool& useRelease = true) noexcept;

	void MsgEvent(const MSG& _message)	noexcept;
	void SendMovedObject()	noexcept;
	
	virtual bool FirstInit()			noexcept { return true; };
	virtual bool Init()					noexcept override;
	virtual bool Frame()				noexcept override;
	virtual bool Render()				noexcept override;
	virtual bool Release()				noexcept override;
public:
	MainClass() = default;
	~MainClass() = default;
public:
	
	int StartupServer();
	int StartupClient();
};

int ProcessServerPacket();
int ProcessClientPacket();