#include "MainClass.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "LobbyScene.h"

#include "../network/PPTCPIOCPClient.h"
#include "../network/PPRecvPacketPool.h"
#pragma comment(lib, "../../lib/libppnetwork.lib")

GameObject*					MainClass::m_pSkyBox	= nullptr;
map<ESceneName, MainClass*> MainClass::m_SceneList;
MainClass*				    MainClass::m_curScene	= nullptr;

PP::PPSender* pSender = PP::GetSender();		//동적 패킷 전송 객체
PP::PPTCPIOCPClient* pClient = PP::GetClient();	//동적 서버객체

int ProcessPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//클라이언트 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	std::wcout << "injected function()..." << std::endl;
	//PPRecvPacketPool에서 저장한 수신패킷을 하나 끄집어내서 처리합니다.
	PP::PPPacketForProcess RecvPacket = PP::PPRecvPacketPool::GetInstance().front();
	PP::PPRecvPacketPool::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (RecvPacket.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		std::wstring wstrBuf;
		PP::PPPacketMessage* packetMsg = (PP::PPPacketMessage*)RecvPacket.m_Packet.m_Payload;
		wcharBuf = (wchar_t*)&packetMsg->m_charMessage;
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << wcharBuf << std::endl;
		OutputDebugStringW(wstrBuf.c_str());
		break;
	}
	case PP::PPPacketType::TYPE_MOVE_HOST_TO_GUEST: {
		//호스트에게 게스트의 움직임을 알리는 패킷 처리
		break;
	}
	default:
		wcharBuf = (wchar_t*)&RecvPacket.m_Packet;
		std::wcout << wcharBuf << std::endl;
		break;
	}
	return 0;
}

bool MainClass::Init() noexcept
{
	// 커서 감추기
	ShowCursor(FALSE);
	
	// 폰트 설정
	WriteManager::Get().SetText({ 0, 0 }, L"", D2D1::ColorF::Black, 50, L"Yu Gothic");
	WriteManager::Get().m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	WriteManager::Get().m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	// 스카이 박스
	m_pSkyBox = new GameObject(L"SkyBox", { new SkySphere(20, L"SkySphere", L"CubeMap/grassenvmap1024.dds"), new CTransformer(Vector3::Zero, Quaternion::Right * 0.05f, Vector3::Zero) }, EObjType::Dummy);
	m_pSkyBox->SetScale(Vector3::One * 100);
	m_pSkyBox->isGlobal(true);

	// 씬 설정
	m_curScene = m_SceneList[ESceneName::Intro]	= new IntroScene();
	m_SceneList[ESceneName::Main]	= new GameScene();
	m_SceneList[ESceneName::Lobby] = new LobbyScene();
	///
	SetScene(ESceneName::Intro, false);

	// 캐릭터
	I_CHARMGR.Init();
	if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), _T("../../data/Model/CharacterTable.cit")))		//경로 중요함
	{
		return false;
	}
	m_pPlayer = new PlayerController(L"Player", EObjType::Object);
	ObjectManager::Cameras[ECamera::Main]->SetParent(m_pPlayer);
	m_Hero.SetPlayerCharacter(L"guard", 0, 0, 0);			
	m_Hero.SetANIM(Guard_IDLE);
	m_Hero.m_objType = EObjType::Object;
	//m_Hero.m_InitBox();
	m_pHeroCollider = new ColliderOBB({ -13.0f, -30.0f , -13.0f }, { 13.0f, 30.0f , 13.0f });//m_Hero.m_CollsionBox.vMin, m_Hero.m_CollsionBox.vMax));
	m_pHeroCollider->m_pivot = Vector3::Up * 30.0f;
	m_Hero.AddComponent(m_pHeroCollider); 
	m_Hero.m_myName = L"Hero";
	//auto pClone = m_Hero.clone();
	//pClone->Translate(Vector3::Up * 80.0f);
	m_pPlayer->SetParent(&m_Hero);
	m_pPlayer->isCharacter(true);
	m_pPlayer->ResetOption(); 
	
	ObjectManager::Get().PushObject(&m_Hero);
	//ObjectManager::Get().PushObject(pClone);

	//서버 기동
	pClient->SetHost("192.168.0.47");				//IPv4
	pClient->SetPortNumber(10000);					//포트번호
	pClient->SetNumberOfThreads(2);					//IOCP 쓰레드
	pClient->SetFP(ProcessPacket);					//패킷 처리용 함수포인터 입력
	pClient->Startup();								//클라이언트 기동

	return true;
}


bool MainClass::Frame() noexcept
{
	static float fCount;

	fCount += Timer::SPF;
	if (fCount >= 2.0f) {
		pSender->BroadcastWString(L"Hello, Server!");	//서버에게 문자열 전송(함수명은 바꿀 예정)
		fCount = 0.0f;
	}
	
	//
	m_pSkyBox->Frame(Timer::SPF, Timer::AccumulateTime);
	m_curScene->Frame();

	// 씬 전환
	if (Input::GetKeyState(VK_NUMPAD1) == EKeyState::DOWN)
		SetScene(ESceneName::Intro);
	if (Input::GetKeyState(VK_NUMPAD2) == EKeyState::DOWN)
		SetScene(ESceneName::Main);
	if (Input::GetKeyState(VK_NUMPAD3) == EKeyState::DOWN)
		SetScene(ESceneName::Lobby);
	return true;
}

bool MainClass::Render() noexcept
{
	m_pSkyBox->Render(DxManager::Get().GetDContext());
	m_Hero.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);//	(월드,뷰,투영)
	m_curScene->Render();

	WriteManager::Get().SetFontSize(50);
	WriteManager::Get().DrawTextW( { getClientRect().right * 0.1f, getClientRect().bottom * 0.3f, 500, 300 }, L"앙앙~ Test ~ 123,,");
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
	if(useRelease)
		m_curScene->Release();
	m_curScene = m_SceneList[sceneName];
	// 쓰레드 실행
	std::thread initer(&MainClass::Init, m_SceneList[sceneName]);
	initer.detach();
	//// 쓰레드 대기
	//if (initer.joinable())
	//	initer.join();
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