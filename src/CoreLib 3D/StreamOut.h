#pragma once
#include "header.h"

// À×¿©
class StreamOut
{
public:
	ID3D11Buffer* m_pDrawFrom;
	ID3D11Buffer* m_pStreamTo;

	UINT m_Offset = 0;
public:
	void CreateStreamBuffer(const DWORD& size, const DWORD& count) noexcept;
	void BeginStream(ID3D11DeviceContext* pDContext) noexcept;
	void EndStream(ID3D11DeviceContext* pDContext)	 noexcept;

	StreamOut() = default;
	virtual ~StreamOut() = default;
};

