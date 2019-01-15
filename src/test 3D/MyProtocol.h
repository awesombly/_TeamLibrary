#pragma once
#pragma warning(disable : 4063)

#include "header.h"

//#define PACKET_SetTransform			1000
//#define PACKET_SetPosition			1001
//#define PACKET_SetRotation			1002
//#define PACKET_SetScale				1003
//#define PACKET_Translate			1004
//#define PACKET_Rotate				1005
//#define PACKET_Scaling				1006
//#define PACKET_SetAnimTransform		1008
//#define PACKET_PlayerDead			1009
//// 예외
//#define PACKET_ReqSync				2000
//#define PACKET_ReqSyncSpawns		2001
//#define PACKET_ReqAddPlayer			2002
//#define PACKET_HostAddPlayer		2003
//// Key X
//#define PACKET_SyncObjects			3016
//#define PACKET_ChatMessage			3015
//#define PACKET_PlaySound			3012
//#define PACKET_TakeObject			3011
//#define PACKET_PossessPlayer		3013

enum EPacketProtocol : USHORT {
	PACKET_SetTransform		= 1000,
	PACKET_SetPosition,
	PACKET_SetRotation,
	PACKET_SetScale,
	PACKET_Translate,
	PACKET_Rotate,
	PACKET_Scaling,
	PACKET_SetAnimTransform,
	PACKET_PlayerDead,
	// 예외
	PACKET_ReqSync			= 2000,
	PACKET_ReqSyncSpawns,
	PACKET_ReqAddPlayer,
	//PACKET_HostAddPlayer,
	// Key X
	PACKET_SyncObjects		= 3000,
	PACKET_ChatMessage,
	PACKET_PlaySound,
	PACKET_TakeObject,
	PACKET_PossessPlayer,
};

// 패킷 배열 제외 사이즈
#define PS_UserInfo					13
#define PS_SyncObjects				2
#define PS_ChatMessage				1
#define PS_PlaySound				17
#define PS_TakeObject				45


#pragma pack(push, 1)
struct UserInfo
{
	UINT UserSocket;
	UINT Score;
	WORD KillCount;
	WORD DeathCount;
	bool isDead;

	UCHAR DataSize;
	WCHAR UserID[21];
};

struct Packet_PlayerDead
{
	UINT KeyValue;
};

struct Packet_TakeObject
{
	UINT KeyValue;
	//D3DXVECTOR3 Force;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Scale;
	D3DXQUATERNION Rotation;

	UCHAR DataSize;
	WCHAR ObjectName[51];
};

struct Packet_ChatMessage
{
	//UINT KeyValue;
	UCHAR DataSize;
	WCHAR Message[101];
};

struct Packet_SoundData
{
	//UINT KeyValue;
	D3DXVECTOR3 Position;
	float MaxDistance;
	char DataSize;
	char SoundName[101];
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

struct Packet_ReqAddPlayer
{
	char ECharacter;
};


struct Packet_SyncTransform
{
	UINT KeyValue;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Force;
	D3DXQUATERNION Rotation;
};

struct Packet_SyncObjects
{
	//UINT KeyValue;

	WCHAR Count;
	Packet_SyncTransform Data[45];
};

#pragma pack(pop)