#pragma once
//#include "ALoadToken.h"
#include "AAseObjStd.h"




class AAseParser : public ALoadToken
{

public:
	//상위에 지오젝트를 만들고 함수애 넘겨서 처리할 생갛임
	//vector<shared_ptr<AAseMesh>>	m_pAseMesh;

public:
	bool			Release();
	bool			Load(const TCHAR* strFileName);

	bool			LoadScene(AScene& tScene);
	bool			LoadMaterialList(vector<AMtrl>&	tMaterialList);
	bool			LoadMaterial(AMtrl* pMaterial);
	bool			LoadTexture(AMtrl*	pMaterial);

	bool			LoadGeomObject(vector<AGeomMesh*>& GeomList);

	bool			LoadHelperObject(vector<AGeomMesh*>& GeomList);

	void			LoadTM(AGeomMesh* pObject);
	void			LoadMesh(AGeomMesh* pObject);

	void			LoadAnimation(AGeomMesh* pObject);
	//void			LoadVisAnimation(AAseMesh* pObject);

public:
	bool			GetData(const TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA, bool bLoop = true);

	bool			GetData(VOID* pData = NULL, INT DataType = NULL_DATA);


	bool			GetVertexFromIndexData(const TCHAR* strToken, DWORD dwSearchIndex, VOID* pData, INT DataType);
	bool			GetVertexListFromString(const TCHAR* strVertexList, DWORD dwNumVertex, vector<D3DXVECTOR3>& VertexList, const TCHAR* strToken = _T("*"));
	bool			GetFaceListFromString(const TCHAR* strFaceList, DWORD dwNumFace, AVertexList& VertexList, const TCHAR* strToken = _T("*"));
	bool			GetPositionFaceListFromString(const TCHAR* strFaceList, DWORD dwNumFace, AVertexList& VertexList,const TCHAR* strToken = _T("*"));
	bool			GetNormalListFromString(const TCHAR* strFaceList, DWORD dwNumFace, AGeomMesh* pGeomObject, const TCHAR* strToken = _T("*"));
	bool			GetTrackListFromString(vector<AAnimTrack>& PosTrack,	AseTrackType TrackType);




public:
	AAseParser();
	virtual ~AAseParser();
};



