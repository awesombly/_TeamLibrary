#pragma once
#include "JPanel.h"
namespace UI
{
	/* Scene 개념 하나의 Scene에서 보여주는 Panel 저장 */
	class JLevel : public JSingleton<JLevel>
	{
	private:
		friend class JSingleton<JLevel>;
	public:
		vector<JPanel*> m_pChildList;
		wstring			m_NodeName;
	public:
		virtual JPanel& find_child(wstring findname);
		virtual void push_child(JPanel& child);
		virtual bool Init();
		virtual bool Frame();
		virtual bool Render(ID3D11DeviceContext* pContext);
		virtual bool Release();
	public:
		JLevel(wstring NodeName);
		JLevel() {}
		virtual ~JLevel();
	};
}
#define I_Level JLevel::GetInstance()