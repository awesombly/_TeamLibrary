#include "PacketManager.h"
#include "ObjectManager.h"
#include "PlayerController.h"
#include "Collider.h"


void PacketManager::SendPacket(char* data, const USHORT& packeyType, const PP::PPSendMode& sendMode) noexcept
{
	if (pSender == nullptr)
	{
		InterceptPacket((PP::PPPacketType)packeyType, data);
		return;
	}
	
	//pSender->BroadcastWString(L"Hello, Server!");	//서버에게 문자열 전송(함수명은 바꿀 예정)
	static PP::PPPacketForProcess packet;

	memcpy(packet.m_Packet.m_Payload, (void*)data, sizeof(data));

	packet.m_socketSession = 0;
	packet.m_SendMode = sendMode;
	packet.m_Packet.m_Header.m_type = (PP::PPPacketType)packeyType;
	packet.m_Packet.m_Header.m_len = (USHORT)(strlen(packet.m_Packet.m_Payload) + PACKET_HEADER_SIZE);

	pSender->Broadcast(packet);
}



void PacketManager::InterceptPacket(const PP::PPPacketType& sendMode, const char* data) noexcept
{
	
	static Packet_KeyValue		p_KeyValue;
	static Packet_Bool			p_Bool;
	static Packet_Vector3		p_Vector3;
	static Packet_Vector2		p_Vector2;
	static Packet_Quaternion	p_Quaternion;
	static Packet_Vec3Quat		p_Vec3Quat;
	static Packet_AnimState		p_AnimState;

	memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
	if (ObjectManager::KeyObjects[p_KeyValue.KeyValue] == nullptr) return;

	switch (sendMode)
	{
	case PACKET_SetPositionRotation:
		{
			memcpy(&p_Vec3Quat, data, sizeof(Packet_Vec3Quat));
			ObjectManager::KeyObjects[p_Vec3Quat.KeyValue]->SetPosition(p_Vec3Quat.Vec3);
			ObjectManager::KeyObjects[p_Vec3Quat.KeyValue]->SetRotation(p_Vec3Quat.Quat);
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
	 case PACKET_SetAnimation:
	 {
		 memcpy(&p_AnimState, data, sizeof(Packet_Vector3));
		 ((PlayerController*)ObjectManager::KeyObjects[p_AnimState.KeyValue])->SetAnimation((PlayerController::EAction)(p_AnimState.EAnimState));
	 }	break;
	 case PACKET_SetDirectionForce:
	 {
		 memcpy(&p_Vector3, data, sizeof(Packet_Vector3));
		 ((PlayerController*)ObjectManager::KeyObjects[p_Vector3.KeyValue])->m_pCollider->SetDirectionForce(p_Vector3.Vec3);
	 }	break;
	 case PACKET_SetMoveStop:
	 {
		 //memcpy(&p_KeyValue, data, sizeof(Packet_KeyValue));
		 ((PlayerController*)ObjectManager::KeyObjects[p_KeyValue.KeyValue])->m_pCollider->isMoving(false);
	 }	break;
		
	}
}