#pragma once
#include "XShape.h"

class XMapData
{
protected:
	FILE*				m_fp;
	int					m_iCellConut;
	float				m_fDistance;
	vector<PNCT_Vertex> m_VertexList;
	vector<DWORD>		m_IndexList;
	TString				m_szSpreatTexture = {_T("none"),};
	map<int, TString>	m_szAlphaTexture;
public:
	bool FileOpen(TCHAR* filename, TCHAR* mode);
	bool FileClose();
	int GetCellCnt() { return m_iCellConut; }
	float GetDistance() { return m_fDistance; }
	vector<PNCT_Vertex>* GetVertexList() { return &m_VertexList; }
	vector<DWORD>* GetIndexList() { return &m_IndexList; }
	TString* GetSpreatTex() { return &m_szSpreatTexture; }
	map<int, TString>* GetAlphaTex() { return &m_szAlphaTexture; }
public:
	XMapData();
	virtual ~XMapData();
};

