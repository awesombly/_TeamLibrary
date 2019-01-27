#include "EventManager.h"
#include "PacketManager.h"
#include "PlayerController.h"
#include "ObjectManager.h"
#include "UIManager.h"


namespace MyEvent {
	void ForceWave(Collider* pMe, Collider* pYou) {
		if (pYou != nullptr && pYou->m_eTag != ETag::Dummy)
		{
			pYou->SetForce((Normalize(pYou->GetCenter() - pMe->GetCenter()) + Vector3::Up) * 60.0f);
			pMe->AddIgnoreList(pYou);
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
			//// 내가 맞았을때
			//if (pB->m_pParent == PlayerController::Get().GetParent())
			//{
			//	UIManager::Get().m_pHitEffect->EffectPlay();
			//}
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
			auto pEffect = ObjectManager::Get().TakeObject(L"PSlash");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void PlayerBomb(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_eTag == ETag::Enemy)
		{
			auto pObject = ObjectManager::Get().TakeObject(L"PBoom");
			pObject->SetPosition(pA->GetCenter());
			pObject->m_pPhysics->m_damage = 1.0f;

			ObjectManager::Get().DisableObject(pA->m_pParent);
		}
	}

	void DaggerHit(Collider* pA, Collider* pB)
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
		auto pEffect = ObjectManager::Get().TakeObject(L"PAttack");
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
			auto pEffect = ObjectManager::Get().TakeObject(L"PSlash");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), SoundRange);
		}
	}

	void ZombieHit(Collider* pA, Collider* pB)
	{
		if (pB != nullptr &&
			pB->m_pParent->m_objType == EObjType::Character)
		{
			pB->SetForce((Normalize(pB->GetCenter() - pA->GetCenter())) * 40.0f);
			pB->m_pParent->OperHP(-pA->m_pPhysics->m_damage);
			// 내가 맞았을때
			if (pB->m_pParent == PlayerController::Get().GetParent())
			{
				UIManager::Get().m_pHitEffect->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
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
				UIManager::Get().m_pHitEffect->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//ObjectManager::Get().RemoveObject(pA->m_pParent);
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
				UIManager::Get().m_pHitEffect->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
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
			auto pEffect = ObjectManager::Get().TakeObject(L"PAttack");
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
	void ShockWave(PlayerController* pPlayer, void*)
	{
		// 충격파
		pPlayer->m_eAction = PlayerController::EAction::ShockWave;
	}

	void ThrowBomb(PlayerController* pPlayer, void*)
	{
		// 폭탄
		pPlayer->m_eAction = PlayerController::EAction::ThrowBomb;
	}
}