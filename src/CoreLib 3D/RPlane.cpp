#include "RPlane.h"
#include "ObjectManager.h"


RPlane::RPlane(const wstring_view& myName, const wstring_view& srcName,
				 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	SetInfo(myName, EComponent::Renderer, srcName, vertexShaderName, pixelShaderName);
	Create();
}

bool RPlane::Init() noexcept
{
	Renderer::Init();
	return true;
}

bool RPlane::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	Renderer::Frame(spf, accTime);
	return true;
}

bool RPlane::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!m_isEnable) return false;
	Renderer::Render(pDContext);
	return true;
}

bool RPlane::Release()	noexcept
{
	Renderer::Release();
	return true;
}

//
//bool RPlane::CollisionCheck(const POINT& point) const noexcept
//{
//	D3DXVECTOR3 tVector[4];
//	for (int i = 0; i < 4; i++)
//	{
//		D3DXVec3TransformCoord(&tVector[i], &m_vertexList[i].pos, &m_matLocal);
//		D3DXVec3TransformCoord(&tVector[i], &tVector[i], &m_pCamera->m_matView);
//	}
//	D3DXVECTOR4 rect;
//	rect.x = tVector[0].x;
//	rect.y = tVector[0].y;
//	rect.z = tVector[1].x;
//	rect.w = tVector[2].y;
//	if (rect.x > rect.z)
//	{
//		rect.x = tVector[1].x;
//		rect.z = tVector[0].x;
//		//ErrorMessage(L""s + m_myName + L" -> 좌우반전");
//	}
//	if (rect.y > rect.w)
//	{
//		rect.y = tVector[2].y;
//		rect.w = tVector[0].y;
//		//ErrorMessage(L""s + m_myName + L" -> 상하반전");
//	}
//	if (point.x > rect.x && point.x < rect.z &&
//		point.y > rect.y && point.y < rect.w)
//	{
//		ErrorMessage(L""s + m_myName + L" -> 충돌했셈!");
//		return true;
//	}
//	return false;
//}


HRESULT RPlane::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);
	// 정점 리스트
	m_vertexList.resize(4);
	
	m_vertexList[0] = { { -1.0f, 1.0f,  0.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 1.0f, 1.0f,  1.0f, 1.0f },
						{ 0.0f, 0.0f } };
	m_vertexList[1] = { { 1.0f, 1.0f,  0.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 1.0f, 1.0f,  1.0f, 1.0f },
						{ 1.0f, 0.0f } };
	m_vertexList[2] = { { -1.0f, -1.0f, 0.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 1.0f, 1.0f,  1.0f, 1.0f },
						{ 0.0f, 1.0f } };
	m_vertexList[3] = { { 1.0f, -1.0f, 0.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 1.0f, 1.0f,  1.0f, 1.0f },
						{ 1.0f, 1.0f } };
	// 버퍼들 생성
	if(FAILED(CreateVertexBuffer()) ||
	   FAILED(CreateIndexBuffer(6)))
		return E_FAIL;
	Init();
	return S_OK;
}

Component* RPlane::clone() noexcept
{
	//return (Component*)(new RPlane(*this));
	return (new RPlane(*this))->cloneAddition();
}