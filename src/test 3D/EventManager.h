#pragma once
class Collider;
class PlayerController;
class GameObject;

namespace MyEvent {
	void ForceWave(Collider* pA, Collider* pB);
	void BuffWave(Collider* pA, Collider* pB);
	void EnergyBall(Collider* pA, Collider* pB);
	void PlayerAttack(Collider* pA, Collider* pB);
	void PlayerBomb(Collider* pA, Collider* pB);
	void DaggerHit(Collider* pA, Collider* pB);
	void MeleeHit(Collider* pA, Collider* pB);
	void ZombieHit(Collider* pA, Collider* pB);
	void ZombieAttack(Collider* pA, Collider* pB);
	void ZombieThrow(Collider* pA, Collider* pB);
	void OneShots(Collider* pA, Collider* pB);
	void GiantItem(Collider* pA, Collider* pB);
	void ItemBox(Collider* pA, Collider* pB);

	// ·Îµù¿ë
	void BulletHit(Collider* pA, Collider* pB);
	void EnemyHit(Collider* pA, Collider* pB);
}

namespace ActiveEvent {
	void ShockWave(PlayerController* pPlayer, void*);
	void ThrowBomb(PlayerController* pPlayer, void*);
}

namespace TimeEvent {
	void ArrowRain(GameObject* pParent, void* pVoid2);
}