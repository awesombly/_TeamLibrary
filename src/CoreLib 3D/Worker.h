//#pragma once
//#include "../../include/server/ServerBasic.h"
//#include "../../include/server/MyProtocol.h"
//
//// 서버를 상속 받았지만, 서버 기능은 사용 안함
//// 패킷 처리 쓰레드
//class Worker :public ServerBasic
//{
//protected:
//	virtual bool Run() override;
//public:
//	bool Start(const USHORT& portNum, const string_view& address, const bool& isClient = false) override;
//	void ProcessPacket(UPACKET* myPacket);
//
//	Worker() = default;
//	virtual ~Worker() = default;
//};
//
