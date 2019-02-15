#pragma once
#include "header.h"
class Collider;
class PlayerController;
class GameObject;

namespace MyEvent {
	//void CarpetChurch(Collider* pA, Collider* pB);
	//void CarpetSmithy(Collider* pA, Collider* pB);
	//void CarpetShop(Collider* pA, Collider* pB);
	//void CarpetTower(Collider* pA, Collider* pB);
	void ForceWave(Collider* pA, Collider* pB);
	void MissileCollision(Collider* pA, Collider* pB);
	//void ShockBoom(Collider* pA, Collider* pB);
	void MineBoom(Collider* pA, Collider* pB);
	void NuclearBoom(Collider* pA, Collider* pB);
	void BuffWave(Collider* pA, Collider* pB);
	void EnergyBall(Collider* pA, Collider* pB);
	void BerserkMode(Collider* pA, Collider* pB);
	void PlayerAttack(Collider* pA, Collider* pB);
	void PlayerBomb(Collider* pA, Collider* pB);
	void OneTimeHit(Collider* pA, Collider* pB);
	void DaggerHit(Collider* pA, Collider* pB);
	void MeleeHit(Collider* pA, Collider* pB);
	void ZombieHit(Collider* pA, Collider* pB);
	void ZombieAttack(Collider* pA, Collider* pB);
	void ZombieThrow(Collider* pA, Collider* pB);
	//void OneShots(Collider* pA, Collider* pB);
	//void GiantItem(Collider* pA, Collider* pB);
	void ItemBox(Collider* pA, Collider* pB);

	// ·Îµù¿ë
	void BulletHit(Collider* pA, Collider* pB);
	void EnemyHit(Collider* pA, Collider* pB);
}

namespace ActiveEvent {
	void ShockWave(PlayerController* pPlayer, void*);
	void ThrowBomb(PlayerController* pPlayer, void*);
	void ThrowTimeBomb(PlayerController* pPlayer, void*);
	void ThrowShockBoom(PlayerController* pPlayer, void*);
	void ThrowMissile(PlayerController* pPlayer, void*);
	void ThrowMine(PlayerController* pPlayer, void*);
	void ThrowNuclear(PlayerController* pPlayer, void*);
	void UsePotion(PlayerController* pPlayer, void*);
}

namespace TimeEvent {
	void ArrowRain(GameObject* pParent, void* pVoid2);
	void TimeBomb(GameObject* pParent, void* pVoid2);
	void MissileShot(GameObject* pParent, void* pVoid2);
	void NuclearLaunch(GameObject* pParent, void* pVoid2);
}

namespace DyingEvent {
	void ZombieDead(Collider* pCollider, const UINT& killUser);
	void ZombieExDead(Collider* pCollider, const UINT& killUser);
	void ZombieKingDead(Collider* pCollider, const UINT& killUser);
	
	void ShockBoomDead(Collider* pCollider, const UINT& killUser);
	void BarrelDead(Collider* pCollider, const UINT& killUser);
}