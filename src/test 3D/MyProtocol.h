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
#define PACKET_SetAnimTransform		1008
#define PACKET_PossessPlayer		1011
#define PACKET_PlaySound			1012
//#define PACKET_TakeObject			1013
#define PACKET_MouseRotate			1014
#define PACKET_ChattingMessage		1015

#define PACKET_ReqSync				2000
#define PACKET_ReqSyncSpawns		2001
#define PACKET_ReqAddPlayer			2002





#pragma pack(push, 1)
//struct Packet_TakeObject
//{
//	UINT KeyValue;
//	
//	D3DXVECTOR3 Position;
//	D3DXVECTOR3 Scale;
//	D3DXQUATERNION Rotation;
//	char ProtoNum;
//};

struct Packet_MouseRotate
{
	UINT KeyValue;
	float RotateSpeed;
};

struct Packet_SoundData
{
	UINT KeyValue;
	D3DXVECTOR3 Position;
	float MaxDistance;
	char NameSize;
	char SoundName[100];
};


struct Packet_PossessPlayer
{
	UINT KeyValue;
	char ECharacter;
};

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

//struct Packet_AnimState
//{
//	UINT KeyValue;
//	D3DXVECTOR3 Direction;
//	char EAnimState;
//};

struct Packet_Transform
{
	UINT KeyValue;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Scale;
	D3DXQUATERNION Rotation;
};

//struct Packet_Collider
//{
//	UINT KeyValue;
//	D3DXVECTOR3 Position;
//	//D3DXVECTOR3 Force;
//	D3DXQUATERNION Rotation;
//};		콜라이더 리스트에서 동기화?


struct Packet_AnimTransform
{
	UINT KeyValue;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Force;
	D3DXVECTOR3 Direction;
	D3DXQUATERNION Rotation;
	char EAnimState;
	char ECharacter;
};
#pragma pack(pop)