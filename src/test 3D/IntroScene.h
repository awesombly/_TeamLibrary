#pragma once
#include "MainClass.h"

class IntroScene : public MainClass
{
public:
	GameObject* m_pTakeObject = nullptr;
	GameObject* m_pTakeObject2 = nullptr;
	GameObject* m_pTakeObject3 = nullptr;
	GameObject* m_pTakeObject4 = nullptr;

	void LoadSound() noexcept;
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;

	IntroScene() = default;
	virtual ~IntroScene() = default;
};