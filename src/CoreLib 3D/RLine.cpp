#include "RLine.h"
#include "ObjectManager.h"

RLine::RLine(const wstring_view& myName, const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_ppCamera = &ObjectManager::CurCamera;
	m_myName = myName;
	SetShaderLayout(vertexShaderName, pixelShaderName);
	//if (srcName.empty())
		//m_srcName = L"None";
	//else
		//m_srcName = srcName;
	Create();
}

bool RLine::Init() noexcept
{
	Renderer::Init();
	m_comptType = EComponent::Etc;
	return true;
}

bool RLine::Frame(const float& spf, const float& accTime)	noexcept
{
	//if (!m_isEnable) return false;
	//Renderer::Frame(spf, accTime);
	return true;
	spf; accTime;
}

bool RLine::PrevRender(ID3D11DeviceContext* pDContext) noexcept
{
	// ���ҽ��� �ȼ� ���̴��� ���ε�(���� �ε���, ���̴� �ڿ� ����, ���ҽ���)
	pDContext->IASetInputLayout(m_pInputLayout);
	// ������Ʈ ���̴�, ����, ���ҽ� ���� -> ��ο�
	UpdateConstBuffer(pDContext);
	pDContext->VSSetConstantBuffers(0, 1, &m_pMatrixCBuffer);	// ��� ����
	pDContext->VSSetShader(m_pVShader, nullptr, 0);			// �������̴�
	pDContext->PSSetShader(m_pPShader, nullptr, 0);			// �ȼ� ���̴�
	return true;
}

bool RLine::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	static UINT stride = (UINT)sizeof(Vertex_PC);
	pDContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &Offset);
	//pDContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//pDContext->DrawIndexed(2, 0, 0);
	pDContext->Draw(2, 0);
	return true;
}

bool RLine::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (m_isDebugLine && !Input::isDebug) return false;
	// �����Ͷ����� ����
	DxManager::GetInstance().SetRasterizerState(ERasterS::Line);
	Renderer::Render(pDContext);
	DxManager::GetInstance().SetRasterizerState(ERasterS::Current);
	return true;
}

bool RLine::Release()	noexcept
{
	Renderer::Release();
	return true;
}

void RLine::SetLineInfo(ID3D11DeviceContext* pDContext, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR4& color, const bool& isDebugLine) noexcept
{
	m_isDebugLine = isDebugLine;
	m_vertexList[0].pos = start;
	m_vertexList[1].pos = end;
	m_vertexList[0].col = color;
	m_vertexList[1].col = color;
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}

HRESULT RLine::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);
	// ���� ����Ʈ
	m_vertexList.resize(2);

	// 2���� �� ǥ��
	m_vertexList[0] = { { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f,  1.0f, 1.0f } };
	m_vertexList[1] = { { 1.0f,  0.0f, 0.0f }, { 0.0f, 1.0f,  1.0f, 1.0f } };

	if (FAILED(CreateVertexBuffer()) ||
		FAILED(CreateIndexBuffer(2) ))
		return E_FAIL;
	Init();
	return S_OK;
}

HRESULT RLine::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC initDesc = { 0, };
	initDesc.ByteWidth = (UINT)(m_vertexList.size() * sizeof(m_vertexList.front()));	// ����ũ�� * ����
	initDesc.Usage = D3D11_USAGE_DEFAULT;				// ��� ���(CPU->GPU ��������)
	initDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// ���� Ÿ��
	initDesc.CPUAccessFlags = 0;						// CPU ���� ����
	initDesc.MiscFlags = 0;
	//initDesc.StructureByteStride = 0;
	//initData.SysMemPitch = 0;
	//initData.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA InitData = { 0, };
	InitData.pSysMem = &m_vertexList.at(0);		// GPU�� ���� �� �޸�

	if (m_pVertexBuffer != nullptr)
		m_pVertexBuffer->Release();
	//					���� ����(��������,	   �ҽ� ����, ��� ����)
	return DxManager::GetDevice()->CreateBuffer(&initDesc, &InitData, &m_pVertexBuffer);
}

bool RLine::isDebugLine()
{
	return m_isDebugLine;
}
void RLine::isDebugLine(const bool& isDebugLine)
{
	m_isDebugLine = isDebugLine;
}

Component* RLine::clone() noexcept
{
	//return (Component*)(new RLine(*this));
	return (new RLine(*this))->cloneAddition();
}