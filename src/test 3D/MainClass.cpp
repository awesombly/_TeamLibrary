#include "MainClass.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "LobbyScene.h"


GameObject*					MainClass::m_pSkyBox	= nullptr;
map<ESceneName, MainClass*> MainClass::m_SceneList;
MainClass*				    MainClass::m_curScene	= nullptr;



bool MainClass::Init() noexcept
{
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
	//m_pPlayer = new GameObject(L"Character", EObjType::Object);
	m_Hero.SetPlayerCharacter(L"guard", 0, 0, 0);			
	m_Hero.SetANIM(Guard_IDLE);
	m_Hero.m_objType = EObjType::Object;
	//m_Hero.Rotate(Quaternion::Left * PI);
	//m_Hero.SetParent(m_pPlayer);
	//m_Hero.m_InitBox();
	m_pHeroCollider = new ColliderOBB({ -13.0f, -30.0f , -13.0f }, { 13.0f, 30.0f , 13.0f });//m_Hero.m_CollsionBox.vMin, m_Hero.m_CollsionBox.vMax));
	m_pHeroCollider->m_pivot = Vector3::Up * 30.0f;
	m_Hero.AddComponent(m_pHeroCollider); 
	ObjectManager::Get().PushObject(&m_Hero);
	ObjectManager::Cameras[ECamera::Main]->SetParent(&m_Hero);
	ObjectManager::Cameras[ECamera::Main]->SetPosition(Vector3::Forward * 50.0f + Vector3::Up * 65.0f);
	ObjectManager::Cameras[ECamera::Main]->SetRotation(Quaternion::Left * PI);
	//m_matLocal
	return true;
}


bool MainClass::Frame() noexcept
{
	m_pSkyBox->Frame(Timer::SPF, Timer::AccumulateTime);
	m_curScene->Frame();

	// 씬 전환
	if (Input::GetKeyState(VK_NUMPAD1) == EKeyState::DOWN)
		SetScene(ESceneName::Intro);
	if (Input::GetKeyState(VK_NUMPAD2) == EKeyState::DOWN)
		SetScene(ESceneName::Main);
	if (Input::GetKeyState(VK_NUMPAD3) == EKeyState::DOWN)
		SetScene(ESceneName::Lobby);

#pragma region PlayerInput
	if (Input::GetKeyState(VK_SHIFT) != EKeyState::HOLD)
	{
		if (Input::GetKeyState('W') == EKeyState::HOLD)
		{
			m_Hero.SetANIM(Guard_FORWARD);
			//m_Hero.SetANIM(Guard_DANCE3);
			m_Hero.Translate(m_Hero.GetForward() * Timer::SPF * 50);
		}
		if (Input::GetKeyState('S') == EKeyState::HOLD)
		{
			m_Hero.SetANIM(Guard_BACKWARD);
			m_Hero.Translate(m_Hero.GetBackward() * Timer::SPF * 50);
		}
		if (Input::GetKeyState('A') == EKeyState::HOLD)
		{
			m_Hero.SetANIM(Guard_LEFT);
			m_Hero.Translate(m_Hero.GetLeft() * Timer::SPF * 50);
		}
		if (Input::GetKeyState('D') == EKeyState::HOLD)
		{
			m_Hero.SetANIM(Guard_RIGHT);
			m_Hero.Translate(m_Hero.GetRight() * Timer::SPF * 50);
		}
		if (Input::GetKeyState('Q') == EKeyState::HOLD)
		{
			m_Hero.SetANIM(Guard_DANCE1);
			m_Hero.Rotate(Quaternion::Left * Timer::SPF);
		}
		if (Input::GetKeyState('E') == EKeyState::HOLD)
		{
			m_Hero.SetANIM(Guard_DANCE2);
			m_Hero.Rotate(Quaternion::Right * Timer::SPF);
		}
		if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
		{
			m_Hero.SetANIM(Guard_DANCE3);
			m_Hero.Translate(Vector3::Up * 5.0f);
			m_pHeroCollider->m_force = Vector3::Up * 180.0f;
		}
	}
#pragma endregion
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