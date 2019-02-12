#pragma once
#include "AMesh.h"
#include "ASkinObj.h"


class AMapObj : public AModel
{
public:
	int									m_iMaxWeight;
	vector<SkinMesh*>					m_pSkinMeshList;
	vector<D3DXMATRIX>					m_matBipedList;

	vector<D3DXMATRIX>* GetMatrix()
	{
		return &m_matBipedList;
	};
public:
	bool	Frame(const float& spf, const float& accTime)  noexcept override;
	bool		Render(ID3D11DeviceContext*    pContext) noexcept override;
	bool		Release()noexcept override;
	bool		Load(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);

	bool		LoadMesh(FILE	*fp, ID3D11Device* pDevice, AMesh* pMesh, SkinMesh* pData, const TCHAR* szFileName);

	//void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	bool		SetInputLayout(ID3D11Device* pDevice);
	bool		UpdateBuffer(ID3D11DeviceContext* pContext);
	bool		CombineBuffer(ID3D11DeviceContext* pContext, ID3D11Buffer* pVB, ID3D11Buffer* pIB);
	bool		Draw(ID3D11DeviceContext*    pContext, AModel* pParent);
	bool		LoadTexture(ID3D11Device* device, const TCHAR* fileName);

	//bool SetMapObject(const TCHAR* fileName, float x, float y, float z);
	GameObject* clone() noexcept override;
public:
	AMapObj();
	virtual ~AMapObj();
};

