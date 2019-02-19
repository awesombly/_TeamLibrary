#pragma once
#include "XQuadTreeIndex.h"


// XSpreatController에서 Texture를 생성하고
// XMultiTextureRender에서는 4장의 Alpha Blending된 텍스처를 혼합해서 Render
class XTileRender:public XQuadTreeIndex
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render(ID3D11DeviceContext* pContext) override;
	virtual bool Release() override;
public:
	XTileRender();
	virtual ~XTileRender();
};

