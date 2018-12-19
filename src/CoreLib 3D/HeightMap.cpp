#include "HeightMap.h"
#include "ObjectManager.h"
#include <wrl.h>						// ComPtr


HeightMap::HeightMap(const wstring_view& myName, const EComponent& eComType, const wstring_view& srcName,
					 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	SetInfo(myName, eComType, srcName, vertexShaderName, pixelShaderName);
	//Create();
	Init();
}

bool HeightMap::Init() noexcept
{
	Renderer::Init();
	m_eRenderType = ERenderType::HeightMap;
	return true;
}

bool HeightMap::Frame(const float& spf, const float& accTime)	noexcept
{
	//if (!m_isEnable) return false;
	//Renderer::Frame(spf, accTime);
	return true;
	spf, accTime;
}

bool HeightMap::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!m_isEnable) return false;
	Renderer::Render(pDContext);
	return true;
}

bool HeightMap::Release()	noexcept
{
	Renderer::Release();
	return true;
}

float HeightMap::GetMapHeight(const D3DXVECTOR3& position, Renderer* pPlane)
{
	// �� �߾��� �������� �ش� ��ǥ�� ���� ���� ������ǥ�� ã��
	float cellX = (m_mapHalfWidth  + position.x) / m_cellSize;
	float cellZ = (m_mapHalfHeight - position.z) / m_cellSize;
	cellX = std::clamp(cellX, 0.0f, (float)(m_vertexCols - 2));
	cellZ = std::clamp(cellZ, 0.0f, (float)(m_vertexRows - 2));
	// ã�� ���� ���� �ε���

	int cellIndexX = std::clamp((int)cellX, 0, (int)m_vertexCols - 2);
	int cellIndexZ = std::clamp((int)cellZ, 0, (int)m_vertexRows - 2);

	// ã�� ���� �� ���� ����
	float vertexH0 = (float)m_vertexHeight[(UINT)( cellIndexZ	   * m_vertexCols + cellIndexX)];
	float vertexH1 = (float)m_vertexHeight[(UINT)( cellIndexZ	   * m_vertexCols + cellIndexX + 1)];
	float vertexH2 = (float)m_vertexHeight[(UINT)((cellIndexZ + 1) * m_vertexCols + cellIndexX)];
	float vertexH3 = (float)m_vertexHeight[(UINT)((cellIndexZ + 1) * m_vertexCols + cellIndexX + 1)];

	// �ٴ� ��ġ ����
	if (pPlane != nullptr)
	{
		pPlane->m_pParent->Transformation(*m_pParent);
		pPlane->m_vertexList[0].pos = m_vertexList[(UINT)(cellIndexZ       * m_vertexCols + cellIndexX)].pos;
		pPlane->m_vertexList[0].nor = m_vertexList[(UINT)(cellIndexZ       * m_vertexCols + cellIndexX)].nor;
		pPlane->m_vertexList[1].pos = m_vertexList[(UINT)(cellIndexZ       * m_vertexCols + cellIndexX + 1)].pos;
		pPlane->m_vertexList[1].nor = m_vertexList[(UINT)(cellIndexZ       * m_vertexCols + cellIndexX + 1)].nor;
		pPlane->m_vertexList[2].pos = m_vertexList[(UINT)((cellIndexZ + 1) * m_vertexCols + cellIndexX)].pos;
		pPlane->m_vertexList[2].nor = m_vertexList[(UINT)((cellIndexZ + 1) * m_vertexCols + cellIndexX)].nor;
		pPlane->m_vertexList[3].pos = m_vertexList[(UINT)((cellIndexZ + 1) * m_vertexCols + cellIndexX + 1)].pos;
		pPlane->m_vertexList[3].nor = m_vertexList[(UINT)((cellIndexZ + 1) * m_vertexCols + cellIndexX + 1)].nor;
	}

	// �� ���� ��ǥ
	float u = cellX - cellIndexX;
	float v = cellZ - cellIndexZ;
	// 0�� ���� ���� 1���� 2���� ���� ����
	float returnHeight = vertexH0;
	float disX = vertexH1 - vertexH0;
	float disY = vertexH2 - vertexH0;
	// ������ �������� �Ÿ��� ���� ����
	if (u + v > 1.0f)
	{
		// �밢�� ���� ��� 3�� ���� ���� ������ ����
		disX = vertexH2 - vertexH3;
		disY = vertexH1 - vertexH3;
		returnHeight = vertexH3
			+ (disX * (1.0f - u))
			+ (disY * (1.0f - v));
	}
	else
	{
		returnHeight += (disX * u) + (disY * v);
	}

	return returnHeight * m_pParent->GetScale().y + m_pParent->GetPosition().y;
}

void HeightMap::InitVertexNormalVector() noexcept
{
	GenFaceNormal();				// 1. ��� ���̽��� ���� �븻 ����
	GenNormalLookupTable();			// 2. �� ������ �����ϴ� ��� ���̽� ����Ʈ ����
	GenVertexNormalFastLookup();	// 3. ���� ���
}

void HeightMap::GenFaceNormal() noexcept
{
	m_faceNormal.resize(m_faceCount, Vector3::Zero);
	// �� �鿡 ���� �븻 ���� ���? ���� �ε����� ���ϴµ�
	int faceIndex = 0;
	for (auto& face : m_faceNormal)
	{
		WORD index0 = m_indexList[faceIndex * 3];
		WORD index1 = m_indexList[faceIndex * 3 + 1];
		WORD index2 = m_indexList[faceIndex * 3 + 2];

		D3DXVECTOR3 vNormal;
		D3DXVECTOR3 v0 = m_vertexList[index1].pos - m_vertexList[index0].pos;
		D3DXVECTOR3 v1 = m_vertexList[index2].pos - m_vertexList[index0].pos;
		
		D3DXVec3Cross(&vNormal, &v0, &v1);
		D3DXVec3Normalize(&vNormal, &vNormal);
		face = vNormal;

		//face = { (float)index0, (float)index1, (float)index2 };

		faceIndex++;
	}
}

void HeightMap::GenNormalLookupTable() noexcept
{
	// �� ������ ���� ���̽� ����Ʈ ����
	m_NormalLookupTable.clear(); 
	m_NormalLookupTable.resize(m_vertexCount);
	for (UINT face = 0; face < m_faceCount; face++)
	{
		for (UINT vertex = 0; vertex < 3; vertex++)
		{
			DWORD index = m_indexList[face * 3 + vertex];
			m_NormalLookupTable[index].push_back(face);
		}
	}
}

void HeightMap::GenVertexNormalFastLookup() noexcept
{
	// �� ������ �����ϴ� ���̽����� �븻���� ���ϰ� ����ȭ���� ������ �븻���� ����
	for (UINT vertex = 0; vertex < m_vertexCount; vertex++)
	{
		D3DXVECTOR3 avgNormal = { 0, 0, 0 };
		auto faceCount = m_NormalLookupTable[vertex].size();
		for (auto face = 0; face < faceCount; face++)
		{
			// �����ϴ� ���̽� �ε����� �޾� �븻�� ����
			DWORD faceIndex = m_NormalLookupTable[vertex][face];
			avgNormal += m_faceNormal[faceIndex];
		}
		// ���� �븻 ���͸� ����ȭ
		D3DXVec3Normalize(&m_vertexList[vertex].nor, &avgNormal);
	}
}

D3DXVECTOR3 HeightMap::GetNormalVector(const UINT& vectorIndex) noexcept
{
	return D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vectorIndex;
}

HRESULT HeightMap::Create(const WORD& cols, const WORD& rows, const float& cellSize, const float& offsetX, const float& offsetY) noexcept
{
	Init();
	CreateConstBuffer(&(*m_ppCamera)->m_cbVS, sizeof(CB_VSMatrix), &m_pMatrixCBuffer);
	// ¦�� �����̸� �ϳ� ����
	m_vertexCols = cols;
	m_vertexRows = rows;
	if (m_vertexCols % 2 == 0)
		--m_vertexCols;
	if (m_vertexRows % 2 == 0)
		--m_vertexRows;
	
	// col * row�� ������ ����
	//m_pParent->SetScaleY(heightRate);
	m_vertexCount = m_vertexCols * m_vertexRows;
	m_vertexList.resize(m_vertexCount, Vertex_PNCT());
	m_cellSize = cellSize;
	m_faceCount = (m_vertexCols - 1) * (m_vertexRows - 1) * 2;
	m_mapHalfWidth =  (m_vertexCols - 1) * m_cellSize * 0.5f;
	m_mapHalfHeight = (m_vertexRows - 1) * m_cellSize * 0.5f;

	float texOffsetX = offsetX / (m_vertexCols - 1);
	float texOffsetZ = offsetY / (m_vertexRows - 1);

	int rowStart = m_vertexRows % 2 == 0 ? -((int)m_vertexRows / 2) + 1 : -((int)m_vertexRows / 2);
	int rowEnd   = m_vertexRows / 2;
	int colStart = m_vertexCols % 2 == 0 ? -((int)m_vertexCols / 2) + 1 : -((int)m_vertexCols / 2);
	int colEnd   = m_vertexCols / 2;
	int index = 0;
	float heightValue = 0.0f;
	// ���� �Է�
	for (int row = rowStart; row <= rowEnd; ++row)
	{
		for (int col = colStart; col <= colEnd; ++col)
		{
			if (m_vertexHeight.empty())
				heightValue = 0.0f;
			else
			{
					
			}
			heightValue = (float)m_vertexHeight[index];
			m_vertexList[index++] = { { col * m_cellSize, heightValue, -row * m_cellSize },
									  { GetNormalVector(index) },
									  { Vector4::One },
									  { index % m_vertexCols * texOffsetX, index / m_vertexCols * texOffsetZ } };
		}
	}

	// ���۵� ����
	if (FAILED(CreateVertexBuffer() ) ||
		FAILED(CreateIndexBuffer((WORD)m_vertexCols, (WORD)m_vertexRows, m_faceCount * 3) ))
		return E_FAIL;
	
	// ���� �븻 ���� ����
	InitVertexNormalVector();
	
	//// ����-���� �÷� ����
	//for (auto& iter : m_vertexList)
	//{
	//	// ���� �븻�� ������ ������ �÷����� ����
	//	float lightDot = max(0.0f, D3DXVec3Dot(&iter.nor, &m_light));
	//	iter.col = Vector4::Zero;// { lightDot, lightDot, lightDot, 1.0f };
	//}

	return S_OK;
}

void HeightMap::SetHeightMap(ID3D11DeviceContext* pContext, const wstring_view& fileName, const wstring_view& directory) noexcept
{
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(loadInfo));
	loadInfo.Usage = D3D11_USAGE_STAGING;
	loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;

	D3DX11_IMAGE_INFO imageInfo = { 0, };
	loadInfo.pSrcInfo = &imageInfo;

	// ���� �� ���� �ҷ�����
	wstring loadUrl = L""s + directory.data() + fileName.data();
	Microsoft::WRL::ComPtr<ID3D11Resource> pHeightTexture = nullptr;
	if (FAILED(D3DX11CreateTextureFromFile(DxManager::GetDevice(), loadUrl.c_str(), &loadInfo, nullptr, &pHeightTexture, nullptr)))
	{
		ErrorMessage(L""s + __FUNCTIONW__ + L" -> File Not Found! : " + loadUrl.c_str());
		return;
	}

	// ���̸� �ؽ��Ŀ� ���� �������̽� ������
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex2D = nullptr;
	pHeightTexture->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTex2D);
	// desc ��ȯ
	D3D11_TEXTURE2D_DESC texDesc = { 0, };
	pTex2D->GetDesc(&texDesc);
	
	// ���̸� �ؽ����� ����*���� �ȼ���ŭ ���� ����Ʈ�� ����, �ȼ����� �� ������ ������
	m_vertexCols = texDesc.Width;
	m_vertexRows = texDesc.Height;
	m_vertexHeight.resize(m_vertexCols * m_vertexRows, 0);
	
	D3D11_MAPPED_SUBRESOURCE mapped = { 0, };
	// �ؽ��Ŀ� ���� ������ ����(����) -> �ȼ��� �̾Ƽ� ���̷� ����
	if (SUCCEEDED(pContext->Map(pTex2D.Get(), D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &mapped) ))
	{
		UCHAR* pTexels = (UCHAR*)mapped.pData;
		UINT index = 0;
		for (UINT row = 0; row < m_vertexRows; ++row)
		{
			UINT rowStart = row * mapped.RowPitch;	// �� * ����ġ?
			for (UINT col = 0; col < m_vertexRows; ++col)
			{
				UINT colStart = col * 4;	// �� * 4����Ʈ
				UINT red = pTexels[rowStart + colStart];
				m_vertexHeight[index++] = red;
			}
		}
		pContext->Unmap(pTex2D.Get(), D3D11CalcSubresource(0, 0, 1));
	}
}

void HeightMap::CreateHeightMap(ID3D11DeviceContext* pContext, const wstring_view& fileName, const float& cellSize, const float& texOffsetX, const float& texOffsetY, const wstring_view& directory) noexcept
{
	SetHeightMap(pContext, fileName, directory);
	Create((WORD)m_vertexCols, (WORD)m_vertexRows, cellSize, texOffsetX, texOffsetY);
	pContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
	pContext->UpdateSubresource(m_pIndexBuffer, 0, nullptr, &m_indexList.at(0), 0, 0);
}


HRESULT HeightMap::CreateIndexBuffer(const WORD& cols, const WORD& rows, const UINT& indexCount) noexcept
{
	// �ε��� ����Ʈ ����
	m_indexCount = indexCount;
	m_indexList.resize(m_indexCount, 0);

	int index = 0;
	for (WORD row = 0; row < rows - 1; row++)
	{
		for (WORD col = 0; col < cols - 1; col++)
		{
			m_indexList[index++] = (row		 * cols) + col;
			m_indexList[index++] = (row		 * cols) + col + 1;
			m_indexList[index++] = (row + 1) * cols  + col;

			m_indexList[index++] = (row + 1) * cols  + col;
			m_indexList[index++] = (row		 * cols) + col + 1;
			m_indexList[index++] = (row + 1) * cols  + col + 1;
		}
	}

	D3D11_BUFFER_DESC initDesc{ 0, };
	initDesc.ByteWidth = m_indexCount * sizeof(WORD);			// ���� ������
	initDesc.Usage = D3D11_USAGE_DEFAULT;
	initDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;			// ���� Ÿ��
	initDesc.CPUAccessFlags = 0;
	initDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{ 0, };
	initData.pSysMem = &m_indexList.at(0);
	if (m_pIndexBuffer != nullptr)
	{
		m_pIndexBuffer->Release();
	}
	return DxManager::GetDevice()->CreateBuffer(&initDesc, &initData, &m_pIndexBuffer);
}

Component* HeightMap::clone() noexcept
{
	//return (Component*)(new HeightMap(*this));
	return (new HeightMap(*this))->cloneAddition();
}

Component* HeightMap::cloneAddition() noexcept
{
	Renderer::cloneAddition();
	m_pIndexBuffer = nullptr;
	CreateIndexBuffer((WORD)m_vertexCols, (WORD)m_vertexRows, m_faceCount * 3);
	return this;
}