#pragma once
#include "stdui.h"
namespace UI
{
	class JTexture
	{
	public:
		ID3D11ShaderResourceView* m_pSRV;
		wstring m_szName;
		wstring m_szFullPath;
	public:
		HRESULT SRVCreate(ID3D11Device* device, const TCHAR* szFileName);
		bool Release();
	public:
		JTexture();
		virtual ~JTexture();
	};
}