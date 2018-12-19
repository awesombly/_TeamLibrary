#include "Transform.h"
#include "GameObject.h"

Transform::Transform(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const D3DXVECTOR3& scale) noexcept
{
	m_position = position;
	m_rotation = rotation;
	m_scale	   = scale;
}

void Transform::Transformation(const Transform& transform) noexcept
{
	m_position += transform.m_position;
	m_rotation += transform.m_rotation;
	m_scale    += transform.m_scale;
}
void Transform::Translate(const D3DXVECTOR3& position) noexcept
{
	m_position += position;
}
void Transform::Translate(const float& x, const float& y, const float& z) noexcept
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}
void Transform::Translate(const float& x, const float& y) noexcept
{
	m_position.x += x;
	m_position.y += y;
}
void Transform::Rotate(const D3DXQUATERNION& rotation) noexcept
{
	m_rotation += rotation;
}
void Transform::Rotate(const D3DXVECTOR3& rotation) noexcept
{
	m_rotation.x += rotation.x;
	m_rotation.y += rotation.y;
	m_rotation.z += rotation.z;
}
void Transform::Rotate(const float& x, const float& y, const float& z) noexcept
{
	m_rotation.x += x;
	m_rotation.y += y;
	m_rotation.z += z;
}
void Transform::Rotate(const float& x, const float& y) noexcept
{
	m_rotation.x += x;
	m_rotation.y += y;
}
void Transform::Scaling(const D3DXVECTOR3& scale) noexcept
{
	m_scale += scale;
}
void Transform::Scaling(const float& x, const float& y, const float& z) noexcept
{
	m_scale.x += x;
	m_scale.y += y;
	m_scale.z += z;
}
void Transform::SetFocus(const D3DXVECTOR3& target) noexcept
{
	D3DXVECTOR3 forward, dirTarget;
	forward = GetForward();
	forward.y = 0.0f;
	dirTarget = target - m_position;
	dirTarget.y = 0.0f;
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&dirTarget, &dirTarget);
	float fRadian = acosf(D3DXVec3Dot(&dirTarget, &forward));

	if(D3DXVec3Dot(&GetRight(), &dirTarget) > 0.0f)
		m_rotation.y = fRadian;
	else
		m_rotation.y = -fRadian;

	//static D3DXMATRIX matFocus;
	//D3DXVECTOR3 dir;
	//dir = target - m_position;
	//D3DXVec3Normalize(&dir, &dir);

	//D3DXMatrixLookAtLH(&matFocus, &m_position, &dir, &GetUp());
	//D3DXVECTOR3 a;
	//D3DXQUATERNION rot;
	//D3DXMatrixDecompose(&m_scale, &rot, &a, &matFocus);
	//m_rotation *= rot;
	//float angle;
	//D3DXQuaternionToAxisAngle(&rot, &a, &angle);
	//D3DXMatrixRotationAxis()
}

void Transform::SetTransform(const Transform& transform) noexcept
{
	*this = transform;
}
void Transform::SetPosition(const D3DXVECTOR3& position) noexcept
{
	m_position = position;
}
void Transform::SetPosition(const float& x, const float& y, const float& z) noexcept
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
void Transform::SetPositionX(const float& x) noexcept
{
	m_position.x = x;
}
void Transform::SetPositionY(const float& y) noexcept
{
	m_position.y = y;
}
void Transform::SetPositionZ(const float& z) noexcept
{
	m_position.z = z;
}
void Transform::SetRotation(const D3DXQUATERNION& rotation) noexcept
{
	m_rotation = rotation;
}
void Transform::SetRotation(const float& x, const float& y, const float& z) noexcept
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}
void Transform::SetRotationX(const float& x) noexcept
{
	m_rotation.x = x;
}
void Transform::SetRotationY(const float& y) noexcept
{
	m_rotation.y = y;
}
void Transform::SetRotationZ(const float& z) noexcept
{
	m_rotation.z = z;
}
void Transform::SetScale(const D3DXVECTOR3& scale) noexcept
{
	m_scale = scale;
}
void Transform::SetScale(const float& x, const float& y, const float& z) noexcept
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}
void Transform::SetScaleX(const float& x) noexcept
{
	m_scale.x = x;
}
void Transform::SetScaleY(const float& y) noexcept
{
	m_scale.y = y;
}
void Transform::SetScaleZ(const float& z) noexcept
{
	m_scale.z = z;
}
const Transform& Transform::GetTransform() const noexcept
{
	return *this;
}
const D3DXVECTOR3& Transform::GetPosition() const noexcept
{
	return m_position;
}
const D3DXQUATERNION& Transform::GetRotation() const noexcept
{
	return m_rotation;
}
const D3DXVECTOR3& Transform::GetScale() const noexcept
{
	return m_scale;
}
float Transform::GetScaleAverage() const noexcept
{
	return (m_scale.x + m_scale.y + m_scale.z) / 3.0f;
}

const D3DXVECTOR3& Transform::GetForward()	const noexcept
{
	return m_Look;
}
const D3DXVECTOR3  Transform::GetBackward()	const noexcept
{
	return -m_Look;
}
const D3DXVECTOR3& Transform::GetRight()	const noexcept
{
	return m_Side;
}
const D3DXVECTOR3  Transform::GetLeft()		const noexcept
{
	return -m_Side;
}
const D3DXVECTOR3& Transform::GetUp()	const noexcept
{
	return m_Up;
}
const D3DXVECTOR3  Transform::GetDown()		const noexcept
{
	return -m_Up;
}