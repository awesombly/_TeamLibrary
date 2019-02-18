#pragma once
#include <functional>
#include "ISingleton.h"
#include "MyProtocol.h"
#include "uiheader.h"
#include "../network/PPTCPIOCPClient.h"
#include "../network/PPRecvPacketPool.h"

class Collider;

enum ESocketType : UINT {
	EZombie = DataInfo::MaxUINT - 20, 
	ECaster, ECrawler, EMutant, ETank, 
	EDummy = DataInfo::MaxUINT
};


class PacketManager : public ISingleton<PacketManager>
{
public:
	map<UINT, UserInfo*> UserList;
	UserInfo*			 pMyInfo;

	PP::PPSender*		pSender;
	bool				isHost;

	UINT				PlayerKeyCount	= 10000;
	queue< PP::PPPacketForProcess> PacketHistory;
	///
	wstring				InputIP;
	JListCtrl*			pChatList		= nullptr;

	JPanel*				pUserPanel[4];
	JListCtrl*			pKillDisplay	= nullptr;
	//
	std::function<int()> PacketFunc;

	D3DXVECTOR3			TowerPos[8];
	char				TowerLevel		= 0;
	float				TowerDelayShot  = 0.0f;
	float				TowerDelayShot2 = 0.0f;
	float				TowerDamage		= 0.0f;
public:
	void SendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept;
	void ReqSendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept;
	void InterceptPacket(const PP::PPPacketType& sendMode, const char* data)		noexcept;

	// 패킷 보내기
	void SendPlaySound(const string_view& soundName, const D3DXVECTOR3& position, const float& maxDistance) noexcept;
	void SendDeadEvent(const UINT& keyValue, const UINT& deadSocket, const UINT& killSocket) noexcept;
	void SendTakeObject(const WCHAR* objName, const UINT& socketNum, const UCHAR& spawnCount, const float& hp, const float& minScale, const float& randScale, const D3DXVECTOR3& minPosition, const D3DXVECTOR3& randPosition)  noexcept;
	void SendSpawnEnemy(const WCHAR* objName, const UINT& socketNum, const UCHAR& spawnCount, const float& hp, const float& minScale, const float& randScale)  noexcept;
public:
	friend class ISingleton<PacketManager>;
	PacketManager()			 = default;
	virtual ~PacketManager() = default;
};