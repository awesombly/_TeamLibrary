#include "PacketManager.h"
#include "ObjectManager.h"



void PacketManager::SendPacket(char* data, const USHORT& packeyType, const PP::PPSendMode& sendMode) noexcept
{
	//pSender->BroadcastWString(L"Hello, Server!");	//서버에게 문자열 전송(함수명은 바꿀 예정)
	static PP::PPPacketForProcess packet;

	memcpy(packet.m_Packet.m_Payload, (void*)data, sizeof(data));

	packet.m_socketSession = 0;
	packet.m_SendMode = sendMode;
	packet.m_Packet.m_Header.m_type = (PP::PPPacketType)packeyType;
	packet.m_Packet.m_Header.m_len = strlen(packet.m_Packet.m_Payload) + PACKET_HEADER_SIZE;

	pSender->Broadcast(packet);
}



void PacketManager::InterceptPacket(const PP::PPPacketType& sendMode, const char* data) noexcept
{
	static Packet_Position p_position;
	switch (sendMode)
	{
	case PACKET_Translate:
	{
		memcpy(&p_position, data, sizeof(Packet_Position));
		ObjectManager::KeyObjects[p_position.KeyValue]->Translate(p_position.position);
	}	break;
	 case PACKET_SetPosition:
	 {
	 	memcpy(&p_position, data, sizeof(Packet_Position));
	 	ObjectManager::KeyObjects[p_position.KeyValue]->SetPosition(p_position.position);
	 }	break;
	}
}