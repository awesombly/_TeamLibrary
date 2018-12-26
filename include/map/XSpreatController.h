#pragma once
#include "XMapController.h"

enum SpreatView { SpreatView_Alpha = 0, SpreatView_Texture };
enum AlphaColor { Spreat_Red = 0, Spreat_Green, Spreat_Blue, Spreat_Alpha, Spreat_None };

class XSpreatController : public XMapController, public XSingleton<XSpreatController>
{
private:
	friend class XSingleton<XSpreatController>;
	SpreatView						m_SpreatView = SpreatView_Alpha;
	ComPtr<ID3D11PixelShader>		m_AlphaPS;
	D3D11_TEXTURE2D_DESC			m_TextureDesc;
	ID3D11Texture2D*				m_SpreatTexture;
	ID3D11Texture2D*				m_StagingTexture;
	ID3D11Texture2D*				m_AlphaTexture[4];
	AlphaColor						m_SpreatColor = Spreat_Red;
	ComPtr<ID3D11ShaderResourceView> m_SpreatingTextureSRV;
	map<int, ComPtr<ID3D11ShaderResourceView>> m_RGBA_TextureSRV; // 랜더에서 필요시 XTileRender class로 이전
public:
	void MakeSRV(ID3D11Texture2D* pTexture, ComPtr<ID3D11ShaderResourceView>* pSRV);
	// 알파맵만 볼지 멀티텍스처를 볼지 상태값 세팅
	void SetSpreatViewState(SpreatView SpreatViewState);
	void SetSpreatColor(AlphaColor color) { m_SpreatColor = color; }
	// 알파맵 텍스처를 생성.
	HRESULT CreateSpreatTexture();
	// 생성한 Texture에 직접 접근해서 컬러값을 바꿈.
	void	Spreating(ID3D11DeviceContext* pContext, X_Box CollisionBox, D3DXVECTOR3 vCrash, float fRadius, int iColor);
	// RGBA마다 컬러값에 Texture를 부여, 랜더에서 필요시 XTileRender class로 이전
	HRESULT RGBA_TextureLoad(ID3D11Device* pDevice, const TCHAR* szFile, AlphaColor Color);

	void SetMapTexture();
	ID3D11Texture2D* GetSpreatTex() { return m_SpreatTexture; }
public:
	virtual void Start()	override;
public:
	virtual bool Init()		override;
	virtual bool Frame(const float& spf, const float& accTime)	override;
	virtual bool Render(ID3D11DeviceContext* pContext) override;
	virtual bool Release()	override;
public:
	XSpreatController();
	virtual ~XSpreatController();
};

#define I_SpreatCtrl XSingleton<XSpreatController>::Get()