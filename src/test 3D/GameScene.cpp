#include "GameScene.h"
#include "PacketManager.h"
#include "JEventBind.h"


bool GameScene::Init() noexcept
{
	m_pPlayer->Init();
	// UI
	LoadUI();
	FirstInit();

	//I_Object.ViewColliderSwitch();
	//for (auto& [name, matrixList] : I_Object.m_ObjectMatrix)
	//{
	//	for (int matIndex = 0; matIndex < matrixList.Matrix.size(); ++matIndex)
	//	{
	//		for (int colIndex = 0; colIndex < I_Object.m_ObjectCollider[name].ColliderAABB.size(); ++colIndex)
	//		{
	//			auto pCollider = new ColliderOBB(Product(I_Object.m_ObjectCollider[name].ColliderAABB[colIndex].vMin, matrixList.vScale[matIndex]), Product(I_Object.m_ObjectCollider[name].ColliderAABB[colIndex].vMax, matrixList.vScale[matIndex]));
	//			auto pObject = new GameObject(L"Dummy", pCollider);
	//
	//			pObject->SetPosition(matrixList.vLocation[matIndex]);
	//			pObject->SetRotation(QuatToRotation(matrixList.qRotation[matIndex]) + QuatToRotation(I_Object.m_ObjectCollider[name].qRotation[colIndex]));
	//			pObject->SetHP(10000.0f);
	//			auto center = Product(I_Object.m_ObjectCollider[name].ColliderAABB[colIndex].vCenter, matrixList.vScale[matIndex]);
	//			
	//			pCollider->m_pivot = center;
	//
	//			pCollider->usePhysics(false);
	//			pCollider->SetGravityScale(0.0f);
	//
	//			pObject->Frame(0.0f, 0.0f);
	//			//ObjectManager::Get().PushObject(pObject);
	//		}
	//	}
	//}

#pragma region Basic
	//GameObject* pEffect = nullptr;
	//m_pParser->CreateFromFile(&pEffect, L"Snow.eff", L"../../data/script");
	//pEffect->SetPosition(Vector3::Up * 400.0f);
	//ObjectManager::Get().PushObject(pEffect);

	//pEffect = new GameObject(L"Bigbang", m_pParser->CreateFromParticle(L"Bigbang.eff", L"../../data/script"), EObjType::Effect);
	//pEffect->SetPosition(Vector3::Up * 400.0f + Vector3::Left * 700.0f);
	//ObjectManager::Get().PushObject(pEffect);

	//auto pParticle = m_pParser->CreateFromParticle(L"Shock.eff", L"../../data/script");
	//pParticle->isRepeat(true);
	//pEffect = new GameObject(L"Shock", pParticle, EObjType::Effect);
	//pEffect->SetPosition(Vector3::Up * 400.0f + Vector3::Right * 700.0f);
	//ObjectManager::Get().PushObject(pEffect);

	//pEffect = new GameObject(L"WheelWind", { m_pParser->CreateFromParticle(L"WheelWind.eff", L"../../data/script"), new CTransformer(Vector3::Zero, Quaternion::Left * 3.0f) }, EObjType::Effect);
	//pEffect->SetPosition(Vector3::Up * 400.0f + Vector3::Forward * 500.0f);
	//ObjectManager::Get().PushObject(pEffect);

	//pEffect = new GameObject(L"Atom", { m_pParser->CreateFromParticle(L"Atom.eff", L"../../data/script"), new CTransformer(Vector3::Zero, {3.0f, 5.0f, 7.0f, 0.0f}) }, EObjType::Effect);
	//pEffect->SetPosition(Vector3::Up * 400.0f + Vector3::Backward * 500.0f);
	//ObjectManager::Get().PushObject(pEffect);

	// ==================================================================================
	ObjectManager::KeyCount = 1000;
	auto pHero = (AHeroObj*)ObjectManager::Get().TakeObject(L"Guard");
	//pHero->GetLeftHandPos
	m_pPlayer->Possess(pHero);
	///
	//ObjectManager::Get().TakeObject(L"Zombie")->SetPosition( -500, 65.5f, 450);
	//ObjectManager::Get().TakeObject(L"Zombie")->SetPosition( -200, 65.5f, -100);
	//ObjectManager::Get().TakeObject(L"Zombie")->SetPosition( -100, 65.5f, 350);
	//ObjectManager::Get().TakeObject(L"Zombie")->SetPosition(-400, 65.5f, -200);
	//ObjectManager::Get().TakeObject(L"Zombie")->SetPosition(300, 65.5f, 0);
	//ObjectManager::Get().TakeObject(L"Zombie")->SetPosition(100, 65.5f, -300);
	//ObjectManager::Get().TakeObject(L"Bird")->SetPosition(  -300, 65.5f, 250);
	//ObjectManager::Get().TakeObject(L"Bird")->SetPosition(  200, 65.5f, 150);
	//ObjectManager::Get().TakeObject(L"Bird")->SetPosition(  400, 65.5f, -400);
#pragma endregion
	///
	SoundManager::Get().SetBGM("bgm_ingame01.mp3");
	Timer::AccumulateTime = 0.0f;
	m_isLoading = false;
	m_Rule.SetReadyTime(7.0f);
	m_Rule.SetPlayTime(180.0f);
	return true;
}


// 프레임
bool GameScene::Frame() noexcept
{
	// IME 채팅
	if (PlayerController::Get().isChatting())
	{
		m_chatMessage = m_pChat->GetString();
	}
	if (Input::GetKeyState(VK_RETURN) == EKeyState::DOWN)
	{
		if (PlayerController::Get().isChatting())
		{
			if (!m_chatMessage.empty())
			{
				CheatMessage();		// 명령어 체크

				static size_t strSize = 0;
				strSize = m_chatMessage.size() * 2;
				strSize = strSize > 200 ? 200 : strSize;
				// 패킷 전송
				Packet_ChatMessage p_ChatMessage;
				memcpy(p_ChatMessage.Message, m_chatMessage.data(), strSize);
				p_ChatMessage.DataSize = (UCHAR)strSize;
				p_ChatMessage.UserSocket = PacketManager::Get().pMyInfo->UserSocket;
				PacketManager::Get().SendPacket((char*)&p_ChatMessage, (USHORT)(PS_ChatMessage + strSize), PACKET_ChatMessage);
			}
			m_chatMessage.clear();
			m_pChat->End();
			m_pChat->Clear();
			m_pChat->m_bRender = false;
			PlayerController::Get().isChatting(false);
		}
		else
		{
			PlayerController::Get().isChatting(true);
			m_pChat->Play();
			m_pChat->m_bRender = true;
		}
	}

	// 시간 출력, 호스트
	m_Rule.Frame();
	if (PacketManager::Get().isHost)
	{
		HostFrame();
	}
	///
	m_pMapTree->Frame();
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();

	// 경계 막기
	for (auto objIter = ObjectManager::Get().GetObjectList(EObjType::Character)->begin();
		objIter != ObjectManager::Get().GetObjectList(EObjType::Character)->end(); ++objIter)
	{
		auto& position = (*objIter)->GetPosition();
		if (position.x > 460.0f)
			(*objIter)->SetPositionX(460.0f);
		if (position.x < -455.0f)
			(*objIter)->SetPositionX(-455.0f);
		if (position.z > 460.0f)
			(*objIter)->SetPositionZ(460.0f);
		if (position.z < -400.0f)
			(*objIter)->SetPositionZ(-400.0f);
	}

	// 맵 높이
	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		if (iter == nullptr || iter->m_pParent == nullptr)
			continue;
		iter->m_mapHeight = m_pMap->GetHeight(iter->m_pParent->GetWorldPosition().x, iter->m_pParent->GetWorldPosition().z);
	}
	return true;
}

// 랜더
bool GameScene::Render() noexcept
{
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
	m_Rule.Release();
	return true;
}

bool GameScene::CheatMessage() noexcept
{
	auto finder = m_chatMessage.find(L" ");
	if (finder != wstring::npos) 
	{
		auto str = m_chatMessage.substr(0, finder);
		if (str._Equal(L"Respawn"))
		{
			auto cha = m_chatMessage.substr(finder + 1);
			if (cha._Equal(L"Guard"))
			{
				PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EGuard);
				return false;
			}
			else if (cha._Equal(L"Zombie"))
			{
				PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EZombie);
				return false;
			}
		}
		else if (str._Equal(L"Dead"))
		{
			if (PlayerController::Get().GetParent() != nullptr)
			{
				Packet_PlayerDead p_PlayerDead;
				p_PlayerDead.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
				p_PlayerDead.KillUser = (UINT)-1;
				p_PlayerDead.DeadUser = PacketManager::Get().pMyInfo->UserSocket;
				PacketManager::Get().SendPacket((char*)&p_PlayerDead, (USHORT)sizeof(Packet_PlayerDead), PACKET_PlayerDead);
			}
			return false;
		}
		else if (str._Equal(L"MoveSpeed"))
		{
			PlayerController::Get().m_moveSpeed = (float)atof(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			return false;
		}
		else if (str._Equal(L"JumpPower"))
		{
			PlayerController::Get().m_jumpPower = (float)atof(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			return false;
		}
		else if (str._Equal(L"Scale"))
		{
			auto scale = (float)atof(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			Packet_Vector3 p_SetScale;
			p_SetScale.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
			p_SetScale.Vec3 = { scale, scale, scale };
			PacketManager::Get().SendPacket((char*)&p_SetScale, (USHORT)sizeof(Packet_Vector3), PACKET_SetScale);
			return false;
		}
		else if (str._Equal(L"HP"))
		{
			Packet_SetHP p_SetHP;
			p_SetHP.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
			p_SetHP.HP = (float)atof(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			PacketManager::Get().SendPacket((char*)&p_SetHP, (USHORT)sizeof(Packet_SetHP), PACKET_SetHP);
			return false;
		}
	}
	return true;
}


void GameScene::DrawBoundingBox()	noexcept
{
	static GameObject* pBox = nullptr;
	static GameObject* pSphere = nullptr;
	if (pBox == nullptr || pSphere == nullptr)
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
			pSphere->SetPosition(iter->GetCenter());
			pSphere->SetRotation(Quaternion::Base);
			pSphere->SetScale(iter->GetWorldRadius() * Vector3::One);
			pSphere->Frame(0.0f, 0.0f);
			pSphere->Render(DxManager::GetDContext());
		}	break;
		}
	}
	DxManager::Get().SetRasterizerState(ERasterS::Current);
}

void GameScene::HostFrame() noexcept
{
	static Packet_TakeObject p_TakeObject;
	static float itemFrame = 0.0f;
	static float enemyFrame = 0.0f;
	itemFrame += Timer::SPF;
	enemyFrame += Timer::SPF;

	// 템 생성
	if (itemFrame >= 60.0f)
	{
		itemFrame = 0.0f;

		wstring objName = L"Atom";
		size_t  strSize = objName.size() * 2;
		strSize = strSize > 100 ? 100 : strSize;

		p_TakeObject.KeyValue = ++PacketManager::Get().PlayerKeyCount;
		memcpy(p_TakeObject.ObjectName, objName.data(), strSize);
		p_TakeObject.DataSize = (UCHAR)strSize;
		p_TakeObject.Position = Vector3::Up * 120.0f;
		p_TakeObject.Rotation = Quaternion::Base;
		p_TakeObject.Scale = Vector3::One;
		p_TakeObject.HP = 100.0f;
		p_TakeObject.UserSocket = (UINT)-1;
		PacketManager::Get().SendPacket((char*)&p_TakeObject, (USHORT)(PS_TakeObject + strSize), PACKET_TakeObject);
	}
	// 적 생성
	if (enemyFrame >= 3.0f)
	{
		enemyFrame = 0.0f;

		wstring objName = L"Zombie";
		size_t  strSize = objName.size() * 2;
		strSize = strSize > 100 ? 100 : strSize;

		p_TakeObject.KeyValue = ++PacketManager::Get().PlayerKeyCount;
		memcpy(p_TakeObject.ObjectName, objName.data(), strSize);
		p_TakeObject.DataSize = (UCHAR)strSize;
		p_TakeObject.Rotation = Quaternion::Base;
		p_TakeObject.HP = 1.0f;
		p_TakeObject.UserSocket = (UINT)-1;
		for (int i = 0; i < PacketManager::Get().UserList.size(); ++i)
		{
			p_TakeObject.Position = { RandomNormal() * 800.0f - 400.0f, 60.0f, RandomNormal() * 800.0f - 400.0f };
			p_TakeObject.Scale = (RandomNormal() * 0.2f + 0.1f) * Vector3::One;
			PacketManager::Get().SendPacket((char*)&p_TakeObject, (USHORT)(PS_TakeObject + strSize), PACKET_TakeObject);
		}
	}
}


bool GameScene::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		// 맵 푸쉬
		ObjectManager::Get().PushObject(m_pMap);
		//
		m_pPlayer->m_myName = L"Player";
		m_pPlayer->m_objType = EObjType::Object;
		ObjectManager::Cameras[ECamera::Main]->SetParent(m_pPlayer);
		return true;
	}
	return false;
}


void GameScene::LoadUI() noexcept
{
	// ===================================== UI =========================================
	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/InGame", *pUIRoot);
	// HP, MP
	PlayerController::Get().m_pHpBar = (JProgressBar*)pUIRoot->find_child(L"HP_Progress");
	auto pProj = (JProgressBar*)pUIRoot->find_child(L"MP_Progress");
	pProj->SetValue(m_pPlayer->m_MP, 1.0f); // 값 bind

	// 상황판
	/* Name Kill Death Score */
	//m_pUser1->PreEvent.first; // bRender 상관없이 돌아감
	PacketManager::Get().pUserPanel[0] = (JPanel*)pUIRoot->find_child(L"State_User1");
	PacketManager::Get().pUserPanel[0]->PostEvent.first = [](void* pVoid) {	 // bRender가 트루일때만 돌아감
		if (PacketManager::Get().UserList.empty())
			return;
		auto pUserPanel = (JPanel*)pVoid;
		auto iter = pUserPanel->m_pChildList.begin();
		((JTextCtrl*)(*iter))->m_Text = PacketManager::Get().UserList[0]->UserID;
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[0]->KillCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[0]->DeathCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[0]->Score);
		((JTextCtrl*)(*++++iter))->m_bRender = PacketManager::Get().UserList[0]->isDead;
	};
	PacketManager::Get().pUserPanel[0]->PostEvent.second = PacketManager::Get().pUserPanel[0];

	PacketManager::Get().pUserPanel[1] = (JPanel*)pUIRoot->find_child(L"State_User2");
	PacketManager::Get().pUserPanel[1]->PostEvent.first = [](void* pVoid) {
		if (PacketManager::Get().UserList.size() < 2)
			return;
		auto pUserPanel = (JPanel*)pVoid;
		auto iter = pUserPanel->m_pChildList.begin();
		((JTextCtrl*)(*iter))->m_Text = PacketManager::Get().UserList[1]->UserID;
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[1]->KillCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[1]->DeathCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[1]->Score);
		((JTextCtrl*)(*++++iter))->m_bRender = PacketManager::Get().UserList[1]->isDead;
	};
	PacketManager::Get().pUserPanel[1]->PostEvent.second = PacketManager::Get().pUserPanel[1];

	PacketManager::Get().pUserPanel[2] = (JPanel*)pUIRoot->find_child(L"State_User3");
	PacketManager::Get().pUserPanel[2]->PostEvent.first = [](void* pVoid) {
		if (PacketManager::Get().UserList.size() < 3)
			return;
		auto pUserPanel = (JPanel*)pVoid;
		auto iter = pUserPanel->m_pChildList.begin();
		((JTextCtrl*)(*iter))->m_Text = PacketManager::Get().UserList[2]->UserID;
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[2]->KillCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[2]->DeathCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[2]->Score);
		((JTextCtrl*)(*++++iter))->m_bRender = PacketManager::Get().UserList[2]->isDead;
	};
	PacketManager::Get().pUserPanel[2]->PostEvent.second = PacketManager::Get().pUserPanel[2];

	PacketManager::Get().pUserPanel[3] = (JPanel*)pUIRoot->find_child(L"State_User4");
	PacketManager::Get().pUserPanel[3]->PostEvent.first = [](void* pVoid) {
		if (PacketManager::Get().UserList.size() < 4)
			return;
		auto pUserPanel = (JPanel*)pVoid;
		auto iter = pUserPanel->m_pChildList.begin();
		((JTextCtrl*)(*iter))->m_Text = PacketManager::Get().UserList[3]->UserID;
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[3]->KillCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[3]->DeathCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(PacketManager::Get().UserList[3]->Score);
		((JTextCtrl*)(*++++iter))->m_bRender = PacketManager::Get().UserList[3]->isDead;
	};
	PacketManager::Get().pUserPanel[3]->PostEvent.second = PacketManager::Get().pUserPanel[3];
	// 옵션, 전광판
	PlayerController::Get().m_pOption = (JPanel*)pUIRoot->find_child(L"Set_Panel");
	PacketManager::Get().pKillDisplay = (JListCtrl*)pUIRoot->find_child(L"KilltoDeath");

	// Skill Icon
	auto pIcon = (JProgressBar*)pUIRoot->find_child(L"Skill_Right");
	pIcon->SetValue(PlayerController::Get().m_curDelayThrow, PlayerController::Get().m_DelayThrow);
	
	pIcon = (JProgressBar*)pUIRoot->find_child(L"Skill_Left"); // dash , left skill icon
	pIcon->SetValue(PlayerController::Get().m_curDelayDash, PlayerController::Get().m_DelayDash);

	// Option Slider
	static auto pSetVolume = [](void* pSlider) {
		SoundManager::Get().SetMasterVolume(*((JSliderCtrl*)pSlider)->GetValue());
	};
	m_pVolume = (JSliderCtrl*)pUIRoot->find_child(L"Set_Volum");
	m_pVolume->EventHover.first = pSetVolume;
	m_pVolume->EventHover.second = m_pVolume;
	m_pVolume->SetValue(0.5f);
	SoundManager::Get().SetMasterVolume(*m_pVolume->GetValue());

	static auto pSetMouseSense = [](void* pSlider) {
		PlayerController::Get().m_mouseSense = *((JSliderCtrl*)pSlider)->GetValue();
	};
	m_pMouseSense = (JSliderCtrl*)pUIRoot->find_child(L"Set_Mouse");
	m_pMouseSense->EventHover.first = pSetMouseSense;
	m_pMouseSense->EventHover.second = m_pMouseSense;
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
	m_Rule.m_TimerText = (JTextCtrl*)pUIRoot->find_child(L"Timer_Text");
	m_Rule.SetResultPanel((JPanel*)pUIRoot);

	// Chatting
	static auto pChatWheel = [](void* pVoid) {
		static short wheel = 0;
		wheel = Input::GetWheelScroll();
		if (wheel != 0)
		{
			auto pList = (JListCtrl*)pVoid;
			if (wheel > 0)
			{
				*pList->m_fValue = min(*pList->m_fValue + pList->fDivisionValue, 1.0f);
			}
			else
			{
				*pList->m_fValue = max(*pList->m_fValue - pList->fDivisionValue, 0.0f);
				//pList->AddValue(clamp(pList->m_fValue - pList->fDivisionValue, 0.0f, 1.0f) - pList->m_fValue);
			}
		}
	};
	PacketManager::Get().pChatList = (JListCtrl*)pUIRoot->find_child(L"Chat_Log");
	PacketManager::Get().pChatList->EventHover.first = pChatWheel;
	PacketManager::Get().pChatList->EventHover.second = PacketManager::Get().pChatList;
	//JSliderCtrl* pSlider = (JSliderCtrl*)pUIRoot->find_child(L"Chat_Slider");
	//m_pList->m_fValue = pSlider->GetValue();

	// 리스폰창
	PlayerController::Get().m_pRespawn	  = (JPanel*)pUIRoot->find_child(L"respawn_panel");		 // bRender용
	((JPanel*)PlayerController::Get().m_pRespawn)->m_bRender = false;
	PlayerController::Get().m_pRespawnBar = (JProgressBar*)pUIRoot->find_child(L"respawn_prog"); // value 넣엇ㅅㅅ
	//JTextCtrl* respawntxt = (JTextCtrl*)pUIRoot->find_child(L"respawn_txt");	 // text

	// 쳐맞 효과
	PlayerController::Get().m_pHitEffect = (JPanel*)pUIRoot->find_child(L"fadeout"); //1234
	PlayerController::Get().m_pRespawnEffect = (JSpriteCtrl*)pUIRoot->find_child(L"JohnSprite");// (JPanel*)pUIRoot->find_child(L"fadeout_white"); //1234
	//PlayerController::Get().m_pRespawnEffect = (JPanel*)pUIRoot->find_child(L"fadein"); //1234
	//auto pJohnEffect = (JSpriteCtrl*)pUIRoot->find_child(L"JohnSprite");

	m_pChat = (JEditCtrl*)pUIRoot->find_child(L"Chat_Edit");



	ObjectManager::Get().PushObject(pUIRoot);
	UI::InGameEvent(pUIRoot);
}