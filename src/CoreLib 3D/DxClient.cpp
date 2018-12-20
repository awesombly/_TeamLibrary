//#include "DxClient.h"
//#include "Worker.h"
//#include "../../include/server/ClientServer.h"
//
//
//
//void DxClientRecv::PushData(const int& size) noexcept
//{
//	//memcpy(m_packetBuffer + m_pushPos, data, size);
//	m_pushPos += size;
//
//	// ����� ���޾Ҵ��� �⺻������ 0���� ó���ǹǷ� ���� X
//	m_pUPacket = (UPACKET*)m_packetBuffer;
//	// ���� �����Ͱ� �ٲ� ��Ŷ ���̺��� ũ�ٸ�
//	while (m_pUPacket->header.length <= m_pushPos)
//	{
//		// ��Ŷ �ϼ�
//		m_pPacket = ClientServer::GetInstance().TakePacket();
//		memcpy(m_pPacket, m_packetBuffer + m_popPos, m_pUPacket->header.length);
//		// ��Ŷ ó��(+�ٹٲ�)
//		if (auto length = strlen(m_pPacket->msg);  m_pPacket->msg[length - 1] != '\n')
//			m_pPacket->msg[length] = '\n';
//		ClientServer::Get().m_PacketPool.push(m_pPacket);
//		// ���� ���� ����
//		m_popPos += m_pUPacket->header.length;				// ���� ������
//		m_pushPos -= m_pUPacket->header.length;				// ��Ŷ ���̸�ŭ ����
//		m_pUPacket = (UPACKET*)&m_packetBuffer[m_popPos];	// ���� ��Ŷ ����
//		if (m_pushPos == 0 || m_pUPacket->header.length == 0)
//			break;
//	}
//	// ���� ���� �����
//	memcpy(m_packetBuffer, m_packetBuffer + m_popPos, MaxBufferSize - m_popPos);
//	ZeroMemory(m_packetBuffer + MaxBufferSize - m_popPos, m_popPos);
//	m_popPos = 0;
//}
//
//bool DxClientRecv::Run()
//{
//	{
//		std::lock_guard<std::mutex> guarder(m_Mutex);
//		cout << __FUNCTION__ << " -> ���� �Ǿ����ϴ�.\n";
//	}
//	// Ŭ�� ���� ����
//	ClientServer::Get().m_ClientSock = m_ListenSocket;
//
//	UPACKET sendPacket = { 0, };
//	std::this_thread::sleep_for(std::chrono::seconds(10));
//	sendPacket.msg[0] = 'A';
//	sendPacket.header.length = (WORD)(strlen(sendPacket.msg) + PACKET_HEADER_SIZE);
//	sendPacket.header.type = EPacket::CHAT_NAME_ACK;
//	send(m_ListenSocket, (char*)&sendPacket, sendPacket.header.length, 0);
//
//	while (m_isCreated)
//	{
//		// ����->���ú�
//		int receiveByte = 0;
//		receiveByte = recv(m_ListenSocket, (char*)&m_packetBuffer + m_pushPos, MaxBufferSize / 2, 0);
//		if (ByteErrorCheck(receiveByte))
//		{
//			Server::ErrorMessage(""s + __FUNCTION__ + " -> DxClient Recv Error!");
//			m_isCreated = false;
//			break;
//		}
//		else
//		{
//			// ��Ŷ ó��
//			std::lock_guard<std::mutex> guarder(m_Mutex);
//			PushData(receiveByte);
//		}
//		std::this_thread::yield();
//	}
//	shutdown(m_ListenSocket, SD_SEND);
//	closesocket(m_ListenSocket);
//	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
//	return false;
//}
//
//bool DxClientSend::Start(const USHORT& portNum, const string_view& address, const bool& isClient)
//{
//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
//	if (ClientServer::Get().m_ClientSock == 0)
//	{
//		Server::ErrorMessage(""s + __FUNCTION__ + " -> DxClient Socket 0!");
//		return false;
//	}
//	m_ListenSocket = ClientServer::Get().m_ClientSock;
//	// ������ ����
//	CreateThread();
//	return true;
//	portNum; address; isClient;
//}
//
//// ��Ŷ ��Ʈ��ȭ?
//bool DxClientSend::Run()
//{
//	{
//		std::lock_guard<std::mutex> guarder(m_Mutex);
//		cout << __FUNCTION__ << " -> ���� �Ǿ����ϴ�.\n";
//	}
//
//	UPACKET* pPacket = nullptr;
//	while (m_isCreated)
//	{
//		// ����->����
//		{
//			std::lock_guard<std::mutex> guarder(m_Mutex);
//			while (!ClientServer::Get().m_SendPacketPool.empty())
//			{
//				pPacket = ClientServer::Get().m_SendPacketPool.front();
//				int sendByte = send(m_ListenSocket, (char*)pPacket, pPacket->header.length, 0);
//				if (ByteErrorCheck(sendByte))
//				{
//					Server::ErrorMessage(""s + __FUNCTION__ + " -> Server DxClient Send Error!");
//					break;
//				}
//				// + ���� ������ ������ ������, ������ �ٷ� ��ŶǮ��
//				ClientServer::Get().m_SendPacketPool.pop();
//				ClientServer::Get().m_DisablePacketPool.push(pPacket);
//			}
//		}
//		std::this_thread::yield();
//	}
//	shutdown(m_ListenSocket, SD_SEND);
//	closesocket(m_ListenSocket);
//	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
//	return false;
//}