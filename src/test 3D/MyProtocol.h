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
	PACKET_PlayerDisable,
	PACKET_Player,
	PACKET_SetHP,
	PACKET_HealHP,
	PACKET_SetPhysicsInfo,
	// 클라 요청
	PACKET_ReqSync			= 2000,
	PACKET_ReqSyncSpawns,
	PACKET_ReqAddPlayer,
	// Key X
	PACKET_SyncObjects		= 3000,
	PACKET_ChatMessage,
	PACKET_PlaySound,
	PACKET_TakeObject,
	PACKET_TowerAttack,
	PACKET_TowerUpgrade,
	PACKET_PossessPlayer,
	PACKET_SendUserInfo,
	PACKET_SendLevelUp,
	PACKET_SendNameUpdate,
	PACKET_StartGame,
	PACKET_EndGame,
	PACKET_WaveStart,
	PACKET_WaveCount,
};

// 패킷 배열 제외 사이즈
#define PS_UserInfo					23
#define PS_SyncObjects				2
#define PS_ChatMessage				5
#define PS_PlaySound				17
#define PS_TakeObject				53


#pragma pack(push, 1)
struct UserInfo
{
	UINT UserSocket = 0;
	UINT KeyValue	= 0;
	UINT Score		= 0;
	WORD KillCount	= 0;
	WORD DeathCount	= 0;
	bool isDead		= false;
	UCHAR Level		= 1;
	UCHAR StatStr	= 0;
	UCHAR StatDex	= 0;
	UCHAR StatInt	= 0;
	UCHAR StatLuk	= 0;
	float AttackRate = 1.0f;
	float MotionRate = 1.0f;

	UCHAR DataSize	= 0;
	WCHAR UserID[11];
};

struct Packet_Physics
{
	UINT KeyValue;
	float Mass;
	float Damping;
	float Repulsion;
	float MaxHP;
	float Armor;
	float GravityScale;
};

struct Packet_TowerAttack
{
	UINT KeyValue;
	char TowerLevel;
};

struct Packet_PlayerDead
{
	UINT KeyValue;
	UINT DeadUser;
	UINT KillUser;
};

struct Packet_Float
{
	UINT KeyValue;
	float Value;
};

struct Packet_TakeObject
{
	UINT KeyValue;
	UINT UserSocket;
	//D3DXVECTOR3 Force;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Scale;
	D3DXQUATERNION Rotation;
	float HP;

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
	UCHAR ECharacter;
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
	UINT UserSocket;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Force;
	D3DXVECTOR3 Direction;
	D3DXQUATERNION Rotation;
	UCHAR EAnimState;
	UCHAR ECharacter;
};

struct Packet_ReqAddPlayer
{
	UCHAR ECharacter;
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