#include "EventManager.h"
#include "PacketManager.h"
#include "PlayerController.h"
#include "ObjectManager.h"
#include "UIManager.h"
#include "SoundManager.h"


namespace MyEvent {
	//void CarpetChurch(Collider* pA, Collider* pB)
	//{
	//	if (pB != nullptr &&
	//		pB->m_pParent == PlayerController::Get().GetParent() &&
	//		PlayerController::Get().m_canChurh)
	//	{
	//		UIManager::Get().m_pXPush->m_bRender = true;
	//		if (Input::GetKeyState('X') == EKeyState::DOWN)
	//		{
	//			UIManager::Get().m_pXPush->m_bRender = false;
	//			PlayerController::Get().m_canChurh = false;
	//			UIManager::Get().m_pRespawnEffect->SetEventTime(1.5f);
	//			UIManager::Get().m_pRespawnEffect->EffectPlay();

	//			Packet_Float p_HealHP;
	//			p_HealHP.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
	//			p_HealHP.Value = 10000.0f;
	//			PacketManager::Get().SendPacket((char*)&p_HealHP, (USHORT)sizeof(Packet_Float), PACKET_HealHP);
	//			// + 포션
	//			//auto value = JItem::Get()->m_pItemList[L"Potion_0"];
	//			if (UIManager::Get().m_pSlot1->Empty())
	//			{
	//				UIManager::Get().m_pSlot1->AddItem(L"Potion_0");
	//				return;
	//			}
	//			if (UIManager::Get().m_pSlot2->Empty())
	//			{
	//				UIManager::Get().m_pSlot2->AddItem(L"Potion_0");
	//				return;
	//			}
	//			if (UIManager::Get().m_pSlot3->Empty())
	//			{
	//				UIManager::Get().m_pSlot3->AddItem(L"Potion_0");
	//				return;
	//			}
	//			UIManager::Get().m_pInvenSlot->AddItem(L"Potion_0");
	//		}
	//	}
	//}

	//void CarpetSmithy(Collider* pA, Collider* pB)
	//{
	//	if (pB != nullptr &&
	//		pB->m_pParent == PlayerController::Get().GetParent())
	//	{
	//		UIManager::Get().m_pXPush->m_bRender = true;
	//		if (Input::GetKeyState('X') == EKeyState::DOWN)
	//		{
	//			if (UIManager::Get().m_pSmithyPanel->m_bRender)
	//			{
	//				// 끔
	//				UIManager::Get().m_pXPush->m_bRender = false;
	//				UIManager::Get().m_pSmithyPanel->m_bRender = false;
	//				UIManager::Get().m_pMouseIcon->m_bRender = false;
	//				pB->m_pParent->SetPosition(pA->GetCenter() - Normalize(pA->GetCenter()) * 100.0f);
	//			}
	//			else
	//			{
	//				// 킴
	//				UIManager::Get().m_pXPush->m_bRender = false;
	//				UIManager::Get().m_pSmithyPanel->m_bRender = true;
	//				UIManager::Get().m_pMouseIcon->m_bRender = true;
	//			}
	//		}
	//	}
	//}

	//void CarpetShop(Collider* pA, Collider* pB)
	//{
	//	if (pB != nullptr &&
	//		pB->m_pParent == PlayerController::Get().GetParent())
	//	{
	//		UIManager::Get().m_pXPush->m_bRender = true;
	//		if (Input::GetKeyState('X') == EKeyState::DOWN)
	//		{
	//			if (UIManager::Get().m_pShopPanel->m_bRender)
	//			{
	//				// 끔
	//				UIManager::Get().m_pXPush->m_bRender = false;
	//				UIManager::Get().m_pShopPanel->m_bRender = false;
	//				UIManager::Get().m_pMouseIcon->m_bRender = false;
	//				pB->m_pParent->SetPosition(pA->GetCenter() - Normalize(pA->GetCenter()) * 100.0f);
	//			}
	//			else
	//			{
	//				// 킴
	//				UIManager::Get().m_pXPush->m_bRender = false;
	//				UIManager::Get().m_pShopPanel->m_bRender = true;
	//				UIManager::Get().m_pMouseIcon->m_bRender = true;
	//			}
	//		}
	//	}
	//}

	//void CarpetTower(Collider* pA, Collider* pB)
	//{
	//	if (pB != nullptr &&
	//		pB->m_pParent == PlayerController::Get().GetParent())
	//	{
	//		UIManager::Get().m_pXPush->m_bRender = true;
	//		if (Input::GetKeyState('X') == EKeyState::DOWN)
	//		{
	//			if (UIManager::Get().m_pTowerPanel->m_bRender)
	//			{
	//				// 끔
	//				UIManager::Get().m_pXPush->m_bRender = false;
	//				UIManager::Get().m_pTowerPanel->m_bRender = false;
	//				UIManager::Get().m_pMouseIcon->m_bRender = false;
	//				pB->m_pParent->SetPosition(pA->GetCenter() - Normalize(pA->GetCenter()) * 100.0f);
	//			}
	//			else
	//			{
	//				// 킴
	//				UIManager::Get().m_pXPush->m_bRender = false;
	//				UIManager::Get().m_pTowerPanel->m_bRender = true;
	//				UIManager::Get().m_pMouseIcon->m_bRender = true;
	//			}
	//		}
	//	}
	//}

	void ForceWave(Collider* pA, Collider* pB) {
		if (pB != nullptr && (pB->m_eTag == ETag::Enemy || pB->m_eTag == ETag::Ally))
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter()) + Vector3::Up) * 180.0f);
			pA->AddIgnoreList(pB);
		}
	}

	//void ShockBoom(Collider* pA, Collider* pB)
	//{
	//	if (pB != nullptr &&
	//		pB->m_eTag == ETag::Dummy)
	//	{
	//		if (pB->m_pPhysics->m_damage >= 0.2f)
	//		{
	//			auto position = pA->GetCenter() + Vector3::Up * 20.0f;
	//			pA->m_pParent->SetPosition(Vector3::Up * 10000.0f);
	//			for (int i = 0; i < 20; ++i)
	//			{
	//				auto pItem = ObjectManager::Get().TakeObject(L"Dagger");
	//				pItem->SetPosition(position);
	//				pItem->SetRotation(RandomNormal() * Quaternion::One * PI);
	//				pItem->SetForce({RandomNormal() * 300.0f - 150.0f, 210.0f, RandomNormal() * 300.0f - 150.0f });
	//				pItem->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
	//				pItem->m_pPhysics->m_damage = 0.28f;
	//			}
	//			ObjectManager::Get().DisableObject(pA->m_pParent);
	//			SoundManager::Get().PlayQueue("SE_fire1.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	//		}
	//	}
	//}

	void MineBoom(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Enemy)
		{
			auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
			pItem->SetPosition(pA->m_pParent->GetPosition());
			pItem->SetScale(Vector3::One);
			pItem->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
			pItem->m_pPhysics->m_damage = pA->m_pPhysics->m_damage;

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
		pItem->SetScale(Vector3::One * 3.0f);
		pItem->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
		pItem->m_pPhysics->m_damage = 0.7f;

		ObjectManager::Get().DisableObject(pA->m_pParent);
		SoundManager::Get().PlayQueue("SE_bomb.mp3", pA->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	}

	void MissileCollision(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			if (pB->m_eTag != ETag::Enemy)
				return;
			if (pA->GetHP() <= 0.0f)
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
				else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
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
				pB->SetForce((Normalize(pA->GetTotalForce())) * 90.0f);
				pB->m_pParent->HealHP(pA->m_pPhysics->m_damage);
				// 내가 맞았을때
				if (pB->m_pParent == PlayerController::Get().GetParent())
				{
					UIManager::Get().m_pRespawnEffect->SetEventTime(0.2f);
					((JPanel*)UIManager::Get().m_pRespawnEffect)->EffectPlay();
				}
				// 내가 때렸을때
				if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
				{
					PlayerController::Get().HitEvent(pB);
				}
			}	break;
			case ETag::Enemy:
			{
				pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 210.0f);
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
				else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
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
			 	pB->SetForce((Normalize(pA->GetTotalForce()) + Vector3::Up) * 45.0f);
			 	pB->m_pParent->HealHP(pA->m_pPhysics->m_damage * 1.5f);
			 	// 내가 맞았을때
			 	if (pB->m_pParent == PlayerController::Get().GetParent())
			 	{
					UIManager::Get().m_pRespawnEffect->SetEventTime(0.2f);
			 		((JPanel*)UIManager::Get().m_pRespawnEffect)->EffectPlay();
			 	}
			 	// 내가 때렸을때
			 	if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
			 	{
			 		PlayerController::Get().HitEvent(pB);
			 	}
			 }	break;
			 case ETag::Enemy:
			 {
			 	pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 210.0f);
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
				else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= 0.0f)
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
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= 0.0f)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
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
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 450.0f);
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
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= 0.0f)
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
		if (pB != nullptr &&
			pB->m_eTag == ETag::Dummy)
		{
			return;
		}
		auto pObject = ObjectManager::Get().TakeObject(L"PBoom");
		pObject->SetPosition(pA->GetCenter());
		pObject->SetScale(Vector3::One);
		pObject->m_pPhysics->m_damage = 0.5f;
		pObject->m_pPhysics->UserSocket = pA->m_pPhysics->UserSocket;
		SoundManager::Get().PlayQueue("SE_bomb.mp3", pA->GetCenter(), PlayerController::Get().SoundRange);
		ObjectManager::Get().DisableObject(pA->m_pParent);
	}

	void OneTimeHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr)
		{
			if (pB->m_eTag == ETag::Dummy)
				return;

			pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 210.0f);
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
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= 0.0f)
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
			if (pB->m_eTag != ETag::Enemy)
				return;

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
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= 0.0f)
			{
				PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, ESocketType::EDummy);
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
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 300.0f);
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
			else if (pA->m_pPhysics->UserSocket == ESocketType::EDummy && PacketManager::Get().isHost && pB->m_pParent->GetHP() <= 0.0f)
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
			pB->m_pParent->m_objType == EObjType::Character &&
			pB->m_eTag == ETag::Ally)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 120.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				//ErrorMessage(L"좀비 힛 : " + pA->m_pParent->m_myName + L", " + to_wstring(pA->m_pPhysics->UserSocket));
				UIManager::Get().m_pHitEffect->SetEventTime(0.2f);
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
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 300.0f);
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

			pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 240.0f);
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
	//			if (pB->m_pParent->GetHP() <= 0.0f)
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
				auto value = (int)(RandomNormal() * JItem::Get()->m_pItemList.size());
				for (auto& iter : JItem::Get()->m_pItemList)
				{
					if (--value < 0)
					{
						if (UIManager::Get().m_pSlot1->Empty())
						{
							UIManager::Get().m_pSlot1->AddItem(iter.first);
							break;
						}
						if (UIManager::Get().m_pSlot2->Empty())
						{
							UIManager::Get().m_pSlot2->AddItem(iter.first);
							break;
						}
						if (UIManager::Get().m_pSlot3->Empty())
						{
							UIManager::Get().m_pSlot3->AddItem(iter.first);
							break;
						}
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

	// 물약
	void UsePotion(PlayerController* pPlayer, void*)
	{
		pPlayer->m_eAction = PlayerController::EAction::IPotion;
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
		pItem->SetScale(Vector3::One * 1.5f);
		pItem->m_pPhysics->UserSocket = pParent->m_pPhysics->UserSocket;
		pItem->m_pPhysics->m_damage = pParent->m_pPhysics->m_damage;
		
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

		SoundManager::Get().PlayQueue("SE_fire1.mp3", pParent->GetPosition(), PlayerController::Get().SoundRange);
	}
}

namespace DyingEvent {
	void ZombieDead(Collider* pCollider, const UINT& killUser)
	{
		if (RandomNormal() >= 0.8f)
		{
			auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
			pObject->SetPosition(pCollider->GetCenter());
			pObject->SetHP(10000.0f);
		}
		PlayerController::Get().OperEXP(0.03f);
		auto pEffect = ObjectManager::Get().TakeObject(L"EZDead");
		pEffect->SetPosition(pCollider->GetCenter());
	}

	void ZombieExDead(Collider* pCollider, const UINT& killUser)
	{
		if (RandomNormal() >= 0.4f)
		{
			auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
			pObject->SetPosition(pCollider->GetCenter());
			pObject->SetHP(10000.0f);
		}
		PlayerController::Get().OperEXP(0.2f);
		auto pEffect = ObjectManager::Get().TakeObject(L"EZDead2");
		pEffect->SetPosition(pCollider->GetCenter());
	}

	void ZombieKingDead(Collider* pCollider, const UINT& killUser)
	{
		auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
		pObject->SetPosition(pCollider->GetCenter());
		pObject->SetHP(10000.0f);

		PlayerController::Get().OperEXP(1.0f);
		auto pEffect = ObjectManager::Get().TakeObject(L"EZDead3");
		pEffect->SetPosition(pCollider->GetCenter());
		//ObjectManager::Get().DisableObject(pObject);
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
			pItem->m_pPhysics->m_damage = 0.28f;
		}
		//ObjectManager::Get().DisableObject(pCollider->m_pParent);
		SoundManager::Get().PlayQueue("SE_fire1.mp3", pCollider->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	}

	void BarrelDead(Collider* pCollider, const UINT& killUser)
	{
		auto pItem = ObjectManager::Get().TakeObject(L"PBoom");
		pItem->SetPosition(pCollider->m_pParent->GetPosition());
		pItem->SetScale(Vector3::One * 1.5f);
		pItem->m_pPhysics->UserSocket = killUser;
		pItem->m_pPhysics->m_damage = 1.0f;

		//ObjectManager::Get().DisableObject(pCollider->m_pParent);
		SoundManager::Get().PlayQueue("SE_bomb.mp3", pCollider->m_pParent->GetPosition(), PlayerController::Get().SoundRange);
	}
}