#pragma once
#include "JTransform.h"
#include "GameObject.h"
#include "JMouse.h"
#include "JPlane.h"
#include "JTextureMgr.h"
namespace UI
{
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
	enum EVENT_LIST
	{
		EVENT_NONE = 7274,
		EVENT_SHOW,
		EVENT_NOT_SHOW,
		EVENT_REVERSE_SHOW,
		EVENT_ROTATEZ,
		EVENT_SHOW_SCL,
		EVENT_NOTSHOW_SCL,
	};
	/* JPanel 개념 */
	/* 하나의 이벤트에 실행되는 UI 또는 한번에 보여주는 UI 단위 */
	class JPanel : public JTransform, public GameObject
	{
	public:
		UIType				m_Type;
		int					m_iNumRender; // 렌더순서지정
	public:
		/* ========= Event ========== */
		bool				m_bEvent = true;
		/* 함수포인터 이름 */
		EVENT_LIST m_pfHover = EVENT_NONE;
		EVENT_LIST m_pfPress = EVENT_NONE;
		EVENT_LIST m_pfClick = EVENT_NONE;
		/* 보이드포인터 이름 */
		wstring m_vpHover;	   // Panel NodeName
		wstring m_vpPress;	   // Panel NodeName
		wstring m_vpClick;	   // Panel NodeName
		/* 실행되는 Event */
		std::pair<void(*)(void*), void*> EventHover;
		std::pair<void(*)(void*), void*> EventPress;
		std::pair<void(*)(void*), void*> EventClick; 
		std::pair<void(*)(void*), void*> EventDefault;
	public:
		bool				m_bRender = true; // Render 여부 확인
		JPanel*				m_pParent = nullptr;
		wstring				m_ParentName;
		wstring				m_NodeName;
		std::list<JPanel*>		m_pChildList;
	public:
		JPlane*				m_pShape;
		JTexture*			m_pTexture;
		vector<int>			m_pIndexList; // TextureMgr 반환 Index 저장
	public:
		D3DXMATRIX			m_matWorld;
		D3DXMATRIX			m_matView;
		D3DXMATRIX			m_matProj;
		RECT				m_rt;      // plane rect
		JMouse				m_ptMouse; // 중앙이 0,0인 POINT
		float				m_fWidth;  // 화면 크기 설정 -> matproj
		float				m_fHeight; // 화면 크기 설정 -> matproj
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
}