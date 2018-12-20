#pragma once
#include <filesystem>
#include <fstream>
#include "GameObject.h"
#include "CAnimation.h"
#include "InstanceRenderer.h"


enum class EClassType : char {
	Geom = 0, Biped,
	Bone, Dummy,
};

struct SceneData
{
	int TotalFrame = 0;		// 총 프레임 수
	float SPF = 0.0f;		// 프레임 간격
	int MeshCount;			// 메쉬오브젝트 개수
	int MaxWeight;			// 정점 당 가중치
	bool BindPose;			// 바인딩 포즈 에니메이션 여부
}; 

struct MaterialData
{
	UINT	MaterialIndex;
	//wstring MaterialName;
	//wstring MaterialClassName;	// 클래스 타입(Standard, SubMaterial...)
	vector<MaterialData> SubMaterials;
	Texture* pTexture = nullptr;
	//vector<Tex>;
	//UINT SubCount;
	//UINT TexMapCount;
	MaterialData* pParent = nullptr;
};



class MaxImporter
{
protected:
	// 검색용 열거형 태그
	enum class EAseTag : char {
		Scene, MaterialList, Mesh
	};
	/*enum class EScene : char {
		_MaterialList = 0,
		FileName, FirstFrame, LastFrame, FrameSpeed, TickPerFrame,
	};*/
	//enum class EMaterial : char {
	//	_Material = 0, _GeomObject, _HelperObject,
	//	MatrialCount, MaterialName, MaterialClass,
	//	NumSubMaterial, SubMaterial,
	//	MapDiffuse, BitMap, MapReflect
	//};
	//enum class EMaterial_Class : char {
	//	Standard, MultySubObject,
	//};
	//enum class EHelper : char {
	//	_GeomObject = 0, _HelperObject,
	//	NodeName, NodeParent, HelperClass,
	//	NodeTM, BoundingMin, BoundingMax, Anim
	//};
	//enum class EGeom : char {
	//	_GeomObject = 0, _HelperObject,
	//	NodeName, NodeParent, MaterialRef,
	//	NodeTM, Mesh, Anim,
	//};
	//enum class EGeom_Mesh : char {
	//	_MaterialRef = 0, _NodeTM, _Animation,
	//	TimeValue,
	//	NumVertex, VertexList, Vertex,			// posision
	//	NumFaces, FaceList, Face,
	//	NumTexVertex, TexVertexList, TexVertex,		// texture
	//	NumTexFaces, TexFaceList, TexFace,
	//	NumColorVertex, ColorVertexList, ColorVertex,	// color
	//	NumColorFace, ColorFaceList, ColorFace,
	//	Nomals, FaceNormal, VertexNormal,	// normal 
	//};
	/*enum class EObject_NodeTM : char {
		_GeomObject = 0, _HelperObject,
		_MaterialRef, _Mesh, _Animation,
		NodeName,
		TM_ROW0, TM_ROW1, TM_ROW2, TM_ROW3,
		TM_POS, TM_ROTAXIS,
		TM_ROTANGLE, TM_SCALE,
		TM_SCALEAXIS, TM_SCALEAXISANG,
	};
	enum class EObject_Animation {
		_GeomObject = 0, _HelperObject, _MaterialRef,
		NodeName,
		PositionTrack, PositionSample,
		RotationTrack, RotationSample,
		ScaleTrack, ScaleSample,
	};*/
protected:
	filesystem::path m_filePath;					// 파일 경로
	std::streampos m_fileSize;						// 파일 바이트

	forward_list<wistringstream> m_LineBuffers;				// 전체 라인 배열
	forward_list<wistringstream>::iterator m_curLineIter;	// 현재 라인
	wstring m_subString;		// 임시 스트링
	SceneData m_SceneData;		// 씬 데이터

	// 검색용 태그 맵
	static map<wstring, EAseTag>			m_AseTag;
	//static map<wstring, EScene>				m_SceneTag;
	//static map<wstring, EMaterial>			m_MaterialTag;
	//static map<wstring, EMaterial_Class>	m_MaterialClass;
	//static map<wstring, EHelper>			m_HelperTag;
	//static map<wstring, EGeom>				m_GeomTag;
	//static map<wstring, EGeom_Mesh>			m_GeomMesh;
	//static map<wstring, EObject_NodeTM>		m_ObjectNodeTM;
	//static map<wstring, EObject_Animation>	m_ObjectAnimation;

	vector<MaterialData>	m_MaterialList;
	map<wstring, GameObject*>			m_Objects;
	map<wstring, InstanceRenderer*>		m_Instances;
private:
	// 버퍼 내용으로 객체 생성
	bool LoadFileData(GameObject** ppReturnObj) noexcept;
	// 각 데이터 채움
	bool LoadScene(SceneData* pScene) noexcept;
	bool LoadMaterialList(const int& parentCnt) noexcept;
	bool LoadObject(const int& objCnt) noexcept;
	//bool LoadGeomMesh(vector<Vertex_PNCT>* pVertexList, vector<WORD>* pIndexList, const D3DXMATRIX& matWorld)	noexcept;
	//bool LoadObjectNodeTM(GameObject* pObject, CAnimation* pAnimation) noexcept;
	//bool LoadObjectAnimation(vector<Anim>* pAnimations) noexcept;
	//void SetVertexIndex(vector<Vertex_PNCT>* pTempVertex, vector<Vertex_PNCT>* pVertexList, vector<WORD>* pIndexList) noexcept;
	void Clear() noexcept;				// 버퍼 초기화
public:
	// 파일 불러오고 객체 생성후 반환
	bool CreateFromFile(GameObject** ppReturnObj, const wstring_view& fileName, const wstring_view& directory = L"..\\..\\data\\model") noexcept;
public:
	MaxImporter() noexcept;
	virtual ~MaxImporter() = default;
};