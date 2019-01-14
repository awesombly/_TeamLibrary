 #include "PacketManager.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "PlayerController.h"
#include "Collider.h"




void PacketManager::SendPacket(const char* data, const USHORT& size, const USHORT& packeyType/*, const PP::PPSendMode& sendMode*/) noexcept
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

	memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
	if (sendMode			< 2000 &&
		p_KeyValue.KeyValue != (UINT)-1			 &&
		ObjectManager::KeyObjects.find(p_KeyValue.KeyValue) == ObjectManager::KeyObjects.end())
	{
		ErrorMessage("KeyObject is Null : " + to_string(p_KeyValue.KeyValue));
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
		 ObjectManager::KeyObjects[p_Vector3.KeyValue]->SetScale(p_Vector3.Vec3);
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
			 PlayerController::SetAnim((AHeroObj*)ObjectManager::KeyObjects[p_AnimTransform.KeyValue], (PlayerController::ECharacter)p_AnimTransform.ECharacter, (PlayerController::EAction)p_AnimTransform.EAnimState, p_AnimTransform.Direction);
		 }
		 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetPosition(p_AnimTransform.Position);
		 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetRotation(p_AnimTransform.Rotation);

		 if (ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->m_pPhysics != nullptr)
		 {
			 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetForce(p_AnimTransform.Force);

			 switch ((PlayerController::EAction)p_AnimTransform.EAnimState)
			 {
			 case PlayerController::EAction::Fly:
				 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetGravityScale(-0.35f);
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
		 memcpy(((char*)&p_TakeObject + PS_TakeObject), ((char*)data + PS_TakeObject), p_TakeObject.MsgSize);

		 auto pObject = ObjectManager::Get().TakeObject(p_TakeObject.ObjectName);
		 pObject->SetKeyValue(p_TakeObject.KeyValue);
		 pObject->SetPosition(p_TakeObject.Position);
		 pObject->SetRotation(p_TakeObject.Rotation);
		 pObject->SetScale(p_TakeObject.Scale);
		 pObject->SetHP(1.0f);
	 }	break;
	 case PACKET_PossessPlayer:
	 {
		 memcpy(&p_PossessPlayer, data, sizeof(Packet_PossessPlayer));
		 PlayerController::Get().Possess(ObjectManager::KeyObjects[p_PossessPlayer.KeyValue]);
		 ((JPanel*)PlayerController::Get().m_pRespawnEffect)->EffectPlay();
	 }	break;
	 case PACKET_PlayerDead:
	 {
		 memcpy(&p_PlayerDead, data, sizeof(Packet_PlayerDead));
		 
		 if (PlayerController::Get().GetParent() == ObjectManager::KeyObjects[p_PlayerDead.KeyValue])
		 {
			 PlayerController::Get().DeadEvent();
		 }
		 auto pCollider = new Collider(140.0f);
		 auto pEffect = new GameObject(L"DeadEffect", { pCollider, ObjectManager::Get().TakeComponent(L"Boom3") });
		 pCollider->CollisionEvent = [](Collider* pMe, Collider* pYou) {
			 if (pYou != nullptr && pYou->m_eTag == ETag::Collider)
			 {
				 pYou->SetForce((Normalize(pYou->GetCenter() - pMe->GetCenter()) + Vector3::Up) * 300.0f);
				 pMe->AddIgnoreList(pYou);
			 }
		 };
		 pCollider->m_eTag = ETag::Dummy;
		 pCollider->SetGravityScale(0.0f);
		 pCollider->usePhysics(false);
		 //pCollider->AddIgnoreList(pA);
		 //pCollider->AddIgnoreList(pB);
		 
		 pEffect->SetPosition(ObjectManager::KeyObjects[p_PlayerDead.KeyValue]->GetPosition() + Vector3::Up * 40.0f);
		 ObjectManager::Get().PushObject(pEffect);
		 ObjectManager::Get().DisableObject(ObjectManager::KeyObjects[p_PlayerDead.KeyValue]);
	 }	break;
	 case PACKET_PlaySound:
	 {
		 ZeroMemory(&p_SoundData, sizeof(p_SoundData));
		 memcpy(&p_SoundData, data, PS_PlaySound);
		 memcpy(((char*)&p_SoundData + PS_PlaySound), ((char*)data + PS_PlaySound), p_SoundData.NameSize);
		 SoundManager::Get().PlayQueue(p_SoundData.SoundName, p_SoundData.Position, p_SoundData.MaxDistance);
	 }	break;
	 case PACKET_ChatMessage:
	 {
		 ZeroMemory(&p_ChatMessage, sizeof(p_ChatMessage));
		 memcpy(&p_ChatMessage, data, PS_ChatMessage);
		 memcpy(((char*)&p_ChatMessage + PS_ChatMessage), ((char*)data + PS_ChatMessage), p_ChatMessage.MsgSize);
		 m_pChatList->push_string(p_ChatMessage.Message);
		 *m_pChatList->m_fValue = 0.0f;
	 }	break;
	 case PS_SyncObjects:
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
	p_SoundData.NameSize = (char)soundName.size();

	SendPacket((char*)&p_SoundData, (USHORT)(PS_PlaySound + soundName.size()), PACKET_PlaySound);
}