#pragma once
#include "XMapController.h"

enum HeightControl { None = 0, Up, Down, Flat };
class XHeightMapController :public XMapController, public XSingleton<XHeightMapController>
{
private:
	friend class XSingleton<XHeightMapController>;
	// Mode
	BOOL m_bHeightCtrlState = None;
	// 높이값을 조절할 강도
	float m_fStrength = 0.0f;
public:
	void SetHeightCtrlState(HeightControl HeightCtrlState);
	void SetControlValue(float fRadius, float fStrength);
	void HeightUpDown(const float& spf);
	void HeightFlat();
public:
	virtual bool Init() { return true; }
	virtual bool Frame(const float& spf, const float& accTime)	override;
	virtual bool Render(ID3D11DeviceContext* pContext) { return true; }
	virtual bool Release()	override;
public:
	XHeightMapController();
	virtual ~XHeightMapController();
};

#define I_HeightCtrl XSingleton<XHeightMapController>::Get()
