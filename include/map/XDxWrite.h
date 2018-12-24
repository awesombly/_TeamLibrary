#pragma once
#include "XStd.h"

class XDxWrite:public XSingleton<XDxWrite>
{
public:
	friend class XSingleton<XDxWrite>;
public:
	//d2d
	ID2D1RenderTarget * m_pRT;   // <<비독립적,리프래쉬시 갱신해야함
								 ////////////디바이스 독립적이다//////////다른거에 영향을 안받음
	ID2D1Factory*			m_pD2DFactory;		//2d 리소스를 생성 및 사용하기 위해 필요한 객체
												//dwrite
	IDWriteFactory*			m_pDWriteFactory;	// 텍스트를 드로잉하기 위해 반드시 필요한 인터페이스
	IDWriteTextFormat*		m_pTextFormat;		// direct에 write할 폰트및 컬러설정, 컬러는 상시 변환 가능
	ID2D1SolidColorBrush*	m_pSolidBrush;		    //독립적인데 m_pRT가 필요하므로 CreateDeviceResourcesdp에서 생성  m_pRT->CreateSolidColorBrush
	float					m_fDpiX;
	float					m_fDpiY;
public:
	bool Release();
	bool Set(IDXGISurface1* pSurface);
	HRESULT CreateDeviceIndependentResources();						//독립적
	HRESULT CreateDeviceResources(IDXGISurface1* pSurface);			//비독립적
private:
	XDxWrite();
public:
	virtual ~XDxWrite();
};

#define I_Write XSingleton<XDxWrite>::Get()

