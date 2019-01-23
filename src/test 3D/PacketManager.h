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
	vector<UserInfo*>	UserList;
	UserInfo*			pMyInfo;

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



namespace MyEvent {
	void ForceWave(Collider* pA, Collider* pB);
	void DaggerHit(Collider* pA, Collider* pB);
	void MeleeHit(Collider* pA, Collider* pB);
	void ZombieHit(Collider* pA, Collider* pB);
	void OneShots(Collider* pA, Collider* pB);
	void GiantItem(Collider* pA, Collider* pB);

	// �ε���
	void BulletHit(Collider* pA, Collider* pB);
	void EnemyHit(Collider* pA, Collider* pB);
}