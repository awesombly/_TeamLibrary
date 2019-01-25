#pragma once
#include <functional>
#include "ISingleton.h"
#include "MyProtocol.h"
#include "uiheader.h"

#include "../network/PPTCPIOCPClient.h"
#include "../network/PPRecvPacketPool.h"

class Collider;

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
public:
	void SendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept;
	void ReqSendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept;
	void InterceptPacket(const PP::PPPacketType& sendMode, const char* data)		noexcept;

	// ��Ŷ ������
	void SendPlaySound(const string_view& soundName, const D3DXVECTOR3& position, const float& maxDistance) noexcept;
	void SendDeadEvent(const UINT& keyValue, const UINT& deadSocket, const UINT& killSocket) noexcept;
public:
	friend class ISingleton<PacketManager>;
	PacketManager()			 = default;
	virtual ~PacketManager() = default;
};