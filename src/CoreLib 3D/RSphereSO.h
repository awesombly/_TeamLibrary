#pragma once
#include "Renderer.h"
#include "StreamOut.h"

// Stream Out으로 만든 구체 클래스(미완성)
class RSphereSO : public Renderer
{
private:
	StreamOut m_RenderSO;
	int		  m_numStreamOut = 2;

	float m_radius;
	UINT  m_slices;
protected:
	ID3D11GeometryShader* m_pGShader = nullptr;
	ID3D11GeometryShader* m_pGShaderSO = nullptr;
protected:
	void SetSO(ID3D11DeviceContext* pDContext) noexcept;
public:
	virtual HRESULT Create() noexcept override;

	virtual bool PrevRender(ID3D11DeviceContext* pDContext)		noexcept override;
	virtual bool PostRender(ID3D11DeviceContext* pDContext)		noexcept override;
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone() noexcept override;
protected:
	RSphereSO() = default;
public:
	RSphereSO(const float& radius, const UINT& slices, const wstring_view& myName, const wstring_view& srcName = L"",
			const string_view& vertexShaderName = "VS_SO", const string_view& pixelShaderName = "PS_Final") noexcept;
	virtual ~RSphereSO() = default;
};