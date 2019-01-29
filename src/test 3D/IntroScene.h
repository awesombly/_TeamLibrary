#pragma once
#include "MainClass.h"
#include "uiheader.h"


class IntroScene : public MainClass
{
public:
	JEditCtrl*  m_pID			 = nullptr;
	JEditCtrl*  m_pPW			 = nullptr;
	JEditCtrl*  m_pSignUpID		 = nullptr;
	JEditCtrl*  m_pSignUpPW		 = nullptr;
	JEditCtrl*  m_pSignUpPWCheck = nullptr;
	JTextCtrl*  m_pHelpText		 = nullptr;
	///

public:
	void LoadSound()			noexcept;
	void LoadUI()				noexcept override;
	void SetObjects()			noexcept;
	bool FirstInit()			noexcept override;

	virtual bool Init()			noexcept override;
	virtual bool Frame()		noexcept override;
	virtual bool Render()		noexcept override;
	virtual bool Release()		noexcept override;

	IntroScene() = default;
	virtual ~IntroScene() = default;
};