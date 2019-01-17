#pragma once
#include "MainClass.h"
#include "uiheader.h"


class IntroScene : public MainClass
{
public:
	JEditCtrl*  m_pID = nullptr;
	JEditCtrl*  m_pPW = nullptr;
	///
	XMapImporter m_Importer;		// 세이브 데이터 로더
public:
	void LoadSound()			noexcept;
	void LoadUI()				noexcept override;
	bool FirstInit()			noexcept override;

	virtual bool Init()			noexcept override;
	virtual bool Frame()		noexcept override;
	virtual bool Render()		noexcept override;
	virtual bool Release()		noexcept override;

	IntroScene() = default;
	virtual ~IntroScene() = default;
};