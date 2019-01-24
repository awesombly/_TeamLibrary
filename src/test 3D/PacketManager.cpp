#include "PacketManager.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "PlayerController.h"
#include "Collider.h"

#include "../network/PPRecvPacketPoolServer.h"					//클라이언트 클래스 정의.


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
	static Packet_SetHP			p_SetHP;

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
		//auto preScale = pObject->GetScaleAverage();
		pObject->SetScale(p_Vector3.Vec3);
		auto pCollider = pObject->GetComponentList(EComponent::Collider);
		if (pCollider != nullptr)
		{
			pObject->SetGravityScale(pObject->GetScaleAverage() * 5.0f);
			if (pObject->m_objType == EObjType::Character)
			{
				for (auto& iter : *pCollider)
				{
					((Collider*)iter)->m_pivot = Vector3::Up * 40.0f * pObject->GetScaleAverage();
					//pObject->SetArmor(pObject->GetScaleAverage());
				}
			}
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
		ObjectManager::KeyObjects[p_Vector3.KeyValue]->Scaling(p_Vector3.Vec3);
	}	break;
	case PACKET_SetAnimTransform:
	{
		memcpy(&p_AnimTransform, data, sizeof(Packet_AnimTransform));
		if (p_AnimTransform.ECharacter != PlayerController::ECharacter::EDummy)
		{
			PlayerController::SetAnim((AHeroObj*)ObjectManager::KeyObjects[p_AnimTransform.KeyValue], p_AnimTransform.UserSocket, (PlayerController::ECharacter)p_AnimTransform.ECharacter, (PlayerController::EAction)p_AnimTransform.EAnimState, p_AnimTransform.Direction);
		}
		ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetPosition(p_AnimTransform.Position);
		ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetRotation(p_AnimTransform.Rotation);

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
			case PlayerController::EAction::Idle:
			{
				ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->isMoving(false);
			}	break;
			case PlayerController::EAction::Forward:
			case PlayerController::EAction::ForwardLeft:
			case PlayerController::EAction::ForwardRight:
			case PlayerController::EAction::Backward:
			case PlayerController::EAction::BackwardLeft:
			case PlayerController::EAction::BackwardRight:
			case PlayerController::EAction::Left:
			case PlayerController::EAction::Right:
			{
				ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetDirectionForce(p_AnimTransform.Direction);
			}	break;
			}
		}
	}	break;
	case PACKET_TakeObject:
	{
		ZeroMemory(&p_TakeObject, sizeof(Packet_TakeObject));
		memcpy(&p_TakeObject, data, PS_TakeObject);
		memcpy(((char*)&p_TakeObject + PS_TakeObject), ((char*)data + PS_TakeObject), p_TakeObject.DataSize);

		auto pObject = ObjectManager::Get().TakeObject(p_TakeObject.ObjectName);
		pObject->m_pPhysics->UserSocket = p_TakeObject.UserSocket;
		pObject->SetKeyValue(p_TakeObject.KeyValue);
		pObject->SetPosition(p_TakeObject.Position);
		pObject->SetRotation(p_TakeObject.Rotation);
		pObject->SetScale(p_TakeObject.Scale);
		if (pObject->m_pPhysics != nullptr)
		{
			pObject->SetHP(p_TakeObject.HP);
			pObject->m_pPhysics->m_maxHP = p_TakeObject.HP;
			pObject->SetGravityScale(pObject->GetScaleAverage() * 5.0f);
			if (auto iter = UserList.find(p_TakeObject.UserSocket);
				iter != UserList.end())
			{
				pObject->SetArmor(iter->second->StatInt);
			}
		}
	}	break;
	case PACKET_SendUserInfo:
	{
		memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
		// 유저 목록에 있으면 갱신
		auto userIter = UserList.find(p_KeyValue.KeyValue);
		if (userIter != UserList.end())
		{
			memcpy((char*)UserList[p_KeyValue.KeyValue], data, PS_UserInfo);
			memcpy(((char*)UserList[p_KeyValue.KeyValue] + PS_UserInfo), ((char*)data + PS_UserInfo), UserList[p_KeyValue.KeyValue]->DataSize);
			return;
		}

		//for (int i = 0; i < UserList.size(); ++i)
		//{
		//	if (pUserPanel[i] != nullptr)
		//		pUserPanel[i]->m_bRender = true;
		//	if (UserList[p_KeyValue.KeyValue]->UserSocket == p_KeyValue.KeyValue)
		//	{
		//	}
		//}
		// 유저 목록에 없는데 자신이면 추가
		if (pMyInfo->UserSocket == p_KeyValue.KeyValue)
		{
			//if (pUserPanel[UserList.size()] != nullptr)
			pUserPanel[UserList.size()]->m_bRender = true;
			memcpy((char*)pMyInfo, data, PS_UserInfo);
			memcpy(((char*)pMyInfo + PS_UserInfo), ((char*)data + PS_UserInfo), pMyInfo->DataSize);
			UserList[pMyInfo->UserSocket] = pMyInfo;
			return;
		}
		// 새 유저면 생성
		//if (pUserPanel[UserList.size()] != nullptr)
		pUserPanel[UserList.size()]->m_bRender = true;
		auto pUser = new UserInfo();
		memcpy((char*)pUser, data, PS_UserInfo);
		memcpy(((char*)pUser + PS_UserInfo), ((char*)data + PS_UserInfo), pUser->DataSize);
		UserList[pUser->UserSocket] = pUser;
	}	break;
	case PACKET_PossessPlayer:
	{
		memcpy(&p_PossessPlayer, data, sizeof(Packet_PossessPlayer));
		PlayerController::Get().Possess(ObjectManager::KeyObjects[p_PossessPlayer.KeyValue]);
		((JPanel*)PlayerController::Get().m_pRespawnEffect)->EffectPlay();

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
		// 죽인게 자신일시
		if (pMyInfo->UserSocket == p_PlayerDead.KillUser)
		{
			++pMyInfo->KillCount;
			pMyInfo->Score += 500;
			PacketManager::Get().SendPacket((char*)pMyInfo, (USHORT)(PS_UserInfo + pMyInfo->DataSize), PACKET_SendUserInfo);
		}
		switch (pObject->m_objType)
		{
		case EObjType::Character:
		{
			auto pCollider = new Collider(80.0f);
			auto pEffect = ObjectManager::Get().TakeObject(L"PDead");
			pEffect->AddComponent(pCollider);
			pCollider->CollisionEvent = MyEvent::ForceWave;
			pCollider->m_eTag = ETag::Dummy;
			pCollider->SetGravityScale(0.0f);
			pCollider->usePhysics(false);
			pEffect->SetPosition(pObject->GetCollider()->GetCenter());
			PlayerController::Get().OperEXP(0.03f);
		}	break;
		case EObjType::Enemy:
		{
			auto pEffect = ObjectManager::Get().TakeObject(L"ZDead");
			pEffect->SetPosition(pObject->GetCollider()->GetCenter());
			PlayerController::Get().OperEXP(0.13f);
		}	break;
		}
		ObjectManager::Get().DisableObject(pObject);

		// 전광판 입력
		wstring killer, dead;
		// 맞은놈
		if (p_PlayerDead.DeadUser == (UINT)-1)
		{
			dead = L"Zombie";
		}
		else
		{
			auto iter = UserList.find(p_PlayerDead.DeadUser);
			if (iter != UserList.end())
			{
				dead = iter->second->UserID;
			}

		}
		// 때린놈
		if (p_PlayerDead.KillUser == (UINT)-1)
		{
			killer = L"Zombie";
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
		memcpy(&p_SetHP, data, sizeof(Packet_SetHP));
		ObjectManager::KeyObjects[p_SetHP.KeyValue]->SetHP(p_SetHP.HP);
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







namespace MyEvent {

	void ForceWave(Collider* pMe, Collider* pYou) {
		if (pYou != nullptr && pYou->m_eTag == ETag::Collider)
		{
			pYou->SetForce((Normalize(pYou->GetCenter() - pMe->GetCenter()) + Vector3::Up) * 60.0f);
			pMe->AddIgnoreList(pYou);
		}
	}

	void DaggerHit(Collider* pA, Collider* pB) 
	{
		if (pB != nullptr)
		{
			if (pB->m_eTag != ETag::Collider ||
				pB->m_pParent->m_objType == EObjType::Dummy)
				return;

			pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 80.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage * pB->m_pPhysics->m_armor);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				((JPanel*)PlayerController::Get().m_pHitEffect)->EffectPlay();
			}
			// 내가 때렸을때
			else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += 50;
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"PAttack");
		pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), 1000.0f);
	}

	void MeleeHit(Collider* pA, Collider* pB)
	{
		if(pB != nullptr)
		{
			if (pB->m_eTag != ETag::Collider ||
				pB->m_pParent->m_objType == EObjType::Dummy)
				return;

			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter()) + Vector3::Up) * 130.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage * pB->m_pPhysics->m_armor);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				((JPanel*)PlayerController::Get().m_pHitEffect)->EffectPlay();
			}
			// 내가 때렸을때
			else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += 200;
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
				pA->AddIgnoreList(pB);
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"Slash");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), 1000.0f);
		}
	}

	void ZombieHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			if (pB->m_eTag != ETag::Collider ||
				pB->m_pParent->m_objType != EObjType::Character)
				return;

			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 130.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage * pB->m_pPhysics->m_armor);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				((JPanel*)PlayerController::Get().m_pHitEffect)->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), 1000.0f);
		}
	}

	void OneShots(Collider* pA, Collider* pB) {
		if (pB != nullptr)
		{
			if (pB->m_eTag != ETag::Collider ||
				pB->m_pParent->m_objType != EObjType::Enemy)
				return;
			//pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter()) + Vector3::Up) * 130.0f);
			///pA->m_pParent->HealHP(pB->m_pPhysics->m_damage * pA->m_pPhysics->m_armor * 0.6f);		0밑으로 가면 1로 됨?
			pB->m_pParent->OperHP(-1.1f * pB->m_pPhysics->m_armor);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				((JPanel*)PlayerController::Get().m_pHitEffect)->EffectPlay();
			}
			// 내가 때렸을때
			else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"PAttack");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
		}
	}

	void GiantItem(Collider* pA, Collider* pB) {
		if (pB != nullptr &&
			(pB->m_pParent->m_objType == EObjType::Character))
		{
			if (pB->m_eTag != ETag::Collider) return;

			pB->m_pParent->SetHP(1.0f);
			// 플레이어 충돌 이벤트
			pB->CollisionEvent = MyEvent::OneShots;
			// 대상이 자신
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				((JPanel*)PlayerController::Get().m_pRespawn)->EffectPlay();
				// SetHP
				Packet_SetHP p_SetHP;
				p_SetHP.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
				p_SetHP.HP = 1.0f;
				PacketManager::Get().SendPacket((char*)&p_SetHP, (USHORT)sizeof(Packet_SetHP), PACKET_SetHP);
				// Score
				PacketManager::Get().pMyInfo->Score += 777;
				PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				// Giant
				std::thread giant(&PlayerController::StartGiantMode, &PlayerController::Get());
				giant.detach();
			}
			ObjectManager::Get().DisableObject(pA->m_pParent);
		}
	}


	//////////////////////////////////////////////////////////////////
	void BulletHit(Collider* pA, Collider* pB) 
	{
		if (pB->m_pParent->m_objType == EObjType::Object &&
			pB->m_pParent->isEnable() &&
			pA->m_pParent->isEnable())
		{
			pB->m_pParent->isEnable(false);
			pA->m_pParent->isEnable(false);
		}
	};
	void EnemyHit(Collider* pA, Collider* pB) 
	{
		if (pB->m_pParent->m_objType == EObjType::Image &&
			pB->m_pParent->isEnable() &&
			pA->m_pParent->isEnable())
		{
			pB->m_pParent->isEnable(false);
			pA->m_pParent->isEnable(false);
		}
	};
}