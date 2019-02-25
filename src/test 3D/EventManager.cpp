#include "EventManager.h"
#include "AHeroObj.h"
#include "PlayerController.h"
#include "ObjectManager.h"
#include "PacketManager.h"
#include "SoundManager.h"
#include "AIZombieKing.h"
#include "UIManager.h"

namespace MyEvent {
	void ForceWave(Collider* pA, Collider* pB) {
		if (pB != nullptr)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 180.0f);
			pA->AddIgnoreList(pB);
		}
	}

	void MineBoom(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Enemy)
		{
			auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
			pItem->SetPosition(pA->m_pParent->GetPosition());
			pItem->SetScale(Vector3::One);
			pItem->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
			pItem->SetDamage(pA->m_pPhysics->m_damage);

			ObjectManager::Get().DisableObject(pA->m_pParent);
			SoundManager::Get().PlayQueue("SE_bomb.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
		}
	}

	void NuclearBoom(Collider* pA, Collider* pB)
	{
		if (pA->m_pParent->isEnable())
		{
			auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
			pItem->SetPosition(pA->m_pParent->GetPosition());
			pItem->SetScale(Vector3::One * 3.0f);
			pItem->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
			pItem->SetDamage(pA->m_pPhysics->m_damage);

			ObjectManager::Get().DisableObject(pA->m_pParent);
			SoundManager::Get().PlayQueue("SE_bomb.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
		}
	}

	void MissileCollision(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 240.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(0.5f);
				((JPanel*)UIManager::Get().m_pHitEffect)->EffectPlay();
			}
			// 내가 때렸을때
			if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"EHit");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		SoundManager::Get().PlayQueue("SE_fire1.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	}

	void BuffWave(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			pA->AddIgnoreList(pB);
			switch (pB->m_eTag)
			{
			case ETag::Dummy:
				return;
			case ETag::Ally:
			{
				pB->SetForce((Normalize(pA->GetTotalForce())) * 90.0f);
				pB->m_pParent->HealHP(pA->m_pPhysics->m_damage);
				// 내가 맞았을때
				if (pB->m_pParent == PlayerController::Get().GetParent())
				{
					UIManager::Get().m_pGreenEffect->SetEventTime(0.2f);
					((JPanel*)UIManager::Get().m_pGreenEffect)->EffectPlay();
				}
				// 내가 때렸을때
				if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
				{
					PlayerController::Get().HitEvent(pB);
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}	break;
			case ETag::Enemy:
			{
				pB->SetForce(Vector3::Up * 100.0f);
				pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
				// 내가 맞았을때
				if (pB->m_pParent == PlayerController::Get().GetParent())
				{
					UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
					((JPanel*)UIManager::Get().m_pHitEffect)->EffectPlay();
				}
				// 내가 때렸을때
				else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
				{
					PlayerController::Get().HitEvent(pB);
					if (pB->m_pParent->GetHP() <= EPSILON)
					{
						PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
					}
					else
					{
						PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
						PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
					}
				}
				else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
				}
			}	break;
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EHitLight");
			pEffect->SetPosition(pB->GetCenter());
		}
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	void EnergyBall(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			switch (pB->m_eTag)
			{
			 case ETag::Dummy:
			 	return;
			 case ETag::Ally:
			 {
			 	pB->SetForce((Normalize(pA->GetTotalForce())) * 45.0f);
			 	pB->m_pParent->HealHP(pA->m_pPhysics->m_damage * 1.5f);
			 	// 내가 맞았을때
			 	if (pB->m_pParent == PlayerController::Get().GetParent())
			 	{
					UIManager::Get().m_pGreenEffect->SetEventTime(0.2f);
			 		((JPanel*)UIManager::Get().m_pGreenEffect)->EffectPlay();
			 	}
			 	// 내가 때렸을때
			 	if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			 	{
			 		PlayerController::Get().HitEvent(pB);
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			 	}
			 }	break;
			 case ETag::Enemy:
			 {
			 	pB->SetForce((Normalize(-pA->GetTotalForce())) * 150.0f);
			 	pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			 	// 내가 맞았을때
			 	if (pB->m_pParent == PlayerController::Get().GetParent())
			 	{
					UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
			 		((JPanel*)UIManager::Get().m_pHitEffect)->EffectPlay();
			 	}
			 	// 내가 때렸을때
			 	else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			 	{
			 		PlayerController::Get().HitEvent(pB);
			 		if (pB->m_pParent->GetHP() <= EPSILON)
			 		{
			 			PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
			 		}
			 		else
			 		{
			 			PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
			 			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			 		}
			 	}
				else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
				}
			 }	break;
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"EHitLight");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	void BerserkMode(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Enemy)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 510.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 때렸을때
			if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EHit");
			pEffect->SetPosition(pA->GetCenter());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void PlayerAttack(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 450.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			pA->AddIgnoreList(pB);
			// 내가 때렸을때
			if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EPSlash");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void PlayerBomb(Collider* pA, Collider* pB)
	{
		auto pObject = ObjectManager::Get().TakeObject(L"PBoom");
		pObject->SetPosition(pA->GetCenter());
		pObject->SetScale(Vector3::One);
		pObject->m_pPhysics->m_damage = pA->m_pPhysics->m_damage;
		pObject->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
		SoundManager::Get().PlayQueue("SE_bomb.mp3", pA->GetCenter(), PlayerController::Get().SoundRange);
		ObjectManager::Get().DisableObject(pA->m_pParent);
	}

	void OneTimeHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			auto tempForce = Normalize(-pA->GetTotalForce()) * 220.0f;
			tempForce.y = 20.0f;
			pB->SetForce(tempForce);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				((JPanel*)UIManager::Get().m_pHitEffect)->EffectPlay();
			}
			// 내가 때렸을때
			else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"EPAttack");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	void DaggerHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			pB->SetForce((Normalize(-pA->GetTotalForce())) * 210.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				((JPanel*)UIManager::Get().m_pHitEffect)->EffectPlay();
			}
			// 내가 때렸을때
			else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"EPAttack");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	void PiercingHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			pA->SetForce(Vector3::Zero);
			pA->AddIgnoreList(pB);
			pB->SetForce((Normalize(-pA->GetTotalForce())) * 210.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				((JPanel*)UIManager::Get().m_pHitEffect)->EffectPlay();
			}
			// 내가 때렸을때
			else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
			pA->m_pPhysics->m_damage *= 0.8f;
			auto pEffect = ObjectManager::Get().TakeObject(L"EPAttack");
			pEffect->SetPosition(pA->m_pParent->GetPosition());
		}
		else
		{
			// 바닥일때만 없앰
			auto pEffect = ObjectManager::Get().TakeObject(L"EPAttack");
			pEffect->SetPosition(pA->m_pParent->GetPosition());
			ObjectManager::Get().DisableObject(pA->m_pParent);
		}
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	void ArrowHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			pB->SetForce((Normalize(-pA->GetTotalForce())) * 210.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				((JPanel*)UIManager::Get().m_pHitEffect)->EffectPlay();
			}
			// 내가 때렸을때
			else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
			pA->m_pParent->SetPosition(pA->m_pParent->GetPosition() - pB->m_pParent->GetWorldPosition());
			pA->m_pParent->SetScale(Divide(pA->m_pParent->GetScale(), pB->m_pParent->GetWorldScale()));
			pA->m_pParent->SetParent(pB->m_pParent);
		}
		pA->m_pParent->isStatic(true);
		pA->m_pParent->usePhysics(false);
		pA->m_pParent->SetGravityScale(0.0f);
		pA->m_pParent->GetCollider()->CollisionEvent = nullptr;

		auto pEffect = ObjectManager::Get().TakeObject(L"EPAttack");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		//ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	void MeleeHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			auto tempForce = Normalize(pB->GetCenter() - pA->GetCenter()) * 300.0f;
			tempForce.y = 20.0f;
			pB->SetForce(tempForce);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			pA->AddIgnoreList(pB);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				UIManager::Get().m_pHitEffect->EffectPlay();
			}
			// 내가 때렸을때
			else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 150.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EPSlash");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void ZombieHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr && 
			pB->m_pParent->m_objType == EObjType::Character)
		{
			/*for (auto& iter : ObjectManager::Get().GetColliderList())
			{
				ObjectManager::Get().TakeObject(L"EHit")->SetPosition(iter->GetCenter());
			}

			int i = 0;
			for (auto& iter : *pA->m_pParent->GetComponentList(EComponent::Collider))
			{
				++i;
			}
			ErrorMessage("1. 충돌체 갯수 : " + to_string(i));
*/
			//if (pA->m_pParent->isEnable())
			//{
			//	ErrorMessage("1. 부모 Enable");
			//}
			////if (auto iter = std::find(ObjectManager::Get().GetColliderList().begin(), ObjectManager::Get().GetColliderList().end(), pA);
			////	iter != ObjectManager::Get().GetColliderList().end())
			//for(auto& iter : ObjectManager::Get().GetColliderList())
			//{
			//	if (iter == pA)
			//	{
			//		ErrorMessage(L"2. 충돌체 등록됨 : " + iter->m_pParent->m_myName);
			//	}
			//}
			//int i = 0;
			//for (auto& iter : ObjectManager::Get().GetObjectList())
			//{
			//	for (auto& initer : iter.second)
			//	{
			//		if (initer == pA->m_pParent)
			//		{
			//			i++;
			//			if (i >= 2)
			//			{
			//				ErrorMessage("3. 리스트 중복 등록 : " + to_string(i));
			//			}
			//		}
			//	}
			//}
			//if (i == 1)
			//{
			//	ErrorMessage("3. 하나만 등록됨");
			//}
			//auto pos = pA->GetCenter();
			//ErrorMessage("4. CenterPos -> " + to_string(pos.x) + ", " + to_string(pos.y) + ", " + to_string(pos.z));
			//pos = pA->m_pParent->GetPosition();
			//ErrorMessage("5. ParentPos -> " + to_string(pos.x) + ", " + to_string(pos.y) + ", " + to_string(pos.z));

			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 180.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				//ErrorMessage(L"좀비 힛 : " + pA->m_pParent->m_myName + L", " + to_wstring(pA->m_pPhysics->UserSocket));
				UIManager::Get().m_pHitEffect->SetEventTime(0.2f);
				UIManager::Get().m_pHitEffect->EffectPlay();
				
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EZHit");
			pEffect->SetPosition(pA->m_pParent->GetPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void ZombieAttack(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 300.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			pA->AddIgnoreList(pB);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				UIManager::Get().m_pHitEffect->EffectPlay();
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			// 중립 때렸을때
			else if (PacketManager::Get().isHost && pB->m_pPhysics->UserSocket == ESocketType::EDummy && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EZHit");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void ZombieThrow(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			pB->SetForce((Normalize(-pA->GetTotalForce())) * 200.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				UIManager::Get().m_pHitEffect->EffectPlay();
				if (pB->m_pParent->GetHP() <= EPSILON)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			// 중립 때렸을때
			else if (PacketManager::Get().isHost && pB->m_pPhysics->UserSocket == ESocketType::EDummy && pB->m_pParent->GetHP() <= EPSILON)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"EZHit");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	//void OneShots(Collider* pA, Collider* pB) {
	//	if (pB != nullptr &&
	//		pB->m_eTag == ETag::Enemy)
	//	{
	//		//pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter()) + Vector3::Up) * 130.0f);
	//		///pA->m_pParent->HealHP(pB->m_pPhysics->m_damage * pA->m_pPhysics->m_armor * 0.6f);		0밑으로 가면 1로 됨?
	//		pB->m_pParent->OperHP(-1.1f);
	//		// 내가 맞았을때
	//		if (pB->m_pParent == PlayerController::Get().GetParent())
	//		{
	//			UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
	//			UIManager::Get().m_pHitEffect->EffectPlay();
	//		}
	//		// 내가 때렸을때
	//		else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
	//		{
	//			PlayerController::Get().HitEvent(pB);
	//			if (pB->m_pParent->GetHP() <= EPSILON)
	//			{
	//				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
	//			}
	//		}
	//		auto pEffect = ObjectManager::Get().TakeObject(L"EPAttack");
	//		pEffect->SetPosition(pA->m_pParent->GetPosition());
	//	}
	//}

	//void GiantItem(Collider* pA, Collider* pB) {
	//	if (pB != nullptr &&
	//		(pB->m_pParent->m_objType == EObjType::Character))
	//	{
	//		// 플레이어 충돌 이벤트
	//		pB->CollisionEvent = MyEvent::OneShots;
	//		// 대상이 자신
	//		if (pB->m_pParent == PlayerController::Get().GetParent())
	//		{
	//			// Giant
	//			std::thread giant(&PlayerController::StartGiantMode, &PlayerController::Get());
	//			giant.detach();
	//		}
	//		ObjectManager::Get().DisableObject(pA->m_pParent);
	//	}
	//}

	void ItemBox(Collider* pA, Collider* pB) {
		if (pB != nullptr &&
			(pB->m_pParent->m_objType == EObjType::Character))
		{
			// 대상이 자신
			if (PlayerController::Get().GetParent() != nullptr &&
				pB->m_pParent == PlayerController::Get().GetParent())
			{
				pB->m_pParent->SetForce(Vector3::Zero);
				auto value = (int)(RandomNormal() * JItem::Get()->m_pItemList.size());
				for (auto& iter : JItem::Get()->m_pItemList)
				{
					if (--value < 0)
					{
						UIManager::Get().AddSlotItem(iter.first);
						break;
					}
				}
				ObjectManager::Get().DisableObject(pA->m_pParent);
			}
		}
	}


	//////////////////////////////////////////////////////////////////
	void BulletHit(Collider* pA, Collider* pB)
	{
		if (pB->m_eTag == ETag::Enemy &&
			pB->m_pParent->isEnable() &&
			pA->m_pParent->isEnable())
		{
			pB->m_pParent->isEnable(false);
			pA->m_pParent->isEnable(false);
		}
	};
	void EnemyHit(Collider* pA, Collider* pB)
	{
		if (pB->m_eTag == ETag::Ally &&
			pB->m_pParent->isEnable() &&
			pA->m_pParent->isEnable())
		{
			pB->m_pParent->isEnable(false);
			pA->m_pParent->isEnable(false);
		}
	};
}


namespace ActiveEvent {
	// 충격파
	void ShockWave(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::ShockWave;
	}

	// 폭탄
	void ThrowBomb(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::ThrowBomb;
	}

	// 시한 폭탄
	void ThrowTimeBomb(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::ITimeBomb;
	}

	// 충격 폭탄
	void ThrowShockBoom(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::IShockBomb;
	}

	// 미사일
	void ThrowMissile(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::IMissile;
	}

	// 지뢰
	void ThrowMine(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::IMine;
	}

	// 핵
	void ThrowNuclear(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::INuclear;
	}

	// 물약
	void UsePotion(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::IPotion;
	}

	// 바리케이드
	void ThrowBarricade(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::IBarricade;
	}
}

namespace TimeEvent {
	void ArrowRain(GameObject* pParent, void* pVoid2)
	{
		D3DXMATRIX matRotation;
		D3DXVECTOR3 vecRot;
		for (int i = 0; i < 30; ++i)
		{
			auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
			pItem->SetPosition(pParent->GetPosition());
			vecRot = { -RandomNormal() * PI * 0.4f - PI * 0.3f, RandomNormal() * PI, 0.0f };
			pItem->SetRotation(vecRot);
			pItem->SetScale(Vector3::One * 1.5f);
			//pItem->SetForce({ RandomNormal() * 100.0f + -50.0f, RandomNormal() * 150.0f - 200.0f, RandomNormal() * 100.0f + -50.0f });
			D3DXMatrixRotationYawPitchRoll(&matRotation, -vecRot.y, vecRot.x, vecRot.z);
			vecRot = { matRotation._31, matRotation._32, matRotation._33 };
			pItem->SetForce(vecRot * (RandomNormal() * 180.0f + 180.0f));
			pItem->m_pPhysics->UserSocket = pParent->m_pPhysics->UserSocket;
			pItem->SetDamage(pParent->m_pPhysics->m_damage);
			//pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
		}
		//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		ObjectManager::Get().DisableObject(pParent);
	}

	void TimeBomb(GameObject* pParent, void* pVoid2)
	{
		auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
		pItem->SetPosition(pParent->GetPosition());
		pItem->SetScale(Vector3::One * 1.5f);
		pItem->m_pPhysics->UserSocket = pParent->m_pPhysics->UserSocket;
		pItem->SetDamage(pParent->m_pPhysics->m_damage);
		
		ObjectManager::Get().DisableObject(pParent);
	}

	void MissileShot(GameObject* pParent, void* pVoid2)
	{
		auto pTarget = (D3DXVECTOR3*)pVoid2;
		pParent->AddForce(Normalize(*pTarget - pParent->GetPosition()) * (RandomNormal() * 200.0f + 300.0f) + Vector3::Up * 15.0f);
	}

	void NuclearLaunch(GameObject* pParent, void* pVoid2)
	{
		auto pItem = ObjectManager::Get().TakeObject(L"Nuclear");
		pItem->SetPosition(pParent->GetPosition() + Vector3::Down * 10.0f);
		pItem->SetScale(Vector3::One * 3.0f);
		pItem->SetRotation(Quaternion::Down * PI);
		pItem->m_pPhysics->UserSocket = pParent->m_pPhysics->UserSocket;
		pItem->SetDamage(pParent->m_pPhysics->m_damage);

		SoundManager::Get().PlayQueue("SE_fire1.mp3", pParent->GetPosition(), PlayerController::Get().SoundRange);
	}
}

namespace DyingEvent {
	void CenterDead(Collider* pCollider, const UINT& killUser)
	{
		//pCollider->m_pParent->isEnable(false);
		SoundManager::Get().Play("SE_fire1.mp3");
	}

	void ZombieDead(Collider* pCollider, const UINT& killUser)
	{
		if (RandomNormal() >= 0.8f * 5.0f / (5.0f + PacketManager::Get().pMyInfo->StatLuk))
		{
			auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
			pObject->SetPosition(pCollider->GetCenter());
			pObject->SetHP(10000.0f);
		}
		PlayerController::Get().OperEXP(0.03f);
		auto pEffect = ObjectManager::Get().TakeObject(L"EZDead");
		pEffect->SetPosition(pCollider->GetCenter());
		// 
		auto pAI = ((AIZombie*)pCollider->m_pParent->GetComponent(EComponent::Etc));
		pAI->m_delay = 99.0f;
		pCollider->m_eTagArray[ETag::Ally] = false;
		pCollider->m_eTagArray[ETag::Enemy] = false;
		pCollider->m_eTagArray[ETag::Dummy] = false;
		pCollider->m_eTagArray[ETag::Collider] = false;
		pCollider->isMoving(false);
		//((AHeroObj*)pCollider->m_pParent)->SetANIM_Loop(ZombieR_DEATH);
		PlayerController::Get().m_dyingEnemys.emplace_back((AHeroObj*)pCollider->m_pParent, 4.0f);
	}

	void ZombieCrawlDead(Collider* pCollider, const UINT& killUser)
	{
		if (RandomNormal() >= 0.8f * 5.0f / (5.0f + PacketManager::Get().pMyInfo->StatLuk))
		{
			auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
			pObject->SetPosition(pCollider->GetCenter());
			pObject->SetHP(10000.0f);
		}
		PlayerController::Get().OperEXP(0.03f);
		auto pEffect = ObjectManager::Get().TakeObject(L"EZDead");
		pEffect->SetPosition(pCollider->GetCenter());
		// 
		auto pAI = ((AIZombie*)pCollider->m_pParent->GetComponent(EComponent::Etc));
		pAI->m_delay = 99.0f;
		pCollider->m_eTagArray[ETag::Ally] = false;
		pCollider->m_eTagArray[ETag::Enemy] = false;
		pCollider->m_eTagArray[ETag::Dummy] = false;
		pCollider->m_eTagArray[ETag::Collider] = false;
		pCollider->isMoving(false);
		((AHeroObj*)pCollider->m_pParent)->SetHeroAnimSpeed(0.3f);
		PlayerController::Get().m_dyingEnemys.emplace_back((AHeroObj*)pCollider->m_pParent, 2.0f);
	}

	void ZombieExDead(Collider* pCollider, const UINT& killUser)
	{
		//SoundManager::Get().PlayQueue("SV_zombie_dead.mp3", pCollider->GetCenter(), PlayerController::Get().SoundRange);
		SoundManager::Get().Play("SV_zombie_dead.mp3");
		
		if (RandomNormal() >= 0.4f * 5.0f / (5.0f + PacketManager::Get().pMyInfo->StatLuk))
		{
			auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
			pObject->SetPosition(pCollider->GetCenter());
			pObject->SetHP(10000.0f);
		}
		PlayerController::Get().OperEXP(0.2f);
		auto pEffect = ObjectManager::Get().TakeObject(L"EZDead2");
		pEffect->SetPosition(pCollider->GetCenter());
		// 
		auto pAI = ((AIZombie*)pCollider->m_pParent->GetComponent(EComponent::Etc));
		pAI->m_delay = 99.0f;
		pCollider->m_eTagArray[ETag::Ally] = false;
		pCollider->m_eTagArray[ETag::Enemy] = false;
		pCollider->m_eTagArray[ETag::Dummy] = false;
		pCollider->m_eTagArray[ETag::Collider] = false;
		pCollider->isMoving(false);
		((AHeroObj*)pCollider->m_pParent)->SetHeroAnimSpeed(0.3f);
		PlayerController::Get().m_dyingEnemys.emplace_back((AHeroObj*)pCollider->m_pParent, 2.0f);
	}

	void ZombieKingDead(Collider* pCollider, const UINT& killUser)
	{
		auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
		pObject->SetPosition(pCollider->GetCenter());
		pObject->SetHP(10000.0f);
		if (RandomNormal() >= 0.5f * 5.0f / (5.0f + PacketManager::Get().pMyInfo->StatLuk))
		{
			pObject = ObjectManager::Get().TakeObject(L"ItemBox");
			pObject->SetPosition(pCollider->GetCenter());
			pObject->SetHP(10000.0f);
		}

		PlayerController::Get().OperEXP(1.0f);
		auto pEffect = ObjectManager::Get().TakeObject(L"EZDead3");
		pEffect->SetPosition(pCollider->GetCenter());
		// 이펙 삭제
		auto pAI = ((AIZombieKing*)pCollider->m_pParent->GetComponent(EComponent::Etc));
		if (pAI->m_Breath != nullptr)
		{
			ObjectManager::Get().DisableObject(pAI->m_Breath);
			pAI->m_Breath = nullptr;
		}
		pAI->m_delay = 99.0f;
		pCollider->m_eTagArray[ETag::Ally]		= false;
		pCollider->m_eTagArray[ETag::Enemy]		= false;
		pCollider->m_eTagArray[ETag::Dummy]		= false;
		pCollider->m_eTagArray[ETag::Collider]	= false;
		pCollider->isMoving(false);
		((AHeroObj*)pCollider->m_pParent)->SetHeroAnimSpeed(0.3f);
		PlayerController::Get().m_dyingEnemys.emplace_back((AHeroObj*)pCollider->m_pParent, 2.0f);
	}

	void ShockBoomDead(Collider* pCollider, const UINT& killUser)
	{
		auto position = pCollider->GetCenter() + Vector3::Up * 20.0f;
		//pA->m_pParent->SetPosition(Vector3::Up * 10000.0f);
		for (int i = 0; i < 20; ++i)
		{
			auto pItem = ObjectManager::Get().TakeObject(L"Dagger");
			pItem->SetPosition(position);
			pItem->SetRotation(RandomNormal() * Quaternion::One * PI);
			pItem->SetForce({ RandomNormal() * 300.0f - 150.0f, RandomNormal() * 230.0f + 100.0f, RandomNormal() * 300.0f - 150.0f });
			pItem->m_pPhysics->UserSocket = killUser;
			pItem->SetDamage(pCollider->m_pPhysics->m_damage);
		}
		ObjectManager::Get().DisableObject(pCollider->m_pParent);
		SoundManager::Get().PlayQueue("SE_fire1.mp3", pCollider->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	}

	void BarrelDead(Collider* pCollider, const UINT& killUser)
	{
		auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
		pItem->SetPosition(pCollider->m_pParent->GetPosition());
		pItem->SetScale(Vector3::One * 1.5f);
		pItem->m_pPhysics->UserSocket = killUser;
		auto finder = PacketManager::Get().UserList.find(killUser);
		if(finder == PacketManager::Get().UserList.end())
			pItem->m_pPhysics->m_damage = 0.75f;
		else
			pItem->m_pPhysics->m_damage = 0.75f * finder->second->AttackRate;

		//ObjectManager::Get().DisableObject(pCollider->m_pParent);
		SoundManager::Get().PlayQueue("SE_bomb.mp3", pCollider->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	}
}