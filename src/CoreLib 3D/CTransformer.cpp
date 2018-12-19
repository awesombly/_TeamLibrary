#include "CTransformer.h"
#include "GameObject.h"


CTransformer::CTransformer()
{
	m_myName = L"Transformer";
	m_comptType = EComponent::Phsics;
	Init();
}

CTransformer::CTransformer(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const D3DXVECTOR3& scale)
{
	m_myName = L"Transformer";
	m_comptType = EComponent::Phsics;
	Init();
	SetTransform({ position, rotation, scale });
}

bool CTransformer::Init() noexcept
{
	TransEvent = [](Transform* pParent, Transform* pTrans, const float& spf, const float& accTime) {
		pParent->Translate(pTrans->GetPosition() * spf);
		pParent->Rotate(pTrans->GetRotation() * spf);
		pParent->Scaling(pTrans->GetScale() * spf);
		return; accTime;
	};
	return true;
}

bool CTransformer::Frame(const float& spf, const float& accTime)	noexcept
{
	TransEvent(m_pParent, this, spf, accTime);
	return true;
	accTime;
}

bool CTransformer::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool CTransformer::Release()	noexcept
{
	return true;
}

Component* CTransformer::clone() noexcept
{
	return (Component*)new CTransformer(*this);
}