#include "WriteManager.h"
//#include "DxManager.h"




bool WriteManager::Init(IDXGISurface1* pSurface) noexcept
{
	// Writer 초기화
	m_pFontBrush = nullptr;
	m_pWriteFactory = nullptr;
	m_pTextFormat = nullptr;
	m_pD2dFactory = nullptr;
	m_pTextLayout = nullptr;
	m_pRT = nullptr;
	m_fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	m_fontStyle = DWRITE_FONT_STYLE_NORMAL;
	m_fontSize = 50.0f;

	m_fontFamily = L"Gabriola";

	if (!CreateIndependentResource() ||
		!CreateDeviceResource(pSurface))
	{
		return false;
	}
	//SetText(D2D1::Point2F(500, 500), L"TBasisSample!", D2D1::ColorF(1, 1, 1, 1));
	pSurface->Release();
	return true;
}


bool WriteManager::CreateIndependentResource() noexcept
{
	// Dx2D 팩토리
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2dFactory);
	m_pD2dFactory->GetDesktopDpi(&m_DPIVector.x, &m_DPIVector.y);
	m_DPIScale = { m_DPIVector.x / 96.0f, m_DPIVector.y / 96.0f };

	// 공유 WriteFactory, 텍스트 포맷 생성
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory))) ||
		FAILED(m_pWriteFactory->CreateTextFormat(m_fontFamily.c_str(), nullptr, m_fontWeight, m_fontStyle, DWRITE_FONT_STRETCH_NORMAL, m_fontSize, L"en-us", &m_pTextFormat)))
	{
		return false;
	}

	return true;
}

bool WriteManager::CreateDeviceResource(IDXGISurface1* pSurface) noexcept
{
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, 
										  D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), 
										  m_DPIVector.x, m_DPIVector.y);
	if (FAILED(m_pD2dFactory->CreateDxgiSurfaceRenderTarget(pSurface, (const D2D1_RENDER_TARGET_PROPERTIES *)&props, &m_pRT)))
		return false;
	if (m_pRT == nullptr)
	{
		ErrorMessage(""s + __FUNCTION__ + " -> RenderTarget nullptr!");
	}
	else
	{
		m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pFontBrush);
	}
	return true;
}

void WriteManager::Draw(const D3DXVECTOR4& rect, const wstring_view& text) noexcept
{
	D2D1_RECT_F layoutRect = D2D1::RectF(
		rect.x / m_DPIScale.x,
		rect.y / m_DPIScale.y,
		rect.z / m_DPIScale.x,
		rect.w / m_DPIScale.y);
	m_pRT->DrawTextW(text.data(), (UINT)text.size(), m_pTextFormat, layoutRect, m_pFontBrush);
}

void WriteManager::SetText(const D2D1_POINT_2F& layoutSize, const wstring_view& text, const D2D1::ColorF& color, const float& fontSize, const wstring_view& fontFamily) noexcept
{
	int textLength = (int)text.size();
	if (m_pTextLayout != nullptr)
		m_pTextLayout->Release();
	//m_fontColor = color;
	m_fontSize = fontSize;
	m_fontFamily = fontFamily;
	if (FAILED(m_pWriteFactory->CreateTextLayout(text.data(), textLength, m_pTextFormat, layoutSize.x, layoutSize.y, &m_pTextLayout)))
	{
		ErrorMessage(""s + __FUNCTION__ + " -> TextLayout Error!");
	}
	IDWriteTypography* pTypography;
	DWRITE_TEXT_RANGE textRange = { 0, (UINT32)textLength };
	
	if (FAILED(m_pWriteFactory->CreateTypography(&pTypography))					||
		FAILED(pTypography->AddFontFeature({ DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7, 1 }))	||
		FAILED(m_pTextLayout->SetTypography(pTypography, textRange))			||
		FAILED(m_pTextLayout->SetUnderline(false, textRange))					||
		FAILED(m_pTextLayout->SetFontFamilyName(fontFamily.data(), textRange))	||
		FAILED(m_pTextLayout->SetFontSize(fontSize, textRange)))
	{
		ErrorMessage(""s + __FUNCTION__ + " -> LayoutSetting Error!");
	}
	m_pFontBrush->SetColor(color);
	
	pTypography->Release();
}

void WriteManager::SetFontSize(const float& fontSize)  noexcept
{
	//m_pTextLayout->SetFontSize(fontSzie, { 0, 100 });
	
	if (m_pTextFormat != nullptr)
	{
		m_pTextFormat->Release();
		m_fontSize = fontSize;
		m_pWriteFactory->CreateTextFormat(m_fontFamily.c_str(), nullptr, m_fontWeight, m_fontStyle, DWRITE_FONT_STRETCH_NORMAL, m_fontSize, L"en-us", &m_pTextFormat);
	}
}

void WriteManager::SetFontColor(const D2D1::ColorF& fontColor) noexcept
{
	m_pFontBrush->SetColor(fontColor);
}

void WriteManager::SetFontFamily(const wstring_view& fontFamily) noexcept
{
	//m_pTextLayout->SetFontFamilyName(fontFamily.data(), { 0, 100 });

	if (m_pTextFormat != nullptr)
	{
		m_pTextFormat->Release();
		m_fontFamily = fontFamily;
		m_pWriteFactory->CreateTextFormat(m_fontFamily.c_str(), nullptr, m_fontWeight, m_fontStyle, DWRITE_FONT_STRETCH_NORMAL, m_fontSize, L"en-us", &m_pTextFormat);
	}
}

//void WriteManager::SetOriginSetting()
//{
//	SetFontSize(m_fontSize);
//	SetFontFamily(m_fontFamily);
//	//m_pTextLayout->SetFontSize(m_fontSize, { 0, 0 });
//	//m_pFontBrush->SetColor(m_fontColor);
//	//m_pTextLayout->SetFontFamilyName(m_fontFamily.data(), { 0, 100 });
//}


bool WriteManager::Begin() noexcept
{
	//if (m_pRT == nullptr) return false;
	m_pRT->BeginDraw();
	//m_pRT->SetTransform(D2D1::IdentityMatrix());
	return true;
}

bool WriteManager::End() noexcept
{
	//if (m_pRT == nullptr) return false;
	m_pRT->EndDraw();
	return true;
}

bool WriteManager::ReleaseDeviceResource() noexcept
{
	if(m_pRT != nullptr)
		m_pRT->Release();
	if (m_pFontBrush != nullptr)
		m_pFontBrush->Release();
	m_pRT = nullptr;
	m_pFontBrush = nullptr;
	return true;
}

bool WriteManager::Release() noexcept
{
	m_pD2dFactory->Release();
	m_pWriteFactory->Release();
	m_pTextFormat->Release();
	m_pTextLayout->Release();
	return true;
}