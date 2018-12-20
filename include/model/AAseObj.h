#pragma once
#include "AAseParser.h"
#include "AMesh.h"


class AAseObj : public AMesh
{
private:
	const TCHAR*					m_FileName;
public:
	float					m_fTickSpeed;
	float					m_fElapseTime;
	float					fFrameTick=5;
	int						m_iMaxVertex = 0;
	int						m_iMaxIndex = 0;


public:
	AScene				m_Scene;
	AAseParser			m_Parser;

	vector<AMtrl>		m_MtrlList;			//텍스처 정보가 들어 있다.
	vector<AGeomMesh*>	m_GeomObjList;		//지오브젝트가 들어 갈 구조체가 필요
	
											
	vector<AMesh*>		m_MeshList;		//모든 정보를 여기에 담게 된다.

public:
	bool	Load(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pContext ,const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	bool	Convert(ID3D11Device* pd3dDevice);

	bool	CreateVertexBuffer(ID3D11Device* pd3dDevice);
	bool	CreateIndexBuffer(ID3D11Device* pd3dDevice);
	//bool	CreateConstantBuffer();


	bool	LoadTexture(ID3D11Device* device, const TCHAR* fileName);

	bool	Frame(const float& spf, const float& accTime)	noexcept override;
	bool	Render(ID3D11DeviceContext* context)noexcept override;
	
public:	

	bool	ConvertAnim(AGeomMesh* GObj, AMesh* pMesh);
	void	AnimPlay(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick);
	bool	GetAnimationTrack(float fFrameTick,vector<AAnimTrack*> pTrackList, AAnimTrack** ppStartTrack, AAnimTrack** ppEndTrack);
	AAnimTrack* SetLinkedTrack(AAnimTrack* pCurrentTrack, AAnimTrack* pPrev);


	float PlayTrackTick(float fFrameTick, float fStartTick,float fEndTick);
	AMesh* SearchToCollects(T_STR	m_strParentName);


	bool AseRender(ID3D11DeviceContext*    pContext, AModel* pParent);
	int GetMapID(AMtrl* pMtrl, int iTexMapType);


	void	SetTriangleBuffer(AGeomMesh* GObjList, AMesh* pMesh);

public:
	AAseObj();
	virtual ~AAseObj();
};






class AAnimation
{
public:
	D3DXMATRIX	Interpolate(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick, AScene tScene);
	D3DXMATRIX	Update(float fElapseTime, AMesh* pMesh, D3DXMATRIX &matWorld, AScene &tScene);
	bool		GetAnimationTrack(float fFrame, vector<AAnimTrack*>& pTrackList, AAnimTrack** pStartTrack, AAnimTrack** pEndTrack);
public:
	AAnimation(void);
	virtual ~AAnimation(void);
};



