#include "GameRule.h"


bool GameRule::Init() noexcept
{
	m_bSeek = false;
	m_fGameTime = 480.0f;
	m_fHideTime = 30.0f;
	return true;
}



bool GameRule::Frame()	noexcept
{

	return true;
}
bool GameRule::Render()	noexcept
{

	return true;
}

bool GameRule::Release()	noexcept
{


	return true;
}




GameRule::GameRule()
{

}


GameRule::~GameRule()
{
}
