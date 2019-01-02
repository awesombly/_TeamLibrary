#include "header.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include "ISingleton.h"


#pragma once
class WriteManager : public ISingleton<WriteManager>
{
private:
	D3DXVECTOR2				m_DPIScale;
	D3DXVECTOR2				m_DPIVector;

	ID2D1RenderTarget*		m_pRT;					// DxWrite ¿¬µ¿
	ID2D1Factory*			m_pD2dFactory;			
	ID2D1SolidColorBrush*	m_pFontBrush;

	IDWriteFactory*			m_pWriteFactory;
	IDWriteTextLayout*		m_pTextLayout;

	DWRITE_FONT_WEIGHT		m_fontWeight;
	DWRITE_FONT_STYLE		m_fontStyle;

	float					m_fontSize;
	wstring					m_fontFamily;
	wstring					m_fontText;
	//D2D1::ColorF			m_fontColor;
public:
	IDWriteTextFormat*		m_pTextFormat;
private:
	bool CreateIndependentResource()							 noexcept;
	bool CreateDeviceResource(IDXGISurface1* pSurface)			 noexcept;
	bool ReleaseDeviceResource()								 noexcept;
																 
	bool Init(IDXGISurface1* pSurface)							 noexcept;
	bool Begin()												 noexcept;
	bool End()													 noexcept;
	bool Release()												 noexcept;
public:
	void Draw(const D3DXVECTOR4& rect, const wstring_view& text) noexcept;
	void SetText(const D2D1_POINT_2F& layoutSize, const wstring_view& text, const D2D1::ColorF& color, const float& fontSize, const wstring_view& fontFamily) noexcept;
	void SetFontSize(const float& fontSize)						 noexcept;
	void SetFontColor(const D2D1::ColorF& fontColor)			 noexcept;
	void SetFontFamily(const wstring_view& fontFamily)			 noexcept;
	//void SetOriginSetting();
private:
	WriteManager() = default;
	friend class ISingleton<WriteManager>;
	friend class DxManager;
public:
	virtual ~WriteManager() = default;
};

