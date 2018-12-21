#pragma once
#include "ISingleton.h"
#include "MyProtocol.h"

#include "../network/PPTCPIOCPClient.h"
#include "../network/PPRecvPacketPool.h"

class PacketManager : public ISingleton<PacketManager>
{
public:
	PP::PPSender* pSender;
public:
	void InterceptPacket(const PP::PPPacketType& sendMode, const char* data)										  noexcept;
	void SendPacket(char* data, const USHORT& packeyType, const PP::PPSendMode& sendMode = PP::PPSendMode::BROADCAST) noexcept;
public:
	friend class ISingleton<PacketManager>;
	PacketManager()			 = default;
	virtual ~PacketManager() = default;
};

