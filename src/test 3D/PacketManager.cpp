#include "PacketManager.h"
#include "ObjectManager.h"
#include "PlayerController.h"
#include "Collider.h"


void PacketManager::SendPacket(char* data, const USHORT& size, const USHORT& packeyType/*, const PP::PPSendMode& sendMode*/) noexcept
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
	//pSender->BroadcastExcept()
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

	memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
	if (ObjectManager::KeyObjects[p_KeyValue.KeyValue] == nullptr) return;

	switch (sendMode)
	{
	 case PACKET_SetTransform:
	 {
	 	memcpy(&p_Transform, data, sizeof(Packet_Transform));
	 	ObjectManager::KeyObjects[p_Transform.KeyValue]->SetPosition(p_Transform.Position);
	 	ObjectManager::KeyObjects[p_Transform.KeyValue]->SetRotation(p_Transform.Rotation);
		ObjectManager::KeyObjects[p_Transform.KeyValue]->SetScale(p_Transform.Scale);
	 }	break;
	 //case PACKET_SetPosition:
	 //{
	 //	memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
	 //	ObjectManager::KeyObjects[p_Vector3.KeyValue]->SetPosition(p_Vector3.Vec3);
	 //}	break;
	 //case PACKET_SetRotation:
	 //{
	//	 memcpy(&p_Quaternion, data, sizeof(Packet_Quaternion));
	//	 ObjectManager::KeyObjects[p_Quaternion.KeyValue]->SetRotation(p_Quaternion.Quat);
	 //}	break;
	 //case PACKET_SetScale:
	 //{
	//	 memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
	//	 ObjectManager::KeyObjects[p_Vector3.KeyValue]->SetScale(p_Vector3.Vec3);
	 //}	break;
	 //case PACKET_Translate:
	 //{
	 //	memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
	 //	ObjectManager::KeyObjects[p_Vector3.KeyValue]->Translate(p_Vector3.Vec3);
	 //}	break;
	 //case PACKET_Rotate:
	 //{
	//	 memcpy(&p_Quaternion, data, sizeof(Packet_Quaternion));
	//	 ObjectManager::KeyObjects[p_Quaternion.KeyValue]->Rotate(p_Quaternion.Quat);
	 //}	break;
	 //case PACKET_Scaling:
	 //{
	//	 memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
	//	 ObjectManager::KeyObjects[p_Vector3.KeyValue]->Scaling(p_Vector3.Vec3);
	 //}	break;
	 case PACKET_SetAnimTransform:
	 {
		 memcpy(&p_AnimTransform, data, sizeof(Packet_AnimTransform));
		 if (p_AnimTransform.ECharacter != PlayerController::ECharacter::EDummy)
		 {
			 PlayerController::SetAnim((AHeroObj*)ObjectManager::KeyObjects[p_AnimTransform.KeyValue], (PlayerController::ECharacter)p_AnimTransform.ECharacter, (PlayerController::EAction)p_AnimTransform.EAnimState);
		 }
		 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetPosition(p_AnimTransform.Position);
		 ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->SetRotation(p_AnimTransform.Rotation);
		 switch ((PlayerController::EAction)p_AnimTransform.EAnimState)
		 {
		  case PlayerController::EAction::Idle:
		  {
		 	 ((Collider*)ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->GetComponentList(EComponent::Collider)->front())->isMoving(false);
		  }	break;
		  case PlayerController::EAction::Jump:
		  {
		 	 ((Collider*)ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->GetComponentList(EComponent::Collider)->front())->SetForce(p_AnimTransform.Direction);
		  }	break;
		  case PlayerController::EAction::Dance1:
		  case PlayerController::EAction::Dance2:
		  case PlayerController::EAction::Dance3:
		  case PlayerController::EAction::Throw:
		  {} break;
		  default:
		  {
		 	((Collider*)ObjectManager::KeyObjects[p_AnimTransform.KeyValue]->GetComponentList(EComponent::Collider)->front())->SetDirectionForce(p_AnimTransform.Direction);
		  }	break;
		 }
	 }	break;
	//case PACKET_SetDirectionForce:
	//{
	//	 memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
	//	 //((PlayerController*)ObjectManager::KeyObjects[p_Vector3.KeyValue])->m_pCollider->SetDirectionForce(p_Vector3.Vec3);
	//}	break;
	//case PACKET_SetMoveStop:
	//{
	//	 //memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
	//	 //((PlayerController*)ObjectManager::KeyObjects[p_KeyValue.KeyValue])->m_pCollider->isMoving(false);
	//}	break;
	 default:
	 {
		 ErrorMessage("처리되지 않은 패킷 : "s + to_string(sendMode));
		 return;
	 }	break;
	}
}