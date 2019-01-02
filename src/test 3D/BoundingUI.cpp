//#include "BoundingUI.h"
//#include "Window.h"
//
//
//bool BoundingUI::Init()	noexcept
//{
//	m_direction = Normalize({ RandomNormal(), RandomNormal(), 0.0f }) * m_moveSpeed;
//	return true;
//}
//
//bool BoundingUI::Frame(const float& spf, const float& accTime)	noexcept
//{
//	if (!m_isEnable) return false;
//	PlaneUI::Frame(spf, accTime);
//	m_prevPosition = m_pParent->GetPosition();
//
//	//static D3DXVECTOR4 newClient = { (float)(Window::getClientRect().left),
//	//								 (float)(Window::getClientRect().top),
//	//								 (float)(Window::getClientRect().right  - Window::getClientRect().left),
//	//								 (float)(Window::getClientRect().bottom - Window::getClientRect().top) };
//	//static D3DXVECTOR4 newClient = { -5.0f, -5.0f, 5.0f, 5.0f };
//	// Ãæµ¹
//	D3DXVECTOR2 thisMin = { m_pParent->GetPosition().x - 0.5f, m_pParent->GetPosition().y - 0.5f };
//	D3DXVECTOR2 thisMax = { m_pParent->GetPosition().x + 0.5f, m_pParent->GetPosition().y + 0.5f };
//	D3DXVECTOR2 otherMin = { -5.0f, -5.0f };
//	D3DXVECTOR2 otherMax = { 5.0f, 5.0f };
//	if (std::max<float>(thisMin.x, otherMin.x) > std::min<float>(thisMax.x, otherMax.x) ||
//		std::max<float>(thisMin.y, otherMin.y) > std::min<float>(thisMax.y, otherMax.y))
//	{
//		if (m_prevPosition.y + 0.5f <= otherMax.y ||
//			m_prevPosition.y - 0.5f >= otherMin.y)
//		{
//			///m_curAngle = 360.0f - m_curAngle;
//			m_direction.y *= -1.0f;
//			m_pParent->Translate(m_direction * spf);
//		}
//		else if (m_prevPosition.x + 0.5f <= otherMin.x ||
//				 m_prevPosition.x - 0.5f >= otherMax.x)
//		{
//			///m_curAngle = 180.0f - m_curAngle;
//			m_direction.x *= -1.0f;
//			m_pParent->Translate(m_direction * spf);
//		}
//		///m_curAngle = (float)((int)m_curAngle % 360);
//		//setBounce(m_curAngle, m_damage);
//	}
//
//	m_pParent->Translate(m_direction * spf);
//	return true;
//}
//
//
//
//Component* BoundingUI::clone() noexcept
//{
//	return (new BoundingUI(*this))->cloneAddition();
//}