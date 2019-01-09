#pragma once
#include "MainClass.h"


class IntroScene : public MainClass
{
public:

public:
	void LoadSound()			noexcept;

	virtual bool FirstInit()	noexcept override;
	virtual bool Init()			noexcept override;
	virtual bool Frame()		noexcept override;
	virtual bool Render()		noexcept override;
	virtual bool Release()		noexcept override;

	IntroScene() = default;
	virtual ~IntroScene() = default;
};