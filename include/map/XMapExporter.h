#pragma once
#include "XMapData.h"
#include "XObjectController.h"

class XMapExporter :public XMapData
{
public:
	void ExportStandard(int iCellCnt, float fDistance);
	bool ExportVertexData(vector<PNCT_Vertex>* pVertexList, vector<DWORD>* pIndexList);
	bool ExportSpreatTexture(ID3D11Texture2D* pSpreatTexture);
	bool ExportAlphaTexture(TCHAR* szAlphaTexture, int iColor);
	bool ExportObject();
	bool Export();
public:
	XMapExporter();
	virtual ~XMapExporter();
};

