//#pragma once
//#include "RSphere.h"
//#include "RDViewCube.h"
//
//
//struct CB_CubeViewMatrix
//{
//	D3DXMATRIX matViewCube[6];
//};
//
//// 동적 환경 매핑 구체 (안돌아감)
//class EnviDySphere : public RSphere
//{
//protected:
//	RDViewCube m_RDViewCube;
//	
//	GameObject* m_pLight;
//
//	ID3D11PixelShader*  m_PS_Fresnel;
//	ID3D11PixelShader*  m_PS_Refraction;
//public:
//	// 큐브맵 행렬
//	static CB_CubeViewMatrix	m_cbCubeMatrix;
//	static ID3D11Buffer*		m_pCubeCBuffer;
//protected:
//	virtual void UpdateConstBuffer(ID3D11DeviceContext* pDContext) noexcept;
//public:
//	bool Init()											noexcept override;
//	bool Frame(const float& spf, const float& accTime)	noexcept override;
//	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
//	bool Release()										noexcept override;
//	virtual Component* clone() noexcept override;
//protected:
//	EnviDySphere() = default;
//public:
//	EnviDySphere(const UINT& slices, const wstring_view& myName, const wstring_view& srcName = L"",
//			   const string_view& vertexShaderName = "VS_Envi", const string_view& pixelShaderName = "PS_Envi") noexcept;
//	virtual ~EnviDySphere() = default;
//};