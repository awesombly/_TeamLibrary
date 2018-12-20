#pragma once
#include "GameObject.h"



class PlayerController : public GameObject
{
public:

public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
public:
	PlayerController() = default;
	virtual ~PlayerController() = default;
};