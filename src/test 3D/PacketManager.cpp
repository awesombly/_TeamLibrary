 #include "PacketManager.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "PlayerController.h"
//#include "MainClass.cpp"
#include "Collider.h"

#include "../network/PPRecvPacketPoolServer.h"					//클라이언트 클래스 정의.


void PacketManager::SendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept
{
	if (isHost)
	{
		InterceptPacket((PP::PPPacketType)packeyType, data);
		//return;
	}
	
	///pSender->BroadcastWString(L"Hello, Server!");	//서버에게 문자열 전송(함수명은 바꿀 예정)
	static PP::PPPacketForProcess packet;

	memcpy(packet.m_Packet.m_Payload, (void*)data, size);
	
	packet.m_socketSession = 0;
	//packet.m_SendMode = sendMode;
	packet.m_Packet.m_Header.m_type = (PP::PPPacketType)packeyType;
	packet.m_Packet.m_Header.m_len = (USHORT)(size + PACKET_HEADER_SIZE);

	pSender->Broadcast(packet);
	//pSender->BroadcastExcept();
}

void PacketManager::ReqSendPacket(const char* data, const USHORT& size, const USHORT& packeyType) noexcept
{
	if (isHost)
	{
		static PP::PPPacketForProcess packet;

		memcpy(packet.m_Packet.m_Payload, (void*)data, size);

		packet.m_socketSession = 0;
		//packet.m_SendMode = sendMode;
		packet.m_Packet.m_Header.m_type = (PP::PPPacketType)packeyType;
		packet.m_Packet.m_Header.m_len = (USHORT)(size + PACKET_HEADER_SIZE);

		PP::PPRecvPacketPoolServer::GetInstance().push_back(packet);
		PacketFunc();
	}
	else
	{
		SendPacket(data, size, packeyType);
	}
}



void PacketManager::InterceptPacket(const PP::PPPacketType& sendMode, const char* data) noexcept
{
	static Packet_KeyValue		p_KeyValue;
	static Packet_Bool			p_Bool;
	static Packet_Vector3		p_Vector3;
	static Packet_Vector2		p_Vector2;
	static Packet_Quaternion	p_Quaternion;
	static Packet_Vec3Quat		p_Vec3Quat;
	//static Packet_AnimState		p_AnimState;
	static Packet_AnimTransform p_AnimTransform;
	static Packet_Transform		p_Transform;
	static Packet_PossessPlayer	p_PossessPlayer;
	static Packet_SoundData		p_SoundData;
	static Packet_ChatMessage	p_ChatMessage;
	static Packet_TakeObject	p_TakeObject;
	static Packet_SyncObjects	p_SyncObjects;
	static Packet_PlayerDead	p_PlayerDead;
	static Packet_SetHP			p_SetHP;

	memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
	if (sendMode			< 2000 &&
		p_KeyValue.KeyValue != (UINT)-1			 &&
		ObjectManager::KeyObjects.find(p_KeyValue.KeyValue) == ObjectManager::KeyObjects.end())
	{
		ErrorMessage("KeyObject is Null(" + to_string(sendMode) + ") : " + to_string(p_KeyValue.KeyValue));
		return;
	}

	switch (sendMode)
	{
	 case PACKET_SetTransform:
	 {
	 	memcpy(&p_Transform, data, sizeof(Packet_Transform));
	 	ObjectManager::KeyObjects[p_Transform.KeyValue]->SetPosition(p_Transform.Position);
	 	ObjectManager::KeyObjects[p_Transform.KeyValue]->SetRotation(p_Transform.Rotation);
		ObjectManager::KeyObjects[p_Transform.KeyValue]->SetScale(p_Transform.Scale);
	 }	break;
	 case PACKET_SetPosition:
	 {
	 	memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
	 	ObjectManager::KeyObjects[p_Vector3.KeyValue]->SetPosition(p_Vector3.Vec3);
	 }	break;
	 case PACKET_SetRotation:
	 {
		memcpy(&p_Quaternion, data, sizeof(Packet_Quaternion));
		ObjectManager::KeyObjects[p_Quaternion.KeyValue]->SetRotation(p_Quaternion.Quat);
	 }	break;
	 case PACKET_SetScale:
	 {
		 memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
		 auto preScale = ObjectManager::KeyObjects[p_Vector3.KeyValue]->GetScaleAverage();
		 ObjectManager::KeyObjects[p_Vector3.KeyValue]->SetScale(p_Vector3.Vec3);
		 auto pCollider = ObjectManager::KeyObjects[p_Vector3.KeyValue]->GetComponentList(EComponent::Collider);
		 if (pCollider != nullptr)
		 {
			 for (auto& iter : *pCollider)
			 {
				 ((Collider*)iter)->m_pivot *= (p_Vector3.Vec3.x / preScale);
			 }
		 }
	 }	break;
	 case PACKET_Translate:
	 {
	 	memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
	 	ObjectManager::KeyObjects[p_Vector3.KeyValue]->Translate(p_Vector3.Vec3);
	 }	break;
	 case PACKET_Rotate:
	 {
		 memcpy(&p_Quaternion, data, sizeof(Packet_Quaternion));
		 ObjectManager::KeyObjects[p_Quaternion.KeyValue]->Rotate(p_Quaternion.Quat);
	 }	break;
	 case PACKET_Scaling:
	 {
		memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
		ObjectManager::KeyObjects[p_Vector3.KeyValue]->Scaling(p_Vector3.Vec3);
	 }	break;
	 case PACKET_SetAnimTransform:
	 {
		 memcpy(&p_AnimTransform, data, sizeof(Packet_AnimTransform));
		 if (p_AnimTransform.ECharacter != PlayerController::ECharacter::EDummy)
		 {
			 PlayerController::SetAnim((AHeroObj*)ObjectManager::KeyObjects[p_AnimTransform.KeyValue], p_AnimTransform.UserSocket, (PlayerController::ECharacter)p_AnimTransform.ECharacter, (PlayerController::EAction)p_AnimTransform.EAnimState, p_AnimTransform.Direction);
		 }
		 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetPosition(p_AnimTransform.Position);
		 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetRotation(p_AnimTransform.Rotation);

		 if (ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->m_pPhysics != nullptr)
		 {
			 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetForce(p_AnimTransform.Force);

			 switch ((PlayerController::EAction)p_AnimTransform.EAnimState)
			 {
			 case PlayerController::EAction::Fly:
				 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetGravityScale(-0.4f);
			 case PlayerController::EAction::Jump:
			 {
				 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->isGround(false);
			 }	break;
			 case PlayerController::EAction::FlyEnd:
			 {
				 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetGravityScale(1.0f);
			 } break;
			 case PlayerController::EAction::Idle:
			 {
				 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->isMoving(false);
			 }	break;
			 case PlayerController::EAction::Forward:
			 case PlayerController::EAction::ForwardLeft:
			 case PlayerController::EAction::ForwardRight:
			 case PlayerController::EAction::Backward:
			 case PlayerController::EAction::BackwardLeft:
			 case PlayerController::EAction::BackwardRight:
			 case PlayerController::EAction::Left:
			 case PlayerController::EAction::Right:
			 {
				 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetDirectionForce(p_AnimTransform.Direction);
			 }	break;
			 }
		 }
	 }	break;
	 case PACKET_TakeObject:
	 {
		 ZeroMemory(&p_TakeObject, sizeof(Packet_TakeObject));
		 memcpy(&p_TakeObject, data, PS_TakeObject);
		 memcpy(((char*)&p_TakeObject + PS_TakeObject), ((char*)data + PS_TakeObject), p_TakeObject.DataSize);

		 auto pObject = ObjectManager::Get().TakeObject(p_TakeObject.ObjectName);
		 pObject->m_pPhysics->UserSocket = p_TakeObject.UserSocket;
		 pObject->SetKeyValue(p_TakeObject.KeyValue);
		 pObject->SetPosition(p_TakeObject.Position);
		 pObject->SetRotation(p_TakeObject.Rotation);
		 pObject->SetScale(p_TakeObject.Scale);
		 pObject->SetHP(1.0f);
	 }	break;
	 case PACKET_SendUserInfo:
	 {
		 memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
		 // 유저 목록에 있으면 갱신
		 for (int i = 0; i < UserList.size(); ++i)
		 {
			 if(pUserPanel[i] != nullptr)
				pUserPanel[i]->m_bRender = true;
			 if (UserList[i]->UserSocket == p_KeyValue.KeyValue)
			 {
				 memcpy((char*)UserList[i], data, PS_UserInfo);
				 memcpy(((char*)UserList[i] + PS_UserInfo), ((char*)data + PS_UserInfo), UserList[i]->DataSize);
				 return;
			 }
		 }
		 // 유저 목록에 없는데 자신이면 추가
		 if (pMyInfo->UserSocket == p_KeyValue.KeyValue)
		 {
			 memcpy((char*)pMyInfo, data, PS_UserInfo);
			 memcpy(((char*)pMyInfo + PS_UserInfo), ((char*)data + PS_UserInfo), pMyInfo->DataSize);
			 UserList.push_back(pMyInfo);
			 return;
		 }
		 // 새 유저면 생성
		 auto pUser = new UserInfo();
		 memcpy((char*)pUser, data, PS_UserInfo);
		 memcpy(((char*)pUser + PS_UserInfo), ((char*)data + PS_UserInfo), pUser->DataSize);
		 UserList.push_back(pUser);
	 }	break;
	 case PACKET_PossessPlayer:
	 {
		 memcpy(&p_PossessPlayer, data, sizeof(Packet_PossessPlayer));
		 PlayerController::Get().Possess(ObjectManager::KeyObjects[p_PossessPlayer.KeyValue]);
		 ((JPanel*)PlayerController::Get().m_pRespawnEffect)->EffectPlay();

		 pMyInfo->isDead = false;
		 PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
	 }	break;
	 case PACKET_PlayerDead:
	 {
		 memcpy(&p_PlayerDead, data, sizeof(Packet_PlayerDead));
		 
		 if (PlayerController::Get().GetParent() == ObjectManager::KeyObjects[p_PlayerDead.KeyValue])
		 {
			 ++pMyInfo->DeathCount;
			 pMyInfo->isDead = true;
			 PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			 PlayerController::Get().DeadEvent();

			 for (auto& iter : UserList)
			 {
				 if (iter->UserSocket == p_PlayerDead.KillUser)
				 {
					 ++(iter->KillCount);
					 iter->Score += 500;
					 PacketManager::Get().SendPacket((char*)iter, (USHORT)(PS_UserInfo + iter->DataSize), PACKET_SendUserInfo);
					 break;
				 }
			 }
		 }
		 auto pCollider = new Collider(140.0f);
		 auto pEffect = ObjectManager::Get().TakeObject(L"Boom3");
		 pEffect->AddComponent(pCollider);
		 pCollider->CollisionEvent = [](Collider* pMe, Collider* pYou) {
			 if (pYou != nullptr && pYou->m_eTag == ETag::Collider)
			 {
				 pYou->SetForce((Normalize(pYou->GetCenter() - pMe->GetCenter()) + Vector3::Up * 1.5f) * 250.0f);
				 pMe->AddIgnoreList(pYou);
			 }
		 };
		 pCollider->m_eTag = ETag::Dummy;
		 pCollider->SetGravityScale(0.0f);
		 pCollider->usePhysics(false);
		 //pCollider->AddIgnoreList(pA);
		 //pCollider->AddIgnoreList(pB);
		 
		 pEffect->SetPosition(ObjectManager::KeyObjects[p_PlayerDead.KeyValue]->GetPosition() + Vector3::Up * 40.0f);
		 //ObjectManager::Get().PushObject(pEffect);
		 ObjectManager::Get().DisableObject(ObjectManager::KeyObjects[p_PlayerDead.KeyValue]);

		 // 전광판 입력
		 if (p_PlayerDead.DeadUser == (UINT)-1)
			 return;
		 wstring killer, dead;
		 for (auto& iter : UserList)
		 {
			 if (iter->UserSocket == p_PlayerDead.DeadUser)
			 {
				 dead = iter->UserID;
			 }
			 if (iter->UserSocket == p_PlayerDead.KillUser)
			 {
				 killer = iter->UserID;
			 }
		 }
		 if (killer.empty())
			 killer = dead;
		 pKillDisplay->push_string(killer + L" -> " + dead);
	 }	break;
	 case PACKET_PlaySound:
	 {
		 ZeroMemory(&p_SoundData, sizeof(p_SoundData));
		 memcpy(&p_SoundData, data, PS_PlaySound);
		 memcpy(((char*)&p_SoundData + PS_PlaySound), ((char*)data + PS_PlaySound), p_SoundData.DataSize);
		 SoundManager::Get().PlayQueue(p_SoundData.SoundName, p_SoundData.Position, p_SoundData.MaxDistance);
	 }	break;
	 case PACKET_ChatMessage:
	 {
		 ZeroMemory(&p_ChatMessage, sizeof(p_ChatMessage));
		 memcpy(&p_ChatMessage, data, PS_ChatMessage);
		 memcpy(((char*)&p_ChatMessage + PS_ChatMessage), ((char*)data + PS_ChatMessage), p_ChatMessage.DataSize);
		 for (auto& iter : UserList)
		 {
			 if (iter->UserSocket == p_ChatMessage.UserSocket)
			 {
				 if (iter == pMyInfo)
				 {
					 pMyInfo->Score += 5;
					 PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				 }
				 pChatList->push_string(iter->UserID + L" : "s + p_ChatMessage.Message);
				*pChatList->m_fValue = 0.0f;
			 }
		 }
	 }	break;
	 case PACKET_SyncObjects:
	 {
		 memcpy(&p_SyncObjects, data, PS_SyncObjects);
		 memcpy(((char*)&p_SyncObjects + PS_SyncObjects), ((char*)data + PS_SyncObjects), p_SyncObjects.Count * sizeof(Packet_SyncTransform));
		 
		 for (int i = 0; i < p_SyncObjects.Count; ++i)
		 {
			 ObjectManager::KeyObjects[p_SyncObjects.Data[i].KeyValue]->SetPosition(p_SyncObjects.Data[i].Position);
			 ObjectManager::KeyObjects[p_SyncObjects.Data[i].KeyValue]->SetRotation(p_SyncObjects.Data[i].Rotation);
			 if(ObjectManager::KeyObjects[p_SyncObjects.Data[i].KeyValue]->m_pPhysics != nullptr)
				ObjectManager::KeyObjects[p_SyncObjects.Data[i].KeyValue]->SetForce(p_SyncObjects.Data[i].Force);
		 }
	 }	break;
	 case PACKET_SetHP:
	 {
		 memcpy(&p_SetHP, data, sizeof(Packet_SetHP));
		 ObjectManager::KeyObjects[p_SetHP.KeyValue]->SetHP(p_SetHP.HP);
	 }	break;
	 default:
	 {
		 ErrorMessage("처리되지 않은 패킷 : "s + to_string(sendMode));
		 return;
	 }	break;
	}
}


void PacketManager::SendPlaySound(const string_view& soundName, const D3DXVECTOR3& position, const float& maxDistance) noexcept
{
	Packet_SoundData p_SoundData;
	//p_SoundData.KeyValue = (UINT)-1;
	p_SoundData.Position = position;
	p_SoundData.MaxDistance = maxDistance;
	memcpy(p_SoundData.SoundName, soundName.data(), soundName.size());
	p_SoundData.DataSize = (char)soundName.size();

	SendPacket((char*)&p_SoundData, (USHORT)(PS_PlaySound + soundName.size()), PACKET_PlaySound);
}