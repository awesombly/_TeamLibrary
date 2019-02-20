#pragma once
#include "AMesh.h"
//#include "AAseObj.h"
#include <fstream>


struct SkinMesh : public ATri_PNCT5
{
public:
	int										m_iNumFace;
	vector<PNCT5_VERTEX>					m_VertexArray;
	vector<DWORD>							m_IndexArray;
	vector<ATri_PNCT5>						m_TriList;
	vector<SkinMesh*>						m_pSubMesh;
	vector<PNCT5_VERTEX*>					m_pChildMesh;
	PNCT5_VERTEX*							m_pDrawVertex;
	D3DXMATRIX								m_matInverse;
public:
	int IsEqulVertexList(vector<PNCT5_VERTEX>&  VertexArray, PNCT5_VERTEX& Vertex);
	int SetUniqueBuffer(vector <ATri_PNCT5>& TriList, int iMtrl, int iStartTri);
	SkinMesh(void)
	{
		m_pDrawVertex = NULL;
		m_iNumFace = 0;
	}
};


class ASkinObj : public AModel
{
public:
	fstream								m_FP;
	int									m_iMaxWeight;
	vector<SkinMesh*>					m_pSkinMeshList;
	vector<D3DXMATRIX>					m_matBipedList;


	int m_SkinTest;

	vector<D3DXMATRIX>* GetMatrix() 
	{
		return &m_matBipedList; 
	};
public:
	bool		Render(ID3D11DeviceContext*    pContext) noexcept override;
	bool		Release()noexcept override;
	bool		Load(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	
	bool		Convert(const TCHAR* strPathName = 0);
	bool		LoadMesh(FILE	*fp, ID3D11Device* pDevice, AMesh* pMesh, SkinMesh* pData, const TCHAR* szFileName);

	//void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	bool		SetInputLayout(ID3D11Device* pDevice);
	bool		UpdateBuffer(ID3D11DeviceContext* pContext);
	bool		CombineBuffer(ID3D11DeviceContext* pContext, ID3D11Buffer* pVB, ID3D11Buffer* pIB);
	bool		Draw(ID3D11DeviceContext*    pContext, AModel* pParent);
	bool		LoadTexture(ID3D11Device* device, const TCHAR* fileName);

//	bool	CreateVertexBuffer();
//	bool	CreateIndexBuffer();
public:
	ASkinObj();
	virtual ~ASkinObj();
};

