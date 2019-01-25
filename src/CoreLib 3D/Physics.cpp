#include "Physics.h"




void Physics::AddForce(const D3DXVECTOR3& vForce) noexcept
{
	m_pPhysics->m_force += vForce;
}

void Physics::SetForce(const D3DXVECTOR3& vForce) noexcept
{
	m_pPhysics->m_force = vForce;
}

void Physics::SetDirectionForce(const D3DXVECTOR3& vForce) noexcept
{
	m_pPhysics->m_isMoving = true;
	m_pPhysics->m_direction = vForce;
}

const D3DXVECTOR3 Physics::GetForce() const noexcept
{
	return m_pPhysics->m_force;
}

const D3DXVECTOR3 Physics::GetTotalForce() const noexcept
{
	return m_pPhysics->m_force + m_pPhysics->m_direction;
}


const float Physics::GetVelocity() const noexcept
{
	return VectorLength(m_pPhysics->m_force + m_pPhysics->m_direction);
}

const float Physics::GetVelocitySq() const noexcept
{
	return VectorLengthSq(m_pPhysics->m_force + m_pPhysics->m_direction);
}



void Physics::SetGravityScale(const float& gravityRate) noexcept
{
	m_pPhysics->m_GravityScale = gravityRate;
}

void Physics::usePhysics(const bool& usePhysics) noexcept
{
	m_pPhysics->m_usePhysics = usePhysics;
}
bool Physics::usePhysics()	noexcept
{
	return m_pPhysics->m_usePhysics;
}

void Physics::isMoving(const bool& isMoving) noexcept
{
	m_pPhysics->m_isMoving = isMoving;
	if (!m_pPhysics->m_isMoving)
		m_pPhysics->m_direction = Vector3::Zero;
}
bool Physics::isMoving() noexcept
{
	return m_pPhysics->m_isMoving;
}

void Physics::isGround(const bool& isGround) noexcept
{
	m_pPhysics->m_isGround = isGround;
}
bool Physics::isGround() noexcept
{
	return m_pPhysics->m_isGround;
}


void Physics::SetDamage(const float& value, const UCHAR& stat)	noexcept
{
	m_pPhysics->m_damage = value + (value * stat * 0.15f);
}
//void Physics::SetArmor(const UCHAR& armorStat)	noexcept
//{
//	m_pPhysics->m_armor = 5.0f / (5 + armorStat);
//}

void Physics::OperHP(const float& value) noexcept
{
	m_pPhysics->m_curHP = max(m_pPhysics->m_curHP + value, 0.0f);
}

void Physics::HealHP(const float& value) noexcept
{
	m_pPhysics->m_curHP = min(m_pPhysics->m_curHP + value, m_pPhysics->m_maxHP);
}

void Physics::SetHP(const float& value) noexcept
{
	m_pPhysics->m_curHP = max(value, 0.0f);
	if (value > m_pPhysics->m_maxHP)
	{
		m_pPhysics->m_maxHP = value;
	}
}


const float& Physics::GetHP() const noexcept
{
	return m_pPhysics->m_curHP;
}