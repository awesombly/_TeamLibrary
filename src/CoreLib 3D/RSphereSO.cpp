#include "RSphereSO.h"
#include "ObjectManager.h"

RSphereSO::RSphereSO(const float& radius, const UINT& slices, const wstring_view& myName, const wstring_view& srcName,
				 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_radius = radius;
	m_slices = slices;
	SetInfo(myName, EComponent::Renderer, srcName, vertexShaderName, pixelShaderName);
	//Create();
	m_pGShader = DxManager::GetInstance().m_GShaderList["GS_SO"];
}

bool RSphereSO::Init() noexcept
{
	Renderer::Init();
	return true;
}

bool RSphereSO::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	Renderer::Frame(spf, accTime);
	return true;
}


bool RSphereSO::PrevRender(ID3D11DeviceContext* pDContext) noexcept
{
	// ���ҽ��� �ȼ� ���̴��� ���ε�(���� �ε���, ���̴� �ڿ� ����, ���ҽ���)
	pDContext->IASetInputLayout(m_pInputLayout);
	// ������Ʈ ���̴�, ����, ���ҽ� ���� -> ��ο�
	UpdateConstBuffer(pDContext);
	pDContext->VSSetConstantBuffers(0, 1, &m_pMatrixCBuffer);	// ��� ����
	pDContext->VSSetShader(m_pVShader, nullptr, 0);			// �������̴�
	pDContext->PSSetShader(m_pPShader, nullptr, 0);			// �ȼ� ���̴�
	pDContext->GSSetShader(m_pGShader, nullptr, 0);			// ���� ���̴�	
	//// �ؽ��� ���ҽ� ���� 
	//if (m_isMultiTexture)
	//{
	//	for (char index = 0; index < m_pSpriteList->size(); ++index)
	//	{
	//		pDContext->PSSetShaderResources(index, 1, &(*m_pSpriteList)[index].m_pTexture->GetSRView());
	//	}
	//}
	//else
	//{
		m_curSprite->m_pTexture->SetShaderResource(pDContext);
	//}
	return true;
}

bool RSphereSO::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	//pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
	ID3D11Buffer* pVB[1] = { m_RenderSO.m_pDrawFrom };
	pDContext->IASetVertexBuffers(0, 1, pVB, &Stride, &Offset);
	pDContext->DrawAuto();
	//pDContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//pDContext->DrawIndexed(m_indexCount, 0, 0);
	return true;
}

bool RSphereSO::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	Renderer::Render(pDContext);
	return true;
}

bool RSphereSO::Release()	noexcept
{
	Renderer::Release();
	return true;
}

void RSphereSO::SetSO(ID3D11DeviceContext* pDContext) noexcept
{
	DxManager::GetInstance().SetDepthStencilState(EDepthS::D_Off_NoWrite);
	m_RenderSO.CreateStreamBuffer(sizeof(Vertex_PNCT), 10000);

	m_RenderSO.BeginStream(pDContext);
	PrevRender(pDContext);
	Renderer::PostRender(pDContext);
	m_RenderSO.EndStream(pDContext);

	for (int iStream = 0; iStream < m_numStreamOut; ++iStream)
	{
		m_RenderSO.BeginStream(pDContext);
		Render(pDContext);
		m_RenderSO.EndStream(pDContext);
	}
	m_pGShader = nullptr;
	DxManager::GetInstance().SetDepthStencilState(EDepthS::Current);
}


HRESULT RSphereSO::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);

	//if (FAILED(CreateVertexBuffer()) ||
		//FAILED(CreateIndexBuffer((UINT)m_indexList.size(), false)))
		//return E_FAIL;
	SetSO(DxManager::GetDContext());
	Init();
	return S_OK;
}


Component* RSphereSO::clone() noexcept
{
	///return (Component*)(new RSphereSO(*this));
	return (new RSphereSO(*this))->cloneAddition();
}