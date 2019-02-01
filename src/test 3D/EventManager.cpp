#include "EventManager.h"
#include "PacketManager.h"
#include "PlayerController.h"
#include "ObjectManager.h"
#include "UIManager.h"
#include "SoundManager.h"


namespace MyEvent {
	void ForceWave(Collider* pMe, Collider* pYou) {
		if (pYou != nullptr && pYou->m_eTag != ETag::Dummy)
		{
			pYou->SetForce((Normalize(pYou->GetCenter() - pMe->GetCenter()) + Vector3::Up) * 60.0f);
			pMe->AddIgnoreList(pYou);
		}
	}

	void ShockBoom(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Dummy)
		{
			if (pB->m_pPhysics->m_damage >= 0.2f)
			{
				auto position = pA->GetCenter() + Vector3::Up * 20.0f;
				pA->m_pParent->SetPosition(Vector3::Up * 10000.0f);
				for (int i = 0; i < 20; ++i)
				{
					auto pItem = ObjectManager::Get().TakeObject(L"Dagger");
					pItem->SetPosition(position);
					pItem->SetRotation(RandomNormal() * Quaternion::One * PI);
					pItem->SetForce({RandomNormal() * 100.0f - 50.0f, 70.0f, RandomNormal() * 100.0f - 50.0f });
					pItem->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
					pItem->SetDamage(0.25f, PacketManager::Get().UserList[pA->m_pPhysics->UserSocket]->StatLuk);
				}
				ObjectManager::Get().DisableObject(pA->m_pParent);
				SoundManager::Get().PlayQueue("SE_fire1.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
			}
		}
	}

	void MineBoom(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Enemy)
		{
			auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
			pItem->SetPosition(pA->m_pParent->GetPosition());
			pItem->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
			pItem->SetDamage(1.0f, PacketManager::Get().UserList[pA->m_pPhysics->UserSocket]->StatLuk);

			ObjectManager::Get().DisableObject(pA->m_pParent);
			SoundManager::Get().PlayQueue("SE_bomb.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
		}
	}

	void NuclearBoom(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			if (pB->m_eTag == ETag::Dummy)
				return;
		}
		auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
		pItem->SetPosition(pA->m_pParent->GetPosition());
		pItem->SetScale(Vector3::One * 2.5f);
		pItem->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
		pItem->SetDamage(5.0f, PacketManager::Get().UserList[pA->m_pPhysics->UserSocket]->StatLuk);

		ObjectManager::Get().DisableObject(pA->m_pParent);
		SoundManager::Get().PlayQueue("SE_bomb.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	}

	void MissileCollision(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Enemy)
		{
			if (pA->GetHP() <= 0.0f)
			{
				pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 80.0f);
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
					if (pB->m_pParent->GetHP() <= 0.0f)
					{
						PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
					}
					else
					{
						PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
						PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
					}
				}
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
				pB->SetForce((Normalize(pA->GetTotalForce())) * 30.0f);
				pB->m_pParent->HealHP(pA->m_pPhysics->m_damage);
				// 내가 때렸을때
				if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
				{
					PlayerController::Get().HitEvent(pB);
				}
			}	break;
			case ETag::Enemy:
			{
				pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 70.0f);
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
					if (pB->m_pParent->GetHP() <= 0.0f)
					{
						PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
					}
					else
					{
						PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
						PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
					}
				}
			}	break;
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EHitLight");
			pEffect->SetPosition(pB->GetCenter());
		}
		//ObjectManager::Get().DisableObject(pA->m_pParent);
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
			 	pB->SetForce((Normalize(pA->GetTotalForce()) + Vector3::Up) * 15.0f);
			 	pB->m_pParent->HealHP(pA->m_pPhysics->m_damage * 1.5f);
			 	//// 내가 맞았을때
			 	//if (pB->m_pParent == PlayerController::Get().GetParent())
			 	//{
			 	//	((JPanel*)UIManager::Get().m_pHitEffect)->EffectPlay();
			 	//}
			 	// 내가 때렸을때
			 	if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			 	{
			 		PlayerController::Get().HitEvent(pB);
			 		//if (pB->m_pParent->GetHP() <= 0.0f)
			 		//{
			 		//	PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
			 		//}
			 		//else
			 		//{
			 		//	PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
			 		//	PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			 		//}
			 	}
			 }	break;
			 case ETag::Enemy:
			 {
			 	pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 70.0f);
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
			 		if (pB->m_pParent->GetHP() <= 0.0f)
			 		{
			 			PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
			 		}
			 		else
			 		{
			 			PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
			 			PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			 		}
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
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 170.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 때렸을때
			if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EHit");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void PlayerAttack(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Enemy)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 150.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			pA->AddIgnoreList(pB);
			// 내가 때렸을때
			if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			{
				PlayerController::Get().HitEvent(pB);
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EPSlash");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void PlayerBomb(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Dummy)
		{
			return;
		}
		auto pObject = ObjectManager::Get().TakeObject(L"PBoom");
		pObject->SetPosition(pA->m_pParent->GetPosition());
		pObject->SetDamage(0.5f, PacketManager::Get().UserList[pA->m_pPhysics->UserSocket]->StatLuk);
		pObject->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
		ObjectManager::Get().DisableObject(pA->m_pParent);
		SoundManager::Get().PlayQueue("SE_bomb.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	}

	void OneTimeHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			if (pB->m_eTag == ETag::Dummy)
				return;

			pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 70.0f);
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
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
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
			if (pB->m_eTag != ETag::Enemy)
				return;

			pB->SetForce((Normalize(-pA->GetTotalForce())) * 70.0f);
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
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 100.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"EPAttack");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	void MeleeHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag != ETag::Dummy)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 100.0f);
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
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
				else
				{
					PacketManager::Get().pMyInfo->Score += (int)(pA->m_pPhysics->m_damage * 150.0f);
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EPSlash");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void ZombieHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_pParent->m_objType == EObjType::Character &&
			pB->m_eTag == ETag::Ally)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 40.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(0.25f);
				UIManager::Get().m_pHitEffect->EffectPlay();
				
				if (pB->m_pParent->GetHP() <= 0.0f)
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
		if (pB != nullptr &&
			pB->m_eTag == ETag::Ally)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 100.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			pA->AddIgnoreList(pB);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				UIManager::Get().m_pHitEffect->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
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
			if (pB->m_eTag != ETag::Ally)
				return;

			pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 80.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->SetEventTime(1.0f);
				UIManager::Get().m_pHitEffect->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"EZHit");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), SoundRange);
	}

	void OneShots(Collider* pA, Collider* pB) {
		if (pB != nullptr &&
			pB->m_eTag == ETag::Enemy)
		{
			//pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter()) + Vector3::Up) * 130.0f);
			///pA->m_pParent->HealHP(pB->m_pPhysics->m_damage * pA->m_pPhysics->m_armor * 0.6f);		0밑으로 가면 1로 됨?
			pB->m_pParent->OperHP(-1.1f);
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
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"EPAttack");
			pEffect->SetPosition(pA->m_pParent->GetPosition());
		}
	}

	void GiantItem(Collider* pA, Collider* pB) {
		if (pB != nullptr &&
			(pB->m_pParent->m_objType == EObjType::Character))
		{
			// 플레이어 충돌 이벤트
			pB->CollisionEvent = MyEvent::OneShots;
			// 대상이 자신
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				// Giant
				std::thread giant(&PlayerController::StartGiantMode, &PlayerController::Get());
				giant.detach();
			}
			ObjectManager::Get().DisableObject(pA->m_pParent);
		}
	}

	void ItemBox(Collider* pA, Collider* pB) {
		if (pB != nullptr &&
			(pB->m_pParent->m_objType == EObjType::Character))
		{
			// 대상이 자신
			if (PlayerController::Get().GetParent() != nullptr &&
				pB->m_pParent == PlayerController::Get().GetParent())
			{
				auto value = (int)(RandomNormal() * JItem::Get()->m_pItemList.size());
				for (auto& iter : JItem::Get()->m_pItemList)
				{
					if (--value < 0)
					{
						UIManager::Get().m_pInvenSlot->AddItem(iter.first);
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
			pItem->SetScale(0.5f, 0.5f, 0.5f);
			//pItem->SetForce({ RandomNormal() * 100.0f + -50.0f, RandomNormal() * 150.0f - 200.0f, RandomNormal() * 100.0f + -50.0f });
			D3DXMatrixRotationYawPitchRoll(&matRotation, -vecRot.y, vecRot.x, vecRot.z);
			vecRot = { matRotation._31, matRotation._32, matRotation._33 };
			pItem->SetForce(vecRot * (RandomNormal() * 60.0f + 60.0f));
			pItem->m_pPhysics->UserSocket = pParent->m_pPhysics->UserSocket;
			pItem->SetDamage(0.4f, PacketManager::Get().UserList[pParent->m_pPhysics->UserSocket]->StatStr);
			//pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
		}
		//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		ObjectManager::Get().DisableObject(pParent);
	}

	void TimeBomb(GameObject* pParent, void* pVoid2)
	{
		auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
		pItem->SetPosition(pParent->GetPosition());
		pItem->m_pPhysics->UserSocket = pParent->m_pPhysics->UserSocket;
		pItem->SetDamage(0.8f, PacketManager::Get().UserList[pParent->m_pPhysics->UserSocket]->StatLuk);
		
		ObjectManager::Get().DisableObject(pParent);
	}

	void MissileShot(GameObject* pParent, void* pVoid2)
	{
		auto pTarget = (D3DXVECTOR3*)pVoid2;
		pParent->SetForce(Normalize(*pTarget - pParent->GetPosition()) * 130.0f + Vector3::Up * 5.0f);
	}

	void NuclearLaunch(GameObject* pParent, void* pVoid2)
	{
		auto pItem = ObjectManager::Get().TakeObject(L"Nuclear");
		pItem->SetPosition(pParent->GetPosition() + Vector3::Down * 10.0f);
		pItem->m_pPhysics->UserSocket = pParent->m_pPhysics->UserSocket;
		pItem->SetDamage(1.0f, PacketManager::Get().UserList[pParent->m_pPhysics->UserSocket]->StatLuk);

		SoundManager::Get().PlayQueue("SE_fire1.mp3", pParent->GetPosition(), PlayerController::Get().SoundRange);
	}
}
