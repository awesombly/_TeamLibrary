#include "GameMap.h"
#include "HeightMap.h"
#include "ObjectManager.h"
#include "PlayerController.h"
#include "EventManager.h"
//#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include "RPlane.h"


bool GameMap::Init() noexcept
{
	m_pHeightMap = new HeightMap(L"HeightMap", EComponent::Renderer, L"gamemap.png");
	auto pObject = new GameObject(L"HeightMap", m_pHeightMap, EObjType::Map);
	m_pHeightMap->CreateHeightMap(DxManager::GetDContext(), L"HeightMap/heightmap.png", 13, 1.0f, 1.0f);
	//m_pHeightMap->SetShadowRate(0.0f);
	m_pHeightMap->SetEnviromentMap(L"CubeMap/A_nightsky.dds", EEnviType::Refraction);
	m_pHeightMap->SetColor(DxManager::GetDContext(), 0.6f, 0.6f, 0.6f);
	//pObject->Translate(Vector3::Down * 100.0f);
	//pObject->SetScale(1.0f, 0.1f, 1.0f);
	ObjectManager::Get().PushObject(pObject, false);

	//////////////////////////////////////////////////////////////////////////////////////
	auto pCollider = new Collider(9.2f);
	m_fountain.AddComponent(pCollider);
	m_fountain.SetPlayerCharacter(L"MAP_Fountain", 0.0f, -50.0f, 0.0f);
	m_fountain.SetScale(D3DXVECTOR3(8.0f, 8.0f, 8.0f));
	m_fountain.m_objType = EObjType::AObject;
	m_fountain.m_myName = L"HolyWater";
	m_fountain.m_keyValue = ++ObjectManager::Get().KeyCount;
	ObjectManager::KeyObjects[m_fountain.m_keyValue] = &m_fountain;
	pCollider->m_eTag = ETag::Ally;
	pCollider->m_pivot = Vector3::Up * 50.0f;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	pCollider->SetHP(400.0f);
	pCollider->m_pPhysics->DeadEvent = DyingEvent::CenterDead;
	ObjectManager::Get().PushObject(&m_fountain, false);
	PlayerController::Get().m_pHome = &m_fountain;
	PlayerController::Get().m_HomePos = m_fountain.GetPosition();

	m_church.SetPlayerCharacter(L"MAP_Church", -450.0f, 0, 450.0f);	//280
	m_church.SetScale(D3DXVECTOR3(4.0f, 4.0f, 4.0f));
	m_church.SetRotationY(67.0f);
	m_church.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_church, false);

	m_blacksmith.SetPlayerCharacter(L"MAP_Blacksmith", 400.0f, 0.0f, 400.0f);	//280
	m_blacksmith.SetScale(D3DXVECTOR3(0.6f, 0.6f, 0.3f));
	m_blacksmith.SetRotationY(70.0f);
	m_blacksmith.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_blacksmith, false);

	m_windmill.SetPlayerCharacter(L"MAP_WINDMILL", 450.0f, 0.0f, -450.0f);	//280
	m_windmill.SetScale(D3DXVECTOR3(1.8f, 1.8f, 1.8f));
	m_windmill.SetRotationY(PI*0.7f);
	m_windmill.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_windmill, false);

	m_towerRound.SetPlayerCharacter(L"MAP_TowerRound", -450.0f, 0.0f, -450);	//280
	m_towerRound.SetScale(D3DXVECTOR3(1.4f, 1.6f, 1.4f));
	m_towerRound.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerRound, false);


#pragma region MyHouse


	m_house00.SetPlayerCharacter(L"MAP_HOUSE_C", -680.0f, 0.0f, 680.0f);	//280
	m_house00.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house00, false);
	m_house01.SetPlayerCharacter(L"MAP_HOUSE_B", -400.0f, 0.0f, 680.0f);	//280
	m_house01.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house01, false);
	m_house02.SetPlayerCharacter(L"MAP_HOUSE_D", -680.0f, 0.0f, 400.0f);	//280
	m_house02.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house02, false);


	m_house10.SetPlayerCharacter(L"MAP_HOUSE_C", 680.0f, 0.0f, 680.0f);	//280
	m_house10.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house10.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house10, false);
	m_house11.SetPlayerCharacter(L"MAP_HOUSE_B", 400.0f, 0.0f, 680.0f);	//280
	m_house11.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house11.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house11, false);
	m_house12.SetPlayerCharacter(L"MAP_HOUSE_D", 680.0f, 0.0f, 400.0f);	//280
	m_house12.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house12.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house12, false);

	m_house20.SetPlayerCharacter(L"MAP_HOUSE_C", 680.0f, 0.0f, -680.0f);	//280
	m_house20.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house20.SetRotationY(-PI);
	m_house20.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house20, false);
	m_house21.SetPlayerCharacter(L"MAP_HOUSE_B", 400, 0.0f, -680);	//280
	m_house21.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house21.SetRotationY(-PI);
	m_house21.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house21, false);
	m_house22.SetPlayerCharacter(L"MAP_HOUSE_D", 680, 0.0f, -400);	//280
	m_house22.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house22.SetRotationY(-PI);
	m_house22.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house22, false);

	m_house30.SetPlayerCharacter(L"MAP_HOUSE_C", -680, 0.0f, -680);	//280
	m_house30.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house30.SetRotationY(-PI);
	m_house30.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house30, false);
	m_house31.SetPlayerCharacter(L"MAP_HOUSE_B", -400, 0.0f, -680);	//280
	m_house31.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house31.SetRotationY(-PI);
	m_house31.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house31, false);
	m_house32.SetPlayerCharacter(L"MAP_HOUSE_D", -680, 0.0f, -400);	//280
	m_house32.SetScale(D3DXVECTOR3(130.0f, 130.0f, 130.0f));
	m_house32.SetRotationY(-PI);
	m_house32.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house32, false);





#pragma endregion

#pragma region MyTower

	//╩С
	m_tower[0].SetPlayerCharacter(L"MAP_Tower", 250.0f, 0.0f, 900.0f);	//280;
	m_tower[0].SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));;
	m_tower[0].m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower[0], false);

	m_tower[1].SetPlayerCharacter(L"MAP_Tower", -250, 0.0f, 900.0f);	//280
	m_tower[1].SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));;
	m_tower[1].m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower[1], false);


	//©Л
	m_tower[2].SetPlayerCharacter(L"MAP_Tower", 900.0f, 0.0f, 250);	//280;
	m_tower[2].SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_tower[2].SetRotationY(PI*0.5f);
	m_tower[2].m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower[2], false);

	m_tower[3].SetPlayerCharacter(L"MAP_Tower", 900.0f, 0.0f, -250);	//280;
	m_tower[3].SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_tower[3].SetRotationY(PI*0.5f);
	m_tower[3].m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower[3], false);

	//го
	m_tower[4].SetPlayerCharacter(L"MAP_Tower", 250, 0.0f, -900.0f);	//280;
	m_tower[4].SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_tower[4].m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower[4], false);

	m_tower[5].SetPlayerCharacter(L"MAP_Tower", -250, 0.0f, -900.0f);	//280;
	m_tower[5].SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_tower[5].m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower[5], false);

	//аб
	m_tower[6].SetPlayerCharacter(L"MAP_Tower", -900.0f, 0.0f, 250);	//280;
	m_tower[6].SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));;
	m_tower[6].SetRotationY(PI*0.5f);
	m_tower[6].m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower[6], false);

	m_tower[7].SetPlayerCharacter(L"MAP_Tower", -900.0f, 0.0f, -250);	//280;
	m_tower[7].SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));;
	m_tower[7].SetRotationY(PI*0.5f);
	m_tower[7].m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower[7], false);


	m_towerTR.SetPlayerCharacter(L"MAP_Tower", 900.0f, 0.0f, 900.0f);
	m_towerTR.SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));;
	m_towerTR.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerTR, false);

	m_towerTL.SetPlayerCharacter(L"MAP_Tower", -900.0f, 0.0f, 900.0f);
	m_towerTL.SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_towerTL.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerTL, false);

	m_towerBR.SetPlayerCharacter(L"MAP_Tower", 900.0f, 0.0f, -900.0f);
	m_towerBR.SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));;
	m_towerBR.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerBR, false);

	m_towerBL.SetPlayerCharacter(L"MAP_Tower", -900.0f, 0.0f, -900.0f);
	m_towerBL.SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_towerBL.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerBL, false);

#pragma endregion

#pragma region MyWall
	//╩С
	m_wall00.SetPlayerCharacter(L"MAP_WALL", 402.0f, 0.0f, 900.0f);
	m_wall00.SetRotationY(PI*-0.5f);
	m_wall00.SetScaleZ(2.2f);
	m_wall00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall00, false);

	m_wall01.SetPlayerCharacter(L"MAP_WALL", 575.0f, 0.0f, 900.0f);
	m_wall01.SetRotationY(PI*-0.5f);
	m_wall01.SetScaleZ(2.2f);
	m_wall01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall01, false);

	m_wall02.SetPlayerCharacter(L"MAP_WALL", 748, 0.0f, 900.0f);
	m_wall02.SetRotationY(PI*-0.5f);
	m_wall02.SetScaleZ(2.2f);
	m_wall02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall02, false);

	m_wall03.SetPlayerCharacter(L"MAP_WALL", -402.0f, 0.0f, 900.0f);
	m_wall03.SetRotationY(PI*-0.5f);
	m_wall03.SetScaleZ(2.2f);
	m_wall03.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall03, false);

	m_wall04.SetPlayerCharacter(L"MAP_WALL", -575.0f, 0.0f, 900.0f);
	m_wall04.SetRotationY(PI*-0.5f);
	m_wall04.SetScaleZ(2.2f);
	m_wall04.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall04, false);

	m_wall05.SetPlayerCharacter(L"MAP_WALL", -748, 0.0f, 900.0f);
	m_wall05.SetRotationY(PI*-0.5f);
	m_wall05.SetScaleZ(2.2f);
	m_wall05.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall05, false);


	//©Л
	m_wall10.SetPlayerCharacter(L"MAP_WALL", 900.0f, 0.0f, 402.0f);
	m_wall10.SetScaleZ(2.2f);
	m_wall10.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall10, false);

	m_wall11.SetPlayerCharacter(L"MAP_WALL", 900.0f, 0.0f, 575.0f);
	m_wall11.SetScaleZ(2.2f);
	m_wall11.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall11, false);

	m_wall12.SetPlayerCharacter(L"MAP_WALL", 900.0f, 0.0f, 748);
	m_wall12.SetScaleZ(2.2f);
	m_wall12.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall12, false);

	m_wall13.SetPlayerCharacter(L"MAP_WALL", 900.0f, 0.0f, -402.0f);
	m_wall13.SetScaleZ(2.2f);
	m_wall13.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall13, false);

	m_wall14.SetPlayerCharacter(L"MAP_WALL", 900.0f, 0.0f, -575.0f);
	m_wall14.SetScaleZ(2.2f);
	m_wall14.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall14, false);

	m_wall15.SetPlayerCharacter(L"MAP_WALL", 900.0f, 0.0f, -748);
	m_wall15.SetScaleZ(2.2f);
	m_wall15.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall15, false);

	//го
	m_wall20.SetPlayerCharacter(L"MAP_WALL", 402.0f, 0.0f, -900.0f);
	m_wall20.SetRotationY(PI*0.5f);
	m_wall20.SetScaleZ(2.2f);
	m_wall20.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall20, false);

	m_wall21.SetPlayerCharacter(L"MAP_WALL", 575.0f, 0.0f, -900.0f);
	m_wall21.SetRotationY(PI*0.5f);
	m_wall21.SetScaleZ(2.2f);
	m_wall21.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall21, false);

	m_wall22.SetPlayerCharacter(L"MAP_WALL", 748, 0.0f, -900.0f);
	m_wall22.SetRotationY(PI*0.5f);
	m_wall22.SetScaleZ(2.2f);
	m_wall22.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall22, false);

	m_wall23.SetPlayerCharacter(L"MAP_WALL", -402.0f, 0.0f, -900.0f);
	m_wall23.SetRotationY(PI*0.5f);
	m_wall23.SetScaleZ(2.2f);
	m_wall23.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall23, false);

	m_wall24.SetPlayerCharacter(L"MAP_WALL", -575.0f, 0.0f, -900.0f);
	m_wall24.SetRotationY(PI*0.5f);
	m_wall24.SetScaleZ(2.2f);
	m_wall24.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall24, false);

	m_wall25.SetPlayerCharacter(L"MAP_WALL", -748, 0.0f, -900.0f);
	m_wall25.SetRotationY(PI*0.5f);
	m_wall25.SetScaleZ(2.2f);
	m_wall25.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall25, false);


	//аб
	m_wall30.SetPlayerCharacter(L"MAP_WALL", -900.0f, 0.0f, 402.0f);
	m_wall30.SetRotationY(-PI);
	m_wall30.SetScaleZ(2.2f);
	m_wall30.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall30, false);

	m_wall31.SetPlayerCharacter(L"MAP_WALL", -900.0f, 0.0f, 575.0f);
	m_wall31.SetRotationY(-PI);
	m_wall31.SetScaleZ(2.2f);
	m_wall31.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall31, false);

	m_wall32.SetPlayerCharacter(L"MAP_WALL", -900.0f, 0.0f, 748);
	m_wall32.SetRotationY(-PI);
	m_wall32.SetScaleZ(2.2f);
	m_wall32.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall32, false);

	m_wall33.SetPlayerCharacter(L"MAP_WALL", -900.0f, 0.0f, -402.0f);
	m_wall33.SetRotationY(-PI);
	m_wall33.SetScaleZ(2.2f);
	m_wall33.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall33, false);

	m_wall34.SetPlayerCharacter(L"MAP_WALL", -900.0f, 0.0f, -575.0f);
	m_wall34.SetRotationY(-PI);
	m_wall34.SetScaleZ(2.2f);
	m_wall34.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall34, false);

	m_wall35.SetPlayerCharacter(L"MAP_WALL", -900.0f, 0.0f, -748);
	m_wall35.SetRotationY(-PI);
	m_wall35.SetScaleZ(2.2f);
	m_wall35.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall35, false);


#pragma endregion

#pragma region MyTree



	m_tree00.SetPlayerCharacter(L"MAP_TREE1", -190.0f, 0.0f, 580.0f);
	m_tree00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree00, false);

	m_tree01.SetPlayerCharacter(L"MAP_TREE1", 190.0f, 0.0f, 580.0f);
	m_tree01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree01, false);

	m_tree02.SetPlayerCharacter(L"MAP_TREE1", 580.0f, 0.0f, -200.0f);
	m_tree02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree02, false);

	m_tree03.SetPlayerCharacter(L"MAP_TREE1", 580.0f, 0.0f, 200.0f);
	m_tree03.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree03, false);

	m_tree04.SetPlayerCharacter(L"MAP_TREE1", -190.0f, 0.0f, -630.0f);
	m_tree04.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree04, false);

	m_tree05.SetPlayerCharacter(L"MAP_TREE1", 190.0f, 0.0f, -630.0f);
	m_tree05.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree05, false);

	m_tree06.SetPlayerCharacter(L"MAP_TREE1", -630.0f, 0.0f, -200.0f);
	m_tree06.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree06, false);

	m_tree07.SetPlayerCharacter(L"MAP_TREE1", -630.0f, 0.0f, 200.0f);
	m_tree07.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree07, false);
#pragma endregion


#pragma region MyBarrel

	m_barrel00.SetPlayerCharacter(L"MAP_Barrel", -190, 0, 640);
	m_barrel00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel00);

	m_barrel01.SetPlayerCharacter(L"MAP_Barrel", -190, 0, 700);
	m_barrel01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel01);

	m_barrel02.SetPlayerCharacter(L"MAP_Barrel", 190, 0, 640);
	m_barrel02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel02);

	m_barrel03.SetPlayerCharacter(L"MAP_Barrel", 190, 0, 700);
	m_barrel03.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel03);


	m_barrel10.SetPlayerCharacter(L"MAP_Barrel", -190, 0, -720);
	m_barrel10.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel10);

	m_barrel11.SetPlayerCharacter(L"MAP_Barrel", -190, 0, -780);
	m_barrel11.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel11);

	m_barrel12.SetPlayerCharacter(L"MAP_Barrel", 190, 0, -720);
	m_barrel12.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel12);

	m_barrel13.SetPlayerCharacter(L"MAP_Barrel", 190, 0, -780);
	m_barrel13.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel13);


	m_barrel20.SetPlayerCharacter(L"MAP_Barrel", 670, 0, 200);
	m_barrel20.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel20);

	m_barrel21.SetPlayerCharacter(L"MAP_Barrel", 730, 0, 200);
	m_barrel21.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel21);

	m_barrel22.SetPlayerCharacter(L"MAP_Barrel", 670, 0, -200);
	m_barrel22.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel22);

	m_barrel23.SetPlayerCharacter(L"MAP_Barrel", 730, 0, -200);
	m_barrel23.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel23);

	m_barrel30.SetPlayerCharacter(L"MAP_Barrel", -670, 0, 200);
	m_barrel30.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel30);

	m_barrel31.SetPlayerCharacter(L"MAP_Barrel", -730, 0, 200);
	m_barrel31.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel31);

	m_barrel32.SetPlayerCharacter(L"MAP_Barrel", -670, 0, -200);
	m_barrel32.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel32);

	m_barrel33.SetPlayerCharacter(L"MAP_Barrel", -730, 0, -200);
	m_barrel33.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_barrel33);
#pragma endregion



#pragma region MyFence
	//1╫ц
	m_fence00.SetPlayerCharacter(L"MAP_FENCE", 190.0f, 0.0f, 520.0f);//190.0f80.0f
	m_fence00.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence00, false);
	m_fence01.SetPlayerCharacter(L"MAP_FENCE", 190.0f, 0.0f, 425.0f);
	m_fence01.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence01, false);
	m_fence02.SetPlayerCharacter(L"MAP_FENCE", 510.0f, 0.0f, 200.0f);
	m_fence02.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence02.SetRotationY(PI*-0.5f);
	m_fence02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence02, false);
	m_fence03.SetPlayerCharacter(L"MAP_FENCE", 415.0f, 0.0f, 200.0f);
	m_fence03.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence03.SetRotationY(PI*-0.5f);
	m_fence03.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence03, false);


	m_fence10.SetPlayerCharacter(L"MAP_FENCE", 190.0f, 0.0f, -520.0f);//190.0f80.0f
	m_fence10.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence10.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence10, false);
	m_fence11.SetPlayerCharacter(L"MAP_FENCE", 190.0f, 0.0f, -425.0f);
	m_fence11.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence11.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence11, false);
	m_fence12.SetPlayerCharacter(L"MAP_FENCE", 510.0f, 0.0f, -200.0f);
	m_fence12.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence12.SetRotationY(PI*0.5f);
	m_fence12.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence12, false);
	m_fence13.SetPlayerCharacter(L"MAP_FENCE", 415.0f, 0.0f, -200.0f);
	m_fence13.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence13.SetRotationY(PI*0.5f);
	m_fence13.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence13, false);


	m_fence20.SetPlayerCharacter(L"MAP_FENCE", -190.0f, 0.0f, -520.0f);//190.0f80.0f
	m_fence20.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence20.SetRotationY(-PI);
	m_fence20.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence20, false);
	m_fence21.SetPlayerCharacter(L"MAP_FENCE", -190.0f, 0.0f, -425.0f);
	m_fence21.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence21.SetRotationY(-PI);
	m_fence21.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence21, false);
	m_fence22.SetPlayerCharacter(L"MAP_FENCE", -510.0f, 0.0f, -200.0f);
	m_fence22.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence22.SetRotationY(PI*0.5f);
	m_fence22.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence22, false);
	m_fence23.SetPlayerCharacter(L"MAP_FENCE", -415.0f, 0.0f, -200.0f);
	m_fence23.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence23.SetRotationY(PI*0.5f);
	m_fence23.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence23, false);


	//11╫ц
	m_fence30.SetPlayerCharacter(L"MAP_FENCE", -190.0f, 0.0f, 520.0f);//190.0f80.0f
	m_fence30.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence30.SetRotationY(-PI);
	m_fence30.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence30, false);
	m_fence31.SetPlayerCharacter(L"MAP_FENCE", -190.0f, 0.0f, 425.0f);
	m_fence31.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence31.SetRotationY(-PI);
	m_fence31.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence31, false);
	m_fence32.SetPlayerCharacter(L"MAP_FENCE", -550, 0.0f, 200.0f);
	m_fence32.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence32.SetRotationY(PI*-0.5f);
	m_fence32.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence32, false);
	m_fence33.SetPlayerCharacter(L"MAP_FENCE", -455.0f, 0.0f, 200.0f);
	m_fence33.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence33.SetRotationY(PI*-0.5f);
	m_fence33.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence33, false);

	m_fence33.m_myName = L"Barricade";
	ObjectManager::Get().SetProtoObject(&m_fence33);
#pragma endregion

#pragma region MyCarpet
	m_carpet_blacksmith.SetPlayerCharacter(L"MAP_Carpet", 300.0f, 0.0f, 300.0f);
	m_carpet_blacksmith.SetScale(D3DXVECTOR3(4.0f, 4.0f, 4.0f));
	m_carpet_blacksmith.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_carpet_blacksmith, false);
	PlayerController::Get().m_CarpetPos[0] = m_carpet_blacksmith.GetPosition();

	m_carpet_church.SetPlayerCharacter(L"MAP_Carpet", -300.0f, 0.0f, 300.0f);
	m_carpet_church.SetScale(D3DXVECTOR3(4.0f, 4.0f, 4.0f));
	m_carpet_church.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_carpet_church, false);
	PlayerController::Get().m_CarpetPos[1] = m_carpet_church.GetPosition();

	m_carpet_windmill.SetPlayerCharacter(L"MAP_Carpet", 300.0f, 0.0f, -300.0f);
	m_carpet_windmill.SetScale(D3DXVECTOR3(4.0f, 4.0f, 4.0f));
	m_carpet_windmill.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_carpet_windmill, false);
	PlayerController::Get().m_CarpetPos[2] = m_carpet_windmill.GetPosition();

	m_carpet_towerRound.SetPlayerCharacter(L"MAP_Carpet", -300.0f, 0.0f, -300.0f);
	m_carpet_towerRound.SetScale(D3DXVECTOR3(4.0f, 4.0f, 4.0f));
	m_carpet_towerRound.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_carpet_towerRound, false);
	PlayerController::Get().m_CarpetPos[3] = m_carpet_towerRound.GetPosition();
#pragma endregion

#pragma region MywagonSack

	m_wagon1.SetPlayerCharacter(L"MAP_Wagon2", 440, 0, 240);	//280
	m_wagon1.SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
	m_wagon1.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wagon1);

	m_wagon2.SetPlayerCharacter(L"MAP_Wagon1", 460, 0, -280);	//280
	m_wagon2.SetScale(D3DXVECTOR3(4.0f, 4.0f, 3.0f));
	m_wagon2.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wagon2);

	m_wagon3.SetPlayerCharacter(L"MAP_Wagon1", -460, 0, -280);	//280
	m_wagon3.SetScale(D3DXVECTOR3(4.0f, 4.0f, 3.0f));
	m_wagon3.SetRotationY(-PI);
	m_wagon3.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wagon3);

	m_Sack00.SetPlayerCharacter(L"MAP_Sack", 240, 0, -430);
	m_Sack00.SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	m_Sack00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_Sack00);
	m_Sack01.SetPlayerCharacter(L"MAP_Sack", 280, 0, -430);
	m_Sack01.SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	m_Sack01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_Sack01);

	m_Sack10.SetPlayerCharacter(L"MAP_Sack", 240, 0, 400);
	m_Sack10.SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	m_Sack10.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_Sack10);
	m_Sack11.SetPlayerCharacter(L"MAP_Sack", 280, 0, 400);
	m_Sack11.SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	m_Sack11.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_Sack11);

	m_Sack20.SetPlayerCharacter(L"MAP_Sack", -240, 0, -400);
	m_Sack20.SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	m_Sack20.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_Sack20);
	m_Sack21.SetPlayerCharacter(L"MAP_Sack", -280, 0, -400);
	m_Sack21.SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	m_Sack21.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_Sack21);


#pragma endregion

#pragma region MyOutTree

	//m_oTree00.SetPlayerCharacter(L"MAP_TREE1", -1300, 0, 0);
	//m_oTree00.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree00.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree00);

	//m_oTree01.SetPlayerCharacter(L"MAP_TREE1", -1200, 0, 100);
	//m_oTree01.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree01.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree01);

	//m_oTree02.SetPlayerCharacter(L"MAP_TREE1", -1100, 0, 200);
	//m_oTree02.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree02.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree02);

	//m_oTree03.SetPlayerCharacter(L"MAP_TREE1", -1200, 0, -100);
	//m_oTree03.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree03.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree03);

	//m_oTree04.SetPlayerCharacter(L"MAP_TREE1", -1100, 0, -200);
	//m_oTree04.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree04.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree04);

	//m_oTree05.SetPlayerCharacter(L"MAP_TREE1", -1300, 0, 200);
	//m_oTree05.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree05.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree05);

	//m_oTree06.SetPlayerCharacter(L"MAP_TREE1", -1300, 0, -200);
	//m_oTree06.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree06.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree06);

	//m_oTree07.SetPlayerCharacter(L"MAP_TREE1", -1350, 0, -100);
	//m_oTree07.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree07.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree07);

	//m_oTree08.SetPlayerCharacter(L"MAP_TREE1", -1350, 0, 100);
	//m_oTree08.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree08.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree08);

	//m_oTree09.SetPlayerCharacter(L"MAP_TREE1", -1100, 0, 000);
	//m_oTree09.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree09.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree09);


	//m_oTree10.SetPlayerCharacter(L"MAP_TREE1", 0, 0, 1300);
	//m_oTree10.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree10.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree10);

	//m_oTree11.SetPlayerCharacter(L"MAP_TREE1", 100, 0, 1200);
	//m_oTree11.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree11.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree11);

	//m_oTree12.SetPlayerCharacter(L"MAP_TREE1", 200, 0, 1100);
	//m_oTree12.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree12.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree12);

	//m_oTree13.SetPlayerCharacter(L"MAP_TREE1", -100, 0, 1200);
	//m_oTree13.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree13.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree13);

	//m_oTree14.SetPlayerCharacter(L"MAP_TREE1", -200, 0, 1100);
	//m_oTree14.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree14.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree14);

	//m_oTree15.SetPlayerCharacter(L"MAP_TREE1", 200, 0, 1300);
	//m_oTree15.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree15.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree15);

	//m_oTree16.SetPlayerCharacter(L"MAP_TREE1", -200, 0, 1300);
	//m_oTree16.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree16.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree16);

	//m_oTree17.SetPlayerCharacter(L"MAP_TREE1", -100, 0, 1350);
	//m_oTree17.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree17.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree17);

	//m_oTree18.SetPlayerCharacter(L"MAP_TREE1", 100, 0, 1350);
	//m_oTree18.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree18.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree18);

	//m_oTree19.SetPlayerCharacter(L"MAP_TREE1", 0, 0, 1100);
	//m_oTree19.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree19.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree19);


	//m_oTree20.SetPlayerCharacter(L"MAP_TREE1", 0, 0, -1300);
	//m_oTree20.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree20.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree20);

	//m_oTree21.SetPlayerCharacter(L"MAP_TREE1", 100, 0, -1200);
	//m_oTree21.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree21.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree21);

	//m_oTree22.SetPlayerCharacter(L"MAP_TREE1", 200, 0, -1100);
	//m_oTree22.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree22.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree22);

	//m_oTree23.SetPlayerCharacter(L"MAP_TREE1", -100, 0, -1200);
	//m_oTree23.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree23.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree23);

	//m_oTree24.SetPlayerCharacter(L"MAP_TREE1", -200, 0, -1100);
	//m_oTree24.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree24.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree24);

	//m_oTree25.SetPlayerCharacter(L"MAP_TREE1", 200, 0, -1300);
	//m_oTree25.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree25.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree25);

	//m_oTree26.SetPlayerCharacter(L"MAP_TREE1", -200, 0, -1300);
	//m_oTree26.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree26.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree26);

	//m_oTree27.SetPlayerCharacter(L"MAP_TREE1", -100, 0, -1350);
	//m_oTree27.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree27.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree27);

	//m_oTree28.SetPlayerCharacter(L"MAP_TREE1", 100, 0, -1350);
	//m_oTree28.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree28.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree28);

	//m_oTree29.SetPlayerCharacter(L"MAP_TREE1", 0, 0, -1100);
	//m_oTree29.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree29.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree29);


	//m_oTree30.SetPlayerCharacter(L"MAP_TREE1", 1300, 0, 0);
	//m_oTree30.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree30.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree30);

	//m_oTree31.SetPlayerCharacter(L"MAP_TREE1", 1200, 0, 100);
	//m_oTree31.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree31.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree31);

	//m_oTree32.SetPlayerCharacter(L"MAP_TREE1", 1100, 0, 200);
	//m_oTree32.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree32.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree32);

	//m_oTree33.SetPlayerCharacter(L"MAP_TREE1", 1200, 0, -100);
	//m_oTree33.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree33.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree33);

	//m_oTree34.SetPlayerCharacter(L"MAP_TREE1", 1100, 0, -200);
	//m_oTree34.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree34.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree34);

	//m_oTree35.SetPlayerCharacter(L"MAP_TREE1", 1300, 0, 200);
	//m_oTree35.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree35.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree35);

	//m_oTree36.SetPlayerCharacter(L"MAP_TREE1", 1300, 0, -200);
	//m_oTree36.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree36.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree36);

	//m_oTree37.SetPlayerCharacter(L"MAP_TREE1", 1350, 0, -100);
	//m_oTree37.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree37.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree37);

	//m_oTree38.SetPlayerCharacter(L"MAP_TREE1", 1350, 0, 100);
	//m_oTree38.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree38.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree38);

	//m_oTree39.SetPlayerCharacter(L"MAP_TREE1", 1100, 0, 0);
	//m_oTree39.SetScale(D3DXVECTOR3(1.3f, 0.9f, 1.3f));
	//m_oTree39.m_objType = EObjType::AObject;
	//ObjectManager::Get().PushObject(&m_oTree39);

#pragma endregion

#pragma region MyBillTree
	auto pPlane = new RPlane(L"T", L"Tree1.png", "VS_Basic", "PS_Basic");
	int i = 0;
	m_pTree2D[i] = new GameObject(L"T", pPlane, EObjType::Dummy);
	m_pTree2D[i]->SetScale(Vector3::One * 50.0f);
	m_pTree2D[i]->SetPosition(-1000.0f, 180.0f, 0.0f);
	m_pTree2D[i]->SetScale(180.0f, 180.0f, 180.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1100.0f, 180.0f, 50.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1150.0f, 180.0f, 100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1200.0f, 180.0f, 150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1250.0f, 180.0f, 200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1300.0f, 180.0f, 250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1250.0f, 180.0f, -200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1200.0f, 180.0f, -150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1150.0f, 180.0f, -100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1100.0f, 180.0f, -50.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1000.0f, 180.0f, 150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1000.0f, 180.0f, -150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1250.0f, 180.0f, 150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1250.0f, 180.0f, -150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1150.0f, 180.0f, 100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1150.0f, 180.0f, -100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1100.0f, 180.0f, 200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1100.0f, 180.0f, -200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1100.0f, 180.0f, 300.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-1100.0f, 180.0f, -300.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);


	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1000.0f, 180.0f, 0.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1100.0f, 180.0f, 50.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1150.0f, 180.0f, 100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1200.0f, 180.0f, 150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1250.0f, 180.0f, 200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1300.0f, 180.0f, 250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1250.0f, 180.0f, -200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1200.0f, 180.0f, -150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1150.0f, 180.0f, -100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1100.0f, 180.0f, -50.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1000.0f, 180.0f, 150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1000.0f, 180.0f, -150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1250.0f, 180.0f, 150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1250.0f, 180.0f, -150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1150.0f, 180.0f, 100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1150.0f, 180.0f, -100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1100.0f, 180.0f, 200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1100.0f, 180.0f, -200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1100.0f, 180.0f, 300.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(1100.0f, 180.0f, -300.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);



	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(0.0f, 180.0f, 1000.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(50.0f, 180.0f, 1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(100.0f, 180.0f, 1150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();

	m_pTree2D[i]->SetPosition(150.0f, 180.0f, 1200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(200.0f, 180.0f, 1250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(250.0f, 180.0f, 1300.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-200.0f, 180.0f, 1250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-150.0f, 180.0f, 1200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-100.0f, 180.0f, 1150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-50.0f, 180.0f, 1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(150.0f, 180.0f, 1000.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-150.0f, 180.0f, 1000.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(150.0f, 180.0f, 1250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-150.0f, 180.0f, 1250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(100.0f, 180.0f, 1150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-100.0f, 180.0f, 1150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(200.0f, 180.0f, 1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-200.0f, 180.0f, 1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(300.0f, 180.0f, 1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-300.0f, 180.0f, 1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);

	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(0.0f, 180.0f, -1000.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(50.0f, 180.0f, -1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(100.0f, 180.0f, -1150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(150.0f, 180.0f, -1200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(200.0f, 180.0f, -1250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(250.0f, 180.0f, -1300.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-200.0f, 180.0f, -1250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-150.0f, 180.0f, -1200.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-100.0f, 180.0f, -1150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-50.0f, 180.0f, -1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(150.0f, 180.0f, -1000.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-150.0f, 180.0f, -1000.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(150.0f, 180.0f, -1250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-150.0f, 180.0f, -1250.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(100.0f, 180.0f, -1150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-100.0f, 180.0f, -1150.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(200.0f, 180.0f, -1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-200.0f, 180.0f, -1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(300.0f, 180.0f, -1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
	m_pTree2D[++i] = m_pTree2D[0]->clone();
	m_pTree2D[i]->SetPosition(-300.0f, 180.0f, -1100.0f);
	ObjectManager::Get().PushObject(m_pTree2D[i]);
#pragma endregion





#pragma region TownCollider
	pObject = new GameObject(L"-");

	pCollider = new ColliderOBB({ -1000, 0.0f, 400.0f }, { -190.0f, 200.0f, 1000.0f });
	pCollider->m_eTag = ETag::Collider;
	pObject->AddComponent(pCollider);
	pCollider = new ColliderOBB({ -1000, 0.0f, 200.0f }, { -400.0f, 200.0f, 1000.0f });
	pCollider->m_eTag = ETag::Collider;
	pObject->AddComponent(pCollider);

	pCollider = new ColliderOBB({ -1000, 0.0f, -1000.0f }, { -190.0f, 200.0f, -400.0f });
	pCollider->m_eTag = ETag::Collider;
	pObject->AddComponent(pCollider);
	pCollider = new ColliderOBB({ -1000, 0.0f, -1000.0f }, { -400.0f, 200.0f, -200.0f });
	pCollider->m_eTag = ETag::Collider;
	pObject->AddComponent(pCollider);

	pCollider = new ColliderOBB({ 400, 0.0f, 200.0f }, { 1000.0f, 200.0f, 1000.0f });
	pCollider->m_eTag = ETag::Collider;
	pObject->AddComponent(pCollider);
	pCollider = new ColliderOBB({ 190, 0.0f, 400.0f }, { 1000.0f, 200.0f, 1000.0f });
	pCollider->m_eTag = ETag::Collider;
	pObject->AddComponent(pCollider);

	pCollider = new ColliderOBB({ 400, 0.0f, -1000.0f }, { 1000.0f, 200.0f, -190.0f });
	pCollider->m_eTag = ETag::Collider;
	pObject->AddComponent(pCollider);
	pCollider = new ColliderOBB({ 190, 0.0f, -1000.0f }, { 1000.0f, 200.0f, -400.0f });
	pCollider->m_eTag = ETag::Collider;
	pObject->AddComponent(pCollider);

	pObject->SetGravityScale(0.0f);
	pObject->usePhysics(false);
	pObject->m_pPhysics->m_repulsion = 0.6f;
	pObject->m_pPhysics->m_armor = 0.0f;
#pragma endregion


	for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::AObject))
	{
		iter->SetPositionY(m_pHeightMap->GetMapHeight(iter->GetPosition()));
	}
	return true;
}

bool GameMap::Frame(const float& spf, const float& accTime)	noexcept
{
	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		if (iter == nullptr || iter->m_pParent == nullptr)
			continue;
		iter->m_mapHeight = m_pHeightMap->GetMapHeight(iter->m_pParent->GetPosition());
	}

	auto rotBill = ObjectManager::Get().CurCamera->GetWorldRotation();
	for (auto& iter : m_pTree2D)
	{
		if (iter == nullptr)
			break;
		iter->SetRotation(0.0f, rotBill.y, 0.0f);
		//iter->UpdateMatrix();
		//iter->Render(pDContext);
	}
	return true;
	accTime; spf;
}

bool GameMap::Render(ID3D11DeviceContext* pDContext)			noexcept
{
	return true;
	pDContext;
}

bool GameMap::Release()	noexcept
{
	return true;
}
