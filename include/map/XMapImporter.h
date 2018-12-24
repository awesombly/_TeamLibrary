#pragma once
#include "XMapData.h"

class XMapImporter :public XMapData
{
public:
	bool Import();
	void ImportVertexData();
	void ImportIndexData();
	void ImportTexturePath();
public:
	XMapImporter();
	virtual ~XMapImporter();
};

