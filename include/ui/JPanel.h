#pragma once
#include "JTransform.h"
#include "GameObject.h"
#include "JMouse.h"
#include "JPlane.h"
#include "JTextureMgr.h"
namespace UI
{
	class JEvent;
	enum ClickType
	{
		txNORMAL = 0, // Normal
		txHOVER,      // 올린상태
		txPRESS,      // 누른상태
	};
	enum UIType
	{
		PANEL = 777, BUTTON, CHECK, PROGRESS, SLIDER, SPRITE, TEXT, PARTICLE,
	};
	/* JPanel 개념 */
	/* 하나의 이벤트에 실행되는 UI 또는 한번에 보여주는 UI 단위 */
	class JPanel : public JTransform, public GameObject
	{
	public:
		UIType				m_Type;
	public:
		/* ======= Event ==========*/
		JEvent* sdtjl;
		std::pair<void(*)(void*), void*> EventHover;
		std::pair<void(*)(void*), void*> EventPress;
		std::pair<void(*)(void*), void*> EventClick;
	public:
		bool				m_bRender = true; // Render 여부 확인
		JPanel*				m_pParent = nullptr;
		wstring				m_ParentName;
		wstring				m_NodeName;
		list<JPanel*>		m_pChildList;
	public:
		JPlane*				m_pShape;
		JTexture*			m_pTexture;
		vector<int>			m_pIndexList; // TextureMgr 반환 Index 저장
	public:
		D3DXMATRIX			m_matWorld;
		D3DXMATRIX			m_matView;
		D3DXMATRIX			m_matProj;
		RECT				m_rt;
		JMouse				m_ptMouse;
		float				m_fWidth;
		float				m_fHeight;
	public:
		bool SetParent(JPanel* pParent);
		void push_child(JPanel* pChild);
		JPanel* find_child(wstring childname);
		void Update(); // worldmatrix update
		void rtUpdate();
	public:
		bool Init() noexcept override;
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext) noexcept override;
		bool Release() noexcept override;
	public:
		JPanel(wstring NodeName);
		virtual ~JPanel();
	};
#define EVENT(s) (s)
}