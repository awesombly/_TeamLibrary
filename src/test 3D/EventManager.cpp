#include "EventManager.h"
#include "PacketManager.h"
#include "PlayerController.h"
#include "ObjectManager.h"


namespace MyEvent {
	void ForceWave(Collider* pMe, Collider* pYou) {
		if (pYou != nullptr && pYou->m_eTag != ETag::Dummy)
		{
			pYou->SetForce((Normalize(pYou->GetCenter() - pMe->GetCenter()) + Vector3::Up) * 60.0f);
			pMe->AddIgnoreList(pYou);
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
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), 1000.0f);
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
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"Slash");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), 1000.0f);
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
				((JPanel*)PlayerController::Get().m_pHitEffect)->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
			pEffect->SetPosition(pA->m_pParent->GetPosition());
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), 1000.0f);
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
				((JPanel*)PlayerController::Get().m_pHitEffect)->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
			auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
			pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
			//ObjectManager::Get().RemoveObject(pA->m_pParent);
			//SoundManager::Get().PlayQueue("SE_HIT.mp3", pA->m_pParent->GetWorldPosition(), 1000.0f);
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
				((JPanel*)PlayerController::Get().m_pHitEffect)->EffectPlay();
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					PacketManager::Get().SendDeadEvent(pB->m_pParent->m_keyValue, pB->m_pPhysics->UserSocket, pA->m_pPhysics->UserSocket);
				}
			}
		}
		auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
		pEffect->SetPosition(pA->m_pParent->GetPosition());
		ObjectManager::Get().DisableObject(pA->m_pParent);
		//SoundManager::Get().Play("SE_HIT.mp3");//, pObject->GetWorldPosition(), 1000.0f);
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