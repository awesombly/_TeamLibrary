#pragma once
//#include "ADxHelperEx.h"
#include "AStd.h"
class ALoadToken
{
public:
	T_STR		m_szDirName;
	T_STR		m_szName;

	HANDLE		m_hHandle;

	CHAR*		m_pmbStaticMeshData;		//멀티바이트 메쉬데이터
	TCHAR*		m_pwcStaticMeshData;		//유니코드 메쉬데이터

	vector<T_STR>	m_ListTokens;			//메쉬데이터를 한줄로 넣을 벡터

	T_STR		m_pwcToken;					//유니코드 토큰

	TCHAR		m_pString[256];
	
	TCHAR*		m_pwcSearchData;			//유니코드 검색 데이터
	T_STR		m_pwcTokenData;				//유니코드 토큰 데이터
	
	
	INT			m_iData;
	DWORD		m_TokenIndex;			//토큰 이넫ㄱ스
	DWORD		m_TokenMaxSize;			//맥스 테이블?

public:
	void			SetToken(T_STR szToken) { m_pwcToken = szToken; }
	bool			BeginToken();
	const TCHAR*	GetNextTokenString();
	const TCHAR*	GetCurrentTokenString();
	
	bool			LoadBuffer(const TCHAR* strFileName);
	bool			CutDirName(const TCHAR* strFileName);

	INT				SearchTokenArray(INT NumObjType,const TCHAR** pObjectType, DWORD dwNextObjectIndex = 0);
	const TCHAR*	GetSearchString(const TCHAR* szSearchData, bool bLoop = true);
	//bool			DeleteBuffer();


public:
	ALoadToken();
	~ALoadToken();
};

