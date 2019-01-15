#pragma once
#include <functional>
#include "ISingleton.h"
#include "MyProtocol.h"
#include "uiheader.h"

#include "../network/PPTCPIOCPClient.h"
#include "../network/PPRecvPacketPool.h"



class PacketManager : public ISingleton<PacketManager>
{
public:
	list<UserInfo*>	UserList;
	UserInfo*		pMyInfo;

	PP::PPSender* pSender;
	bool isHost;

	UINT	PlayerKeyCount = 10000;
	queue< PP::PPPacketForProcess> PacketHistory;
	///
	wstring InputIP;
	JListCtrl*   m_pChatList = nullptr;
	//
	std::function<int()> PacketFunc;
public:
	void SendPlaySound(const string_view& soundName, const D3DXVECTOR3& position, const float& maxDistance) noexcept;

	void SendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept;
	void ReqSendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept;
	void InterceptPacket(const PP::PPPacketType& sendMode, const char* data)		noexcept;
public:
	friend class ISingleton<PacketManager>;
	PacketManager()			 = default;
	virtual ~PacketManager() = default;
};