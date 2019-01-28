#pragma once
#include "stdui.h"
namespace UI
{
	class JTransform
	{
	public:
		JTransform*			m_pCurTransform;
		D3DXMATRIX			m_matScl;
		D3DXMATRIX			m_matRot;
		D3DXMATRIX			m_matTrans;
		D3DXQUATERNION		m_qRot;
		D3DXVECTOR3			m_vLook = { 0, 0, 1 };
		D3DXVECTOR3			m_vUp = { 0, 1, 0 };
		D3DXVECTOR3			m_vRight = { 1, 0, 0 };
		D3DXVECTOR3			m_vScl = { 1, 1, 1 };
		D3DXVECTOR3			m_vRot = { 0, 0, 0 };
		D3DXVECTOR3			m_vPos = { 0, 0, 0 };
	private:
		virtual bool UpdateTransform();
	public:
		virtual void Translate(const float&, const float&, const float&);
		virtual void Rotate(const float&, const float&, const float&);
		virtual void Scale(const float&, const float&, const float&);
		JTransform GetUITransform();
	public:
		JTransform();
		virtual ~JTransform();
	};
}