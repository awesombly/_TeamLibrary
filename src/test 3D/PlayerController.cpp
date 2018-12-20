#include "PlayerController.h"



bool PlayerController::Init() noexcept
{
	GameObject::Init();
}

bool PlayerController::Frame(const float& spf, const float& accTime)	noexcept
{
	GameObject::Frame(spf, accTime);
}


bool PlayerController::Render(ID3D11DeviceContext* pDContext) noexcept
{
	GameObject::Render(pDContext);
}

bool PlayerController::Release() noexcept
{
	GameObject::Release();
}