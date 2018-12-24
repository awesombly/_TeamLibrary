#pragma once
#include "stdui.h"
namespace UI
{
	class JSelect : public JSingleton<JSelect>
	{
	private:
		friend class JSingleton<JSelect>;
	public:
		D3DXVECTOR3 m_vOrigin;
		D3DXVECTOR3 m_vDirection;
		D3DXMATRIX	m_matWorld;
		D3DXMATRIX	m_matView;
		D3DXMATRIX	m_matProj;
		D3DXVECTOR3 m_vIntersection;
		D3DXVECTOR3 qvec;
		D3DXVECTOR3 tvec;
		D3DXVECTOR3 pvec;

	public:
		bool Init();
		bool Render(ID3D11DeviceContext* context);
		bool Release();
	public:
		void SetMatrix(D3DXMATRIX* matWorld, D3DXMATRIX* matView, D3DXMATRIX* matProj,
			POINT ptCursor, const float fWidth, const float fHeight);
		void Update(POINT ptCursor, const float fWidth, const float fHeight);
		bool ChkPick(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2);
		bool IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
			D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
			FLOAT* t, FLOAT* u, FLOAT* v);
	public:
		JSelect();
		virtual ~JSelect();
	};
}
#define I_Select JSelect::GetInstance()