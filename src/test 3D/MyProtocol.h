#pragma once
#include "header.h"

#define PACKET_SetPosition 1000
#define PACKET_Translate   1001

struct Packet_Position
{
	UINT KeyValue;
	D3DXVECTOR3 position;
};

