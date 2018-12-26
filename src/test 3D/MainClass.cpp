#include "MainClass.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "LobbyScene.h"
#include "PacketManager.h"

#include "../network/PPTCPIOCPServer.h"							//서버 클래스 정의.
#include "../network/PPTCPIOCPClient.h"							//서버 구동시 필요합니다. 싱글톤 객체.
#include "../network/PPServerRecvPacketPool.h"					//클라이언트 클래스 정의.
#include "../network/PPClientRecvPacketPool.h"					//클라이언트 구동시 필요합니다. 싱글톤 객체
#pragma comment(lib, "../../lib/libppnetwork.lib")

GameObject*					MainClass::m_pSkyBox	= nullptr;
MainClass*				    MainClass::m_curScene	= nullptr;
map<ESceneName, MainClass*> MainClass::m_SceneList;

PP::PPSender* g_pSender			= PP::GetSender();		//동적 패킷 전송 객체
PP::PPTCPIOCPClient* g_pClient	= PP::GetClient();
PP::PPTCPIOCPServer* g_pServer	= PP::GetServer();		//동적 서버객체


bool MainClass::Init() noexcept
{
	// 커서 감추기
	//ShowCursor(FALSE);

	// 폰트 설정
	WriteManager::Get().SetText({ 0, 0 }, L"", D2D1::ColorF::Black, 50, L"Yu Gothic");
	WriteManager::Get().m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	WriteManager::Get().m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	// 스카이 박스
	m_pSkyBox = new GameObject(L"SkyBox", { new SkySphere(20, L"SkySphere", L"CubeMap/grassenvmap1024.dds"), new CTransformer(Vector3::Zero, Quaternion::Right * 0.05f, Vector3::Zero) }, EObjType::Dummy);
	//m_pSkyBox = new GameObject(L"SkyBox", { new SkyBox(L"SkyBox", L"../../data/map/Texture/Sunny_01A_"), new CTransformer(Vector3::Zero, Quaternion::Right * 0.05f, Vector3::Zero) }, EObjType::Dummy);
	m_pSkyBox->SetScale(Vector3::One * 100);
	m_pSkyBox->isGlobal(true);

	// 씬 설정
	m_SceneList[ESceneName::Intro]	= new IntroScene();
	m_SceneList[ESceneName::Main]	= new GameScene();
	m_SceneList[ESceneName::Lobby] = new LobbyScene();
	///

	// ========================= 캐릭터 초기화 ===========================
	I_CHARMGR.Init();
	if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), TablePATH))		//경로 중요함
	{
		return false;
	}

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

	//WriteManager::Get().SetFontSize(50);
	//WriteManager::Get().DrawTextW( { getClientRect().right * 0.1f, getClientRect().bottom * 0.3f, 500, 300 }, L"앙앙~ Test ~ 123,,");
	return true;
}

bool MainClass::Release() noexcept
{
	m_curScene->Release();
	return true;
}

// 씬 전환
void MainClass::SetScene(const ESceneName& sceneName, const bool& useRelease) noexcept
{	
	if (m_curScene == m_SceneList[sceneName]) 
		return;
	if(useRelease)
		m_curScene->Release();

	m_curScene = m_SceneList[sceneName];
	// 쓰레드 실행
	std::thread initer(&MainClass::Init, m_SceneList[sceneName]);
	//initer.detach();
	// 쓰레드 대기
	if (initer.joinable())
		initer.join();
}

void MainClass::SendPlaySound(const string_view& soundName, const D3DXVECTOR3& position, const float& maxDistance) noexcept
{
	Packet_SoundData p_SoundData;
	p_SoundData.KeyValue = ObjectManager::KeyObjects.begin()->first;// PlayerController::Get().m_keyValue;
	p_SoundData.Position = position;
	p_SoundData.MaxDistance = maxDistance;
	memcpy(p_SoundData.SoundName,  soundName.data(), soundName.size());
	p_SoundData.NameSize = (char)soundName.size();

	PacketManager::Get().SendPacket((char*)&p_SoundData, (USHORT)(21 + soundName.size()), PACKET_PlaySound);
}

void MainClass::SendMovedObject() noexcept
{
	static Packet_Vec3Quat f_Vec3Quat;
	for (auto& [key, iter] : ObjectManager::KeyObjects)
	{
		if (iter->isMoved())
		{
			iter->isMoved(false);
			// Position, Rotation 갱신
			f_Vec3Quat.KeyValue = iter->m_keyValue;
			f_Vec3Quat.Vec3 = iter->GetPosition();
			f_Vec3Quat.Quat = iter->GetRotation();
			PacketManager::Get().SendPacket((char*)&f_Vec3Quat, sizeof(Packet_Vec3Quat), PACKET_SetTransform);
		}
	}
}






int ProcessServerPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//서버 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	//std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv;
	PP::PPPacketForProcess packetSend;
	//IOCP 스레드에서 넣엇던 패킷을 담은 패킷풀 접근
	packetRecv = PP::PPServerRecvPacketPool::GetInstance().front();
	//패킷풀 맨 앞 pop()
	PP::PPServerRecvPacketPool::GetInstance().pop_front();
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
	 case PACKET_ReqSync:
	 {
	 	// 동기화 요청
	 	packetSend = packetRecv;
	 	//pSender->Send(packetSend);
	 
	 	static Packet_Transform p_Transform;
	 	static list<UINT> deleteKeys;
	 	for (auto& [key, iter] : ObjectManager::KeyObjects)
	 	{
	 		if (iter == nullptr)
	 		{
	 			//ObjectManager::KeyObjects.erase(key);
	 			deleteKeys.push_back(key);
	 			continue;
	 		}
	 		p_Transform.KeyValue = key;
	 		p_Transform.Position = iter->GetPosition();
	 		p_Transform.Rotation = iter->GetRotation();
	 		p_Transform.Scale	 = iter->GetScale();
	 		
	 		memcpy(packetSend.m_Packet.m_Payload, (void*)&p_Transform, sizeof(Packet_Transform));
	 		packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_SetTransform;
	 		packetSend.m_Packet.m_Header.m_len  = (USHORT)(sizeof(Packet_Transform) + PACKET_HEADER_SIZE);
	 		pSender->Send(packetSend);
	 	}
	 	// 이상한 키값 삭제
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
	 	static Packet_Vector3 p_player;
	 	static Packet_PossessPlayer p_character;
		packetSend = packetRecv;

	 	p_player.KeyValue = ObjectKeyCount;
	 	p_player.Vec3 = {RandomNormal() * 500.0f, 500.0f, RandomNormal() * 500.0f };
	 	memcpy(packetSend.m_Packet.m_Payload, (void*)&p_player, sizeof(Packet_Vector3));
	 	packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_SetTransform;
	 	packetSend.m_Packet.m_Header.m_len = (USHORT)(sizeof(Packet_Vector3) + PACKET_HEADER_SIZE);
	 	pSender->Broadcast(packetSend);
		PacketManager::Get().PacketHistory.push(packetSend);
	 
	 	p_character.KeyValue = ObjectKeyCount + 1;
	 	p_character.ECharacter = PlayerController::ECharacter::EGuard;
	 	memcpy(packetSend.m_Packet.m_Payload, (void*)&p_character, sizeof(Packet_PossessPlayer));
	 	packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PACKET_PossessPlayer;
	 	packetSend.m_Packet.m_Header.m_len = (USHORT)(sizeof(Packet_PossessPlayer) + PACKET_HEADER_SIZE);
	 	pSender->Send(packetSend);

	 }	break;
	 default:
	 {
	 	// 미정의 패킷 처리
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
	PP::PPPacketForProcess RecvPacket = PP::PPClientRecvPacketPool::GetInstance().front();
	PP::PPClientRecvPacketPool::GetInstance().pop_front();
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
	iReturn = g_pClient->SetHost("192.168.0."s +  WCharToChar(PacketManager::Get().InputIP.c_str()));		// 서버의 IPv4
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