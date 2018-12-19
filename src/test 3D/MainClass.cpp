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



	I_CHARMGR.Init();
if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), _T("../../data/Model/CharacterTable.cit")))		//경로 중요함
{
	return false;
}


	m_Hero.SetPlayerCharacter(L"guard", 0, 0, 0);			



	m_Hero.SetANIM(Guard_DANCE3);					


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

	/// BGM 변경
	///if (m_Input.getKeyState(VK_F1) == EKeyState::DOWN)
	///	m_SoundManager.setBGM("BGM/Mischievous Masquerade");
	///if (m_Input.getKeyState(VK_F2) == EKeyState::DOWN)
	///	m_SoundManager.setBGM("BGM/Sandbag Mini Game");
	///if (m_Input.getKeyState(VK_F3) == EKeyState::DOWN)
	///	m_SoundManager.setBGM("BGM/Brawl Breaks");
	///if (m_Input.getKeyState(VK_F4) == EKeyState::DOWN)
	///	m_SoundManager.setBGM("BGM/PLAY ROUGH");

	m_Hero.Frame(Timer::SPF, Timer::AccumulateTime);
	return true;
}

bool MainClass::Render() noexcept
{
	m_pSkyBox->Render(DxManager::Get().GetDContext());
	m_curScene->Render();
	WriteManager::Get().SetFontSize(50);
	WriteManager::Get().DrawTextW( { getClientRect().right * 0.1f, getClientRect().bottom * 0.3f, 500, 300 }, L"앙앙~ Test ~ 123,,");


	m_Hero.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);//	(월드,뷰,투영)
	m_Hero.Render(DxManager::GetDContext());	
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