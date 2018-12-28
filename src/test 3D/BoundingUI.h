#pragma once
#include "PlaneUI.h"


class BoundingUI : public PlaneUI
{
public:
	//D3DXVECTOR3 m_TargetPosition;
	D3DXVECTOR3 m_prevPosition;
	D3DXVECTOR2 m_direction;

	float m_moveSpeed = 2.0f;
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	//virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;
	virtual Component* clone()									noexcept override;
protected:
	//BoundingUI() = default;
public:
	using PlaneUI::PlaneUI;
	//BoundingUI(const wstring_view& myName, const wstring_view& srcName = L"",
			//const string_view& vertexShaderName = "VS_PNCT", const string_view& pixelShaderName = "PS_PCT") noexcept;
	virtual ~BoundingUI() = default;
};