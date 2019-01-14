#include "MainClass.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "LobbyScene.h"
#include "PacketManager.h"
#include "PlayerController.h"
#include "ime.h"
#include "../network/PPTCPIOCPServer.h"							//서버 클래스 정의.
#include "../network/PPTCPIOCPClient.h"							//서버 구동시 필요합니다. 싱글톤 객체.
#include "../network/PPRecvPacketPoolServer.h"					//클라이언트 클래스 정의.
#include "../network/PPRecvPacketPoolClient.h"					//클라이언트 구동시 필요합니다. 싱글톤 객체

MaxImporter*				MainClass::m_pParser = nullptr;
GameObject*					MainClass::m_pSkyBox = nullptr;
MainClass*				    MainClass::m_curScene = nullptr;
map<ESceneName, MainClass*> MainClass::m_SceneList;
GameObject*					MainClass::m_LoadingImage = nullptr;
GameObject*					MainClass::m_Icon = nullptr;
GameObject*					MainClass::m_Icon2 = nullptr;
GameObject*					MainClass::m_Icon3 = nullptr;
GameObject*					MainClass::m_Icon4 = nullptr;
bool						MainClass::m_isLoading = true;
///
GameObject*					MainClass::m_pBullet = nullptr;
list<GameObject*>			MainClass::m_BulletList;
GameObject*					MainClass::m_pEnemy = nullptr;
list<GameObject*>			MainClass::m_EnemyList;

XMap*						MainClass::m_pMap = nullptr;
XQuadTreeIndex*				MainClass::m_pMapTree = nullptr;
XMapImporter				MainClass::m_Importer;


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
	// IME
	ime::Get()->Set(m_hWnd);

	// 커서 감추기
	ShowCursor(FALSE);

	// 스카이 박스
	m_pSkyBox = new GameObject(L"SkyBox", { new SkySphere(20, L"SkySphere", L"CubeMap/grassenvmap1024.dds"), new CTransformer(Vector3::Zero, Quaternion::Right * 0.05f, Vector3::Zero) }, EObjType::Dummy);
	//m_pSkyBox = new GameObject(L"SkyBox", { new SkyBox(L"SkyBox", L"../../data/map/Texture/Sunny_01A_"), new CTransformer(Vector3::Zero, Quaternion::Right * 0.05f, Vector3::Zero) }, EObjType::Dummy);
	m_pSkyBox->SetScale(Vector3::One * 100);
	m_pSkyBox->isGlobal(true);

	// 로딩 이미지
	ObjectManager::Get().ReadSpriteScript();
	m_LoadingImage = new GameObject(L"Load", new PlaneUI(L"ui/logo_null.jpg"));
	m_LoadingImage->SetScale(Vector3::One * 5.0f);
	m_Icon = new GameObject(L"LoadIcon", new PlaneUI(L"PlayerBall"));
	m_Icon2 = new GameObject(L"LoadIcon2", new PlaneUI(L"ui/logo_g.jpg"));
	m_Icon4 = new GameObject(L"LoadIcon4", new PlaneUI(L"PlayerBomb"));
	
	Collider* pMyCollider = new Collider(0.5f);
	m_Icon3 = new GameObject(L"LoadIcon3", { new PlaneUI(L"Fairy"), pMyCollider }, EObjType::Image);
	m_Icon3->SetGravityScale(0.0f);
	m_Icon3->usePhysics(false);

	///
	static auto pBulletEvent = [](Collider* Me, Collider* You) {
		if (You->m_pParent->m_objType == EObjType::Object &&
			You->m_pParent->isEnable() &&
			Me->m_pParent->isEnable())
		{
			You->m_pParent->isEnable(false);
			Me->m_pParent->isEnable(false);
		}
	};
	static auto pEnemyEvent = [](Collider* Me, Collider* You) {
		if (You->m_pParent->m_objType == EObjType::Image &&
			You->m_pParent->isEnable() &&
			Me->m_pParent->isEnable())
		{
			You->m_pParent->isEnable(false);
			Me->m_pParent->isEnable(false);
		}
	};
	Collider* pCollider = new Collider(1.0f);
	ObjectManager::Get().PopCollider(pCollider);
	//pCollider->AddIgnoreList(pMyCollider);
	m_pBullet = new GameObject(L"FairyClash", { new PlaneUI(L"FairyClash"), pCollider, new CTransformer(Vector3::Up * 5.0f, Quaternion::Base) });
	m_pBullet->SetScale(Vector3::One * 0.15f);
	m_pBullet->SetGravityScale(0.0f);
	m_pBullet->usePhysics(false);
	pCollider->CollisionEvent = pBulletEvent;

	pCollider = new Collider(1.0f);
	ObjectManager::Get().PopCollider(pCollider);
	m_pEnemy = new GameObject(L"PlayerBomb", { new PlaneUI(L"PlayerBomb"), pCollider/*, new CTransformer(Vector3::Down, Quaternion::Base)*/ }, EObjType::Object);
	m_pEnemy->SetScale(Vector3::One * 0.7f);
	m_pEnemy->SetGravityScale(0.08f);
	m_pEnemy->usePhysics(false);
	pCollider->CollisionEvent = pEnemyEvent;


	// 씬 설정
	m_SceneList[ESceneName::Intro] = new IntroScene();
	m_SceneList[ESceneName::Main] = new GameScene();
	m_SceneList[ESceneName::Lobby] = new LobbyScene();
	///
	SetScene(ESceneName::Intro, false);
	return true;
}


bool MainClass::Frame() noexcept
{
	m_pSkyBox->Frame(Timer::SPF, Timer::AccumulateTime);
	m_curScene->Frame();

	//if (isHost)
	//{
		//SendMovedObject();
	//}
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
static const float LoadingSPF = 0.04f;
void MainClass::SetScene(const ESceneName& sceneName, const bool& useRelease) noexcept
{
	if (m_curScene == m_SceneList[sceneName])
		return;
	if (useRelease)
		m_curScene->Release();

	m_isLoading = true;
	if (m_Icon3 == nullptr)
		m_Icon3 = ObjectManager::Get().TakeObject(L"LoadIcon3", false);
	m_Icon->SetPosition(-3.2f, 0.0f, 0.0f);
	m_Icon2->SetPosition(-1.0f, 0.0f, 0.0f);
	m_Icon3->SetPosition(1.0f, 0.0f, 0.0f);
	m_Icon4->SetPosition(3.5f, 0.0f, 0.0f);

	m_curScene = m_SceneList[sceneName];
	// 쓰레드 실행
	std::thread initer(&MainClass::Init, m_SceneList[sceneName]);
	initer.detach();
	static bool useMiniGame = false;
	useMiniGame = false;
	while (m_isLoading)
	{
		Input::Get().Frame();
		if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
		{
			useMiniGame = true;
		}
		///
		DxManager::Get().PrevRender();
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
		}
		DxManager::Get().PostRender();
		this_thread::yield();
		this_thread::sleep_for(chrono::milliseconds(40));
	}
	for (auto& iter : m_BulletList)
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
	m_Icon3 = nullptr;
	// 쓰레드 대기
	//if (initer.joinable())
	//	initer.join();
}

void MainClass::LoadSceneInput() noexcept
{
	static float frameCount = 0.0f;
	frameCount += LoadingSPF;
	if(frameCount >= 0.25f)
	{
		frameCount -= 0.25f;
		m_EnemyList.push_back(m_pEnemy->clone());
		m_EnemyList.back()->SetPosition(RandomNormal() * 8.0f - 4.0f, 5.0f, 0.0f);
	}

	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_Icon3->Translate(Vector3::Up * LoadingSPF * 4.5f);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_Icon3->Translate(Vector3::Down * LoadingSPF * 4.5f);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_Icon3->Translate(Vector3::Left * LoadingSPF * 4.5f);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_Icon3->Translate(Vector3::Right * LoadingSPF * 4.5f);
	}
	if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
	{
		m_BulletList.push_back(m_pBullet->clone());
		m_BulletList.back()->SetPosition(m_Icon3->GetPosition());
	}
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






int ProcessServerPacket() {
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
			if (++p_SyncObjects.Count >= 45)
				break;
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
		static size_t strSize = 0;
		wstring objName = L"Guard";
		strSize = objName.size() * 2;
		strSize = strSize > 100 ? 100 : strSize;

		Packet_TakeObject p_TakeObject;
		p_TakeObject.KeyValue = ++PacketManager::Get().PlayerKeyCount;
		memcpy(p_TakeObject.ObjectName, objName.data(), strSize);
		p_TakeObject.MsgSize = (UCHAR)strSize;
		p_TakeObject.Position = { RandomNormal() * 1000.0f - 500.0f, 800.0f, RandomNormal() * 1000.0f - 500.0f };
		p_TakeObject.Rotation = Quaternion::Base;
		p_TakeObject.Scale = Vector3::One * 0.5f;

		//PacketManager::Get().SendPacket((char*)&p_TakeObject, (USHORT)(PS_TakeObject + strSize), PACKET_TakeObject);

		packetSend = packetRecv;
		memcpy(packetSend.m_Packet.m_Payload, (void*)&p_TakeObject, PS_TakeObject + strSize);
		//packetSend.m_socketSession = 0;
		packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_TakeObject;
		packetSend.m_Packet.m_Header.m_len  = (USHORT)(sizeof(Packet_TakeObject) + PACKET_HEADER_SIZE);
		PacketManager::Get().PacketHistory.push(packetSend);	// 패킷 기록

		pSender->Broadcast(packetSend);
		PacketManager::Get().InterceptPacket(packetSend.m_Packet.m_Header.m_type, packetSend.m_Packet.m_Payload);


	   //static Packet_Vector3 p_player;
	   //packetSend = packetRecv;
	   //
	   //p_player.KeyValue = ++PacketManager::Get().PlayerKeyCount;
	   //p_player.Vec3 = {RandomNormal() * 500.0f, 800.0f, RandomNormal() * 500.0f };
	   //memcpy(packetSend.m_Packet.m_Payload, (void*)&p_player, sizeof(Packet_Vector3));
	   //packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_ReqAddPlayer;
	   //packetSend.m_Packet.m_Header.m_len = (USHORT)(sizeof(Packet_Vector3) + PACKET_HEADER_SIZE);
	   //
	   //pSender->Broadcast(packetSend);
	   //PacketManager::Get().InterceptPacket(packetSend.m_Packet.m_Header.m_type, packetSend.m_Packet.m_Payload);
	   //PacketManager::Get().PacketHistory.push(packetSend);	// 패킷 기록

	   // 빙의 파트
		static Packet_PossessPlayer p_character;
		p_character.KeyValue = p_TakeObject.KeyValue;
		p_character.ECharacter = PlayerController::ECharacter::EGuard;
		memcpy(packetSend.m_Packet.m_Payload, (void*)&p_character, sizeof(Packet_PossessPlayer));
		packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_PossessPlayer;
		packetSend.m_Packet.m_Header.m_len = (USHORT)(sizeof(Packet_PossessPlayer) + PACKET_HEADER_SIZE);
		pSender->Send(packetSend);

	}	break;
	//case PACKET_TakeObject:
		//PacketManager::Get().PacketHistory.push(packetRecv);	// 패킷 기록
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

int ProcessClientPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//클라이언트 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	//std::wcout << "injected ProcessClientPacket()..." << std::endl;
	//PPRecvPacketPool에서 저장한 수신패킷을 하나 끄집어내서 처리합니다.
	PP::PPPacketForProcess RecvPacket = PP::PPRecvPacketPoolClient::GetInstance().front();
	PP::PPRecvPacketPoolClient::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (RecvPacket.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		//서버에게 문자열 패킷을 수신받을 때 처리하는 소스
		std::wstring wstrBuf;
		PP::PPPacketMessage* packetMsg = (PP::PPPacketMessage*)RecvPacket.m_Packet.m_Payload;
		wcharBuf = (wchar_t*)&packetMsg->m_charMessage;
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << wcharBuf;
		OutputDebugStringW(wstrBuf.c_str());
		break;
	}
	case PP::PPPacketType::TYPE_NOTICE_SESSION_EXIT: {
		std::wstring wstrBuf;
		PP::PPPacketNoticeSessionExit* packetRecvNotice = (PP::PPPacketNoticeSessionExit*)RecvPacket.m_Packet.m_Payload;
		wstrBuf.append(L"[공지] 탈주한 세션번호(소켓번호): ");
		wstrBuf.append(std::to_wstring(packetRecvNotice->m_socketSession));
		std::wcout << wstrBuf << std::endl;
	}	break;
	default:
	{
		// 미정의 패킷 처리
		PacketManager::Get().InterceptPacket(RecvPacket.m_Packet.m_Header.m_type, RecvPacket.m_Packet.m_Payload);
	}	break;
	}
	return 0;
}

int MainClass::StartupServer() {
	int iReturn = 0;
	iReturn = g_pServer->SetPortNumber(10000);				//서버의 포트 번호 지정
	iReturn = g_pServer->SetNumberOfThreads(2);				//IOCP 스레드 개수
	iReturn = g_pServer->SetFP(ProcessServerPacket);		//패킷을 처리할 함수 포인터 지정
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
	iReturn = g_pClient->SetHost("192.168.0."s + WCharToChar(PacketManager::Get().InputIP.c_str()));		// 서버의 IPv4
	iReturn = g_pClient->SetPortNumber(10000);				//서버의 포트 번호
	iReturn = g_pClient->SetNumberOfThreads(2);				//생성할 IOCP 스레드 개수
	iReturn = g_pClient->SetFP(ProcessClientPacket);		//패킷을 처리할 함수 포인터 지정
	iReturn = g_pClient->Startup();							//클라이언트 시동
	PacketManager::Get().pSender = g_pSender;
	if (iReturn == -1)
	{
		exit(0);
	}
	return 0;
}





//					윈도우 핸들,         호출한 윈도우 핸들
//  호출형식(언어)  ?                    보여줄 속성값
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPWSTR	  lpCmdLine, int   nCmdShow)
{
	// 윈도우 생성
	static MainClass mainWindow;
	mainWindow.SetClientRect(250, 250, 1024, 700);

	// 윈도우 설정
	if (mainWindow.SetWindow(hInstance, nCmdShow, L"MyGame"))
	{
		// 윈도우 가동
		mainWindow.GameRun();
	}

	return 0;
	lpCmdLine;	hPrevInstance;
}