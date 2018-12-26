#pragma once
#include"XStd.h"

enum DataType {
	INT_TYPE = 0, FLOAT_TYPE, STR_TYPE, VECTOR_TYPE, VERTEX_TYPE, CVERTEX_TYPE, UVW_TYPE, TFACE_LIST_TYPE, CFACE_LIST_TYPE, FACE_LIST_TYPE, QUATERNION_TYPE, SCALE_TYPE
};

struct XFaceList
{
	union
	{
		struct { DWORD	_0, _1, _2; };
		DWORD v[3];
	};
	DWORD   MtrlId;
	XFaceList() :MtrlId(0), _0(0), _1(0), _2(0)
	{}
};

class XParser
{
public:
	FILE*		m_fp;
	fpos_t		m_FilePos;
	TCHAR		m_strBuffer[256];
	TCHAR		m_strName[256];
	int			m_iTemp;
public:
	bool		FileOpen(TCHAR* filename, TCHAR* mode);
	bool		FileClose();
	TCHAR*		GetNextLine();
	bool		FindLine(TCHAR* token);
	BOOL		FindToken(TCHAR** token, int TokenSize);
	TCHAR*		DeleteDir(TCHAR* path);
	TCHAR*		SetDir(TCHAR* filename);
	TCHAR*		ExtType(TCHAR* path);
	bool		GetData(TCHAR* string, void* pData, int DataType = 0);
public:
	XParser();
	~XParser();
};
