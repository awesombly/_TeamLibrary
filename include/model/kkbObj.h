#pragma once
#include "AAseParser.h"
#include "AMesh.h"
#include "ATexture.h"



//MAX kkb 파일 
class kkbObj : public AModel

{
private:
	const TCHAR*					m_FileName;
public:
	float					m_fTickSpeed;
	float					m_fElapseTime;
	float					fFrameTick = 5;
	int						m_iMaxVertex = 0;
	int						m_iMaxIndex = 0;


public:
	AScene				m_Scene;
	AAseParser			m_Parser;

	vector<AMtrl>		m_Material;			//텍스처 정보가 들어 있다.
	vector<AGeomMesh*>	m_GeomObjList;		//지오브젝트가 들어 갈 구조체가 필요

	vector<AMesh*>		m_MeshList;		//모든 정보를 여기에 담게 된다.

public:
	bool	Load(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);


//	bool	Convert(ID3D11Device* pd3dDevice);
//
	bool	CreateVertexBuffer(ID3D11Device* pdevice);
	bool	CreateIndexBuffer(ID3D11Device* pdevice);
//	//bool	CreateConstantBuffer();
//
//
	bool	LoadTexture(ID3D11Device* device, const TCHAR* fileName);
//
//	bool	Frame();
//	bool	Render(ID3D11DeviceContext* context);
//
//public:
//	//bool	SetMeshFaceList(AGeomMesh* GObj, vector<AMesh*>& MeshList, AMesh* pMesh);
//	bool	SetMeshFaceList(AGeomMesh* GObj, AMesh* pMesh);
//	bool	SetMeshSubFaceList(AGeomMesh* GObj, vector<AMesh*>& MeshList);
//
//	bool	ConvertAnim(AGeomMesh* GObj, AMesh* pMesh);
//	void	AnimPlay(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick);
//	bool	GetAnimationTrack(float fFrameTick, vector<AAnimTrack*> pTrackList, AAnimTrack** ppStartTrack, AAnimTrack** ppEndTrack);
//	AAnimTrack* SetLinkedTrack(AAnimTrack* pCurrentTrack, AAnimTrack* pPrev);
//
//
//	float PlayTrackTick(float fFrameTick, float fStartTick, float fEndTick);
//	AMesh* SearchToCollects(T_STR	m_strParentName);
//
//	//bool SetInputLayout();
//	bool AseRender(ID3D11DeviceContext*    pContext, AModel* pParent);
//	//int GetMapID(AMtrl* pMtrl, int iTexMapType);
//
//////////////////////////////////////////////////////////

	bool		Frame(const float& spf, const float& accTime)	noexcept override;
	bool		Render(ID3D11DeviceContext*    pContext)		noexcept override;

	virtual bool		Draw(ID3D11DeviceContext*    pContext, AModel* pParent);
////	bool		Release();
////	bool		ResetResource();
	////////////////////////////////////////////////
	//  오브젝트 로드 관련 함수
	////////////////////////////////////////////////
//	int			GetMeshCounter() { return m_pData.size(); }
//	bool		Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	//  씬 정보 로드
	bool		LoadScene(const TCHAR* strFileName);
//	//  메터리얼 정보 로드
	bool		LoadMaterial(ID3D11Device* pdevice);
	void		LoadMaterialHeader(AMtrl* pMtrl);
	void		LoadTexMap(ID3D11Device* pdevice, AMtrl* pMtrl, T_STR szDirName);

//	 오브젝트 정보 로드
	bool		LoadObject();
	int			LoadVertexIndex(AMesh* pMesh);
//	//  에니메이션 정보 로드
	void		LoadAnimation(AMesh* pMesh);
	bool		LoadAnimationTrack(int iNumTrack, vector<AAnimTrack*>& pTrackList);
	float		PlayTrackTick(float fFrameTick, float fStartTick, float fEndTick);
	D3DXMATRIX AnimPlay(AMesh* pMesh, D3DXMATRIX* matParents, float fFrameTick);
	bool GetAnimationTrack(float fFrame, vector<AAnimTrack*> pTrackList, AAnimTrack** pStartTrack, AAnimTrack** pEndTrack);
//	//--------------------------------------------------------------------------------------
//	// 랜더링 관련 정보 생성
//	//--------------------------------------------------------------------------------------
//	// 로딩 된 정보를 AMesh로 변환
	bool		Convert(ID3D11Device* pd3dDevice);
//	// 매터리얼 세팅
	bool		SetMaterial();
	int			GetMapID(AMtrl* pMtrl, int iTexMapType); // 맵 인덱스 검색
//	// 상속관계 구축
	bool		InheriteCollect();
	AMesh*		SearchToCollects(T_STR	m_strParentName);
	AAnimTrack* SetDoublyLinkedList(AAnimTrack* pCurrentTrack, AAnimTrack* pPrev);
public:
	kkbObj();
	virtual ~kkbObj();
};

