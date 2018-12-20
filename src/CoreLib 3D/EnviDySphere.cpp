//#include "EnviDySphere.h"
//#include "ObjectManager.h"
//#include "Light.h"
//
//CB_CubeViewMatrix	EnviDySphere::m_cbCubeMatrix;
//ID3D11Buffer*		EnviDySphere::m_pCubeCBuffer;
//
//EnviDySphere::EnviDySphere(const UINT& slices, const wstring_view& myName, const wstring_view& srcName,
//					   const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
//{
//	m_radius = 1.0f;
//	m_slices = slices;
//	SetInfo(myName, EComponent::Etc, srcName, vertexShaderName, pixelShaderName);
//	Create();
//	Init();
//	CreateConstBuffer((void*)&m_cbCubeMatrix, sizeof(m_cbCubeMatrix), &m_pCubeCBuffer);
//
//	m_PS_Fresnel = DxManager::GetInstance().m_PShaderList["PS_Fresnel"];
//	m_PS_Refraction = DxManager::GetInstance().m_PShaderList["PS_Refraction"];
//	m_pLight = ObjectManager::Lights.front();
//
//	m_pSpriteList = new vector<Sprite>();
//	m_pSpriteList->emplace_back(*m_curSprite);
//	//m_pSpriteList->emplace_back(*DxManager::GetInstance().m_RTDSViewCube.GetScreenSprite());
//	m_isMultiTexture = true;
//}
//
//bool EnviDySphere::Init() noexcept
//{
//	RSphere::Init();
//	D3DXMatrixLookAtLH(&m_cbCubeMatrix.matViewCube[0], &Vector3::Zero, &Vector3::Right,		&Vector3::Up);
//	D3DXMatrixLookAtLH(&m_cbCubeMatrix.matViewCube[1], &Vector3::Zero, &Vector3::Left,		&Vector3::Up);
//	D3DXMatrixLookAtLH(&m_cbCubeMatrix.matViewCube[2], &Vector3::Zero, &Vector3::Up,		&Vector3::Backward);
//	D3DXMatrixLookAtLH(&m_cbCubeMatrix.matViewCube[3], &Vector3::Zero, &Vector3::Down,		&Vector3::Forward);
//	D3DXMatrixLookAtLH(&m_cbCubeMatrix.matViewCube[4], &Vector3::Zero, &Vector3::Forward,	&Vector3::Up);
//	D3DXMatrixLookAtLH(&m_cbCubeMatrix.matViewCube[5], &Vector3::Zero, &Vector3::Backward,	&Vector3::Up);
//	for (auto& iter : m_cbCubeMatrix.matViewCube)
//	{
//		D3DXMatrixTranspose(&iter, &iter);
//	}
//	return true;
//}
//
//bool EnviDySphere::Frame(const float& spf, const float& accTime) noexcept
//{
//	if (!m_isEnable) return false;
//	//RSphere::Frame(spf, accTime);
//	return true;
//	spf; accTime;
//}
//
//
//bool EnviDySphere::Render(ID3D11DeviceContext* pDContext) noexcept
//{
//	if (!m_isEnable) return false;
//
//	/////////////////////////////////////////////
//	RSphere::PrevRender(pDContext);
//	pDContext->PSSetShaderResources(2, 1, &DxManager::GetInstance().m_RTDSViewCube.GetScreenSprite()->m_pTexture->GetSRView());
//	// 상수 버퍼
//	//pDContext->GSSetConstantBuffers(3, 1, &m_pCubeCBuffer);
//	pDContext->UpdateSubresource(m_pCubeCBuffer, 0, nullptr, &m_cbCubeMatrix, 0, 0);
//	pDContext->GSSetShader(nullptr, nullptr, 0);
//
//
//	static int flagPS = 0;
//	if (Input::GetKeyState('A') == EKeyState::HOLD)
//	{
//		flagPS = 0;
//	}
//	if (Input::GetKeyState('S') == EKeyState::HOLD)
//	{
//		flagPS = 1;
//	}
//	else if (Input::GetKeyState('D') == EKeyState::HOLD)
//	{
//		flagPS = 2;
//	}
//	if (flagPS == 1)
//		pDContext->PSSetShader(m_PS_Fresnel, nullptr, 0);
//	else if (flagPS == 2)
//		pDContext->PSSetShader(m_PS_Refraction, nullptr, 0);
//	RSphere::PostRender(pDContext);
//	return true;
//}
//
//bool EnviDySphere::Release()	noexcept
//{
//	RSphere::Release();
//	return true;
//}
//
//
//void EnviDySphere::UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept
//{
//	// 행렬 버퍼 갱신
//	static D3D11_MAPPED_SUBRESOURCE MappedResource;
//	MappedResource = { 0, };
//	// 접근하고 수정하고 접근해제
//	pDContext->Map(m_pMatrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
//	CB_VSMatrix* pConstData = (CB_VSMatrix*)MappedResource.pData;		// 바꿀 객체
//
//	*pConstData = (*m_ppCamera)->m_cbVS;
//	pConstData->m_matWorld = m_pParent->GetWorldMatrix();
//	pDContext->Unmap(m_pMatrixCBuffer, 0);
//
//	
//}
//
//Component* EnviDySphere::clone() noexcept
//{
//	//return (Component*)(new EnviDySphere(*this));
//	return (new EnviDySphere(*this))->cloneAddition();
//}