#pragma once
#include "ISingleton.h"
#include "MyProtocol.h"
#include "uiheader.h"

#include "../network/PPTCPIOCPClient.h"
#include "../network/PPRecvPacketPool.h"

class PacketManager : public ISingleton<PacketManager>
{
public:
	bool isHost;
	PP::PPSender* pSender;

	wstring InputIP;
	UINT	PlayerKeyCount = 10000;
	queue< PP::PPPacketForProcess> PacketHistory;
	///
	JListCtrl*   m_pChatList = nullptr;
public:
	void SendPlaySound(const string_view& soundName, const D3DXVECTOR3& position, const float& maxDistance) noexcept;

	void InterceptPacket(const PP::PPPacketType& sendMode, const char* data) noexcept;
	void SendPacket(char* data, const USHORT& size, const USHORT& packeyType/*, const PP::PPSendMode& sendMode = PP::PPSendMode::BROADCAST*/) noexcept;
public:
	friend class ISingleton<PacketManager>;
	PacketManager()			 = default;
	virtual ~PacketManager() = default;
};