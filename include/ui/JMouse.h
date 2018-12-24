#pragma once
#include "stdui.h"
namespace UI
{
	class JMouse
	{
	public:
		POINT m_ptMouse;
	public:
		void Update(float fWidth, float fHeight);
		POINT& Getpt();
	public:
		JMouse() {};
		virtual ~JMouse() {};
	};
}