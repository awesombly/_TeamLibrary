#include "InstanceRenderer.h"
#include "ObjectManager.h"

InstanceRenderer::InstanceRenderer(const wstring_view& myName, const EComponent& eComType, const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_ppCamera = &ObjectManager::CurCamera;
	m_myName = m_srcName = myName;
	m_comptType = eComType;
	SetShaderLayout(vertexShaderName, pixelShaderName);
	m_eRenderType = ERenderType::Instance;
	m_isMultiTexture = true;
}

bool InstanceRenderer::Init() noexcept
{
	m_curObject = m_objectList.begin();
	m_curSprite = m_pSpriteList->begin();
	m_pBuffers[0] = m_pVertexBuffer;
	m_pBuffers[1] = m_pInstanceBuffer;
	Strides[0] = sizeof(Vertex_PNCT);
	Strides[1] = sizeof(InstanceData);
	Offsets[0] = 0;
	Offsets[1] = 0;
	return true;
}

bool InstanceRenderer::Frame(const float& spf, const float& accTime) noexcept
{
	if (m_dataList.empty())
		return false;

	m_curObject = m_objectList.begin();
	for (auto& iter : m_dataList)
	{
		if ((*m_curObject)->m_refTexture >= m_pSpriteList->size())
			(*m_curObject)->m_refTexture = 0.1f;
		iter.matWorld = (*m_curObject)->m_pParent->GetWorldMatrix();
		iter.numTexture = (*m_curObject)->m_refTexture;
		//iter.color = Color::White;

		++m_curObject;
	}
	return true;
	spf; accTime;
}


bool InstanceRenderer::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	pDContext->IASetVertexBuffers(0, 2, m_pBuffers, Strides, Offsets);
	pDContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pDContext->DrawIndexedInstanced(m_indexCount, (UINT)m_dataList.size(), 0, 0, 0);
	return true;
}

bool InstanceRenderer::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable || m_dataList.empty())	return false;
	// �ν��Ͻ� ����
	if (m_pInstanceBuffer != nullptr)
		UpdateInstanceBuffer(pDContext);
	PrevRender(pDContext);
	PostRender(pDContext);
	return true;
}

bool InstanceRenderer::Release() noexcept
{
	Renderer::Release();
	return true;
}


HRESULT InstanceRenderer::Create() noexcept
{
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);

	if (FAILED(CreateVertexBuffer()) ||
		FAILED(CreateIndexBuffer((UINT)m_indexList.size(), false)))
		return E_FAIL;
	Init();
	return S_OK;
}


void InstanceRenderer::UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept
{
	// ������۸� ����
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	MappedResource = { 0, };
	// �����ϰ� �����ϰ� ��������
	pDContext->Map(m_pMatrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	CB_VSMatrix* pConstData = (CB_VSMatrix*)MappedResource.pData;		// �ٲ� ��ü

	*pConstData = (*m_ppCamera)->m_cbVS;
	pConstData->m_matWorld = Matrix::Identity;
	pDContext->Unmap(m_pMatrixCBuffer, 0);
	//pDContext->UpdateSubresource(m_pMatrixCBuffer, 0, nullptr, &m_pCamera->m_cbVS, 0, 0);
}

void InstanceRenderer::UpdateInstanceBuffer(ID3D11DeviceContext* pDContext) noexcept
{
	// �ν��Ͻ� ���� ����
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	MappedResource = { 0, };
	// �����ϰ� �����ϰ� ��������
	pDContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	InstanceData* pInstance = (InstanceData*)MappedResource.pData;		// �ٲ� ��ü

	//pInstance = &m_dataList.at(0);
	memcpy(pInstance, &m_dataList.at(0), sizeof(InstanceData) * m_dataList.size());
	pDContext->Unmap(m_pInstanceBuffer, 0);
}

HRESULT InstanceRenderer::CreateInstanceBuffer() noexcept
{
	if (m_dataList.empty())
		return E_FAIL;
	D3D11_BUFFER_DESC initDesc = { 0, };
	initDesc.ByteWidth = (UINT)(sizeof(InstanceData) * m_dataList.size());	// ������ ũ�� * ����
	initDesc.Usage = D3D11_USAGE_DYNAMIC;				// ��� ���(CPU->GPU ��������)
	initDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// ���� Ÿ��
	initDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU ���� ����
	initDesc.MiscFlags = 0;
	//initDesc.StructureByteStride = 0;
	//initData.SysMemPitch = 0;
	//initData.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA InitData = { 0, };
	InitData.pSysMem = &m_dataList.at(0);		// GPU�� ���� �� �޸�

	if (m_pInstanceBuffer != nullptr)
		m_pInstanceBuffer->Release();
	//					���� ����(��������,	   �ҽ� ����, ��� ����)
	DxManager::GetDevice()->CreateBuffer(&initDesc, &InitData, &m_pInstanceBuffer);
	m_pBuffers[1] = m_pInstanceBuffer;
	return S_OK;
}

void InstanceRenderer::AddInstance(Instancer* pObject) noexcept
{
	m_objectList.push_front(pObject);
	m_dataList.push_back({});
	CreateInstanceBuffer();
}

void InstanceRenderer::RemoveInstance(Instancer* pObject) noexcept
{
	auto&& iter = find(m_objectList.begin(), m_objectList.end(), pObject);
	if (iter != m_objectList.end())
	{
		m_dataList.pop_back();
		m_objectList.remove(*iter);
		CreateInstanceBuffer();
	}
}

Component* InstanceRenderer::clone() noexcept
{
	//return (Component*)(new InstanceRenderer(*this));
	return (new InstanceRenderer(*this))->cloneAddition();
}

Component* InstanceRenderer::cloneAddition() noexcept
{
	Renderer::cloneAddition();
	m_pInstanceBuffer = nullptr;
	CreateInstanceBuffer();
	return this;
}