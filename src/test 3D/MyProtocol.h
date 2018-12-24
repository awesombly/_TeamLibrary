#pragma once
#pragma warning(disable : 4063)

#include "header.h"

#define PACKET_SetTransform			1000
#define PACKET_SetPosition			1001
#define PACKET_SetRotation			1002
#define PACKET_SetScale				1003
#define PACKET_Translate			1004
#define PACKET_Rotate				1005
#define PACKET_Scaling				1006
//#define PACKET_SetAnimation			1007
//#define PACKET_SetDirectionForce	1008
//#define PACKET_SetMoveStop			1009
#define PACKET_RequestSync			1010
//#define PACKET_Sync					1011
#define PACKET_AddPlayer			1012
#define PACKET_SetAnimTransform		1013


struct Packet_KeyValue
{
	UINT KeyValue;
};

struct Packet_Bool
{
	UINT KeyValue;
	bool value;
};

struct Packet_Vector3
{
	UINT KeyValue;
	D3DXVECTOR3 Vec3;
};

struct Packet_Vector2
{
	UINT KeyValue;
	D3DXVECTOR2 Vec2;
};

struct Packet_Quaternion
{
	UINT KeyValue;
	D3DXQUATERNION Quat;
};

struct Packet_Vec3Quat
{
	UINT KeyValue;
	D3DXVECTOR3 Vec3;
	D3DXQUATERNION Quat;
};

struct Packet_AnimState
{
	UINT KeyValue;
	char EAnimState;
};

struct Packet_AnimTransform
{
	UINT KeyValue;
	D3DXVECTOR3 Vec3;
	D3DXQUATERNION Quat;
	char EAnimState;
	char ECharacter;
};