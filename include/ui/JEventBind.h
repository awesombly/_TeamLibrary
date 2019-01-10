#pragma once
#include "uiheader.h"
namespace UI
{
	static void IntroEvent(JPanel* pRoot)
	{
		JPanel* pMouseParticle = pRoot->find_child(L"mouse_particle");
		if (pMouseParticle == nullptr) return;
		pMouseParticle->PreEvent.first = UI::E_MOUSE_PARTICLE;
		pMouseParticle->PreEvent.second = pMouseParticle;
	}
	static void LobbyEvent(JPanel* pRoot)
	{

	}
	static void InGameEvent(JPanel* pRoot)
	{

	}
}