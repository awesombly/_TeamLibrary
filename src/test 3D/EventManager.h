#pragma once
class Collider;
class GameObject;


namespace MyEvent {
	void ForceWave(Collider* pA, Collider* pB);
	void DaggerHit(Collider* pA, Collider* pB);
	void MeleeHit(Collider* pA, Collider* pB);
	void ZombieHit(Collider* pA, Collider* pB);
	void ZombieAttack(Collider* pA, Collider* pB);
	void OneShots(Collider* pA, Collider* pB);
	void GiantItem(Collider* pA, Collider* pB);

	// ·Îµù¿ë
	void BulletHit(Collider* pA, Collider* pB);
	void EnemyHit(Collider* pA, Collider* pB);
}