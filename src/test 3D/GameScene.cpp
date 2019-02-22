#include "GameScene.h"
#include "PacketManager.h"
#include "JEventBind.h"
#include "UIManager.h"
//#include "../../data/shader/ShaderData.hlsl"
#include "ColliderAABB.h"

bool GameScene::Init() noexcept
{
	// UI
	LoadUI();
	m_pPlayer = &PlayerController::Get();
	PlayerController::Get().Init();
	m_pPlayer->SetPosition(Vector3::Up * 500.0f + Vector3::Backward * 750.0f);
	//m_pPlayer->SetRotation(Quaternion::Up * PI * 0.8f);
	ObjectManager::Cameras[ECamera::Main]->SetRotation(Quaternion::Up * PI * 0.2f);
	FirstInit();
	ObjectManager::KeyCount = 1000;

	m_MapObjects.Init();
	for (int i = 0; i < 8; ++i)
	{
		PacketManager::Get().TowerPos[i] = m_MapObjects.Get().m_tower[i].GetPosition();
	}

	SoundManager::Get().Stop("bgm_Lobby_Theme.mp3");
	//SoundManager::Get().SetBGM("bgm_InGame_Theme.mp3");
	Timer::AccumulateTime = 0.0f;
	m_pFrameCount = &PlayerController::Get().m_GameFrameCount;
	*m_pFrameCount = 3.0f;
	// 리스폰 요청
	std::thread sendStart(&PlayerController::SendGameStart, &PlayerController::Get());
	sendStart.detach();
	m_isLoading = false;
	return true;
}


// 프레임
bool GameScene::Frame() noexcept
{
	// IME 채팅
	if (m_pChat->m_bRender)
	{
		m_chatMessage = m_pChat->GetString();
	}
	if (Input::GetKeyState(VK_RETURN) == EKeyState::DOWN)
	{
		if (m_pChat->m_bRender)
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
		}
		else
		{
			m_pChat->Play();
			m_pChat->m_bRender = true;
		}
	}
	
	static float frameBGM = 0.0f;
	frameBGM -= Timer::SPF;
	if (frameBGM <= 0.0f)
	{
		frameBGM = 180.0f;
		SoundManager::Get().Play("bgm_InGame_Theme.mp3");
	}
	// 시간 출력
	UIManager::Get().m_TimerText->m_Text = to_wstring((int)(*m_pFrameCount));
	// 호스트
	*m_pFrameCount -= Timer::SPF;
	if (PacketManager::Get().isHost)
	{
		HostFrame();
	}
	///
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	m_MapObjects.Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();

	//// 경계 막기
	//for (auto objIter = ObjectManager::Get().GetObjectList(EObjType::Character)->begin();
	//	objIter != ObjectManager::Get().GetObjectList(EObjType::Character)->end(); ++objIter)
	//{
	//	auto& position = (*objIter)->GetPosition();
	//	if (position.x > 460.0f)
	//		(*objIter)->SetPositionX(460.0f);
	//	if (position.x < -455.0f)
	//		(*objIter)->SetPositionX(-455.0f);
	//	if (position.z > 460.0f)
	//		(*objIter)->SetPositionZ(460.0f);
	//	if (position.z < -400.0f)
	//		(*objIter)->SetPositionZ(-400.0f);
	//}


	/// 재생성
	if (Input::GetKeyState(VK_NUMPAD1) == EKeyState::DOWN)
	{
		if (PlayerController::Get().GetParent() != nullptr)
			PlayerController::Get().GetParent()->m_pPhysics->UserSocket = (UINT)-1;
		PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EGuard);
	}
	if (Input::GetKeyState(VK_NUMPAD2) == EKeyState::DOWN)
	{
		if (PlayerController::Get().GetParent() != nullptr)
			PlayerController::Get().GetParent()->m_pPhysics->UserSocket = (UINT)-1;
		PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EArcher);
	}
	if (Input::GetKeyState(VK_NUMPAD3) == EKeyState::DOWN)
	{
		if (PlayerController::Get().GetParent() != nullptr)
			PlayerController::Get().GetParent()->m_pPhysics->UserSocket = (UINT)-1;
		PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EMage);
	}
	return true;
}

// 랜더
bool GameScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::GetDContext());
	m_MapObjects.Render(DxManager::GetDContext());
	//DxManager::Get().ClearDepthStencilView();
	SoundManager::Get().Render();

	// 바운딩 박스 표시
	if (!m_pCheckBox->m_bCheck)
	{
		DrawBoundingBox();
	}
	return true;
}

// 릴리즈
bool GameScene::Release() noexcept
{
	ObjectManager::Cameras[ECamera::Main]->CutParent();
	ObjectManager::Get().PopObject(ObjectManager::Cameras[ECamera::Main]);
	ObjectManager::Get().Release();
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
			if (cha._Equal(L"Paladin"))
			{
				PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EGuard);
				Packet_KeyValue p_KeyValue;
				p_KeyValue.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
				PacketManager::Get().SendPacket((char*)&p_KeyValue, (USHORT)sizeof(Packet_KeyValue), PACKET_PlayerDisable);
				return false;
			}
			else if (cha._Equal(L"Archer"))
			{
				PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EArcher);
				Packet_KeyValue p_KeyValue;
				p_KeyValue.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
				PacketManager::Get().SendPacket((char*)&p_KeyValue, (USHORT)sizeof(Packet_KeyValue), PACKET_PlayerDisable);
				return false;
			}
			else if (cha._Equal(L"Mage"))
			{
				PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EMage);
				Packet_KeyValue p_KeyValue;
				p_KeyValue.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
				PacketManager::Get().SendPacket((char*)&p_KeyValue, (USHORT)sizeof(Packet_KeyValue), PACKET_PlayerDisable);
				return false;
			}
		}
		else if (str._Equal(L"Zombie"))
		{
			PacketManager::Get().SendSpawnEnemy(L"Zombie", ESocketType::EZombie, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 1.0f, 0.25f, 0.05f);
			return false;
		}
		else if (str._Equal(L"Caster"))
		{
			PacketManager::Get().SendSpawnEnemy(L"Caster", ESocketType::ECaster, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 0.8f, 0.22f, 0.05f);
			return false;
		}
		else if (str._Equal(L"Crawler"))
		{
			PacketManager::Get().SendSpawnEnemy(L"Crawler", ESocketType::ECrawler, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 0.6f, 0.15f, 0.1f);
			return false;
		}
		else if (str._Equal(L"Mutant"))
		{
			PacketManager::Get().SendSpawnEnemy(L"Mutant", ESocketType::EMutant, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 5.0f, 0.5f, 0.1f);
			return false;
		}
		else if (str._Equal(L"Tank"))
		{
			PacketManager::Get().SendSpawnEnemy(L"Tank", ESocketType::ETank, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 15.0f * PacketManager::Get().UserList.size(), 1.1f, 0.1f);
			return false;
		}
		else if (str._Equal(L"Dead"))
		{
			if (PlayerController::Get().GetParent() == nullptr)
				return false;
			Packet_PlayerDead p_PlayerDead;
			p_PlayerDead.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
			p_PlayerDead.KillUser = (UINT)-1;
			p_PlayerDead.DeadUser = PacketManager::Get().pMyInfo->UserSocket;
			PacketManager::Get().SendPacket((char*)&p_PlayerDead, (USHORT)sizeof(Packet_PlayerDead), PACKET_PlayerDead);
			return false;
		}
		else if (str._Equal(L"Level"))
		{
			PacketManager::Get().pMyInfo->Level = atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			PlayerController::Get().m_NeedEXP = 1.0f + PacketManager::Get().pMyInfo->Level * 0.3f;
			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			return false;
		}
		else if (str._Equal(L"StatPoint"))
		{
			PlayerController::Get().m_statPoint += (UCHAR)atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			return false;
		}
		else if (str._Equal(L"StatStr"))
		{
			PacketManager::Get().pMyInfo->StatStr = atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			PlayerController::Get().UpdateStatus(false);
			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			return false;
		}
		else if (str._Equal(L"StatDex"))
		{
			PacketManager::Get().pMyInfo->StatDex = atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			PlayerController::Get().UpdateStatus(false);
			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			return false;
		}
		else if (str._Equal(L"StatInt"))
		{
			PacketManager::Get().pMyInfo->StatInt = atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			PlayerController::Get().UpdateStatus(false);
			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			return false;
		}
		else if (str._Equal(L"StatLuk"))
		{
			PacketManager::Get().pMyInfo->StatLuk = atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			PlayerController::Get().UpdateStatus(false);
			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			return false;
		}
		else if (str._Equal(L"Scale"))
		{
			if (PlayerController::Get().GetParent() == nullptr)
				return false;
			auto scale = (float)atof(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			Packet_Vector3 p_SetScale;
			p_SetScale.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
			p_SetScale.Vec3 = { scale, scale, scale };
			PacketManager::Get().SendPacket((char*)&p_SetScale, (USHORT)sizeof(Packet_Vector3), PACKET_SetScale);
			return false;
		}
		else if (str._Equal(L"HP"))
		{
			if (PlayerController::Get().GetParent() == nullptr)
				return false;
			Packet_Float p_SetHP;
			p_SetHP.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
			p_SetHP.Value = (float)atof(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			PacketManager::Get().SendPacket((char*)&p_SetHP, (USHORT)sizeof(Packet_Float), PACKET_SetHP);
			return false;
		}
		else if (str._Equal(L"Heal"))
		{
			if (PlayerController::Get().GetParent() == nullptr)
				return false;
			Packet_Float p_HealHP;
			p_HealHP.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
			p_HealHP.Value = (float)atof(WCharToChar(m_chatMessage.substr(finder + 1).c_str()));
			PacketManager::Get().SendPacket((char*)&p_HealHP, (USHORT)sizeof(Packet_Float), PACKET_HealHP);
			return false;
		}
		//else if (str._Equal(L"Clear"))
		//{
		//	return false;
		//}
		else if (str._Equal(L"ClearAll"))
		{
			for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
			{
				ObjectManager::Get().DisableObject(iter);
			}
			for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Dummy))
			{
				ObjectManager::Get().DisableObject(iter);
			}

			//for (auto& iter : ObjectManager::Get().GetColliderList())
			//{
			//	if (iter->m_pParent->m_myName == L"Melee")
			//	{
			//		ObjectManager::Get().RemoveObject(iter->m_pParent);
			//	}
			//}
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
	DxManager::Get().SetDepthStencilState(EDepthS::D_Always_S_Off);
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
	DxManager::Get().SetDepthStencilState(EDepthS::Current);
}

void GameScene::HostFrame() noexcept
{
	if (!PlayerController::Get().m_pHome->isEnable() &&
		m_eState != EGameState::End)
	{
		m_eState = EGameState::End;
		PacketManager::Get().SendPacket((char*)&PI, sizeof(Packet_KeyValue), PACKET_EndGame);
	}

	static float spawnBarrel = 0.0f;
	static float spawnShot = 0.0f;
	static float spawnShot2 = 0.0f;
	//static float spawnShot3 = 0.0f;
	//static float spawnShot4 = 0.0f;
	//spawnShot3 += Timer::SPF;
	//spawnShot4 += Timer::SPF;

	spawnBarrel += Timer::SPF;
	if (spawnBarrel >= 15.0f)
	{
		spawnBarrel = 0.0f;
		PacketManager::Get().SendTakeObject(L"Explosive", ESocketType::EDummy, (UCHAR)PacketManager::Get().UserList.size(), 0.6f, 0.3f, 0.0f, { -400.0f, 30.0f, -400.0f }, { 800.0f, 0.0f, 800.0f });
	}

	if (PacketManager::Get().TowerLevel >= 1)
	{
		// 일반
		spawnShot += Timer::SPF;
		if (spawnShot >= PacketManager::Get().TowerDelayShot)
		{
			spawnShot = 0.0f;

			Packet_TowerAttack p_Tower;
			p_Tower.KeyValue = 0;
			p_Tower.TowerLevel = PacketManager::Get().TowerLevel;
			PacketManager::Get().SendPacket((char*)&p_Tower, sizeof(Packet_TowerAttack), PACKET_TowerAttack);
		}

		if (PacketManager::Get().TowerLevel >= 5)
		{
			// 폭탄
			spawnShot2 += Timer::SPF;
			if (spawnShot2 >= PacketManager::Get().TowerDelayShot2)
			{
				spawnShot2 = 0.0f;

				Packet_TowerAttack p_Tower;
				p_Tower.KeyValue = 1;
				p_Tower.TowerLevel = PacketManager::Get().TowerLevel;
				PacketManager::Get().SendPacket((char*)&p_Tower, sizeof(Packet_TowerAttack), PACKET_TowerAttack);
			}
		}
	}

	//// 지뢰
	//if (spawnShot3 >= 21.0f)
	//{
	//	spawnShot3 = 0.0f;

	//	Packet_KeyValue p_KeyValue;
	//	p_KeyValue.KeyValue = 2;
	//	PacketManager::Get().SendPacket((char*)&p_KeyValue, sizeof(Packet_KeyValue), PACKET_TowerAttack);
	//}
	//// 미사일
	//if (spawnShot4 >= 30.0f)
	//{
	//	spawnShot4 = 0.0f;

	//	Packet_KeyValue p_KeyValue;
	//	p_KeyValue.KeyValue = 3;
	//	PacketManager::Get().SendPacket((char*)&p_KeyValue, sizeof(Packet_KeyValue), PACKET_TowerAttack);
	//}

	///
	if (*m_pFrameCount <= 0.0f)
	{
		switch (m_eState)
		{
		case EGameState::PreWait:
		{
			m_eState = EGameState::Wait;
			*m_pFrameCount = 1.0f;
		}	break;
		case EGameState::Wait:
		{
			m_eState = EGameState::WaveInit;
			*m_pFrameCount = 3.0f;
			PacketManager::Get().m_waveCount = 0;

			Packet_Float p_WaveStart;
			p_WaveStart.KeyValue = 0;
			p_WaveStart.Value = *m_pFrameCount;
			PacketManager::Get().SendPacket((char*)&p_WaveStart, sizeof(Packet_Float), PACKET_WaveCount);
		}	break;
		//case EGameState::GameStart:
		//{
		//	//m_eState = EGameState::WaveInit;
		//	//PacketManager::Get().SendPacket((char*)&PI, sizeof(PI), PACKET_StartGame);
		//}	break;
		case EGameState::WaveInit:
		{
			m_eState = EGameState::Spawn;
			++PacketManager::Get().m_waveCount;
			m_spawnCount = 4;

			Packet_Float p_WaveStart;
			p_WaveStart.KeyValue = PacketManager::Get().m_waveCount;
			p_WaveStart.Value = *m_pFrameCount;
			PacketManager::Get().SendPacket((char*)&p_WaveStart, sizeof(Packet_Float), PACKET_WaveStart);
		}	break;
		case EGameState::Spawn:
		{
			switch (PacketManager::Get().m_waveCount)
			{
			case 1:
			{
				PacketManager::Get().SendSpawnEnemy(L"Zombie", ESocketType::EZombie, (UCHAR)PacketManager::Get().UserList.size() * 2, 1.0f, 0.25f, 0.05f);
				PacketManager::Get().SendSpawnEnemy(L"Caster", ESocketType::ECaster, (UCHAR)PacketManager::Get().UserList.size(), 0.8f, 0.22f, 0.05f);
				PacketManager::Get().SendSpawnEnemy(L"Crawler", ESocketType::ECrawler, (UCHAR)PacketManager::Get().UserList.size(), 0.6f, 0.2f, 0.05f);
			}	break;
			case 2:
			{
				PacketManager::Get().SendSpawnEnemy(L"Zombie", ESocketType::EZombie, (UCHAR)PacketManager::Get().UserList.size(), 1.0f, 0.25f, 0.05f);
				PacketManager::Get().SendSpawnEnemy(L"Caster", ESocketType::ECaster, (UCHAR)PacketManager::Get().UserList.size() * 2, 0.8f, 0.22f, 0.05f);
				PacketManager::Get().SendSpawnEnemy(L"Crawler", ESocketType::ECrawler, (UCHAR)PacketManager::Get().UserList.size(), 0.6f, 0.2f, 0.05f);
				PacketManager::Get().SendSpawnEnemy(L"Mutant", ESocketType::EMutant, 1, 5.0f, 0.5f, 0.1f);
			}	break;
			case 3:
			{
				if(m_spawnCount == 2)
					PacketManager::Get().SendSpawnEnemy(L"Tank", ESocketType::ETank, 1, 15.0f * PacketManager::Get().UserList.size(), 1.1f, 0.1f);
				PacketManager::Get().SendSpawnEnemy(L"Zombie", ESocketType::EZombie, (UCHAR)PacketManager::Get().UserList.size(), 1.0f, 0.25f, 0.05f);
				PacketManager::Get().SendSpawnEnemy(L"Caster", ESocketType::ECaster, (UCHAR)PacketManager::Get().UserList.size(), 0.8f, 0.22f, 0.05f);
				PacketManager::Get().SendSpawnEnemy(L"Crawler", ESocketType::ECrawler, (UCHAR)PacketManager::Get().UserList.size() * 2, 0.6f, 0.2f, 0.05f);
			}	break;
			case 4:
			{
				if (m_spawnCount == 2)
					PacketManager::Get().SendSpawnEnemy(L"Tank", ESocketType::ETank, 1, 15.0f * PacketManager::Get().UserList.size(), 1.1f, 0.1f);
				PacketManager::Get().SendSpawnEnemy(L"Mutant", ESocketType::EMutant, (UCHAR)PacketManager::Get().UserList.size(), 5.0f, 0.5f, 0.1f);
				PacketManager::Get().SendSpawnEnemy(L"Caster", ESocketType::ECaster, (UCHAR)PacketManager::Get().UserList.size(), 0.8f, 0.22f, 0.05f);
				PacketManager::Get().SendSpawnEnemy(L"Crawler", ESocketType::ECrawler, (UCHAR)PacketManager::Get().UserList.size(), 0.6f, 0.15f, 0.1f);
			}	break;
			case 5:
			{
				if (m_spawnCount >= 2)
				{
					PacketManager::Get().SendSpawnEnemy(L"Tank", ESocketType::ETank, 1, 15.0f * PacketManager::Get().UserList.size(), 1.1f, 0.1f);
					UIManager::Get().m_TimerText->m_bRender = false;
				}
			}	break;
			case 6:
			{
				//PacketManager::Get().SendPacket((char*)&PI, sizeof(Packet_KeyValue), PACKET_EndGame);
			}	break;
			}

			// 클리어 조건
			if (PacketManager::Get().m_waveCount >= 5 && m_spawnCount <= 1)
			{
				if (ObjectManager::Get().GetObjectList(EObjType::Enemy)->empty())
				{
					m_eState = EGameState::End;
					PacketManager::Get().SendPacket((char*)&PI, sizeof(Packet_KeyValue), PACKET_EndGame);
				}
				*m_pFrameCount = 2.0f;
				return;
			}

			*m_pFrameCount = 15.0f;
			--m_spawnCount;
			if (m_spawnCount <= 0)
			{
				m_eState = EGameState::WaveInit;
				*m_pFrameCount = 60.0f;
			}
			Packet_Float p_WaveStart;
			p_WaveStart.KeyValue = m_spawnCount;
			p_WaveStart.Value = *m_pFrameCount;
			PacketManager::Get().SendPacket((char*)&p_WaveStart, sizeof(Packet_Float), PACKET_WaveCount);
		}	break;
		}
	}
}


bool GameScene::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		//
		// 라이트
		//auto pTrans = new CTransformer(Vector3::Up * 550.0f, Quaternion::Down * PI * 0.35f, Vector3::One);
		//pTrans->TransEvent = [](Transform* pParent, Transform* pTrans, const float& spf, const float& accTime) {
		//	pParent->SetTransform(*pTrans);
		//	pParent->Translate({ cosf(0.2f * accTime) * 800.0f, 0.0f, sinf(0.2f * accTime) * 800.0f });
		//	return; spf; accTime; pTrans;
		//};
		//ObjectManager::Get().Lights.front()->AddComponent(pTrans);
		ObjectManager::Get().Lights.front()->SetRotation(Quaternion::Up * PI * 0.65f);
		ObjectManager::Get().Lights.front()->SetPosition(0.0f, 550.0f, 800.0f);
		//ObjectManager::Get().Lights.front()->SetFocus(Vector3::Zero);
		// 라이트 랜더러
		auto pShpere = (Renderer*)ObjectManager::GetInstance().TakeComponent(L"RowSphere");
		pShpere->SetShaderLayout("VS_Basic", "PS_Basic");
		auto pObject = new GameObject(L"Sun", pShpere);
		pObject->isGlobal(true);
		pObject->SetScale(Vector3::One * 10);
		pObject->SetParent(ObjectManager::Get().Lights.front());
		///
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
	PlayerController::Get().pUIManager = &UIManager::Get();
	// HP, MP
	UIManager::Get().m_pHpBar = (JProgressBar*)pUIRoot->find_child(L"HP_Progress");
	UIManager::Get().m_pMpBar = (JProgressBar*)pUIRoot->find_child(L"MP_Progress");
	UIManager::Get().m_FightPanel = (JPanel*)pUIRoot->find_child(L"fight_panel");
	UIManager::Get().m_pStatePanel = (JPanel*)pUIRoot->find_child(L"State_Panel");
	//UIManager::Get().m_pMpBar->m_pShape->m_cbData.vColor;

	// 상황판
	/* Name Kill Death Score */
	//m_pUser1->PreEvent.first; // bRender 상관없이 돌아감
	PacketManager::Get().pUserPanel[0] = (JPanel*)pUIRoot->find_child(L"State_User1");
	PacketManager::Get().pUserPanel[0]->PostEvent.first = [](void* pVoid) {	 // bRender가 트루일때만 돌아감
		if (PacketManager::Get().UserList.empty())
			return;
		auto iter = ((JPanel*)pVoid)->m_pChildList.begin();
		auto pUser = PacketManager::Get().UserList.begin()->second;
		((JTextCtrl*)(*iter))->m_Text = pUser->UserID;
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->KillCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->DeathCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->Score);
		((JTextCtrl*)(*++++iter))->m_bRender = pUser->isDead;
	};
	PacketManager::Get().pUserPanel[0]->PostEvent.second = PacketManager::Get().pUserPanel[0];

	PacketManager::Get().pUserPanel[1] = (JPanel*)pUIRoot->find_child(L"State_User2");
	PacketManager::Get().pUserPanel[1]->PostEvent.first = [](void* pVoid) {
		if (PacketManager::Get().UserList.size() < 2)
			return;
		auto iter = ((JPanel*)pVoid)->m_pChildList.begin();
		auto pUser = (++PacketManager::Get().UserList.begin())->second;
		((JTextCtrl*)(*iter))->m_Text = pUser->UserID;
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->DeathCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->KillCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->Score);
		((JTextCtrl*)(*++++iter))->m_bRender = pUser->isDead;
	};
	PacketManager::Get().pUserPanel[1]->PostEvent.second = PacketManager::Get().pUserPanel[1];

	PacketManager::Get().pUserPanel[2] = (JPanel*)pUIRoot->find_child(L"State_User3");
	PacketManager::Get().pUserPanel[2]->PostEvent.first = [](void* pVoid) {
		if (PacketManager::Get().UserList.size() < 3)
			return;
		auto iter = ((JPanel*)pVoid)->m_pChildList.begin();
		auto pUser = (++++PacketManager::Get().UserList.begin())->second;
		((JTextCtrl*)(*iter))->m_Text = pUser->UserID;
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->DeathCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->KillCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->Score);
		((JTextCtrl*)(*++++iter))->m_bRender = pUser->isDead;
	};
	PacketManager::Get().pUserPanel[2]->PostEvent.second = PacketManager::Get().pUserPanel[2];

	PacketManager::Get().pUserPanel[3] = (JPanel*)pUIRoot->find_child(L"State_User4");
	PacketManager::Get().pUserPanel[3]->PostEvent.first = [](void* pVoid) {
		if (PacketManager::Get().UserList.size() < 4)
			return;
		auto iter = ((JPanel*)pVoid)->m_pChildList.begin();
		auto pUser = (++++++PacketManager::Get().UserList.begin())->second;
		((JTextCtrl*)(*iter))->m_Text = pUser->UserID;
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->DeathCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->KillCount);
		((JTextCtrl*)(*++iter))->m_Text = to_wstring(pUser->Score);
		((JTextCtrl*)(*++++iter))->m_bRender = pUser->isDead;
	};
	PacketManager::Get().pUserPanel[3]->PostEvent.second = PacketManager::Get().pUserPanel[3];
	// 마우스, 전광판
	UIManager::Get().m_pMouseIcon = pUIRoot->find_child(L"mouse_cursor");
	UIManager::Get().m_pMouseIcon->m_bRender = false;
	PacketManager::Get().pKillDisplay = (JListCtrl*)pUIRoot->find_child(L"KilltoDeath");
	// Skill Icon
	UIManager::Get().m_pLeftIcon = (JProgressBar*)pUIRoot->find_child(L"Skill_Left");
	UIManager::Get().m_pRightIcon = (JProgressBar*)pUIRoot->find_child(L"Skill_Right");
	// 옵션
	UIManager::Get().m_pOptionPanel = (JPanel*)pUIRoot->find_child(L"Set_Panel");
	// Option Volume
	auto pVolume = (JSliderCtrl*)pUIRoot->find_child(L"Set_Volum");
	pVolume->SetValue(SoundManager::Get().m_masterVolume);
	SoundManager::Get().m_masterVolume = 0.5f;
	// Mouse
	auto pMouseSense = (JSliderCtrl*)pUIRoot->find_child(L"Set_Mouse");
	pMouseSense->SetValue(PlayerController::Get().m_mouseSense);
	// 음영
	auto pLightOption = (JSliderCtrl*)pUIRoot->find_child(L"Set_Light");
	pLightOption->SetValue(PlayerController::Get().m_LightAmount);

	// Exit
	auto pExit = (JTextCtrl*)pUIRoot->find_child(L"Set_GameExit");
	pExit->EventClick.first = [](void* pScene) {
		Core::isPlaying = false;
		pScene;
		//PlayerController::Get().CutParent();
		//PacketManager::Get().pSender->Release();
		//((MainClass*)pScene)->SetScene(ESceneName::Lobby);
	};
	pExit->EventClick.second = this;
	// 충돌 박스
	m_pCheckBox = (JCheckCtrl*)pUIRoot->find_child(L"Set_Collision_Check");
	m_pCheckBox->SetCheck(true);
	// 그래픽 체크
	auto pLowGraphic = (JCheckCtrl*)pUIRoot->find_child(L"Set_LowMode_Check");
	pLowGraphic->SetCheck(true);
	pLowGraphic->EventClick.first = [](void* pVoid) {
		auto pCheck = (JCheckCtrl*)pVoid;
		if (pCheck->GetCheck())
		{
			for (auto& [name, iter] : ObjectManager::Get().m_ProtoPull)
			{
				if (auto pList = iter->GetComponentList(EComponent::Renderer);
					pList != nullptr)
				{
					for (auto& pRenderer : *pList)
					{
						((Renderer*)pRenderer)->SetShadowRate(0.0f);
						((Renderer*)pRenderer)->SetLightRate(1.0f);
						((Renderer*)pRenderer)->SetEnviromentMap(L"");
					}
				}
			}
			for (auto&[eType, objList] : ObjectManager::Get().m_ObjectList)
			{
				for (auto& iter : objList)
				{
					if (auto pList = iter->GetComponentList(EComponent::Renderer);
						pList != nullptr)
					{
						for (auto& pRenderer : *pList)
						{
							((Renderer*)pRenderer)->SetShadowRate(0.0f);
							((Renderer*)pRenderer)->SetLightRate(1.0f);
							((Renderer*)pRenderer)->SetEnviromentMap(L"");
						}
					}
				}
			}
		}
		else
		{
			auto skySrc = ((Renderer*)m_pSkyBox->GetComponent(EComponent::Renderer))->m_srcName;
			for (auto&[name, iter] : ObjectManager::Get().m_ProtoPull)
			{
				if (auto pList = iter->GetComponentList(EComponent::Renderer);
					pList != nullptr)
				{
					for (auto& pRenderer : *pList)
					{
						((Renderer*)pRenderer)->SetShadowRate(0.5f);
						((Renderer*)pRenderer)->SetLightRate(0.3f);
						((Renderer*)pRenderer)->SetEnviromentMap(skySrc, EEnviType::Refraction);
					}
				}
			}
			for (auto&[eType, objList] : ObjectManager::Get().m_ObjectList)
			{
				for (auto& iter : objList)
				{
					if (auto pList = iter->GetComponentList(EComponent::Renderer);
						pList != nullptr)
					{
						for (auto& pRenderer : *pList)
						{
							((Renderer*)pRenderer)->SetShadowRate(0.5f);
							((Renderer*)pRenderer)->SetLightRate(0.3f);
							((Renderer*)pRenderer)->SetEnviromentMap(skySrc, EEnviType::Refraction);
						}
					}
				}
			}
		}
	};
	pLowGraphic->EventClick.second = pLowGraphic;
	// Timer
	UIManager::Get().m_TimerText = (JTextCtrl*)pUIRoot->find_child(L"Timer_Text");

	// Chatting
	PacketManager::Get().pChatList = (JListCtrl*)pUIRoot->find_child(L"Chat_Log");
	PacketManager::Get().pChatList->EventHover.first = [](void* pVoid) {
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
			}
		}
	};
	PacketManager::Get().pChatList->EventHover.second = PacketManager::Get().pChatList;
	//JSliderCtrl* pSlider = (JSliderCtrl*)pUIRoot->find_child(L"Chat_Slider");
	//m_pList->m_fValue = pSlider->GetValue();

	// 리스폰창
	UIManager::Get().m_pRespawn = (JPanel*)pUIRoot->find_child(L"respawn_panel");		 // bRender용
	UIManager::Get().m_pRespawn->m_bRender = false;
	UIManager::Get().m_pRespawnBar = (JProgressBar*)pUIRoot->find_child(L"respawn_prog"); // value 넣엇ㅅㅅ
	//JTextCtrl* respawntxt = (JTextCtrl*)pUIRoot->find_child(L"respawn_txt");	 // text

	// 쳐맞 효과
	UIManager::Get().m_pHitEffect = (JPanel*)pUIRoot->find_child(L"fadeout");
	UIManager::Get().m_pRespawnEffect = (JPanel*)pUIRoot->find_child(L"fadeout_white");
	UIManager::Get().m_pGreenEffect = (JPanel*)pUIRoot->find_child(L"fadeout_green");

	UIManager::Get().m_pChat = m_pChat = (JEditCtrl*)pUIRoot->find_child(L"Chat_Edit");
	// 적 체력바
	UIManager::Get().m_pEnemyPanel = (JPanel*)pUIRoot->find_child(L"Enemy_Panel"); //bRender true/false
	UIManager::Get().m_pEnemyPanel->m_bRender = false;
	UIManager::Get().m_pEnemyHP = (JProgressBar*)pUIRoot->find_child(L"Enemy_HP");
	UIManager::Get().m_pEnemyName = (JTextCtrl*)pUIRoot->find_child(L"Enemy_Name");
	UIManager::Get().m_pEnemyHPText = (JTextCtrl*)pUIRoot->find_child(L"Enemy_HP_txt");

	// 인포창
	UIManager::Get().m_pInfoPanel = (JPanel*)pUIRoot->find_child(L"Info_Panel");
	UIManager::Get().m_pExpProgress = (JProgressBar*)pUIRoot->find_child(L"Exp_Progress");
	UIManager::Get().m_pInfoTitle = (JTextCtrl*)pUIRoot->find_child(L"Info_Title");
	UIManager::Get().m_pInfoHP = (JTextCtrl*)pUIRoot->find_child(L"Info_HP");
	UIManager::Get().m_pInfoMP = (JTextCtrl*)pUIRoot->find_child(L"Info_MP");
	UIManager::Get().m_pInfoEXP = (JTextCtrl*)pUIRoot->find_child(L"Info_EXP");
	///////
	UIManager::Get().m_pInfoAttackSpeed = (JTextCtrl*)pUIRoot->find_child(L"Info_AttackSpeed");
	UIManager::Get().m_pInfoMoveSpeed = (JTextCtrl*)pUIRoot->find_child(L"Info_MoveSpeed");
	UIManager::Get().m_pInfoLevel = (JTextCtrl*)pUIRoot->find_child(L"Info_Level");
	UIManager::Get().m_pInfoDamage = (JTextCtrl*)pUIRoot->find_child(L"Info_Damage");
	UIManager::Get().m_pInfoArmor = (JTextCtrl*)pUIRoot->find_child(L"Info_Armor");
	//auto pArmor = (JTextCtrl*)pUIRoot->find_child(L"Info_Armor_txt");
	//pArmor->SetString(L"리스폰");
	auto pText = (JTextCtrl*)pUIRoot->find_child(L"Info_MoveSpeed_txt");
	pText->SetString(L"획득률");
	UIManager::Get().m_pInfoSP = (JTextCtrl*)pUIRoot->find_child(L"Info_SP");
	UIManager::Get().m_pInfoStr = (JTextCtrl*)pUIRoot->find_child(L"Info_STR");
	UIManager::Get().m_pInfoDex = (JTextCtrl*)pUIRoot->find_child(L"Info_DEX");
	UIManager::Get().m_pInfoInt = (JTextCtrl*)pUIRoot->find_child(L"Info_INT");
	UIManager::Get().m_pInfoLuk = (JTextCtrl*)pUIRoot->find_child(L"Info_LUK");
	auto pStatUp = [](void* pStat) {
		SoundManager::Get().Play("SE_Click01.mp3");
		if (PlayerController::Get().m_statPoint >= 1)
		{
			--PlayerController::Get().m_statPoint;
			++(*(UCHAR*)pStat);
			PlayerController::Get().UpdateStatus();
			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
		}
	};
	UIManager::Get().m_pInfoStrBtn = (JTextCtrl*)pUIRoot->find_child(L"Info_STR_Btn");
	UIManager::Get().m_pInfoStrBtn->EventClick.first = pStatUp;
	UIManager::Get().m_pInfoStrBtn->EventClick.second = &PacketManager::Get().pMyInfo->StatStr;
	UIManager::Get().m_pInfoDexBtn = (JTextCtrl*)pUIRoot->find_child(L"Info_DEX_btn");
	UIManager::Get().m_pInfoDexBtn->EventClick.first = pStatUp;
	UIManager::Get().m_pInfoDexBtn->EventClick.second = &PacketManager::Get().pMyInfo->StatDex;
	UIManager::Get().m_pInfoIntBtn = (JTextCtrl*)pUIRoot->find_child(L"Info_INT_btn");
	UIManager::Get().m_pInfoIntBtn->EventClick.first = pStatUp;
	UIManager::Get().m_pInfoIntBtn->EventClick.second = &PacketManager::Get().pMyInfo->StatInt;
	UIManager::Get().m_pInfoLukBtn = (JTextCtrl*)pUIRoot->find_child(L"Info_LUK_btn");
	UIManager::Get().m_pInfoLukBtn->EventClick.first = pStatUp;
	UIManager::Get().m_pInfoLukBtn->EventClick.second = &PacketManager::Get().pMyInfo->StatLuk;
	// 인벤, 슬롯
	UIManager::Get().m_pInvenSlot = (JInventory*)pUIRoot->find_child(L"Inventory_Slot");
	UIManager::Get().m_pInvenPanel = (JPanel*)pUIRoot->find_child(L"Inventory_Panel");
	UIManager::Get().m_pInvenMoney = (JTextCtrl*)pUIRoot->find_child(L"Inventory_Money");

	UIManager::Get().m_pInvenWeapon = (JPanel*)pUIRoot->find_child(L"Inventory_Weapon_Back");
	UIManager::Get().m_pInvenArmor = (JPanel*)pUIRoot->find_child(L"Inventory_Armor_Back");
	UIManager::Get().m_pInvenAcce1 = (JPanel*)pUIRoot->find_child(L"Inventory_Accessories_Back");
	UIManager::Get().m_pInvenAcce2 = (JPanel*)pUIRoot->find_child(L"Inventory_Accessories2_Back");

	// 인벤토리 정보
	UIManager::Get().m_pInvenInfoName = (JTextCtrl*)pUIRoot->find_child(L"Item_Info_Name");
	UIManager::Get().m_pInvenInfoEffect = (JTextCtrl*)pUIRoot->find_child(L"Item_Info_Effect");
	UIManager::Get().m_pInvenInfoReinforce = (JTextCtrl*)pUIRoot->find_child(L"Item_Info_Reinforce");

	UIManager::Get().m_pSlot1 = (JSlot*)pUIRoot->find_child(L"Slot1");
	UIManager::Get().m_pSlot2 = (JSlot*)pUIRoot->find_child(L"Slot2");
	UIManager::Get().m_pSlot3 = (JSlot*)pUIRoot->find_child(L"Slot3");
	 //UIManager::Get().m_pSlot3->m_pItem->m_pIndexList[0] = -1;
	//UIManager::Get().m_pSlot1->AddItem(L"");
	//UIManager::Get().m_pSlot1->m_bEmpty; // > AddItem(L"");

	// 이름 변경
	UIManager::Get().m_pInfoName = (JEditCtrl*)pUIRoot->find_child(L"Info_Name");
	auto pNameChange = (JButtonCtrl*)pUIRoot->find_child(L"Info_Name_Change_Btn");
	pNameChange->EventClick.first = [](void* pScene) {
		// 아이디 변경
		auto pUser = PacketManager::Get().pMyInfo;
		const wstring preName = pUser->UserID;
		ZeroMemory(pUser->UserID, 20);
		pUser->DataSize = (UCHAR)UIManager::Get().m_pInfoName->GetString().size() * 2;
		pUser->DataSize = pUser->DataSize > 12 ? 12 : pUser->DataSize;
		memcpy(pUser->UserID, UIManager::Get().m_pInfoName->GetString().c_str(), pUser->DataSize);
		PacketManager::Get().SendPacket((char*)pUser, (USHORT)(PS_UserInfo + pUser->DataSize), PACKET_SendNameUpdate);
		///
		UIManager::Get().m_pInfoName->SetString(pUser->UserID);
		PacketManager::Get().pChatList->push_string(L"[System] '"s + preName + L"' 님의 아이디가" + pUser->UserID + L" 로 변경 되였습니다.");
		//// 디비 변경
		//if(m_loginCheck == 1)
		//	((GameScene*)pScene)->RequestUpdateUsername(preName.c_str(), pUser->UserID);
	};
	pNameChange->EventClick.second = this;


	UIManager::Get().m_pXPush = (JImageCtrl*)pUIRoot->find_child(L"KeyPush_Img"); // F
	// 상점
	UIManager::Get().m_pShopPanel = (JPanel*)pUIRoot->find_child(L"Shop_Panel");
	UIManager::Get().m_pShopItem0 = (JTextCtrl*)pUIRoot->find_child(L"Shop_Item0_Btn");
	UIManager::Get().m_pShopItem0->EventClick.first = [](void*) {
		if(PlayerController::Get().m_money >= 500)
		{
			PlayerController::Get().m_money -= 500;
			UIManager::Get().AddSlotItem(L"Bomb1");
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pShopItem1 = (JTextCtrl*)pUIRoot->find_child(L"Shop_Item1_Btn");
	UIManager::Get().m_pShopItem1->EventClick.first = [](void*) {
		if (PlayerController::Get().m_money >= 500)
		{
			PlayerController::Get().m_money -= 500;
			UIManager::Get().AddSlotItem(L"Bomb2");
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pShopItem2 = (JTextCtrl*)pUIRoot->find_child(L"Shop_Item2_Btn");
	UIManager::Get().m_pShopItem2->EventClick.first = [](void*) {
		if (PlayerController::Get().m_money >= 500)
		{
			PlayerController::Get().m_money -= 500;
			UIManager::Get().AddSlotItem(L"Bomb3");
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pShopItem3 = (JTextCtrl*)pUIRoot->find_child(L"Shop_Item3_Btn");
	UIManager::Get().m_pShopItem3->EventClick.first = [](void*) {
		if (PlayerController::Get().m_money >= 500)
		{
			PlayerController::Get().m_money -= 500;
			UIManager::Get().AddSlotItem(L"LandMine");
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pShopItem4 = (JTextCtrl*)pUIRoot->find_child(L"Shop_Item4_Btn");
	UIManager::Get().m_pShopItem4->EventClick.first = [](void*) {
		if (PlayerController::Get().m_money >= 500)
		{
			PlayerController::Get().m_money -= 500;
			UIManager::Get().AddSlotItem(L"Missile");
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pShopItem5 = (JTextCtrl*)pUIRoot->find_child(L"Shop_Item5_Btn");
	UIManager::Get().m_pShopItem5->EventClick.first = [](void*) {
		if (PlayerController::Get().m_money >= 500)
		{
			PlayerController::Get().m_money -= 500;
			UIManager::Get().AddSlotItem(L"Barge");
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pShopItem6 = (JTextCtrl*)pUIRoot->find_child(L"Shop_Item6_Btn");
	UIManager::Get().m_pShopItem6->EventClick.first = [](void*) {
		if (PlayerController::Get().m_money >= 500)
		{
			PlayerController::Get().m_money -= 500;
			UIManager::Get().AddSlotItem(L"TNT");
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pShopItem7 = (JTextCtrl*)pUIRoot->find_child(L"Shop_Item7_Btn");
	UIManager::Get().m_pShopItem7->EventClick.first = [](void*) {
		if (PlayerController::Get().m_money >= 500)
		{
			PlayerController::Get().m_money -= 500;
			UIManager::Get().AddSlotItem(L"Nuclear");
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	// 타워
	UIManager::Get().m_pTowerPanel = (JPanel*)pUIRoot->find_child(L"Tower_Panel");
	UIManager::Get().m_pTowerCurLevel = (JTextCtrl*)pUIRoot->find_child(L"Tower_CurrentLv");
	UIManager::Get().m_pTowerCurAtkDamage = (JTextCtrl*)pUIRoot->find_child(L"Tower_CurrentAtk");
	UIManager::Get().m_pTowerCurAtkSpeed = (JTextCtrl*)pUIRoot->find_child(L"Tower_CurrentAtkSpeed");
	UIManager::Get().m_pTowerText1 = (JTextCtrl*)pUIRoot->find_child(L"Tower_Explanation0");

	UIManager::Get().m_pTowerNextLevel = (JTextCtrl*)pUIRoot->find_child(L"Tower_NextLv");
	UIManager::Get().m_pTowerNextAtkDamage = (JTextCtrl*)pUIRoot->find_child(L"Tower_NextAtk");
	UIManager::Get().m_pTowerNextAtkSpeed = (JTextCtrl*)pUIRoot->find_child(L"Tower_NextAtkSpeed");
	UIManager::Get().m_pTowerText2 = (JTextCtrl*)pUIRoot->find_child(L"Tower_Explanation1");
	UIManager::Get().m_pTowerUpgrade = (JTextCtrl*)pUIRoot->find_child(L"Tower_Btn");
	UIManager::Get().m_pTowerUpgrade->EventClick.first = [](void*) {
		auto needMoney = 2000 + 2000 * PacketManager::Get().TowerLevel;
		if (PlayerController::Get().m_money >= needMoney)
		{
			PlayerController::Get().m_money -= needMoney;
			PacketManager::Get().SendPacket((char*)&PI, sizeof(PI), PACKET_TowerUpgrade);
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	// 대장간
	UIManager::Get().m_pSmithyPanel = (JPanel*)pUIRoot->find_child(L"Smithy_Panel");
	UIManager::Get().m_pSmithyInfo1Weapon = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Weapon_Info_Text0");
	UIManager::Get().m_pSmithyInfo2Weapon = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Weapon_Info_Text1");
	UIManager::Get().m_pSmithyInfo1Armor = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Armor_Info_Text0");
	UIManager::Get().m_pSmithyInfo2Armor = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Armor_Info_Text1");
	UIManager::Get().m_pSmithyInfo1Acce1 = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Accessories_Info_Text0");
	UIManager::Get().m_pSmithyInfo2Acce1 = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Accessories_Info_Text1");
	UIManager::Get().m_pSmithyInfo1Acce2 = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Accessories2_Info_Text0");
	UIManager::Get().m_pSmithyInfo2Acce2 = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Accessories2_Info_Text1");
	UIManager::Get().m_pSmithyBtnWeapon = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Weapon_Btn");
	UIManager::Get().m_pSmithyBtnWeapon->EventClick.first = [](void*) {
		auto needMoney = 2000 + 2000 * PlayerController::Get().m_upgradeWeapon;
		if (PlayerController::Get().m_money >= needMoney)
		{
			PlayerController::Get().m_money -= needMoney;
			++PlayerController::Get().m_upgradeWeapon;
			PlayerController::Get().UpdateStatus();
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pSmithyBtnArmor = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Armor_Btn");
	UIManager::Get().m_pSmithyBtnArmor->EventClick.first = [](void*) {
		auto needMoney = 2000 + 2000 * PlayerController::Get().m_upgradeArmor;
		if (PlayerController::Get().m_money >= needMoney)
		{
			PlayerController::Get().m_money -= needMoney;
			++PlayerController::Get().m_upgradeArmor;
			if (PlayerController::Get().GetParent() != nullptr)
				PlayerController::Get().GetParent()->SetArmor(PlayerController::Get().m_defencePoint + PlayerController::Get().m_upgradeArmor);
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pSmithyBtnAcce1 = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Accessories_Btn");
	UIManager::Get().m_pSmithyBtnAcce1->EventClick.first = [](void*) {
		auto needMoney = 2000 + 2000 * PlayerController::Get().m_upgradeAcce1;
		if (PlayerController::Get().m_money >= needMoney)
		{
			PlayerController::Get().m_money -= needMoney;
			++PlayerController::Get().m_upgradeAcce1;
			PlayerController::Get().UpdateStatus(false);
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	UIManager::Get().m_pSmithyBtnAcce2 = (JTextCtrl*)pUIRoot->find_child(L"Smithy_Accessories2_Btn");
	UIManager::Get().m_pSmithyBtnAcce2->EventClick.first = [](void*) {
		auto needMoney = 10000 + 10000 * PlayerController::Get().m_upgradeAcce2;
		if (PlayerController::Get().m_money >= needMoney)
		{
			PlayerController::Get().m_money -= needMoney;
			++PlayerController::Get().m_upgradeAcce2;
			++PacketManager::Get().pMyInfo->StatStr;
			++PacketManager::Get().pMyInfo->StatDex;
			++PacketManager::Get().pMyInfo->StatInt;
			++PacketManager::Get().pMyInfo->StatLuk;
			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
		}
		else {
			UIManager::Get().m_pHelpTextPanel->m_bRender = true;
		}
	};
	///
	// 돈 부족
	UIManager::Get().m_pHelpTextPanel = (JPanel*)pUIRoot->find_child(L"HelpText")->m_pParent;
	UIManager::Get().m_pHelpTextPanel->m_bRender = false;


	ObjectManager::Get().PushObject(pUIRoot);
	UI::InGameEvent(pUIRoot);
}