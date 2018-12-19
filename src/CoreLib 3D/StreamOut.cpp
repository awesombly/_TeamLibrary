#include "StreamOut.h"
#include "DxManager.h"


void StreamOut::BeginStream(ID3D11DeviceContext* pDContext) noexcept
{
	static UINT StrideOffset[16] = { 0, };
	static ID3D11Buffer* pBuffers[16] = { nullptr, };
	pDContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);

	ID3D11Buffer* pVB[1] = { nullptr };
	// StreamOut스테이지로 출력
	pDContext->SOSetTargets(1, pVB, &m_Offset);

	pVB[0] = m_pStreamTo;
	pDContext->SOSetTargets(1, pVB, &m_Offset);
}

void StreamOut::EndStream(ID3D11DeviceContext* pDContext)	 noexcept
{
	ID3D11Buffer* pVB[1] = { nullptr };
	pDContext->SOSetTargets(1, pVB, &m_Offset);

	std::swap(m_pStreamTo, m_pDrawFrom);
}

void StreamOut::CreateStreamBuffer(const DWORD& size, const DWORD& count) noexcept
{
	D3D11_BUFFER_DESC initDesc = { 0, };
	initDesc.ByteWidth = size * count;
	initDesc.Usage = D3D11_USAGE_DEFAULT;
	initDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	//initDesc.CPUAccessFlags = 0;
	//initDesc.MiscFlags = 0;

	DxManager::GetDevice()->CreateBuffer(&initDesc, nullptr, &m_pDrawFrom);
	DxManager::GetDevice()->CreateBuffer(&initDesc, nullptr, &m_pStreamTo);
}