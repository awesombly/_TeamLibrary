#pragma once
#include "uiheader.h"
namespace UI
{
	class JUIWriter
	{
	public:
		FILE* m_pStream;
		wstring m_strFileName; // 출력 파일명
	public:
		bool RunWrite(JPanel* pRoot, wstring SaveFileName);
		bool WriteCircuit(JPanel* pChild);
	public:
		void PanelWrite(JPanel* pChild);
		void ButtonWrite(JPanel* pChild);
		void CheckWrite(JPanel* pChild);
		void ProgressWrite(JPanel* pChild);
		void SliderWrite(JPanel* pChild);
		void SpriteWrite(JPanel* pChild);
		void TextWrite(JPanel* pChild);
		void ParticleWrite(JPanel* pChild);
		void ListWrite(JPanel* pChild);
		void EditWrite(JPanel* pChild);
		void ImageWrite(JPanel* pChild);
	public:
		JUIWriter() {};
		virtual ~JUIWriter() {};
	};
}