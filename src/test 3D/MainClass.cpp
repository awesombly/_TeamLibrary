#define _CRT_SECURE_NO_WARNINGS
#include "MainClass.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "AHeroObj.h"
#include "LobbyScene.h"
#include "PacketManager.h"
#include "PlayerController.h"
#include "EventManager.h"
#include "ime.h"
#include "JEventBind.h"
#include "UIManager.h"


MaxImporter*				MainClass::m_pParser = nullptr;
GameObject*					MainClass::m_pSkyBox = nullptr;
MainClass*				    MainClass::m_curScene = nullptr;
map<ESceneName::ESceneName, MainClass*> MainClass::m_SceneList;
//GameObject*					MainClass::m_LoadingImage = nullptr;
//GameObject*					MainClass::m_Icon = nullptr;
//GameObject*					MainClass::m_Icon2 = nullptr;
//GameObject*					MainClass::m_Icon3 = nullptr;
//GameObject*					MainClass::m_Icon4 = nullptr;
//
//GameObject*					MainClass::m_pBullet = nullptr;
//list<GameObject*>			MainClass::m_BulletList;
//GameObject*					MainClass::m_pEnemy = nullptr;
//list<GameObject*>			MainClass::m_EnemyList;

bool						MainClass::m_isLoading = true;
char						MainClass::m_loginCheck = 0;

std::string					MainClass::m_strHostIPv4;
int							MainClass::m_iMatchingStatus = 0;

#include "../network/PPTCPIOCPServer.h"							//서버 클래스 정의.
#include "../network/PPTCPIOCPClient.h"							//서버 구동시 필요합니다. 싱글톤 객체.
#include "../network/PPRecvPacketPoolServer.h"					//클라이언트 클래스 정의.
#include "../network/PPRecvPacketPoolClient.h"					//클라이언트 구동시 필요합니다. 싱글톤 객체
#include "../network/PPProtocolMatchingServer.h"		//매칭 서버용 패킷
PP::PPSender*		 g_pSender = PP::GetSender();		//동적 패킷 전송 객체
PP::PPTCPIOCPClient* g_pClient = PP::GetClient();
PP::PPTCPIOCPServer* g_pServer = PP::GetServer();		//동적 서버객체




void MainClass::MsgEvent(const MSG& _message) noexcept
{
	Input::Get().MsgEvent(_message);
	ime::Get()->WndProc(_message);
}

bool MainClass::Init() noexcept
{
	ErrorMessage(__FUNCTION__ + " -> Start."s);
	PacketManager().Get().PacketFunc = std::bind(&MainClass::ProcessServerPacket, this);
	// IME
	ime::Get()->Set(m_hWnd);
	// 커서 감추기
	ShowCursor(FALSE);
	// 씬 설정
	m_SceneList[ESceneName::Intro] = new IntroScene();
	m_SceneList[ESceneName::Main] = new GameScene();
	m_SceneList[ESceneName::Lobby] = new LobbyScene();
	///
	SoundManager::Get().SetMasterVolume(0.5f);
	//SoundManager::Get().Load("bgm_Intro_Theme.mp3", true);
	//SoundManager::Get().Play("bgm_Intro_Theme.mp3");
	// 로딩 이미지 등
	FirstInit();
	///
	SetScene(ESceneName::Intro, false);
	return true;
}


bool MainClass::Frame() noexcept
{
	m_pSkyBox->Frame(Timer::SPF, Timer::AccumulateTime);
	m_curScene->Frame();
	return true;
}

bool MainClass::Render() noexcept
{
	m_pSkyBox->Render(DxManager::GetDContext());
	m_curScene->Render();
	return true;
}

bool MainClass::Release() noexcept
{
	m_curScene->Release();
	return true;
}

// 씬 전환
static const float LoadingSPF = 0.03f;
void MainClass::SetScene(const ESceneName::ESceneName& sceneName, const bool& useRelease) noexcept
{
	ErrorMessage(__FUNCTION__ + " -> Start.\n"s);
	LoadClass::LoadingCount = 0;
	if (m_curScene == m_SceneList[sceneName])	
		return;
	if (useRelease)
		m_curScene->Release();
	m_curScene = m_SceneList[sceneName];
	m_isLoading = true;

	//if (m_Icon3 == nullptr)
	//	m_Icon3 = ObjectManager::Get().TakeObject(L"LoadIcon3", false);
	//m_Icon->SetPosition(-3.2f, 0.0f, 0.0f);
	//m_Icon2->SetPosition(-1.0f, 0.0f, 0.0f);
	//m_Icon3->SetPosition(1.0f, 0.0f, 0.0f);
	//m_Icon4->SetPosition(3.5f, 0.0f, 0.0f);
	//static bool useMiniGame = false;
	//useMiniGame = false;
	
	// Init 쓰레드
	std::thread initer(&MainClass::Init, m_curScene);
	//initer.detach();
	//m_curScene->Init();

	m_loadPercent = 0.0f;
	UIManager::Get().m_pProgress->SetValue(m_loadPercent, 1.0f, m_loadPercent);
	while (m_isLoading)
	{
		//std::unique_lock<mutex> lock(Timer::Instance->m_mutex);
		//Timer::Instance->m_FrameEvent.wait(lock);

		//Input::Get().Frame();
		MessageProcess();
		DxManager::Get().PrevRender();

		/*if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
		{
			useMiniGame = true;
		}
		
		m_LoadingImage->Frame(LoadingSPF, 0.0f);
		m_LoadingImage->Render(DxManager::GetDContext());
		m_Icon->Frame(LoadingSPF, 0.0f);
		m_Icon->Render(DxManager::GetDContext());
		m_Icon2->Frame(LoadingSPF, 0.0f);
		m_Icon2->Render(DxManager::GetDContext());
		m_Icon2->Rotate(Quaternion::Forward * 0.5f);
		m_Icon3->Frame(LoadingSPF, 0.0f);
		m_Icon3->Render(DxManager::GetDContext());
		m_Icon4->Frame(LoadingSPF, 0.0f);
		m_Icon4->Render(DxManager::GetDContext());
		if (useMiniGame)
		{
			LoadSceneInput();
		
			for (auto& iter : m_BulletList)
			{
				iter->Frame(LoadingSPF, 0.0f);
				iter->Render(DxManager::GetDContext());
			}
			for (auto& iter : m_EnemyList)
			{
				iter->Frame(LoadingSPF, 0.0f);
				iter->Render(DxManager::GetDContext());
			}
		}*/

		// 총 로딩 수
		static float totalLoading[ESceneName::size] = { 415.0f, 300.0f, 1648.0f };
		m_loadPercent = LoadClass::LoadingCount / totalLoading[sceneName];
		UIManager::Get().m_pPercent->SetString(LoadClass::LoadingString);
		UIManager::Get().m_pObjectText->SetString(to_wstring(m_loadPercent * 100.0f).substr(0, 5) + L"%");
		//OutputDebugString((LoadClass::LoadingString + L"\n").c_str());
		
		UIManager::Get().m_pUIRoot->Frame(LoadingSPF, Timer::AccumulateTime);
		UIManager::Get().m_pUIRoot->Render(DxManager::GetDContext());
		DxManager::Get().PostRender();

		
		//this_thread::yield();
		this_thread::sleep_for(chrono::milliseconds(30));
	}
	/*for (auto& iter : m_BulletList)
	{
		ObjectManager::Get().RemoveObject(iter);
	}
	for (auto& iter : m_EnemyList)
	{
		ObjectManager::Get().RemoveObject(iter);
	}
	m_BulletList.clear();
	m_EnemyList.clear();
	ObjectManager::Get().DisableObject(m_Icon3);
	m_Icon3 = nullptr;*/
	
	// 쓰레드 대기
	if (initer.joinable())
		initer.join();
	ErrorMessage(L"씬 로딩값 : " + to_wstring(LoadClass::LoadingCount));
	//ErrorMessage(__FUNCTION__ + " -> End."s);
	// 1초 대기
	DxManager::Get().PrevRender();
	UIManager::Get().m_pObjectText->SetString(L"100.0%");
	UIManager::Get().m_pUIRoot->Frame(LoadingSPF, Timer::AccumulateTime);
	UIManager::Get().m_pUIRoot->Render(DxManager::GetDContext());
	DxManager::Get().PostRender();
	this_thread::sleep_for(chrono::seconds(1));
}

void MainClass::LoadSceneInput() noexcept
{
	//static float frameCount = 0.0f;
	//frameCount += LoadingSPF;
	//if(frameCount >= 0.25f)
	//{
	//	frameCount -= 0.25f;
	//	m_EnemyList.push_back(m_pEnemy->clone());
	//	ObjectManager::Get().ProcessPostEvent();
	//	m_EnemyList.back()->SetPosition(RandomNormal() * 8.0f - 4.0f, 5.0f, 0.0f);
	//}

	//if (Input::GetKeyState('W') == EKeyState::HOLD)
	//{
	//	m_Icon3->Translate(Vector3::Up * LoadingSPF * 4.5f);
	//}
	//if (Input::GetKeyState('S') == EKeyState::HOLD)
	//{
	//	m_Icon3->Translate(Vector3::Down * LoadingSPF * 4.5f);
	//}
	//if (Input::GetKeyState('A') == EKeyState::HOLD)
	//{
	//	m_Icon3->Translate(Vector3::Left * LoadingSPF * 4.5f);
	//}
	//if (Input::GetKeyState('D') == EKeyState::HOLD)
	//{
	//	m_Icon3->Translate(Vector3::Right * LoadingSPF * 4.5f);
	//}
	//if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
	//{
	//	m_BulletList.push_back(m_pBullet->clone());
	//	m_BulletList.back()->SetPosition(m_Icon3->GetPosition());
	//}
}

//void MainClass::SendMovedObject() noexcept
//{
//	//static Packet_Vec3Quat f_Vec3Quat;
//	//for (auto& [key, iter] : ObjectManager::KeyObjects)
//	//{
//	//	if (iter->isMoved())
//	//	{
//	//		iter->isMoved(false);
//	//		// Position, Rotation 갱신
//	//		f_Vec3Quat.KeyValue = iter->m_keyValue;
//	//		f_Vec3Quat.Vec3 = iter->GetPosition();
//	//		f_Vec3Quat.Quat = iter->GetRotation();
//	//		PacketManager::Get().SendPacket((char*)&f_Vec3Quat, sizeof(Packet_Vec3Quat), PACKET_SetTransform);
//	//	}
//	//}
//}

bool MainClass::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		LoadUI();

		//// 로딩 이미지
		//m_LoadingImage = new GameObject(L"Load", new PlaneUI(L"ui/logo_null.jpg"));
		//m_LoadingImage->SetScale(Vector3::One * 5.0f);
		//m_Icon = new GameObject(L"LoadIcon", new PlaneUI(L"PlayerBall"));
		//m_Icon2 = new GameObject(L"LoadIcon2", new PlaneUI(L"ui/logo_g.jpg"));
		//m_Icon4 = new GameObject(L"LoadIcon4", new PlaneUI(L"PlayerBomb"));
		//
		//Collider* pCollider = new Collider(0.5f);
		//pCollider->m_eTag = ETag::Ally;
		//m_Icon3 = new GameObject(L"LoadIcon3", { new PlaneUI(L"Fairy"), pCollider }, EObjType::UI);
		//m_Icon3->SetGravityScale(0.0f);
		//m_Icon3->usePhysics(false);
		//
		//pCollider = new Collider(1.0f);
		//ObjectManager::Get().PopCollider(pCollider);
		//m_pBullet = new GameObject(L"FairyClash", { new PlaneUI(L"FairyClash"), pCollider, new CTransformer(Vector3::Up * 5.0f, Quaternion::Base) });
		//m_pBullet->SetScale(Vector3::One * 0.15f);
		//m_pBullet->SetGravityScale(0.0f);
		//m_pBullet->usePhysics(false);
		//pCollider->CollisionEvent = MyEvent::BulletHit;
		//pCollider->m_eTag = ETag::Dummy;
		//
		//pCollider = new Collider(1.0f);
		//ObjectManager::Get().PopCollider(pCollider);
		//m_pEnemy = new GameObject(L"PlayerBomb", { new PlaneUI(L"PlayerBomb"), pCollider, new CTransformer(Vector3::Down * 14.0f, Quaternion::Base) }, EObjType::Object);
		//m_pEnemy->SetScale(Vector3::One * 0.7f);
		//m_pEnemy->SetGravityScale(0.0f);
		//m_pEnemy->usePhysics(false);
		//pCollider->CollisionEvent = MyEvent::EnemyHit;
		//pCollider->m_eTag = ETag::Enemy;
		return true;
	}
	return false;
}

void MainClass::LoadUI() noexcept { 

	UIManager::Get().m_pUIRoot = new JPanel(L"UI_IntroRoot");
	UIManager::Get().m_pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/Loading", *UIManager::Get().m_pUIRoot);
	//PlayerController::Get().pUIManager = &UIManager::Get();

	UIManager::Get().m_pObjectText = (JTextCtrl*)UIManager::Get().m_pUIRoot->find_child(L"Loading_Object"); // object name
	UIManager::Get().m_pObjectText->SetString(L"");

	UIManager::Get().m_pPercent = (JTextCtrl*)UIManager::Get().m_pUIRoot->find_child(L"Loading_Percent"); // object name

	UIManager::Get().m_pProgress = (JProgressBar*)UIManager::Get().m_pUIRoot->find_child(L"Loading_Progress"); // object name
	UIManager::Get().m_pProgress->SetValue(m_loadPercent, 1.0f, m_loadPercent);

	//ObjectManager::Get().PushObject(m_pUIRoot);
	UI::LoadingEvent(UIManager::Get().m_pUIRoot);
};

void MainClass::StartToHost()
{
	g_pClient->Release();
}
void MainClass::StartToGuest()
{
	g_pClient->Release();
}

int MainClass::ProcessServerPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//서버 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	//std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv;
	PP::PPPacketForProcess packetSend;
	//IOCP 스레드에서 넣엇던 패킷을 담은 패킷풀 접근
	packetRecv = PP::PPRecvPacketPoolServer::GetInstance().front();
	//패킷풀 맨 앞 pop()
	PP::PPRecvPacketPoolServer::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (packetRecv.m_Packet.m_Header.m_type) {
		//클라이언트에게 문자열 패킷을 수신받을 때 처리하는 소스
	case PP::PPPacketType::TYPE_STRING: {
		std::wstring wstrBuf;
		PP::PPPacketMessage* ppacketRecvMsg = (PP::PPPacketMessage*)packetRecv.m_Packet.m_Payload;
		PP::PPPacketMessage packetSendMsg = {};
		int iSizeOfpakcetSendMsg;

		//패킷에서 꺼낸 문자열을 서버의 콘솔창으로 출력하는 소스
		wcharBuf = (wchar_t*)&ppacketRecvMsg->m_charMessage;
		wstrBuf = std::to_wstring(packetRecv.m_socketSession);
		wstrBuf.append(L" socket Broadcasting :");
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << packetRecv.m_socketSession << L" socket: " << wcharBuf << std::endl;
		OutputDebugStringW(wstrBuf.c_str());

		//구조체 packetSendMsg 작성
		iSizeOfpakcetSendMsg = (int)wstrBuf.size() * 2;
		memcpy(packetSendMsg.m_charMessage, wstrBuf.c_str(), iSizeOfpakcetSendMsg);
		//보낼 패킷 작성
		packetSend.m_socketSession = packetRecv.m_socketSession;							//PPPacketForProcess::m_socketSession으로 패킷 수신자 또는 송신자를 지정할 수 있다.
		//packetSend.m_SendMode = PP::PPSendMode::SEND;										//현재 아무 기능에 관여하지 않는 변수다.
		memcpy(packetSend.m_Packet.m_Payload,												//패킷 적재부 작성
			(void*)&packetSendMsg, iSizeOfpakcetSendMsg);									//패킷 적재부에 memcpy로 적재할 구조체를 deep copy해서 입력하면 된다.		
		packetSend.m_Packet.m_Header.m_len = (USHORT)(PACKET_HEADER_SIZE + iSizeOfpakcetSendMsg);		//패킷 헤더길이 4바이트 + 적재부 길이를 합친 총 길이
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_STRING;				//패킷 타입. 열거형 변수 PP::PPPacketType에 정의되어있다.

		//pSender->Send(packetSend);
		pSender->Broadcast(packetSend);														//PPSessionManager에 있는 모든 세션들을 순회하여 send를 실시함.
		break;
	}
	case PP::PPPacketType::TYPE_REQ_SOCKET: {
		PP::PPPacketNoticeSession packetSession = {};
		int iSizeOfPayload = sizeof(packetSession);
		packetSession.m_socketSession = packetRecv.m_socketSession;
		packetSend.m_socketSession = packetRecv.m_socketSession;
		packetSend.m_Mode = PP::PPPacketMode::SEND;
		memcpy(packetSend.m_Packet.m_Payload,												//패킷 적재부 작성
			(void*)&packetSession, iSizeOfPayload);									//패킷 적재부에 memcpy로 적재할 구조체를 deep copy해서 입력하면 된다.		
		packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizeOfPayload;		//패킷 헤더길이 4바이트 + 적재부 길이를 합친 총 길이
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_ACK_SOCKET;

		pSender->Send(packetSend);
		break;
	}
	case PACKET_ReqSyncSpawns:
	{
		packetSend = packetRecv;
		while (!PacketManager::Get().PacketHistory.empty())
		{
			packetSend = PacketManager::Get().PacketHistory.front();
			packetSend.m_socketSession = packetRecv.m_socketSession;
			pSender->Send(packetSend);
			PacketManager::Get().PacketHistory.pop();
		}

		// 동기화 요청
		packetSend = packetRecv;

		static Packet_SyncObjects p_SyncObjects;
		static list<UINT> deleteKeys;
		p_SyncObjects.Count = (WCHAR)-1;
		//p_SyncObjects.KeyValue = (UINT)-1;
		for (auto&[key, iter] : ObjectManager::KeyObjects)
		{
			if (key < 1000)					 continue;
			if (++p_SyncObjects.Count >= 45) break;
			if (iter == nullptr)
			{
				deleteKeys.push_back(key);
				continue;
			}
			p_SyncObjects.Data[p_SyncObjects.Count].KeyValue = key;
			p_SyncObjects.Data[p_SyncObjects.Count].Position = iter->GetPosition();
			p_SyncObjects.Data[p_SyncObjects.Count].Rotation = iter->GetRotation();
			if(iter->m_pPhysics != nullptr)
				p_SyncObjects.Data[p_SyncObjects.Count].Force = iter->GetForce();
		}
		memcpy(packetSend.m_Packet.m_Payload, (void*)&p_SyncObjects, PS_SyncObjects);
		memcpy(((char*)&packetSend.m_Packet.m_Payload + PS_SyncObjects), ((char*)&p_SyncObjects + PS_SyncObjects), p_SyncObjects.Count * sizeof(Packet_SyncTransform));
		packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_SyncObjects;
		packetSend.m_Packet.m_Header.m_len = (USHORT)(p_SyncObjects.Count * sizeof(Packet_SyncTransform) + PS_SyncObjects + PACKET_HEADER_SIZE);
		pSender->Send(packetSend);

		// null 키 삭제
		if (!deleteKeys.empty())
		{
			for (auto& iter : deleteKeys)
			{
				ObjectManager::KeyObjects.erase(iter);
			}
			deleteKeys.clear();
		}
	}	break;
	case PACKET_ReqSync:
	{
		// 동기화 요청
		packetSend = packetRecv;

		static Packet_SyncObjects p_SyncObjects;
		static list<UINT> deleteKeys;
		p_SyncObjects.Count = (WCHAR)-1;
		//p_SyncObjects.KeyValue = (UINT)-1;
		for (auto&[key, iter] : ObjectManager::KeyObjects)
		{
			if (key < 1000)					 continue;
			if (++p_SyncObjects.Count >= 45) break;
			if (iter == nullptr)
			{
				deleteKeys.push_back(key);
				continue;
			}
			p_SyncObjects.Data[p_SyncObjects.Count].KeyValue = key;
			p_SyncObjects.Data[p_SyncObjects.Count].Position = iter->GetPosition();
			p_SyncObjects.Data[p_SyncObjects.Count].Rotation = iter->GetRotation();
			if (iter->m_pPhysics != nullptr)
				p_SyncObjects.Data[p_SyncObjects.Count].Force = iter->GetForce();
		}
		memcpy(packetSend.m_Packet.m_Payload, (void*)&p_SyncObjects, PS_SyncObjects);
		memcpy(((char*)&packetSend.m_Packet.m_Payload + PS_SyncObjects), ((char*)&p_SyncObjects + PS_SyncObjects), p_SyncObjects.Count * sizeof(Packet_SyncTransform));
		packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_SyncObjects;
		packetSend.m_Packet.m_Header.m_len = (USHORT)(p_SyncObjects.Count * sizeof(Packet_SyncTransform) + PS_SyncObjects + PACKET_HEADER_SIZE);
		pSender->Send(packetSend);
		
		// null 키 삭제
		if (!deleteKeys.empty())
		{
			for (auto& iter : deleteKeys)
			{
				ObjectManager::KeyObjects.erase(iter);
			}
			deleteKeys.clear();
		}
	}	break;
	case PACKET_ReqAddPlayer:
	{
		packetSend = packetRecv;

		static Packet_ReqAddPlayer p_ReqAddPlayer;
		memcpy((char*)&p_ReqAddPlayer, (char*)packetSend.m_Packet.m_Payload, sizeof(Packet_ReqAddPlayer));
		
		wstring objName;
		// 생성 파트
		auto spawnPos = PlayerController::Get().m_CarpetPos[(int)(RandomNormal() * 4.0f)] + Vector3::Up * 60.0f;
		switch(p_ReqAddPlayer.ECharacter)
		{
		 case PlayerController::ECharacter::EArcher:
		 {
			 PacketManager::Get().SendTakeObject(L"Archer", packetSend.m_socketSession, 1, 1.0f + PacketManager::Get().UserList[packetSend.m_socketSession]->StatStr * 0.2f,
				 0.2f, 0.0f, spawnPos, Vector3::Zero);
		 }	break;
		 case PlayerController::ECharacter::EMage:
		 {
			 PacketManager::Get().SendTakeObject(L"Mage", packetSend.m_socketSession, 1, 1.0f + PacketManager::Get().UserList[packetSend.m_socketSession]->StatStr * 0.2f,
				 0.2f, 0.0f, spawnPos, Vector3::Zero);
		 }	break;
		 case PlayerController::ECharacter::EGuard:
		 default:
		 {
			 PacketManager::Get().SendTakeObject(L"Paladin", packetSend.m_socketSession, 1, 1.0f + PacketManager::Get().UserList[packetSend.m_socketSession]->StatStr * 0.2f,
				 0.25f, 0.0f, spawnPos, Vector3::Zero);
		 }	break;
		}

		//memcpy(packetSend.m_Packet.m_Payload, (void*)&p_TakeObject, PS_TakeObject + strSize);
		////packetSend.m_socketSession = 0;
		//packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_TakeObject;
		//packetSend.m_Packet.m_Header.m_len  = (USHORT)(sizeof(Packet_TakeObject) + PACKET_HEADER_SIZE);
		//PacketManager::Get().PacketHistory.push(packetSend);	// 패킷 기록
		//
		//pSender->Broadcast(packetSend);
		//PacketManager::Get().InterceptPacket(packetSend.m_Packet.m_Header.m_type, packetSend.m_Packet.m_Payload);

	    // 빙의 파트
		static Packet_PossessPlayer p_character;
		p_character.KeyValue = PacketManager::Get().PlayerKeyCount;
		p_character.ECharacter = p_ReqAddPlayer.ECharacter;
		memcpy(packetSend.m_Packet.m_Payload, (void*)&p_character, sizeof(Packet_PossessPlayer));
		packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_PossessPlayer;
		packetSend.m_Packet.m_Header.m_len = (USHORT)(sizeof(Packet_PossessPlayer) + PACKET_HEADER_SIZE);
		if(packetSend.m_socketSession == 0)
			PacketManager::Get().InterceptPacket(packetSend.m_Packet.m_Header.m_type, packetSend.m_Packet.m_Payload);
		else
			pSender->Send(packetSend);
	}	break;
	default:
	{
		// 미정의 패킷 처리(브로드캐스팅)
		packetSend = packetRecv;
		pSender->Broadcast(packetSend);
		PacketManager::Get().InterceptPacket(packetSend.m_Packet.m_Header.m_type, packetSend.m_Packet.m_Payload);
	}	break;
	}
	return 0;
}

int MainClass::ProcessClientPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//클라이언트 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	//std::wcout << "injected ProcessClientPacket()..." << std::endl;
	//PPRecvPacketPool에서 저장한 수신패킷을 하나 끄집어내서 처리합니다.
	PP::PPPacketForProcess packetRecv = PP::PPRecvPacketPoolClient::GetInstance().front();
	PP::PPRecvPacketPoolClient::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (packetRecv.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		//서버에게 문자열 패킷을 수신받을 때 처리하는 소스
		std::wstring wstrBuf;
		PP::PPPacketMessage* packetMsg = (PP::PPPacketMessage*)packetRecv.m_Packet.m_Payload;
		wcharBuf = (wchar_t*)&packetMsg->m_charMessage;
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << wcharBuf;
		OutputDebugStringW(wstrBuf.c_str());
		break;
	}
	case PP::PPPacketType::TYPE_NOTICE_SESSION_EXIT: {
		std::wstring wstrBuf;
		PP::PPPacketNoticeSessionExit* packetRecvNotice = (PP::PPPacketNoticeSessionExit*)packetRecv.m_Packet.m_Payload;
		wstrBuf.append(L"[공지] 탈주한 세션번호(소켓번호): ");
		wstrBuf.append(std::to_wstring(packetRecvNotice->m_socketSession));
		std::wcout << wstrBuf << std::endl;
		break;
	}
	case PP::PPPacketType::TYPE_ACK_SOCKET: {
		PP::PPPacketNoticeSession* packetSession = (PP::PPPacketNoticeSession*)packetRecv.m_Packet.m_Payload;
		std::wstring wstrSession = std::to_wstring(packetSession->m_socketSession);
		wstrSession.append(L"\n");
		std::wcout << wstrSession << std::endl;
		OutputDebugStringW(wstrSession.c_str());
		PacketManager::Get().pMyInfo->UserSocket = atoi(WCharToChar(wstrSession.c_str()));
		break;
	}
	case PP::PPAdditionalPacketType::TYPE_ACK_MATCHING_HOST: {
		std::wcout << L"매칭(호스트) 요청 허가됨" << std::endl;
		m_iMatchingStatus = 1;
		//이 패킷을 받은 후 5초 뒤에 클라를 끄고 새로 서버를 구동하면 됨
		break;
	}
	case PP::PPAdditionalPacketType::TYPE_ACK_MATCHING_GUEST: {
		std::wcout << L"매칭(게스트) 요청 허가됨" << std::endl;
		m_iMatchingStatus = 2;
		PP::PPPacketAckMatchingGuest* packetMatching = (PP::PPPacketAckMatchingGuest*)packetRecv.m_Packet.m_Payload;
		m_strHostIPv4 = packetMatching->charHostAddress; //게스트의 IPv4
		//이 패킷을 받은 후 5초 뒤에 클라를 끄로 새로 클라를 구동하면 됨
		break;
	}
	case PP::PPAdditionalPacketType::TYPE_ACK_SIGN_UP: {
		PP::PPPacketAckSignUp* ppacketSignUp = (PP::PPPacketAckSignUp*)packetRecv.m_Packet.m_Payload;
		switch (ppacketSignUp->m_sReturn) {
		case 0: {
			MessageBoxW(0, L"가입성공.", nullptr, 0);
			break;
		}
		default: {
			MessageBoxW(0, L"가입실패.", nullptr, 0);
			break;
		}
		}
		break;
	}
	case PP::PPAdditionalPacketType::TYPE_ACK_UPDATE_USERNAME: {
		PP::PPPacketAckSignUp* ppacketSignUp = (PP::PPPacketAckSignUp*)packetRecv.m_Packet.m_Payload;
		switch (ppacketSignUp->m_sReturn) {
		case 0: {
			MessageBoxW(0, L"계정명 변경신청 성공.", nullptr, 0);
			break;
		}
		default: {
			MessageBoxW(0, L"계정명 변경신청 실패.", nullptr, 0);
			break;
		}
		}
		break;
	}
	case PP::PPAdditionalPacketType::TYPE_ACK_SIGN_IN: {
		PP::PPPacketAckSignIn* ppacketSignIn = (PP::PPPacketAckSignIn*)packetRecv.m_Packet.m_Payload;
		switch (ppacketSignIn->m_sReturn) {
		case 0: {
			//MessageBoxW(0, L"로그인 성공.", nullptr, 0);
			m_loginCheck = 1;
			break;
		}
		default: {
			//MessageBoxW(0, L"계정명과 비밀번호를 확인해주세요.", nullptr, 0);
			m_loginCheck = -1;
			break;
		}
		}
		break;
	}
	default:
	{
		// 미정의 패킷 처리
		PacketManager::Get().InterceptPacket(packetRecv.m_Packet.m_Header.m_type, packetRecv.m_Packet.m_Payload);
	}	break;
	}
	return 0;
}

int MainClass::StartupServer() {
	int iReturn = 0;
	iReturn = g_pServer->SetPortNumber(10000);				//서버의 포트 번호 지정
	iReturn = g_pServer->SetNumberOfThreads(2);				//IOCP 스레드 개수
	iReturn = g_pServer->SetFP(std::bind(&MainClass::ProcessServerPacket, this));		//패킷을 처리할 함수 포인터 지정
	iReturn = g_pServer->Startup();							//서버 시동
	PacketManager::Get().pSender = g_pSender;
	if (iReturn == -1)
	{
		exit(0);
	}
	return 0;
}
int MainClass::StartupClient() {
	int iReturn = 0;

	//iReturn = g_pClient->SetHost("127.0.0.1");				//서버의 IPv4
	//iReturn = g_pClient->SetHost("192.168.0."s + WCharToChar(PacketManager::Get().InputIP.c_str()));		// 서버의 IPv4
	iReturn = g_pClient->SetHost(m_strHostIPv4);			// 서버의 IPv4
	
	iReturn = g_pClient->SetPortNumber(10000);				//서버의 포트 번호
	iReturn = g_pClient->SetNumberOfThreads(2);				//생성할 IOCP 스레드 개수
	iReturn = g_pClient->SetFP(std::bind(&MainClass::ProcessClientPacket, this));		//패킷을 처리할 함수 포인터 지정
	iReturn = g_pClient->Startup();							//클라이언트 시동
	PacketManager::Get().pSender = g_pSender;
	if (iReturn == -1)
	{
		exit(0);
	}
	// 유저 소켓 입력
	g_pClient->ReqSocketNumber();
	return 0;
}
int MainClass::ConnectMatchingServer() {
	int iReturn = 0;
	std::string strIPv4;
	std::ifstream ifMatching;
	ifMatching.open("..\\..\\data\\script\\network.txt");
	ifMatching >> strIPv4;
	ifMatching.close();

	iReturn = g_pClient->SetHost(strIPv4);				//서버의 IPv4
	iReturn = g_pClient->SetPortNumber(10001);				//서버의 포트 번호
	iReturn = g_pClient->SetNumberOfThreads(1);				//생성할 IOCP 스레드 개수
	iReturn = g_pClient->SetFP(std::bind(&MainClass::ProcessClientPacket, this));		//패킷을 처리할 함수 포인터 지정
	iReturn = g_pClient->Startup();							//클라이언트 시동
	PacketManager::Get().pSender = g_pSender;
	if (iReturn != 0) 	{
		return iReturn;
	}
	return 0;
}

int MainClass::RequestMatch(const short & playerCnt) {
	PP::PPPacketForProcess packetSend = {};
	PP::PPPacketReqMatching packetMatching = {};
	int iPayloadSize = sizeof(packetMatching);
	packetMatching.iMaximumPlayer = playerCnt;
	packetSend.m_Mode = PP::PPPacketMode::SEND;
	packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PP::PPAdditionalPacketType::TYPE_REQ_MATCHING;
	packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iPayloadSize;
	memcpy(packetSend.m_Packet.m_Payload, &packetMatching, iPayloadSize);
	g_pClient->m_Sender.SendToServer(packetSend);

	std::thread initer(&MainClass::MatchingCheck, m_curScene);
	initer.detach();
	return 0;
}

int MainClass::RequestSignUp(const wchar_t * wcharUsername, const wchar_t * wcharPassword)
{
	int iSizePayload = 0;
	PP::PPPacketForProcess packetSend;
	PP::PPPacketReqSignUp packetSignUp = {};
	wcscpy(packetSignUp.m_wcharUsername, wcharUsername);
	wcscpy(packetSignUp.m_wcharPassword, wcharPassword);
	iSizePayload = sizeof(packetSignUp);

	packetSend.m_Mode = PP::PPPacketMode::SEND;
	packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PP::PPAdditionalPacketType::TYPE_REQ_SIGN_UP;
	packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizePayload;
	memcpy(packetSend.m_Packet.m_Payload, &packetSignUp, iSizePayload);

	g_pClient->m_Sender.SendToServer(packetSend);
	return 0;
}

int MainClass::RequestSignIn(const wchar_t * wcharUsername, const wchar_t * wcharPassword) {
	int iSizePayload = 0;
	PP::PPPacketForProcess packetSend;
	PP::PPPacketReqSignIn packetSignIn = {}; 
	wcscpy(packetSignIn.m_wcharUsername, wcharUsername);
	wcscpy(packetSignIn.m_wcharPassword, wcharPassword);
	iSizePayload = sizeof(packetSignIn);

	packetSend.m_Mode = PP::PPPacketMode::SEND;
	packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PP::PPAdditionalPacketType::TYPE_REQ_SIGN_IN;
	packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizePayload;
	memcpy(packetSend.m_Packet.m_Payload, &packetSignIn, iSizePayload);

	g_pClient->m_Sender.SendToServer(packetSend);
	return 0;
}

int MainClass::RequestUpdateUsername(const wchar_t * wcharUsername, const wchar_t * wcharNewUsername)
{
	int iSizePayload = 0;
	PP::PPPacketForProcess packetSend;
	PP::PPPacketReqUpdateUsername packetUpdateUsername = {};
	wcscpy(packetUpdateUsername.m_wcharUsername, wcharUsername);
	wcscpy(packetUpdateUsername.m_wcharNewUsername, wcharNewUsername);
	iSizePayload = sizeof(packetUpdateUsername);

	packetSend.m_Mode = PP::PPPacketMode::SEND;
	packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PP::PPAdditionalPacketType::TYPE_REQ_UPDATE_USERNAME;
	packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizePayload;
	memcpy(packetSend.m_Packet.m_Payload, &packetUpdateUsername, iSizePayload);

	g_pClient->m_Sender.SendToServer(packetSend);
	return 0;
}

void MainClass::MatchingCheck()
{
	// 매칭 완료
	while (true)
	{
		if (m_iMatchingStatus != 0) {
			if (!m_isMatching)
			{
				m_isMatching = true;
				OutputDebugStringW(L"자동매칭 성립, 5초 뒤에 시작합니다\n");
			}

			static float startFrame = 0.0f;
			startFrame += 0.05f;
			this_thread::sleep_for(chrono::milliseconds(50));
			if (startFrame >= 5.0f)
			{
				g_pClient->Release();
				switch (m_iMatchingStatus) {
				case 1: {
					OutputDebugStringW(L"호스트로 시작합니다.\n");
					m_iMatchingStatus = 0;

					StartToHost();
					break;
				}
				case 2: {
					std::string strTemp;
					strTemp.append("IP: ");
					strTemp.append(m_strHostIPv4);
					OutputDebugStringW(L"게스트로 시작합니다.\n");
					OutputDebugStringA(m_strHostIPv4.c_str());
					m_iMatchingStatus = 0;

					StartToGuest();
					break;
				}
				default: {
					break;
				}
				}
				break;
			}
		}
	}
}




//					윈도우 핸들,         호출한 윈도우 핸들
//  호출형식(언어)  ?                    보여줄 속성값
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPWSTR	  lpCmdLine, int   nCmdShow)
{
	// 윈도우 생성
	static MainClass mainWindow;
	D3DXVECTOR4 vScreen;
	std::wifstream readStream(L"../../screen.txt");
	readStream >> vScreen.x >> vScreen.y >> vScreen.z >> vScreen.w;
	mainWindow.SetClientRect(vScreen.x, vScreen.y, vScreen.z, vScreen.w);
	//mainWindow.SetClientRect(250, 250, 1024, 700);

	if (!readStream.is_open())
	{
		ErrorMessage(__FUNCTION__ + " -> Read File Error!"s);
		return false;
	}

	// 윈도우 설정
	if (mainWindow.SetWindow(hInstance, nCmdShow, L"MyGame"))
	{
		// 윈도우 가동
		ErrorMessage("\n\n"s + __TIME__ + " -> Game Run Start.");
		mainWindow.GameRun();
	}

	return 0;
	lpCmdLine;	hPrevInstance;
}