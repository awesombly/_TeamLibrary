#pragma once
#include "JTransform.h"
#include "GameObject.h"
#include "JMouse.h"
#include "JPlane.h"
#include "JTextureMgr.h"
#include "Input.h"
//
// Thread ���� �� ��
//
namespace UI
{
	enum ClickType
	{
		txNORMAL = 0, // Normal
		txHOVER,      // �ø�����
		txPRESS,      // ��������
	};
	enum VHType
	{
		Vertical = 1446, // ����, ����
		Horizontal,      // ����, ����
		NONE,
	};
	enum UIType
	{
		PANEL = 777, BUTTON, CHECK, PROGRESS, SLIDER, SPRITE, TEXT, PARTICLE, LIST, LISTBOX, EDIT, IMAGE, SLOT, INVENTORY, INVENTORYSLOT,
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
	/* JPanel ���� */
	/* �ϳ��� �̺�Ʈ�� ����Ǵ� UI �Ǵ� �ѹ��� �����ִ� UI ���� */
	class JPanel : public JTransform, public GameObject
	{
	public:
		UIType				m_Type;
		int					m_iNumRender; // ������������
	public:
		/* ========= Event ========== */
		bool				m_bEvent = true;
		/* �Լ������� �̸� */
		EVENT_LIST m_pfHover = EVENT_NONE;
		EVENT_LIST m_pfPress = EVENT_NONE;
		EVENT_LIST m_pfClick = EVENT_NONE;
		/* ���̵������� �̸� */
		wstring m_vpHover;	   // Panel NodeName
		wstring m_vpPress;	   // Panel NodeName
		wstring m_vpClick;	   // Panel NodeName
		/* ����Ǵ� Event */
		std::pair<void(*)(void*), void*> EventHover;
		std::pair<void(*)(void*), void*> EventPress;
		std::pair<void(*)(void*), void*> EventClick; 
		std::pair<void(*)(void*), void*> PreEvent;
		std::pair<void(*)(void*), void*> PostEvent;
		float m_fEventTime = 1.0f;
	public:
		int m_pKeyHold = VK_F24;
		int m_pKeyDown = VK_F24;
	public:
		bool				m_bRender = true; // Render ���� Ȯ��
		JPanel*				m_pParent = nullptr;
		JPanel*				m_pRoot = nullptr;
		wstring				m_ParentName;
		wstring				m_NodeName;
		std::list<JPanel*>	m_pChildList;
		bool				m_bEffect = false;
	public:
		JPlane*				m_pShape;
		JTexture*			m_pTexture;
		vector<int>			m_pIndexList; // TextureMgr ��ȯ Index ����
	public:
		D3DXMATRIX			m_matWorld;
		D3DXMATRIX			m_matView;
		D3DXMATRIX			m_matProj;
		RECT				m_rt;      // plane rect
		JMouse				m_ptMouse; // �߾��� 0,0�� POINT
		float				m_fWidth;  // ȭ�� ũ�� ���� -> matproj
		float				m_fHeight; // ȭ�� ũ�� ���� -> matproj
		float				m_fUITimer;
	public:
		bool SetParent(JPanel* pParent);
		void push_child(JPanel* pChild);
		JPanel* find_child(wstring childname);
		JPanel*	find_root();
		void Update(); // worldmatrix update
		void rtUpdate();
		void* uiclone();
	public:
		void SetEventTime(const float fTime);
		float GetEventTime();
		void EffectPlay();
		bool Init() noexcept override;
		bool PreFrame(const float& spf, const float& accTime);
		bool PostFrame(const float& spf, const float& accTime);
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext) noexcept override;
		bool Release() noexcept override;
	public:
		JPanel(wstring NodeName);
		virtual ~JPanel();
	};
}
//#include <thread>
//static void TestThread(int iTest, float fTest)
//{
//
//}
//class SampleThread
//{
//public:
//	void TestThread(int iTest, float fTest);
//};
//static void TestThreadStart()
//{
//	SampleThread* test = new SampleThread();
//	//1)
//	std::thread ThreadName(&TestThread, 1, 1.0f);
//	if (ThreadName.joinable()) // join�����ѻ�������
//		ThreadName.join();     // ��ϰɸ� thread����������
//	ThreadName.detach();	   // �������� , ThreadName�� ���������� / �����Լ��϶� detach���ϰ� ��ϳ����� ����
//							   // join���ҰŸ� detach�����ݽô�.
//	//2)
//	std::thread ThreadName2(&SampleThread::TestThread, &test, 1, 1.0f);
//
//}	