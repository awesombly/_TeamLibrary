#pragma once
#include "JToken.h"
#include "JPanel.h"
namespace UI
{
	class JParser : public JToken
	{
	public:
		bool FileLoad(ID3D11Device* pDevice, const TCHAR* szFileName, JPanel& pRoot, wstring plusPath = L"../../data/ui/");
	public:
		void PanelRead(JPanel& pRoot);
		void ButtonRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void CheckRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void ProgressRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void SliderRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void SpriteRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void TextRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
		void ParticleRead(ID3D11Device* pDevice, JPanel& pRoot, wstring plusPath);
	public:
		JParser() {};
		virtual ~JParser() {};
	};
}