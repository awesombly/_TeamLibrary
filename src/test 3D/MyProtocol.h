#pragma once
#pragma warning(disable : 4063)
#include "header.h"



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
	PACKET_SetHP,
	// 클라 요청
	PACKET_ReqSync			= 2000,
	PACKET_ReqSyncSpawns,
	PACKET_ReqAddPlayer,
	// Key X
	PACKET_SyncObjects		= 3000,
	PACKET_ChatMessage,
	PACKET_PlaySound,
	PACKET_TakeObject,
	PACKET_PossessPlayer,
	PACKET_SendUserInfo,
};

// 패킷 배열 제외 사이즈
#define PS_UserInfo					13
#define PS_SyncObjects				2
#define PS_ChatMessage				5
#define PS_PlaySound				17
#define PS_TakeObject				49
#define PS_UserInfo					18


#pragma pack(push, 1)
struct UserInfo
{
	UINT UserSocket = 0;
	UINT IconNum	= 0;
	UINT Score		= 0;
	WORD KillCount	= 0;
	WORD DeathCount	= 0;
	bool isDead		= false;

	UCHAR DataSize	= 0;
	WCHAR UserID[11];
};

struct Packet_PlayerDead
{
	UINT KeyValue;
	UINT KillUser;
};

struct Packet_SetHP
{
	UINT KeyValue;
	float HP;
};

struct Packet_TakeObject
{
	UINT KeyValue;
	UINT UserSocket;
	//D3DXVECTOR3 Force;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Scale;
	D3DXQUATERNION Rotation;

	UCHAR DataSize;
	WCHAR ObjectName[51];
};

struct Packet_ChatMessage
{
	UINT UserSocket;
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