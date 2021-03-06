#include "PacketManager.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "PlayerController.h"
#include "Collider.h"
#include "EventManager.h"
#include "UIManager.h"

#include "../network/PPRecvPacketPoolServer.h"					//클라이언트 클래스 정의.
#include "AIZombie.h"



void PacketManager::SendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept
{
	if (isHost)
	{
		InterceptPacket((PP::PPPacketType)packeyType, data);
		//return;
	}

	///pSender->BroadcastWString(L"Hello, Server!");	//서버에게 문자열 전송(함수명은 바꿀 예정)
	static PP::PPPacketForProcess packet;

	memcpy(packet.m_Packet.m_Payload, (void*)data, size);

	packet.m_socketSession = 0;
	//packet.m_SendMode = sendMode;
	packet.m_Packet.m_Header.m_type = (PP::PPPacketType)packeyType;
	packet.m_Packet.m_Header.m_len = (USHORT)(size + PACKET_HEADER_SIZE);

	pSender->Broadcast(packet);
	//pSender->BroadcastExcept();
}

void PacketManager::ReqSendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept
{
	if (isHost)
	{
		static PP::PPPacketForProcess packet;

		memcpy(packet.m_Packet.m_Payload, (void*)data, size);

		packet.m_socketSession = 0;
		//packet.m_SendMode = sendMode;
		packet.m_Packet.m_Header.m_type = (PP::PPPacketType)packeyType;
		packet.m_Packet.m_Header.m_len = (USHORT)(size + PACKET_HEADER_SIZE);

		PP::PPRecvPacketPoolServer::GetInstance().push_back(packet);
		PacketFunc();
	}
	else
	{
		SendPacket(data, size, packeyType);
	}
}



void PacketManager::InterceptPacket(const PP::PPPacketType& sendMode, const char* data) noexcept
{
	static Packet_KeyValue		p_KeyValue;
	static Packet_Bool			p_Bool;
	static Packet_Vector3		p_Vector3;
	static Packet_Vector2		p_Vector2;
	static Packet_Quaternion	p_Quaternion;
	static Packet_Vec3Quat		p_Vec3Quat;
	//static Packet_AnimState		p_AnimState;
	static Packet_AnimTransform p_AnimTransform;
	static Packet_Transform		p_Transform;
	static Packet_PossessPlayer	p_PossessPlayer;
	static Packet_SoundData		p_SoundData;
	static Packet_ChatMessage	p_ChatMessage;
	static Packet_TakeObject	p_TakeObject;
	static Packet_SyncObjects	p_SyncObjects;
	static Packet_PlayerDead	p_PlayerDead;
	static Packet_Float			p_Float;
	static Packet_Physics		p_ColliderInfo;
	static Packet_TowerAttack	p_Tower;

	memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
	if (sendMode < 2000 &&
		p_KeyValue.KeyValue != (UINT)-1 &&
		ObjectManager::KeyObjects.find(p_KeyValue.KeyValue) == ObjectManager::KeyObjects.end())
	{
		ErrorMessage("KeyObject is Null(" + to_string(sendMode) + ") : " + to_string(p_KeyValue.KeyValue));
		return;
	}

	switch (sendMode)
	{
	case PACKET_SetTransform:
	{
		memcpy(&p_Transform, data, sizeof(Packet_Transform));
		ObjectManager::KeyObjects[p_Transform.KeyValue]->SetPosition(p_Transform.Position);
		ObjectManager::KeyObjects[p_Transform.KeyValue]->SetRotation(p_Transform.Rotation);
		ObjectManager::KeyObjects[p_Transform.KeyValue]->SetScale(p_Transform.Scale);
	}	break;
	case PACKET_SetPosition:
	{
		memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
		ObjectManager::KeyObjects[p_Vector3.KeyValue]->SetPosition(p_Vector3.Vec3);
	}	break;
	case PACKET_SetRotation:
	{
		memcpy(&p_Quaternion, data, sizeof(Packet_Quaternion));
		ObjectManager::KeyObjects[p_Quaternion.KeyValue]->SetRotation(p_Quaternion.Quat);
	}	break;
	case PACKET_SetScale:
	{
		memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
		auto pObject = ObjectManager::KeyObjects[p_Vector3.KeyValue];
		pObject->SetScale(p_Vector3.Vec3);
		auto pCollider = pObject->GetComponentList(EComponent::Collider);
		if (pCollider != nullptr)
		{
			pObject->SetGravityScale(pObject->GetScale().x * 1.5f);
			if (pObject->m_objType == EObjType::Character)
			{
				for (auto& iter : *pCollider)
				{
					((Collider*)iter)->m_pivot = Vector3::Up * 40.0f * pObject->GetScale().x;
				}
			}
		}
		if (PlayerController::Get().GetParent() != nullptr &&
			PlayerController::Get().GetParent()->m_keyValue == p_Vector3.KeyValue)
		{
			PlayerController::Get().ResetOption();
		}
	}	break;
	case PACKET_Translate:
	{
		memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
		ObjectManager::KeyObjects[p_Vector3.KeyValue]->Translate(p_Vector3.Vec3);
	}	break;
	case PACKET_Rotate:
	{
		memcpy(&p_Quaternion, data, sizeof(Packet_Quaternion));
		ObjectManager::KeyObjects[p_Quaternion.KeyValue]->Rotate(p_Quaternion.Quat);
	}	break;
	case PACKET_Scaling:
	{
		memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
		auto pObject = ObjectManager::KeyObjects[p_Vector3.KeyValue];
		pObject->Scaling(p_Vector3.Vec3);
		auto pCollider = pObject->GetComponentList(EComponent::Collider);
		if (pCollider != nullptr)
		{
			pObject->SetGravityScale(pObject->GetScale().x * 1.5f);
			if (pObject->m_objType == EObjType::Character)
			{
				for (auto& iter : *pCollider)
				{
					((Collider*)iter)->m_pivot = Vector3::Up * 40.0f * pObject->GetScale().x;
				}
			}
		}
		if (PlayerController::Get().GetParent() != nullptr &&
			PlayerController::Get().GetParent()->m_keyValue == p_Vector3.KeyValue)
		{
			PlayerController::Get().ResetOption();
		}
	}	break;
	case PACKET_SetAnimTransform:
	{
		memcpy(&p_AnimTransform, data, sizeof(Packet_AnimTransform));
		ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetPosition(p_AnimTransform.Position);
		ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetRotation(p_AnimTransform.Rotation);

		//if (p_AnimTransform.ECharacter != PlayerController::ECharacter::EDummy)
		//{
			PlayerController::SetAnim((AHeroObj*)ObjectManager::KeyObjects[p_AnimTransform.KeyValue], p_AnimTransform.UserSocket, (PlayerController::ECharacter)p_AnimTransform.ECharacter, (PlayerController::EAction)p_AnimTransform.EAnimState, p_AnimTransform.Direction);
		//}

		if (ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->m_pPhysics != nullptr)
		{
			ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetForce(p_AnimTransform.Force);

			switch ((PlayerController::EAction)p_AnimTransform.EAnimState)
			{
			case PlayerController::EAction::Fly:
				ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetGravityScale(-0.4f);
			case PlayerController::EAction::Jump:
			{
				ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->isGround(false);
			}	break;
			case PlayerController::EAction::FlyEnd:
			{
				ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetGravityScale(1.0f);
			} break;
			case PlayerController::EAction::Special:
			case PlayerController::EAction::Dash:
			case PlayerController::EAction::DashLeft:
			case PlayerController::EAction::DashRight:
			case PlayerController::EAction::Idle:
			case PlayerController::EAction::NIdle:
			{
				ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->isMoving(false);
			}	break;
			case PlayerController::EAction::Left:
			case PlayerController::EAction::Right:
			case PlayerController::EAction::Run:
			case PlayerController::EAction::RunLeft:
			case PlayerController::EAction::RunRight:
			case PlayerController::EAction::Forward:
			case PlayerController::EAction::ForwardLeft:
			case PlayerController::EAction::ForwardRight:
			case PlayerController::EAction::Backward:
			case PlayerController::EAction::BackwardLeft:
			case PlayerController::EAction::BackwardRight:
			case PlayerController::EAction::RForward:
			case PlayerController::EAction::NForwardLeft:
			case PlayerController::EAction::NForwardRight:
			case PlayerController::EAction::RBackward:
			case PlayerController::EAction::NBackwardLeft:
			case PlayerController::EAction::NBackwardRight:
			case PlayerController::EAction::RLeft:
			case PlayerController::EAction::RRight:
			{
				ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetDirectionForce(p_AnimTransform.Direction);
			}	break;
			}
		}
	}	break;
	case PACKET_TakeObject:
	{
		ZeroMemory(&p_TakeObject.ObjectName, 100);
		memcpy(&p_TakeObject, data, PS_TakeObject);
		memcpy(((char*)&p_TakeObject + PS_TakeObject), ((char*)data + PS_TakeObject), p_TakeObject.DataSize);

		auto pObject = ObjectManager::Get().TakeObject(p_TakeObject.ObjectName);
		pObject->m_pPhysics->UserSocket = p_TakeObject.UserSocket;
		pObject->SetKeyValue(PlayerKeyCount = p_TakeObject.KeyValue);
		pObject->SetPosition(p_TakeObject.Position);
		pObject->SetRotation(p_TakeObject.Rotation);
		pObject->SetScale(p_TakeObject.Scale);
		if (pObject->m_pPhysics != nullptr)
		{
			pObject->SetHP(p_TakeObject.HP);
			pObject->m_pPhysics->m_maxHP = p_TakeObject.HP;
			pObject->SetGravityScale(pObject->GetScale().x * 1.5f);
			pObject->GetCollider()->m_pivot = Vector3::Up * 40.0f * pObject->GetScale().x;
		}
	}	break;
	case PACKET_TowerAttack:
	{
		memcpy(&p_Tower, data, sizeof(Packet_TowerAttack));
		static const float shotRange = 200000.0f;
		static D3DXVECTOR3 targetPos[4] = { Vector3::Forward * 700.0f,
								Vector3::Right * 700.0f,
								Vector3::Backward * 700.0f,
								Vector3::Left * 700.0f };
		//TowerLevel = p_Tower.TowerLevel;
		switch (p_Tower.KeyValue)
		{
		case 0:
		{
			// 일반
			for (int i = 0; i < 4; ++i)
			{
				int index = i * 2;
				for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
				{
					if (VectorLengthSq(iter->GetPosition() - targetPos[i]) <= shotRange)
					{
						SoundManager::Get().PlayQueue("SE_bow_shot.mp3", TowerPos[index], PlayerController::Get().SoundRange * 0.5f);

						auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
						pItem->SetPosition(TowerPos[index] + Vector3::Up * 250.0f);
						pItem->SetScale(20.0f, 20.0f, 2.0f);
						pItem->SetFocus(iter->GetPosition());
						pItem->SetForce((iter->GetPosition() - TowerPos[index] - Vector3::Up * 250.0f) * 1.4f + Vector3::Up * 100.0f);
						pItem->m_pPhysics->m_damage = TowerDamage;
						pItem->m_pPhysics->UserSocket = ESocketType::EDummy;

						++index;
						pItem = ObjectManager::Get().TakeObject(L"Arrow");
						pItem->SetPosition(TowerPos[index] + Vector3::Up * 250.0f);
						pItem->SetScale(20.0f, 20.0f, 2.0f);
						pItem->SetFocus(iter->GetPosition());
						pItem->SetForce((iter->GetPosition() - TowerPos[index] - Vector3::Up * 250.0f) * 1.4f + Vector3::Up * 100.0f);
						pItem->m_pPhysics->m_damage = TowerDamage;
						pItem->m_pPhysics->UserSocket = ESocketType::EDummy;
						break;
					}
				}
			}
		}	break;
		case 1:
		{
			// 폭탄
			for (int i = 0; i < 4; ++i)
			{
				int index = i * 2;
				for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
				{
					if (VectorLengthSq(iter->GetPosition() - targetPos[i]) <= shotRange)
					{
						auto pItem = ObjectManager::Get().TakeObject(L"PBomb");
						pItem->SetPosition(TowerPos[index] + Vector3::Up * 250.0f);
						pItem->SetForce((iter->GetPosition() - TowerPos[index]) + Vector3::Up * 200.0f);
						pItem->m_pPhysics->m_damage = TowerDamage * 3.0f;
						pItem->m_pPhysics->UserSocket = ESocketType::EDummy;

						++index;
						pItem = ObjectManager::Get().TakeObject(L"PBomb");
						pItem->SetPosition(TowerPos[index] + Vector3::Up * 250.0f);
						pItem->SetForce((iter->GetPosition() - TowerPos[index]) + Vector3::Up * 200.0f);
						pItem->m_pPhysics->m_damage = TowerDamage * 3.0f;
						pItem->m_pPhysics->UserSocket = ESocketType::EDummy;
						break;
					}
				}
			}
		}	break;
		//case 1:
		//{
		//	// 시한
		//	for (int i = 0; i < 4; ++i)
		//	{
		//		int index = i * 2;
		//		for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
		//		{
		//			if (VectorLengthSq(iter->GetPosition() - targetPos[i]) <= shotRange)
		//			{
		//				auto pItem = ObjectManager::Get().TakeObject(L"TimeBomb");
		//				pItem->SetPosition(TowerPos[index] + Vector3::Up * 200.0f);
		//				//pItem->SetScale(Vector3::One * 1.0f);
		//				//pItem->SetRotation();
		//				pItem->SetForce((iter->GetPosition() - TowerPos[index]) * 0.7f + Vector3::Up * 200.0f);
		//				pItem->m_pPhysics->m_damage = 0.3f;
		//				pItem->m_pPhysics->UserSocket = ESocketType::EDummy;
		//
		//				++index;
		//				pItem = ObjectManager::Get().TakeObject(L"TimeBomb");
		//				pItem->SetPosition(TowerPos[index] + Vector3::Up * 200.0f);
		//				pItem->SetForce((iter->GetPosition() - TowerPos[index]) * 0.7f + Vector3::Up * 200.0f);
		//				pItem->m_pPhysics->m_damage = 0.3f;
		//				pItem->m_pPhysics->UserSocket = ESocketType::EDummy;
		//				break;
		//			}
		//		}
		//	}
		//}	break;
		//case 2:
		//{
		//	// 지뢰
		//	for (int i = 0; i < 4; ++i)
		//	{
		//		int index = i * 2;
		//		for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
		//		{
		//			if (VectorLengthSq(iter->GetPosition() - targetPos[i]) <= shotRange)
		//			{
		//				auto pItem = ObjectManager::Get().TakeObject(L"Mine");
		//				pItem->SetPosition(TowerPos[index] + Vector3::Up * 100.0f);
		//				pItem->SetForce((iter->GetPosition() - TowerPos[index]) * 0.35f + Vector3::Up * 600.0f);
		//				pItem->m_pPhysics->m_damage = 0.3f;
		//				pItem->m_pPhysics->UserSocket = ESocketType::EDummy;
		//
		//				++index;
		//				pItem = ObjectManager::Get().TakeObject(L"Mine");
		//				pItem->SetPosition(TowerPos[index] + Vector3::Up * 100.0f);
		//				pItem->SetForce((iter->GetPosition() - TowerPos[index]) * 0.35f + Vector3::Up * 600.0f);
		//				pItem->m_pPhysics->m_damage = 0.3f;
		//				pItem->m_pPhysics->UserSocket = ESocketType::EDummy;
		//				break;
		//			}
		//		}
		//	}
		//}	break;
		default:
			break;
		}
	}	break;
	case PACKET_TowerUpgrade:
	{
		TowerDamage = 0.2f + TowerLevel * 0.05f;
		++TowerLevel;
		if (TowerLevel == 1)
		{
			TowerDelayShot = 8.0f;
			TowerDelayShot2 = 99999.0f;
			return;
		}
		else if (TowerLevel == 5)
		{
			TowerDelayShot *= 0.85f;
			TowerDelayShot2 = 15.0f;
			return;
		}
		TowerDelayShot *= 0.85f;
		TowerDelayShot2 *= 0.85f;
	}	break;
	case PACKET_SendNameUpdate:
	{
		memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
		// 유저 정보 갱신
		auto userIter = UserList.find(p_KeyValue.KeyValue);
		if (userIter != UserList.end())
		{
			const wstring preName = userIter->second->UserID;
			memcpy((char*)userIter->second, data, PS_UserInfo);
			memcpy(((char*)userIter->second + PS_UserInfo), ((char*)data + PS_UserInfo), UserList[p_KeyValue.KeyValue]->DataSize);
			// 자기가 아니라면
			if (pMyInfo != userIter->second)
			{
				pChatList->push_string(L"[System] '"s + preName + L"' 님의 아이디가" + userIter->second->UserID + L" 로 변경 되였습니다.");
			}
		}
	}	break;
	case PACKET_SendLevelUp:
	{
		memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
		// 유저 정보 갱신
		auto userIter = UserList.find(p_KeyValue.KeyValue);
		if (userIter != UserList.end())
		{
			memcpy((char*)userIter->second, data, PS_UserInfo);
			memcpy(((char*)userIter->second + PS_UserInfo), ((char*)data + PS_UserInfo), UserList[p_KeyValue.KeyValue]->DataSize);
			pChatList->push_string(L"[LevelUp!] '"s + userIter->second->UserID + L"' 님이 " + to_wstring(userIter->second->Level) + L" Level이 되었습니다.");

			// 체력 갱신, 이펙
			auto iter = ObjectManager::KeyObjects.find(userIter->second->KeyValue);
			if (iter != ObjectManager::KeyObjects.end())
			{
				auto pEffect = ObjectManager::Get().TakeObject(L"EPLevelUp");
				pEffect->SetPosition(iter->second->GetPosition());
				//pEffect->SetParent(iter->second);
				iter->second->m_pPhysics->m_maxHP = 1.0f + pMyInfo->StatStr * 0.2f;
				iter->second->HealHP(iter->second->m_pPhysics->m_maxHP * 0.5f);
			}
		}
		// 자신일시
		if (pMyInfo == userIter->second)
		{
			SoundManager::Get().Play("SE_levelup.mp3");
			PlayerController::Get().UpdateStatus();
		}
	}	break;
	case PACKET_SendUserInfo:
	{
		memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
		// 유저 목록에 있으면 갱신
		int i = 0;
		for (auto& iter : UserList)
		{
			if (iter.first == p_KeyValue.KeyValue)
			{
				pUserPanel[i]->m_bRender = true;
				auto pUserInfo = UserList[p_KeyValue.KeyValue];
				memcpy((char*)pUserInfo, data, PS_UserInfo);
				memcpy(((char*)pUserInfo + PS_UserInfo), ((char*)data + PS_UserInfo), pUserInfo->DataSize);
				return;
			}
			++i;
		}

		// 유저 목록에 없는데 자신이면 추가
		if (pMyInfo->UserSocket == p_KeyValue.KeyValue)
		{
			//pUserPanel[UserList.size()]->m_bRender = true;
			memcpy((char*)pMyInfo, data, PS_UserInfo);
			memcpy(((char*)pMyInfo + PS_UserInfo), ((char*)data + PS_UserInfo), pMyInfo->DataSize);
			UserList[pMyInfo->UserSocket] = pMyInfo;
			return;
		}
		// 새 유저면 생성
		//pUserPanel[UserList.size()]->m_bRender = true;
		auto pUser = new UserInfo();
		memcpy((char*)pUser, data, PS_UserInfo);
		memcpy(((char*)pUser + PS_UserInfo), ((char*)data + PS_UserInfo), pUser->DataSize);
		UserList[pUser->UserSocket] = pUser;
	}	break;
	case PACKET_PossessPlayer:
	{
		memcpy(&p_PossessPlayer, data, sizeof(Packet_PossessPlayer));
		PlayerController::Get().Possess(ObjectManager::KeyObjects[p_PossessPlayer.KeyValue]);
		UIManager::Get().m_pRespawnEffect->SetEventTime(1.5f);
		UIManager::Get().m_pRespawnEffect->EffectPlay();

		pMyInfo->isDead = false;
		PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
		//SoundManager::Get().Play("SV_jocena.mp3");
	}	break;
	case PACKET_PlayerDead:
	{
		memcpy(&p_PlayerDead, data, sizeof(Packet_PlayerDead));
		// 죽은게 자신일시
		auto pObject = ObjectManager::KeyObjects[p_PlayerDead.KeyValue];

		if (PlayerController::Get().GetParent() == pObject)
		{
			++pMyInfo->DeathCount;
			pMyInfo->isDead = true;
			PacketManager::Get().SendPacket((char*)pMyInfo, (USHORT)(PS_UserInfo + pMyInfo->DataSize), PACKET_SendUserInfo);
			PlayerController::Get().DeadEvent();
		}


		// 
		switch (pObject->m_objType)
		{
		case EObjType::Character:
		{
			auto pEffect = ObjectManager::Get().TakeObject(L"PDead");
			pEffect->SetPosition(pObject->GetCollider()->GetCenter());
			// 죽인게 자신일시
			if (pMyInfo->UserSocket == p_PlayerDead.KillUser)
			{
				PlayerController::Get().OperEXP(0.03f);
				++pMyInfo->KillCount;
				pMyInfo->Score += 500;
				PacketManager::Get().SendPacket((char*)pMyInfo, (USHORT)(PS_UserInfo + pMyInfo->DataSize), PACKET_SendUserInfo);
			}
			ObjectManager::Get().DisableObject(pObject);
		}	break;
		case EObjType::AObject:
		{
			if (pObject->m_pPhysics->DeadEvent != nullptr)
				pObject->m_pPhysics->DeadEvent(pObject->GetCollider(), p_PlayerDead.KillUser);
			ObjectManager::Get().DisableObject(pObject);
			return;
		}	break;
		case EObjType::Enemy:
		{
			if (pObject->m_pPhysics->DeadEvent != nullptr)
			{
				std::thread toDead(pObject->m_pPhysics->DeadEvent, pObject->GetCollider(), p_PlayerDead.KillUser);
				toDead.detach();
				//pObject->m_pPhysics->DeadEvent(pObject->GetCollider(), p_PlayerDead.KillUser);
			}

			float userRate = 1.0001f;
			// 컴터가 죽였을시
			if (p_PlayerDead.KillUser == ESocketType::EDummy)
				userRate /= PacketManager::Get().UserList.size();
			// 자기가 죽였을시
			if (p_PlayerDead.KillUser == pMyInfo->UserSocket || p_PlayerDead.KillUser == ESocketType::EDummy)
			{
				switch (pObject->m_pPhysics->UserSocket)
				{
				case EZombie:
				case ECaster:
				case ECrawler:
				{
					PlayerController::Get().OperEXP(0.12f * userRate);
					pMyInfo->KillCount += userRate;
					pMyInfo->Score += 300 * userRate;
					PacketManager::Get().SendPacket((char*)pMyInfo, (USHORT)(PS_UserInfo + pMyInfo->DataSize), PACKET_SendUserInfo);
				}	break;
				case EMutant:
				{
					PlayerController::Get().OperEXP(0.8f * userRate);
					pMyInfo->KillCount += userRate;
					pMyInfo->Score += 1000 * userRate;
					PacketManager::Get().SendPacket((char*)pMyInfo, (USHORT)(PS_UserInfo + pMyInfo->DataSize), PACKET_SendUserInfo);
				}	break;
				case ETank:
				{
					PlayerController::Get().OperEXP(2.0f * userRate);
					pMyInfo->KillCount += userRate;
					pMyInfo->Score += 5000 * userRate;
					PacketManager::Get().SendPacket((char*)pMyInfo, (USHORT)(PS_UserInfo + pMyInfo->DataSize), PACKET_SendUserInfo);
				}	break;
				}
			}
		}	break;
		}

		// 전광판 입력
		wstring killer, dead;
		// 맞은놈
		if (pObject->m_objType == EObjType::Enemy)
		{
			dead = pObject->m_myName;
		}
		else
		{
			auto iter = UserList.find(p_PlayerDead.DeadUser);
			if (iter != UserList.end())
			{
				dead = iter->second->UserID;

				pChatList->push_string(L"[System] '" + dead + L"' 님이 사망 했습니다.");
				*pChatList->m_fValue = 0.0f;
			}
		}
		// 때린놈
		if (p_PlayerDead.KillUser >= ESocketType::EZombie)
		{
			killer = pObject->m_myName;
		}
		else
		{
			auto iter = UserList.find(p_PlayerDead.KillUser);
			if (iter != UserList.end())
			{
				killer = iter->second->UserID;
			}
		}
		pKillDisplay->push_string(killer + L" -> " + dead);
	}	break;
	case PACKET_PlayerDisable:
	{
		ObjectManager::Get().DisableObject(ObjectManager::KeyObjects[p_KeyValue.KeyValue]);
	}	break;
	case PACKET_PlaySound:
	{
		ZeroMemory(&p_SoundData, sizeof(p_SoundData));
		memcpy(&p_SoundData, data, PS_PlaySound);
		memcpy(((char*)&p_SoundData + PS_PlaySound), ((char*)data + PS_PlaySound), p_SoundData.DataSize);
		SoundManager::Get().PlayQueue(p_SoundData.SoundName, p_SoundData.Position, p_SoundData.MaxDistance);
	}	break;
	case PACKET_ChatMessage:
	{
		ZeroMemory(&p_ChatMessage, sizeof(p_ChatMessage));
		memcpy(&p_ChatMessage, data, PS_ChatMessage);
		memcpy(((char*)&p_ChatMessage + PS_ChatMessage), ((char*)data + PS_ChatMessage), p_ChatMessage.DataSize);
		auto iter = UserList.find(p_ChatMessage.UserSocket);
		if (iter != UserList.end())
		{
			if (iter->second == pMyInfo)
			{
				pMyInfo->Score += 2;
				PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			}
			pChatList->push_string(iter->second->UserID + L" : "s + p_ChatMessage.Message);
			*pChatList->m_fValue = 0.0f;
		}
	}	break;
	case PACKET_SyncObjects:
	{
		memcpy(&p_SyncObjects, data, PS_SyncObjects);
		memcpy(((char*)&p_SyncObjects + PS_SyncObjects), ((char*)data + PS_SyncObjects), p_SyncObjects.Count * sizeof(Packet_SyncTransform));

		for (int i = 0; i < p_SyncObjects.Count; ++i)
		{
			ObjectManager::KeyObjects[p_SyncObjects.Data[i].KeyValue]->SetPosition(p_SyncObjects.Data[i].Position);
			ObjectManager::KeyObjects[p_SyncObjects.Data[i].KeyValue]->SetRotation(p_SyncObjects.Data[i].Rotation);
			if (ObjectManager::KeyObjects[p_SyncObjects.Data[i].KeyValue]->m_pPhysics != nullptr)
				ObjectManager::KeyObjects[p_SyncObjects.Data[i].KeyValue]->SetForce(p_SyncObjects.Data[i].Force);
		}
	}	break;
	case PACKET_SetHP:
	{
		memcpy(&p_Float, data, sizeof(Packet_Float));
		ObjectManager::KeyObjects[p_Float.KeyValue]->SetHP(p_Float.Value);
		// 자신일시
		if (PlayerController::Get().GetParent() != nullptr &&
			PlayerController::Get().GetParent()->m_keyValue == p_Float.KeyValue)
		{
			PlayerController::Get().UpdateStatus();
		}
	}	break;
	case PACKET_HealHP:
	{
		memcpy(&p_Float, data, sizeof(Packet_Float));
		ObjectManager::KeyObjects[p_Float.KeyValue]->HealHP(p_Float.Value);
	}	break;
	case PACKET_SetPhysicsInfo:
	{
		memcpy(&p_ColliderInfo, data, sizeof(Packet_Physics));
		auto pPhysics = ObjectManager::KeyObjects[p_ColliderInfo.KeyValue]->m_pPhysics;
		if (pPhysics != nullptr)
		{
			pPhysics->m_mass		 = p_ColliderInfo.Mass;
			pPhysics->m_damping		 = p_ColliderInfo.Damping;
			pPhysics->m_repulsion	 = p_ColliderInfo.Repulsion;
			pPhysics->m_maxHP		 = p_ColliderInfo.MaxHP;
			pPhysics->m_armor		 = p_ColliderInfo.Armor;
			pPhysics->m_GravityScale = p_ColliderInfo.GravityScale;
		}
	}	break;
	//case PACKET_StartGame:
	//{
	//	//pChatList->push_string(L"====================== 게임 시작 =======================");
	//	//UIManager::Get().m_FightPanel->m_bRender = true;
	//	//PlayerController::Get().SendReqRespawn(PlayerController::Get().m_selectCharacter);
	//	//PlayerController::Get().m_GameFrameCount = 3.0f;
	//}	break;
	case PACKET_EndGame:
	{
		pChatList->push_string(L"========================== 게임 종료 ============================");
		UIManager::Get().m_pStatePanel->m_bRender = true;
		UIManager::Get().m_pStatePanel->PreEvent.first = nullptr;
		//PlayerController::Get().m_GameFrameCount = 3.0f;
		UIManager::Get().m_TimerText->m_bRender = false;
		SoundManager::Get().Play("SE_chicken.mp3");
	}	break;
	case PACKET_WaveStart:
	{
		memcpy(&p_Float, data, sizeof(Packet_Float));
		pChatList->push_string(L"========================= Wave " + to_wstring(p_Float.KeyValue) + L" Start! ===========================");
		m_waveCount = p_Float.KeyValue;
		PlayerController::Get().m_GameFrameCount = p_Float.Value;
		PlayerController::Get().m_canChurh = true;
		SoundManager::Get().Play("SE_wave.mp3");
	}	break;
	case PACKET_WaveCount:
	{
		memcpy(&p_Float, data, sizeof(Packet_Float));
		//pChatList->push_string(L"=============== Wave " + to_wstring(p_Float.KeyValue) + L" Start! ===============");
		PlayerController::Get().m_GameFrameCount = p_Float.Value;
	}	break;
	default:
	{
		ErrorMessage("처리되지 않은 패킷 : "s + to_string(sendMode));
		return;
	}	break;
	}
}


void PacketManager::SendPlaySound(const string_view& soundName, const D3DXVECTOR3& position, const float& maxDistance) noexcept
{
	Packet_SoundData p_SoundData;
	//p_SoundData.KeyValue = (UINT)-1;
	p_SoundData.Position = position;
	p_SoundData.MaxDistance = maxDistance;
	memcpy(p_SoundData.SoundName, soundName.data(), soundName.size());
	p_SoundData.DataSize = (char)soundName.size();

	SendPacket((char*)&p_SoundData, (USHORT)(PS_PlaySound + soundName.size()), PACKET_PlaySound);
}


void PacketManager::SendDeadEvent(const UINT& keyValue, const UINT& deadSocket, const UINT& killSocket) noexcept
{
	static Packet_PlayerDead p_PlayerDead;
	p_PlayerDead.KeyValue = keyValue;
	p_PlayerDead.DeadUser = deadSocket;
	p_PlayerDead.KillUser = killSocket;
	PacketManager::Get().SendPacket((char*)&p_PlayerDead, (USHORT)sizeof(Packet_PlayerDead), PACKET_PlayerDead);
}

void PacketManager::SendTakeObject(const WCHAR* objName, const UINT& socketNum, const UCHAR& spawnCount, const float& hp, const float& minScale, const float& randScale, const D3DXVECTOR3& minPosition, const D3DXVECTOR3& randPosition)  noexcept
{
	static Packet_TakeObject p_TakeObject;
	static size_t			 strSize;

	//strSize = objName.size() * 2;
	strSize = lstrlenW(objName) * 2;
	strSize = strSize > 100 ? 100 : strSize;

	ZeroMemory(&p_TakeObject.ObjectName, 100);
	memcpy(p_TakeObject.ObjectName, objName, strSize);
	p_TakeObject.DataSize = (UCHAR)strSize;
	p_TakeObject.Rotation = Quaternion::Base;
	p_TakeObject.HP = hp;
	p_TakeObject.UserSocket = socketNum;
	for (int i = 0; i < spawnCount; ++i)
	{
		p_TakeObject.KeyValue = ++PacketManager::Get().PlayerKeyCount;
		p_TakeObject.Position = { randPosition.x * RandomNormal() + minPosition.x, randPosition.y * RandomNormal() + minPosition.y, randPosition.z * RandomNormal() + minPosition.z };
		p_TakeObject.Scale = (randScale * RandomNormal() + minScale) * 3.0f * Vector3::One;
		PacketManager::Get().SendPacket((char*)&p_TakeObject, (USHORT)(PS_TakeObject + strSize), PACKET_TakeObject);
	}
}

void PacketManager::SendSpawnEnemy(const WCHAR* objName, const UINT& socketNum, const UCHAR& spawnCount, const float& hp, const float& minScale, const float& randScale)  noexcept
{
	static Packet_TakeObject p_TakeObject;
	static size_t			 strSize;

	//strSize = objName.size() * 2;
	strSize = lstrlenW(objName) * 2;
	strSize = strSize > 100 ? 100 : strSize;

	ZeroMemory(&p_TakeObject.ObjectName, 100);
	memcpy(p_TakeObject.ObjectName, objName, strSize);
	p_TakeObject.DataSize = (UCHAR)strSize;
	p_TakeObject.Rotation = Quaternion::Base;
	p_TakeObject.UserSocket = socketNum;
	for (int i = 0; i < spawnCount; ++i)
	{
		// 체력
		switch (socketNum)
		 {
		 case ESocketType::EZombie:
		 case ESocketType::ECaster:
		 case ESocketType::ECrawler:
		 {
		 	p_TakeObject.HP = hp * m_waveCount;
		 }	break;
		 case ESocketType::EMutant:
		 {
		 	p_TakeObject.HP = hp + hp * m_waveCount * 0.4f;
		 }	break;
		 case ESocketType::ETank:
		 {
		 	p_TakeObject.HP = hp + hp * m_waveCount * 0.2f;
			PacketManager::Get().SendPlaySound("SV_zombie_king_idle.mp3", Vector3::Zero, PlayerController::Get().SoundRange * 2.0f);
		 }	break;
		 default:
		 {
		 	p_TakeObject.HP = hp;
		 }	break;
		}
		
		// 스폰 위치
		switch ((int)(RandomNormal() * 4.0f))
		{
		case 0:
		{
			p_TakeObject.Position = { 1500.0f, 0.0f, RandomNormal() * 100.0f - 50.0f };
		}	break;
		case 1:
		{
			p_TakeObject.Position = { -1500.0f, 0.0f, RandomNormal() * 100.0f - 50.0f };
		}	break;
		case 2:
		{
			p_TakeObject.Position = { RandomNormal() * 100.0f - 50.0f, 0.0f, 1500.0f };
		}	break;
		case 3:
		{
			p_TakeObject.Position = { RandomNormal() * 100.0f - 50.0f, 0.0f, -1500.0f };
		}	break;
		}
		p_TakeObject.KeyValue = ++PacketManager::Get().PlayerKeyCount;
		p_TakeObject.Scale = (randScale * RandomNormal() + minScale) * 3.0f * Vector3::One;
		PacketManager::Get().SendPacket((char*)&p_TakeObject, (USHORT)(PS_TakeObject + strSize), PACKET_TakeObject);
	}
}