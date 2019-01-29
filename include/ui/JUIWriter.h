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
		bool RunBinaryWrite(JPanel* pRoot, wstring SaveFileName);
		//bool BinaryWriteCircuit(JPanel* pChild);
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
		void SlotWrite(JPanel* pChild);
		void InventoryWrite(JPanel* pChild);
	public:
		//void BinaryPanelWrite(JPanel* pChild);
		//void BinaryButtonWrite(JPanel* pChild);
		//void BinaryCheckWrite(JPanel* pChild);
		//void BinaryProgressWrite(JPanel* pChild);
		//void BinarySliderWrite(JPanel* pChild);
		//void BinarySpriteWrite(JPanel* pChild);
		//void BinaryTextWrite(JPanel* pChild);
		//void BinaryParticleWrite(JPanel* pChild);
		//void BinaryListWrite(JPanel* pChild);
		//void BinaryEditWrite(JPanel* pChild);
		//void BinaryImageWrite(JPanel* pChild);
		//void BinarySlotWrite(JPanel* pChild);
		//void BinaryInventoryWrite(JPanel* pChild);
	public:
		JUIWriter() {};
		virtual ~JUIWriter() {};
	};
}