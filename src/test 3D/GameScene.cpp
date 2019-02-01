#include "GameScene.h"
#include "PacketManager.h"
#include "JEventBind.h"
#include "UIManager.h"


bool GameScene::Init() noexcept
{
	// UI
	LoadUI();
	m_pPlayer = &PlayerController::Get();
	PlayerController::Get().Init();
	m_pPlayer->SetPosition(Vector3::Up * 100.0f + Vector3::Backward * 50.0f);
	FirstInit();
	ObjectManager::KeyCount = 1000;

	auto pCollider = new Collider(1.0f);
	pCollider->m_eTag = ETag::Ally;
	PlayerController::Get().m_pHome = new GameObject(L"Shelter", { pCollider /*, ObjectManager::Get().TakeComponent(L"RowSphere")*/ });
	PlayerController::Get().m_pHome->SetPosition(-15.8f, 18.0f, 41.5f);
	PlayerController::Get().m_pHome->SetScale(Vector3::One * 20.0f);
	PlayerController::Get().m_pHome->SetGravityScale(0.0f);
	PlayerController::Get().m_pHome->usePhysics(false);
	PlayerController::Get().m_pHome->SetHP(100.0f);
	//pCollider->CollisionEvent = 
	ObjectManager::Get().PushObject(PlayerController::Get().m_pHome);


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

	SoundManager::Get().Stop("bgm_Lobby_Theme.mp3");
	//SoundManager::Get().SetBGM("bgm_InGame_Theme.mp3");
	Timer::AccumulateTime = 0.0f;
	//m_Rule.SetReadyTime(7.0f);
	//m_Rule.SetPlayTime(180.0f);
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
	UIManager::Get().m_TimerText->m_Text = to_wstring(*m_pFrameCount).substr(0, 5);
	// 호스트
	*m_pFrameCount -= Timer::SPF;
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
		iter->m_mapHeight = m_pMap->GetHeight(iter->m_pParent->GetPosition().x, iter->m_pParent->GetPosition().z);
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
	//m_Rule.Release();
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
				return false;
			}
			else if (cha._Equal(L"Archer"))
			{
				PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EArcher);
				return false;
			}
			else if (cha._Equal(L"Mage"))
			{
				PlayerController::Get().SendReqRespawn(PlayerController::ECharacter::EMage);
				return false;
			}
		}
		else if (str._Equal(L"Dance"))
		{
			if (PlayerController::Get().GetParent() == nullptr)
				return false;
			switch (atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())))
			{
			case 1:
			{
				PlayerController::Get().SendAnimTransform(PlayerController::EAction::Dance1, PlayerController::Get().m_curCharacter);
			}	break;
			case 2:
			{
				PlayerController::Get().SendAnimTransform(PlayerController::EAction::Dance2, PlayerController::Get().m_curCharacter);
			}	break;
			case 3:
			{
				PlayerController::Get().SendAnimTransform(PlayerController::EAction::Dance3, PlayerController::Get().m_curCharacter);
			}	break;
			}
			return false;
		}
		else if (str._Equal(L"Zombie"))
		{
			PacketManager::Get().SendTakeObject(L"Zombie", ESocketType::EZombie, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 1.0f, 0.1f, 0.2f, { -500.0f, 0.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			return false;
		}
		else if (str._Equal(L"Caster"))
		{
			PacketManager::Get().SendTakeObject(L"Caster", ESocketType::ECaster, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 0.8f, 0.1f, 0.2f, { -500.0f, 0.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			return false;
		}
		else if (str._Equal(L"Crawler"))
		{			
			PacketManager::Get().SendTakeObject(L"Crawler", ESocketType::ECrawler, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 0.6f, 0.15f, 0.1f, { -500.0f, 0.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			return false;
		}
		else if (str._Equal(L"Mutant"))
		{
			PacketManager::Get().SendTakeObject(L"Mutant", ESocketType::EMutant, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 5.0f, 0.5f, 0.1f, { -500.0f, 0.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			return false;
		}
		else if (str._Equal(L"Tank"))
		{
			PacketManager::Get().SendTakeObject(L"Tank", ESocketType::ETank, atoi(WCharToChar(m_chatMessage.substr(finder + 1).c_str())), 15.0f * PacketManager::Get().UserList.size(), 1.1f, 0.1f, { -500.0f, 0.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
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
		else if (str._Equal(L"Clear"))
		{
			/*for (auto& iter : ObjectManager::Get().GetColliderList())
			{
				if (iter->m_pParent->m_myName == L"Melee")
				{
					ObjectManager::Get().RemoveObject(iter->m_pParent);
				}
			}*/
			return false;
		}
		else if (str._Equal(L"ClearAll"))
		{
			for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
			{
				ObjectManager::Get().DisableObject(iter);
			}
			/*for (auto& iter : ObjectManager::Get().GetColliderList())
			{
				if (iter->m_pParent->m_myName == L"Melee")
				{
					ObjectManager::Get().RemoveObject(iter->m_pParent);
				}
			}*/
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
	if (PlayerController::Get().m_pHome->GetHP() <= 0.0f)
	{
		PacketManager::Get().SendPacket((char*)&PI, sizeof(Packet_KeyValue), PACKET_EndGame);
	}

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
			m_waveCount = 0;

			Packet_Float p_WaveStart;
			p_WaveStart.KeyValue = 0;
			p_WaveStart.Value = *m_pFrameCount;
			PacketManager::Get().SendPacket((char*)&p_WaveStart, sizeof(Packet_KeyValue), PACKET_WaveCount);
		}	break;
		//case EGameState::GameStart:
		//{
		//	//m_eState = EGameState::WaveInit;
		//	//PacketManager::Get().SendPacket((char*)&PI, sizeof(PI), PACKET_StartGame);
		//}	break;
		case EGameState::WaveInit:
		{
			if (m_waveCount == 5)
			{
				if (ObjectManager::Get().GetObjectList(EObjType::Enemy)->empty())
				{
					m_eState = EGameState::End;
					PacketManager::Get().SendPacket((char*)&PI, sizeof(Packet_KeyValue), PACKET_EndGame);
				}
				return;
			}
			m_eState = EGameState::Spawn;
			++m_waveCount;
			m_spawnCount = 4;

			Packet_Float p_WaveStart;
			p_WaveStart.KeyValue = m_waveCount;
			p_WaveStart.Value = *m_pFrameCount;
			PacketManager::Get().SendPacket((char*)&p_WaveStart, sizeof(Packet_KeyValue), PACKET_WaveStart);
		}	break;
		case EGameState::Spawn:
		{
			switch (m_waveCount)
			{
			case 1:
			{
				PacketManager::Get().SendTakeObject(L"Zombie", ESocketType::EZombie, (UCHAR)PacketManager::Get().UserList.size(), 1.0f, 0.2f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Caster", ESocketType::ECaster, (UCHAR)PacketManager::Get().UserList.size(), 0.8f, 0.2f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Crawler", ESocketType::ECrawler, (UCHAR)PacketManager::Get().UserList.size(), 0.6f, 0.15f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			}	break;
			case 2:
			{
				PacketManager::Get().SendTakeObject(L"Zombie", ESocketType::EZombie, (UCHAR)PacketManager::Get().UserList.size(), 1.0f, 0.2f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Caster", ESocketType::ECaster, (UCHAR)PacketManager::Get().UserList.size(), 0.8f, 0.2f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Crawler", ESocketType::ECrawler, (UCHAR)PacketManager::Get().UserList.size(), 0.6f, 0.15f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Mutant", ESocketType::EMutant, 1, 5.0f, 0.5f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			}	break;
			case 3:
			{
				if(m_spawnCount == 2)
					PacketManager::Get().SendTakeObject(L"Tank", ESocketType::ETank, 1, 15.0f * PacketManager::Get().UserList.size(), 1.1f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Zombie", ESocketType::EZombie, (UCHAR)PacketManager::Get().UserList.size(), 1.0f, 0.2f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Caster", ESocketType::ECaster, (UCHAR)PacketManager::Get().UserList.size(), 0.8f, 0.2f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Crawler", ESocketType::ECrawler, (UCHAR)PacketManager::Get().UserList.size(), 0.6f, 0.15f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			}	break;
			case 4:
			{
				if (m_spawnCount == 2)
					PacketManager::Get().SendTakeObject(L"Tank", ESocketType::ETank, 1, 15.0f * PacketManager::Get().UserList.size(), 1.1f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Mutant", ESocketType::EMutant, (UCHAR)PacketManager::Get().UserList.size(), 5.0f, 0.5f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
				PacketManager::Get().SendTakeObject(L"Crawler", ESocketType::ECrawler, (UCHAR)PacketManager::Get().UserList.size(), 0.6f, 0.15f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			}	break;
			case 5:
			{
				if (m_spawnCount >= 2)
					PacketManager::Get().SendTakeObject(L"Tank", ESocketType::ETank, 1, 15.0f * PacketManager::Get().UserList.size(), 1.1f, 0.1f, { -500.0f, 10.0f, -500.0f }, { 1000.0f, 0.0f, 1000.0f });
			}	break;
			case 6:
			{
				//PacketManager::Get().SendPacket((char*)&PI, sizeof(Packet_KeyValue), PACKET_EndGame);
			}	break;
			}

			*m_pFrameCount = 15.0f;
			--m_spawnCount;
			if (m_spawnCount <= 0)
			{
				m_eState = EGameState::WaveInit;
				*m_pFrameCount = 30.0f;
			}
			Packet_Float p_WaveStart;
			p_WaveStart.KeyValue = m_spawnCount;
			p_WaveStart.Value = *m_pFrameCount;
			PacketManager::Get().SendPacket((char*)&p_WaveStart, sizeof(Packet_KeyValue), PACKET_WaveCount);
		}	break;
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
	PlayerController::Get().pUIManager = &UIManager::Get();			// 매니저 등록
	// HP, MP
	UIManager::Get().m_pHpBar = (JProgressBar*)pUIRoot->find_child(L"HP_Progress");
	UIManager::Get().m_pMpBar = (JProgressBar*)pUIRoot->find_child(L"MP_Progress");
	UIManager::Get().m_FightPanel = (JPanel*)pUIRoot->find_child(L"fight_panel");
	UIManager::Get().m_pStatePanel = (JPanel*)pUIRoot->find_child(L"State_Panel");
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
	// 마우스, 옵션, 전광판
	UIManager::Get().m_pMouseIcon = pUIRoot->find_child(L"mouse_cursor");
	UIManager::Get().m_pMouseIcon->m_bRender = false;
	//PlayerController::Get().m_pOption = (JPanel*)pUIRoot->find_child(L"Set_Panel");
	PacketManager::Get().pKillDisplay = (JListCtrl*)pUIRoot->find_child(L"KilltoDeath");

	// Skill Icon
	UIManager::Get().m_pLeftIcon = (JProgressBar*)pUIRoot->find_child(L"Skill_Left");
	UIManager::Get().m_pRightIcon = (JProgressBar*)pUIRoot->find_child(L"Skill_Right");

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
		Core::isPlaying = false;
		pScene;
		//PlayerController::Get().CutParent();
		//PacketManager::Get().pSender->Release();
		//((MainClass*)pScene)->SetScene(ESceneName::Lobby);
	};
	auto pExit = (JTextCtrl*)pUIRoot->find_child(L"Set_GameExit");
	pExit->EventClick.first = pGameExit;
	pExit->EventClick.second = this;
	// CheckBox
	m_pCheckBox = (JCheckCtrl*)pUIRoot->find_child(L"Set_Collision_Check");
	// Timer
	UIManager::Get().m_TimerText = (JTextCtrl*)pUIRoot->find_child(L"Timer_Text");
	//m_Rule.m_TimerText = (JTextCtrl*)pUIRoot->find_child(L"Timer_Text");
	//m_Rule.SetResultPanel((JPanel*)pUIRoot);

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
			}
		}
	};
	PacketManager::Get().pChatList = (JListCtrl*)pUIRoot->find_child(L"Chat_Log");
	PacketManager::Get().pChatList->EventHover.first = pChatWheel;
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
	//PlayerController::Get().m_pRespawnEffect = (JPanel*)pUIRoot->find_child(L"fadein"); //(L"JohnSprite");

	UIManager::Get().m_pChat = m_pChat = (JEditCtrl*)pUIRoot->find_child(L"Chat_Edit");
	// 적 체력바
	UIManager::Get().m_pEnemyPanel = (JPanel*)pUIRoot->find_child(L"Enemy_Panel"); //bRender true/false
	UIManager::Get().m_pEnemyPanel->m_bRender = false;
	UIManager::Get().m_pEnemyHP = (JProgressBar*)pUIRoot->find_child(L"Enemy_HP");
	UIManager::Get().m_pEnemyName = (JTextCtrl*)pUIRoot->find_child(L"Enemy_Name");
	UIManager::Get().m_pEnemyHPText = (JTextCtrl*)pUIRoot->find_child(L"Enemy_HP_txt");

	// 인포창
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

			//// Luk 올렸을시
			//if (&PacketManager::Get().pMyInfo->StatLuk == pStat &&
			//	PlayerController::Get().GetParent() != nullptr)
			//{
			//	Packet_Vector3 p_Scailing;
			//	p_Scailing.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
			//	p_Scailing.Vec3 = Vector3::One * 0.005f;
			//	PacketManager::Get().SendPacket((char*)&p_Scailing, (USHORT)sizeof(Packet_Vector3), PACKET_Scaling);
			//}
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
	// 슬롯
	UIManager::Get().m_pSlot1 = (JSlot*)pUIRoot->find_child(L"Slot1");
	UIManager::Get().m_pSlot2 = (JSlot*)pUIRoot->find_child(L"Slot2");
	UIManager::Get().m_pSlot3 = (JSlot*)pUIRoot->find_child(L"Slot3");

	 //UIManager::Get().m_pSlot3->m_pItem->m_pIndexList[0] = -1;
	//UIManager::Get().m_pSlot1->AddItem(L"");
	//UIManager::Get().m_pSlot1->m_bEmpty; // > AddItem(L"");

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

	UIManager::Get().m_pInvenSlot = (JInventory*)pUIRoot->find_child(L"Inventory_Slot");
	//
	ObjectManager::Get().PushObject(pUIRoot);
	UI::InGameEvent(pUIRoot);
}