#include "GameScene.h"
#include "PacketManager.h"

bool GameScene::Init() noexcept
{
#pragma region Basic
	
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		// 맵 푸쉬
		ObjectManager::Get().PushObject(m_pMap);
		///
		m_pPlayer->m_myName  = L"Player";
		m_pPlayer->m_objType = EObjType::Object;
		ObjectManager::Cameras[ECamera::Main]->SetParent(m_pPlayer);
		// 기사 
		m_pHero = new AHeroObj();
		m_pHero->SetPlayerCharacter(Guard, 0.0f, 100.0f, 0.0f);
		m_pHero->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		m_pHero->SetANIM_Loop(Guard_IDLE);
		m_pHero->m_myName = L"Guard";
		m_pHero->m_objType = EObjType::Object;
		auto pCollider = new ColliderOBB( { -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		m_pHero->AddComponent(pCollider);
		ObjectManager::Get().SetProtoObject(m_pHero);

		// 좀비
		m_pZombie = new AHeroObj();
		m_pZombie->SetPlayerCharacter(Zombie, 80.0f, 200.0f, -300.0f);
		m_pZombie->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		m_pZombie->SetANIM_Loop(Zombie_IDLE);
		m_pZombie->m_myName = L"Zombie";
		m_pZombie->m_objType = EObjType::Object;
		pCollider = new ColliderOBB( { -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		m_pZombie->AddComponent(pCollider);
		ObjectManager::Get().SetProtoObject(m_pZombie);

		// 새 생성
		m_pBird = new AHeroObj();
		m_pBird->SetPlayerCharacter(NPC_Bird, 0.0f, 80.0f, 300.0f);
		m_pBird->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		m_pBird->m_myName = L"Bird";
		m_pBird->m_objType = EObjType::Object;
		m_pBird->SetScale(Vector3::One * 15.0f);
		pCollider = new ColliderOBB( { -1.0f, 0.0f , -1.0f }, { 1.0f, 2.0f , 1.0f });
		pCollider->m_pivot *= 15.0f;
		pCollider->SetGravityScale(0.3f);
		m_pBird->AddComponent(pCollider);
		ObjectManager::Get().SetProtoObject(m_pBird);

		//// 닭 생성
		//m_pChicken = new AHeroObj();
		//m_pChicken->SetPlayerCharacter(NPC_Chicken, 0.0f, 300.0f, -400.0f);
		//m_pChicken->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		//m_pChicken->m_myName = L"Chicken";
		//m_pChicken->m_objType = EObjType::Object;
		////m_pChicken->SetScale(Vector3::One * 15.0f);
		//pCollider = new ColliderOBB(23.0f, { -15.0f, 0.0f , -15.0f }, { 15.0f, 30.0f , 15.0f });
		////pCollider->m_pivot *= 15.0f;
		//m_pChicken->AddComponent(pCollider);
		//ObjectManager::Get().SetProtoObject(m_pChicken);
	}
	m_pHero = (AHeroObj*)ObjectManager::Get().TakeObject(L"Guard");
	m_pPlayer->Possess(m_pHero);
	m_pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
	m_pPlayer->ResetOption();
	

	ObjectManager::Get().TakeObject(L"ParticleSystem");
	ObjectManager::Get().TakeObject(L"Guard")  ->SetPosition(RandomNormal() * 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);
	ObjectManager::Get().TakeObject(L"Zombie") ->SetPosition(RandomNormal() * 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);
	ObjectManager::Get().TakeObject(L"Bird")   ->SetPosition(RandomNormal()	* 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);
	ObjectManager::Get().TakeObject(L"Guard")  ->SetPosition(RandomNormal() * 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);
	ObjectManager::Get().TakeObject(L"Zombie") ->SetPosition(RandomNormal() * 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);
	ObjectManager::Get().TakeObject(L"Bird")   ->SetPosition(RandomNormal()	* 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);
	ObjectManager::Get().TakeObject(L"Guard")  ->SetPosition(RandomNormal() * 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);
	ObjectManager::Get().TakeObject(L"Zombie") ->SetPosition(RandomNormal() * 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);
	ObjectManager::Get().TakeObject(L"Bird")   ->SetPosition(RandomNormal()	* 1000.0f - 500.0f, RandomNormal() * 500.0f, RandomNormal() * 1000.0f - 500.0f);

#pragma endregion
	
	// ===================================== UI =========================================
	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/UI_InGame", *pUIRoot);
	PlayerController::Get().m_pHpBar = (JProgressBar*)pUIRoot->find_child(L"HP_Progress");
	//pProj->SetValue(m_pPlayer->m_HP, 1.0f); // 값 bind
	auto pProj = (JProgressBar*)pUIRoot->find_child(L"MP_Progress");
	pProj->SetValue(m_pPlayer->m_MP, 1.0f); // 값 bind

	// Right Icon
	auto pIcon = (JProgressBar*)pUIRoot->find_child(L"Skill_0");
	pIcon->SetValue(PlayerController::Get().m_curDelayThrow, PlayerController::Get().m_DelayThrow);
	// Option Slider
	static auto pSetVolume = [](void* pSlider) {
		SoundManager::Get().SetMasterVolume(*((JSliderCtrl*)pSlider)->GetValue());
	};
	m_pVolume = (JSliderCtrl*)pUIRoot->find_child(L"Set_Volum");
	m_pVolume->EventPress.first  = pSetVolume;
	m_pVolume->EventPress.second = m_pVolume;
	m_pVolume->SetValue(0.5f);
	SoundManager::Get().SetMasterVolume(*m_pVolume->GetValue());

	static auto pSetMouseSense = [](void* pSlider) {
		PlayerController::Get().m_mouseSense = *((JSliderCtrl*)pSlider)->GetValue();
	};
	m_pMouseSense = (JSliderCtrl*)pUIRoot->find_child(L"Set_Mouse");
	m_pMouseSense->EventPress.first  = pSetMouseSense;
	m_pMouseSense->EventPress.second = m_pMouseSense;
	m_pMouseSense->SetValue(0.5f);
	PlayerController::Get().m_mouseSense = *m_pMouseSense->GetValue();
	// Exit
	static auto pGameExit = [](void* pScene) {
		exit(0); pScene;
		//PlayerController::Get().CutParent();
		//PacketManager::Get().pSender->Release();
		//((MainClass*)pScene)->SetScene(ESceneName::Lobby);
	};
	auto pExit = (JTextCtrl*)pUIRoot->find_child(L"Set_GameExit");
	pExit->EventClick.first = pGameExit;
	pExit->EventClick.second = this;
	// CheckBox
	m_pCheckBox = (JCheckCtrl*)pUIRoot->find_child(L"temp_Check0");
	// Timer
	m_TimerText = (JTextCtrl*)pUIRoot->find_child(L"Timer_Text");

	// Chatting
	static auto pChatWheel = [](void* pVoid) {
		static short wheel = 0;
		wheel = Input::GetWheelScroll();
		if (wheel != 0)
		{
			auto pList = (JListCtrl*)pVoid;
			if (wheel > 0)
			{
				pList->m_fValue = min(pList->m_fValue + pList->fDivisionValue, 1.0f);
			}
			else
			{
				pList->m_fValue = max(pList->m_fValue - pList->fDivisionValue, 0.0f);
				//pList->AddValue(clamp(pList->m_fValue - pList->fDivisionValue, 0.0f, 1.0f) - pList->m_fValue);
			}
		}
	};
	PacketManager::Get().m_pChatList = (JListCtrl*)pUIRoot->find_child(L"Chat_Log");
	PacketManager::Get().m_pChatList->EventHover.first = pChatWheel;
	PacketManager::Get().m_pChatList->EventHover.second = PacketManager::Get().m_pChatList;
	//JSliderCtrl* pSlider = (JSliderCtrl*)pUIRoot->find_child(L"Chat_Slider");
	//m_pList->m_fValue = pSlider->GetValue();
	ObjectManager::Get().PushObject(pUIRoot);
	// ==================================================================================

	SoundManager::Get().SetBGM("bgm_ingame01.mp3");
	Timer::AccumulateTime = 0.0f;
	m_isLoading = false;
	return true;
}


// 프레임
bool GameScene::Frame() noexcept
{
	// IME 채팅
	if (PlayerController::Get().isChatting())
	{
		m_chatMessage = ime::Get()->GetString();
	}
	if (Input::GetKeyState(VK_RETURN) == EKeyState::DOWN)
	{
		if (PlayerController::Get().isChatting())
		{
			static size_t strSize = 0;
			strSize = m_chatMessage.size() * 2;
			strSize = strSize > 200 ? 200 : strSize;
			// 패킷 전송
			Packet_ChatMessage p_ChatMessage;
			p_ChatMessage.KeyValue = ObjectManager::KeyObjects.begin()->first;// PlayerController::Get().m_keyValue;
			memcpy(p_ChatMessage.Message, m_chatMessage.data(), strSize);
			p_ChatMessage.MsgSize = (UCHAR)strSize;
			PacketManager::Get().SendPacket((char*)&p_ChatMessage, (USHORT)(PS_ChatMessage + strSize), PACKET_ChatMessage);

			m_chatMessage.clear();
			ime::Get()->imeEnd();
			PlayerController::Get().isChatting(false);
		}
		else
		{
			PlayerController::Get().isChatting(true);
			ime::Get()->imeStart();
		}
	}

	// 플레이어 변경
	if (Input::GetKeyState(VK_TAB) == EKeyState::DOWN)
	{
		static auto curCollider = ObjectManager::Get().GetColliderList().begin();
		if (curCollider == ObjectManager::Get().GetColliderList().end())
			curCollider = ObjectManager::Get().GetColliderList().begin();
		if (++curCollider == ObjectManager::Get().GetColliderList().end())
			curCollider = ObjectManager::Get().GetColliderList().begin();

		m_pPlayer->Possess((*curCollider)->m_pParent);
		//SoundManager::Get().m_pListenerPos = &(*curCollider)->m_pParent->GetRoot()->GetPosition();
	}
	// 시간 출력



	if (m_Rule.m_bSeek==true)
	{
		m_TimerText->m_Text = to_wstring(m_Rule.GetPlayTime()-Timer::AccumulateTime).substr(0, 5);
	}
	else
	{
		m_TimerText->m_Text = to_wstring(m_Rule.GetHideTime()-Timer::AccumulateTime).substr(0, 5);
	}

	
	
	///
	m_pMapTree->Frame();
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();

	// 맵 높이
	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		iter->m_mapHeight = m_pMap->GetHeight(iter->m_pParent->GetWorldPosition().x, iter->m_pParent->GetWorldPosition().z);
	}
	return true;
}

// 랜더
bool GameScene::Render() noexcept
{
	// 채팅
	if (PlayerController::Get().isChatting())
	{
		WriteManager::Get().Draw({ 330, 850, 900, 200 }, m_chatMessage);
	}

	m_pMap->SetMatrix(NULL, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	I_Object.SetMatrix(&ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	I_Object.Render(DxManager::Get().GetDContext());
	///
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::Get().GetDContext());
	SoundManager::Get().Render();

	// 바운딩 박스 표시
	if (m_pCheckBox->m_bCheck)
	{
		DrawBoundingBox();
	}
	return true;
}

// 릴리즈
bool GameScene::Release() noexcept
{
	if (m_pMapTree) m_pMapTree->Release();

	ObjectManager::Cameras[ECamera::Main]->CutParent();
	ObjectManager::Get().PopObject(ObjectManager::Cameras[ECamera::Main]);
	ObjectManager::Get().Release();
	return true;
}


void GameScene::DrawBoundingBox()	noexcept
{
	static GameObject* pBox = nullptr;
	static GameObject* pSphere = nullptr;
	if (pBox == nullptr)
	{
		pBox = new GameObject(L"DebugBox", ObjectManager::Get().TakeComponent(L"Cube"));
		pBox->isGlobal(true);
		pSphere = new GameObject(L"DebugSphere", ObjectManager::Get().TakeComponent(L"RowSphere"));
		pSphere->isGlobal(true);
	}

	DxManager::Get().SetRasterizerState(ERasterS::Wireframe);
	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		switch (iter->m_eCollider)
		{
		case ECollider::AABB:
		{
			pBox->SetPosition(iter->GetCenter());
			pBox->SetRotation(Quaternion::Base);
			pBox->SetScale(((ColliderAABB*)iter)->GetLength() * 0.5f);
			pBox->Frame(0.0f, 0.0f);
			pBox->Render(DxManager::GetDContext());
		}	break;
		case ECollider::OBB:
		{
			pBox->SetPosition(iter->GetCenter());
			pBox->SetRotation(iter->m_pParent->GetRotation());
			pBox->SetScale(((ColliderOBB*)iter)->GetExtents());
			pBox->Frame(0.0f, 0.0f);
			pBox->Render(DxManager::GetDContext());
		}	break;
		case ECollider::Sphere:
		{
		}	break;
		}
		pSphere->SetPosition(iter->GetCenter());
		pSphere->SetRotation(Quaternion::Base);
		pSphere->SetScale(iter->GetWorldRadius() * Vector3::One);
		pSphere->Frame(0.0f, 0.0f);
		pSphere->Render(DxManager::GetDContext());
	}
	DxManager::Get().SetRasterizerState(ERasterS::Current);
}