#pragma once
#include "ADxObjStd.h"
#include "ALoadToken.h"

#define ID_BASIS_AM 0   // ambient
#define ID_BASIS_DI 1   // diffuse
#define ID_BASIS_SP 2   // specular
#define ID_BASIS_SH 3   // shininesNs
#define ID_BASIS_SS 4   // shininess strength
#define ID_BASIS_SI 5   // self-illumination
#define ID_BASIS_OP 6   // opacity
#define ID_BASIS_FI 7   // filter color
#define ID_BASIS_BU 8   // bump 
#define ID_BASIS_RL 9   // reflection
#define ID_BASIS_RR 10  // refraction 
#define ID_BASIS_DP 11  // displacement
#define ID_BASIS_GN 12  // Generic
#define ID_BASIS_EV 13  // EnvMap

const enum OBJECTCLASS_TYPE 
{
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};

const enum FILE_TYPE {
	NULLFILE = 0,
	ASEFILE,
	KKBFILE,
	SKMFILE,
	MATFILE,
};

struct VersionMark
{
	int		iVersion;
	TCHAR	description[128];
};

struct ATri
{
	PNCT_VERTEX					vVertex[3];
	D3DXVECTOR3				vNormal;
	int						iSubIndex;
	ATri(int Index) : iSubIndex(Index) {}
	ATri() : iSubIndex(-1) {}
};


struct AScene
{
	int iVersion;		// 버전
	int	iFirstFrame;    // 시작 프레임
	int	iLastFrame;     // 마지막 프레임
	int	iFrameSpeed;    // 1초당 프레임 개수(30)
	int	iTickPerFrame;  // 1프레임의 틱 값(160)
	int iNumMesh;		// 메쉬오브젝트 개수		매트릭스 카운터
	int iMaxWeight;		// 정점 당 가중치			매트릭스 가중치
	int iBindPose;		// 바인딩 포즈 에니메이션 여부
};


struct ATextexMap
{
	// 오브젝트 텍스쳐 관리자에서 매터리얼의 텍스쳐파일명을 보고 DX텍스쳐 생성 후 참조인덱스를 저장한다.
	DWORD		m_dwIndex;
	// 텍스쳐 맵 타입인덱스
	DWORD		m_dwType;
	// 맵 이름
	T_STR		m_strName;
	// 맵 클래스 이름
	T_STR		m_strClassName;
	// 맵 텍스쳐 이름.
	T_STR		m_strTextureName;
	ATextexMap() : m_dwType(1), m_dwIndex(0) {}
};

struct AMtrl
{
	T_STR		m_strName;
	T_STR		m_strClassName;
	
	vector<ATextexMap>	m_TexMaps;
	vector<AMtrl>		m_SubMtrl;

	//TBS
	DWORD m_dwSubCount;
	DWORD m_dwTexMapCount;
	DWORD m_dwIndex;

	AMtrl() {}
	~AMtrl()
	{
		m_TexMaps.clear();
		m_SubMtrl.clear();
		//stl_wipe_vector(m_SubMaterial);
		//stl_wipe_vector(m_TexMaps);
	}
};


struct AFaceList
{
	union
	{
		struct { DWORD	_0, _1, _2; };
		DWORD v[3];
	};
	DWORD   dwMtrl;
	AFaceList() : dwMtrl(0), _0(0), _1(0), _2(0)
	{}
};

struct AVertexList
{
	DWORD					dwNumVertex;
	DWORD					dwNumFace;
	vector<D3DXVECTOR3>		pVertexList;
	vector<AFaceList>		pFaceList;
	vector<DWORD>			pSubListMtl;
	AVertexList() : dwNumVertex(0), dwNumFace(0)
	{
	}
	~AVertexList() {
	//	stl_wipe_vector(pVertexList); 	stl_wipe_vector(pVertexList); 
		pVertexList.clear();
		pFaceList.clear();
		pSubListMtl.clear();
	}
};

struct AAnimTrack
{
	int					iTick;
	D3DXQUATERNION		qRotate;	//사원수 D3 angle
	D3DXVECTOR3			vVector;
	AAnimTrack*			pNext;
	AAnimTrack*			pPrev;
};


struct AGeomMesh
{
	D3DXMATRIX				m_matLocal;
	T_STR					m_strNodeName;
	T_STR					m_strParentName;	// 부모이름

	DWORD					m_iNumFace;
	INT						m_iMtrlRef;

	AVertexList				m_PosVertexList;
	AVertexList				m_NorVertexList;
	AVertexList				m_ColVertexList;
	AVertexList				m_TexVertexList;

	D3DXMATRIX				m_matWorldTrans;	// 월드 이동 행렬
	D3DXMATRIX				m_matWorldRotate;	// 월드 회전 행렬
	D3DXMATRIX				m_matWorldScale;	// 월드 신축 행렬

	vector<AAnimTrack>		m_PosTrack;		// 이동 트랙 
	vector<AAnimTrack>		m_RotTrack;		// 회전 트랙
	vector<AAnimTrack>		m_SclTrack;		// 신축 트랙												
	vector<AAnimTrack>		m_VisTrack;		// 알파 트랙

	OBJECTCLASS_TYPE		m_ClassType;

	A_Box					m_Box;

	AGeomMesh() : m_iNumFace(0), m_iMtrlRef(-1)
	{}
	
};







//띄어쓰기 공백 주위하기
enum AseSectionType { SCENE = 0, MATERIALLIST, GEOMOBJECT, HELPEROBJECT, };
static const TCHAR* AseSections[] =
{
	_T("SCENE "),
	_T("MATERIAL_LIST "),
	_T("GEOMOBJECT "),
	_T("HELPEROBJECT "),
};

enum AseMaterialType { NUM_SUBMTRLS = 0, MAP_NAME, };
static const TCHAR* AseMaterialTokens[] =
{
	_T("NUMSUBMTLS"),
	_T("MAP_NAME"),	//맵 네임 반복부터 지오브젝트까지
	_T("}"),		//제일앞이 } =>> 지 오브젝트 바로 전 라인에서 리턴한다.
};


enum AseTextureType { MAP_SUBNO = 0, BITMAP_FILE = 1/*, MAP_CLASS*/ };
static const TCHAR* AseTextureTokens[] =
{
	_T("MAP_SUBNO"),
	_T("BITMAP "),
	//_T("MAP_CLASS "),
	_T("SUBMATERIAL"),  
	_T("MATERIAL "),
	_T("HELPEROBJECT "), 
	_T("GEOMOBJECT ")
};


enum AseGeomObjectType
{
	NODE_PARENT = 0, MODE_TM,
	MESH, TM_ANIMATION, TM_VISIBILITY, MATERIAL_REF,
};


static const TCHAR* AseGeomObjectTokens[] =
{
	_T("NODE_PARENT"),
	_T("NODE_TM"),
	_T("MESH {"),
	_T("TM_ANIMATION"),
	_T("NODE_VISIBILITY_TRACK"),
	_T("MATERIAL_REF"),
	_T("GEOMOBJECT"),	
	_T("HELPEROBJECT"),
};


enum AseHelperObjectType 
{
	HELPER_NODE_PARENT = 0, HELPER_CLASS, HELPER_MODE_TM,
	BOUNDINGBOX_MIN, HELPER_TM_ANIMATION,
};
static const TCHAR* AseHelperObjectTokens[] =
{
	_T("NODE_PARENT"),
	_T("HELPER_CLASS"),
	_T("NODE_TM"),
	_T("BOUNDINGBOX_MIN"),
	_T("TM_ANIMATION"),
	_T("GEOMOBJECT"),	// Return
	_T("HELPEROBJECT"),// Return
};




enum SET_DATA_TYPES
{
	// Gameplay semantics
	NULL_DATA = 0,
	BOOL_DATA,
	INT_DATA,
	FLOAT_DATA,
	STRING_DATA,
	VERTEX_DATA,
	VECTOR_DATA,
	INDEX_DATA,
	FACE_DATA,
	MATRIX_DATA,
};


enum AseTrackType 
{
	POS_SAMLE_TRACK = 40, ROT_SAMLE_TRACK, SCL_SAMLE_TRACK, VIS_SAMLE_TRACK
};

