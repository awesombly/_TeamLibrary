#pragma once
#include "JToken.h"
#include "uiheader.h"
namespace UI
{
	class JParser : public JToken
	{
	public:
		JPanel* m_pRoot;
	public:
		bool FileLoad(ID3D11Device* pDevice, const TCHAR* szFileName, JPanel& pRoot, wstring plusPath = L"../../data/ui/");
		bool EventLoad(JPanel& pRoot);
		bool EventBind(JPanel& pRoot);
	public:
		void PanelRead(JPanel& pRoot);
		void ButtonRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void CheckRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void ProgressRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void SliderRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void SpriteRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void TextRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void ParticleRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void ListRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void EditRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void ImageRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void SlotRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void InventoryRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
	public:
		JParser() {};
		virtual ~JParser() {};
	};
}