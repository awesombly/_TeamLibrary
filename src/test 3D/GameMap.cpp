#include "GameMap.h"
#include "ObjectManager.h"

bool GameMap::Init()											noexcept
{
	m_pHeightMap = new HeightMap(L"HeightMap", EComponent::Renderer, L"mounds.jpg");
	auto pObject = new GameObject(L"HeightMap", m_pHeightMap, EObjType::Map);
	m_pHeightMap->CreateHeightMap(DxManager::GetDContext(), L"HeightMap/Islands, Leafy.bmp", 25, 1.0f, 1.0f);
	m_pHeightMap->SetShadowRate(0.0f);
	//mapMap->SetEnviromentMap(((Renderer*)m_pSkyBox->GetComponent(EComponent::Renderer))->m_srcName, EEnviType::Fresnel);
	//pObject->Translate(Vector3::Down * 100.0f);
	pObject->SetScaleY(0.2f);
	ObjectManager::Get().PushObject(pObject);

	//////////////////////////////////////////////////////////////////////////////////////

	m_fountain.SetPlayerCharacter(L"MAP_FOUNTAIN", 0, 0, 0);
	m_fountain.SetScale(D3DXVECTOR3(6, 6, 6));
	m_fountain.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fountain);

	m_church.SetPlayerCharacter(L"MAP_Church", -450, 0, 450);	//280
	m_church.SetScale(D3DXVECTOR3(4, 4, 4));
	m_church.SetRotationY(67);
	m_church.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_church);

	m_blacksmith.SetPlayerCharacter(L"MAP_Blacksmith", 400, 0, 400);	//280
	m_blacksmith.SetScale(D3DXVECTOR3(0.6, 0.6, 0.3));
	m_blacksmith.SetRotationY(70);
	m_blacksmith.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_blacksmith);

	m_windmill.SetPlayerCharacter(L"MAP_WINDMILL", 450, 0, -450);	//280
	m_windmill.SetScale(D3DXVECTOR3(1.8, 1.8, 1.8));
	m_windmill.SetRotationY(PI*0.7);
	m_windmill.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_windmill);

	m_towerRound.SetPlayerCharacter(L"MAP_TowerRound", -450, 0, -450);	//280
	m_towerRound.SetScale(D3DXVECTOR3(1.4, 1.6, 1.4));
	m_towerRound.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerRound);


#pragma region MyHouse


	m_house00.SetPlayerCharacter(L"MAP_HOUSE_C", -680, 0, 680);	//280
	m_house00.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house00);
	m_house01.SetPlayerCharacter(L"MAP_HOUSE_B", -400, 0, 680);	//280
	m_house01.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house01);
	m_house02.SetPlayerCharacter(L"MAP_HOUSE_D", -680, 0, 400);	//280
	m_house02.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house02);


	m_house10.SetPlayerCharacter(L"MAP_HOUSE_C", 680, 0, 680);	//280
	m_house10.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house10.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house10);
	m_house11.SetPlayerCharacter(L"MAP_HOUSE_B", 400, 0, 680);	//280
	m_house11.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house11.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house11);
	m_house12.SetPlayerCharacter(L"MAP_HOUSE_D", 680, 0, 400);	//280
	m_house12.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house12.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house12);

	m_house20.SetPlayerCharacter(L"MAP_HOUSE_C", 680, 0, -680);	//280
	m_house20.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house20.SetRotationY(-PI);
	m_house20.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house20);
	m_house21.SetPlayerCharacter(L"MAP_HOUSE_B", 400, 0, -680);	//280
	m_house21.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house21.SetRotationY(-PI);
	m_house21.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house21);
	m_house22.SetPlayerCharacter(L"MAP_HOUSE_D", 680, 0, -400);	//280
	m_house22.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house22.SetRotationY(-PI);
	m_house22.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house22);

	m_house30.SetPlayerCharacter(L"MAP_HOUSE_C", -680, 0, -680);	//280
	m_house30.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house30.SetRotationY(-PI);
	m_house30.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house30);
	m_house31.SetPlayerCharacter(L"MAP_HOUSE_B", -400, 0, -680);	//280
	m_house31.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house31.SetRotationY(-PI);
	m_house31.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house31);
	m_house32.SetPlayerCharacter(L"MAP_HOUSE_D", -680, 0, -400);	//280
	m_house32.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house32.SetRotationY(-PI);
	m_house32.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_house32);



	

#pragma endregion

#pragma region MyTower

	//╩С
	m_tower0.SetPlayerCharacter(L"MAP_Tower", 250, 0, 900);	//280;
	m_tower0.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_tower0.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower0);

	m_tower1.SetPlayerCharacter(L"MAP_Tower", -250, 0, 900);	//280
	m_tower1.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_tower1.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower1);


	//©Л
	m_tower2.SetPlayerCharacter(L"MAP_Tower", 900, 0, 250);	//280;
	m_tower2.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_tower2.SetRotationY(PI*0.5);
	m_tower2.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower2);

	m_tower3.SetPlayerCharacter(L"MAP_Tower", 900, 0, -250);	//280;
	m_tower3.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_tower3.SetRotationY(PI*0.5);
	m_tower3.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower3);

	//го
	m_tower4.SetPlayerCharacter(L"MAP_Tower", 250, 0, -900);	//280;
	m_tower4.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_tower4.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower4);

	m_tower5.SetPlayerCharacter(L"MAP_Tower", -250, 0, -900);	//280;
	m_tower5.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_tower5.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower5);

	//аб
	m_tower6.SetPlayerCharacter(L"MAP_Tower", -900, 0, 250);	//280;
	m_tower6.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_tower6.SetRotationY(PI*0.5);
	m_tower6.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower6);

	m_tower7.SetPlayerCharacter(L"MAP_Tower", -900, 0, -250);	//280;
	m_tower7.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_tower7.SetRotationY(PI*0.5);
	m_tower7.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tower7);


	m_towerTR.SetPlayerCharacter(L"MAP_Tower", 900, 0, 900);
	m_towerTR.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_towerTR.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerTR);

	m_towerTL.SetPlayerCharacter(L"MAP_Tower", -900, 0, 900);
	m_towerTL.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_towerTL.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerTL);

	m_towerBR.SetPlayerCharacter(L"MAP_Tower", 900, 0, -900);
	m_towerBR.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_towerBR.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerBR);

	m_towerBL.SetPlayerCharacter(L"MAP_Tower", -900, 0, -900);
	m_towerBL.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_towerBL.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_towerBL);

#pragma endregion

#pragma region MyWall
	//╩С
	m_wall00.SetPlayerCharacter(L"MAP_WALL", 402, 0, 900);
	m_wall00.SetRotationY(PI*-0.5);
	m_wall00.SetScaleZ(2.2f);
	m_wall00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall00);

	m_wall01.SetPlayerCharacter(L"MAP_WALL", 575, 0, 900);
	m_wall01.SetRotationY(PI*-0.5);
	m_wall01.SetScaleZ(2.2f);
	m_wall01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall01);

	m_wall02.SetPlayerCharacter(L"MAP_WALL", 748, 0, 900);
	m_wall02.SetRotationY(PI*-0.5);
	m_wall02.SetScaleZ(2.2f);
	m_wall02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall02);

	m_wall03.SetPlayerCharacter(L"MAP_WALL", -402, 0, 900);
	m_wall03.SetRotationY(PI*-0.5);
	m_wall03.SetScaleZ(2.2f);
	m_wall03.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall03);

	m_wall04.SetPlayerCharacter(L"MAP_WALL", -575, 0, 900);
	m_wall04.SetRotationY(PI*-0.5);
	m_wall04.SetScaleZ(2.2f);
	m_wall04.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall04);

	m_wall05.SetPlayerCharacter(L"MAP_WALL", -748, 0, 900);
	m_wall05.SetRotationY(PI*-0.5);
	m_wall05.SetScaleZ(2.2f);
	m_wall05.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall05);


	//©Л
	m_wall10.SetPlayerCharacter(L"MAP_WALL", 900, 0, 402);
	m_wall10.SetScaleZ(2.2f);
	m_wall10.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall10);

	m_wall11.SetPlayerCharacter(L"MAP_WALL", 900, 0, 575);
	m_wall11.SetScaleZ(2.2f);
	m_wall11.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall11);

	m_wall12.SetPlayerCharacter(L"MAP_WALL", 900, 0, 748);
	m_wall12.SetScaleZ(2.2f);
	m_wall12.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall12);

	m_wall13.SetPlayerCharacter(L"MAP_WALL", 900, 0, -402);
	m_wall13.SetScaleZ(2.2f);
	m_wall13.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall13);

	m_wall14.SetPlayerCharacter(L"MAP_WALL", 900, 0, -575);
	m_wall14.SetScaleZ(2.2f);
	m_wall14.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall14);

	m_wall15.SetPlayerCharacter(L"MAP_WALL", 900, 0, -748);
	m_wall15.SetScaleZ(2.2f);
	m_wall15.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall15);

	//го
	m_wall20.SetPlayerCharacter(L"MAP_WALL", 402, 0, -900);
	m_wall20.SetRotationY(PI*0.5);
	m_wall20.SetScaleZ(2.2f);
	m_wall20.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall20);

	m_wall21.SetPlayerCharacter(L"MAP_WALL", 575, 0, -900);
	m_wall21.SetRotationY(PI*0.5);
	m_wall21.SetScaleZ(2.2f);
	m_wall21.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall21);

	m_wall22.SetPlayerCharacter(L"MAP_WALL", 748, 0, -900);
	m_wall22.SetRotationY(PI*0.5);
	m_wall22.SetScaleZ(2.2f);
	m_wall22.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall22);

	m_wall23.SetPlayerCharacter(L"MAP_WALL", -402, 0, -900);
	m_wall23.SetRotationY(PI*0.5);
	m_wall23.SetScaleZ(2.2f);
	m_wall23.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall23);

	m_wall24.SetPlayerCharacter(L"MAP_WALL", -575, 0, -900);
	m_wall24.SetRotationY(PI*0.5);
	m_wall24.SetScaleZ(2.2f);
	m_wall24.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall24);

	m_wall25.SetPlayerCharacter(L"MAP_WALL", -748, 0, -900);
	m_wall25.SetRotationY(PI*0.5);
	m_wall25.SetScaleZ(2.2f);
	m_wall25.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall25);


	//аб
	m_wall30.SetPlayerCharacter(L"MAP_WALL", -900, 0, 402);
	m_wall30.SetRotationY(-PI);
	m_wall30.SetScaleZ(2.2f);
	m_wall30.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall30);

	m_wall31.SetPlayerCharacter(L"MAP_WALL", -900, 0, 575);
	m_wall31.SetRotationY(-PI);
	m_wall31.SetScaleZ(2.2f);
	m_wall31.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall31);

	m_wall32.SetPlayerCharacter(L"MAP_WALL", -900, 0, 748);
	m_wall32.SetRotationY(-PI);
	m_wall32.SetScaleZ(2.2f);
	m_wall32.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall32);

	m_wall33.SetPlayerCharacter(L"MAP_WALL", -900, 0, -402);
	m_wall33.SetRotationY(-PI);
	m_wall33.SetScaleZ(2.2f);
	m_wall33.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall33);

	m_wall34.SetPlayerCharacter(L"MAP_WALL", -900, 0, -575);
	m_wall34.SetRotationY(-PI);
	m_wall34.SetScaleZ(2.2f);
	m_wall34.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall34);

	m_wall35.SetPlayerCharacter(L"MAP_WALL", -900, 0, -748);
	m_wall35.SetRotationY(-PI);
	m_wall35.SetScaleZ(2.2f);
	m_wall35.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_wall35);


#pragma endregion

#pragma region MyTree



	m_tree00.SetPlayerCharacter(L"MAP_TREE1", -190, 0, 580);
	m_tree00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree00);

	m_tree01.SetPlayerCharacter(L"MAP_TREE1", 190, 0, 580);
	m_tree01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree01);

	m_tree02.SetPlayerCharacter(L"MAP_TREE1", 580, 0, -200);
	m_tree02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree02);

	m_tree03.SetPlayerCharacter(L"MAP_TREE1", 580, 0, 200);
	m_tree03.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree03);

	m_tree04.SetPlayerCharacter(L"MAP_TREE1", -190, 0, -630);
	m_tree04.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree04);

	m_tree05.SetPlayerCharacter(L"MAP_TREE1", 190, 0, -630);
	m_tree05.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree05);

	m_tree06.SetPlayerCharacter(L"MAP_TREE1", -630, 0, -200);
	m_tree06.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree06);

	m_tree07.SetPlayerCharacter(L"MAP_TREE1", -630, 0, 200);
	m_tree07.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_tree07);
#pragma endregion

#pragma region MyFence


	//1╫ц
	m_fence00.SetPlayerCharacter(L"MAP_FENCE", 190, 0, 520);//190 580
	m_fence00.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence00.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence00);
	m_fence01.SetPlayerCharacter(L"MAP_FENCE", 190, 0, 425);
	m_fence01.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence01.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence01);
	m_fence02.SetPlayerCharacter(L"MAP_FENCE", 510, 0, 200);
	m_fence02.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence02.SetRotationY(PI*-0.5);
	m_fence02.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence02);
	m_fence03.SetPlayerCharacter(L"MAP_FENCE", 415, 0, 200);
	m_fence03.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence03.SetRotationY(PI*-0.5);
	m_fence03.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence03);


	m_fence10.SetPlayerCharacter(L"MAP_FENCE", 190, 0, -520);//190 580
	m_fence10.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence10.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence10);
	m_fence11.SetPlayerCharacter(L"MAP_FENCE", 190, 0, -425);
	m_fence11.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence11.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence11);
	m_fence12.SetPlayerCharacter(L"MAP_FENCE", 510, 0, -200);
	m_fence12.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence12.SetRotationY(PI*0.5);
	m_fence12.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence12);
	m_fence13.SetPlayerCharacter(L"MAP_FENCE", 415, 0, -200);
	m_fence13.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence13.SetRotationY(PI*0.5);
	m_fence13.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence13);


	m_fence20.SetPlayerCharacter(L"MAP_FENCE", -190, 0, -520);//190 580
	m_fence20.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence20.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence20);
	m_fence21.SetPlayerCharacter(L"MAP_FENCE", -190, 0, -425);
	m_fence21.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence21.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence21);
	m_fence22.SetPlayerCharacter(L"MAP_FENCE", -510, 0, -200);
	m_fence22.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence22.SetRotationY(PI*0.5);
	m_fence22.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence22);
	m_fence23.SetPlayerCharacter(L"MAP_FENCE", -415, 0, -200);
	m_fence23.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence23.SetRotationY(PI*0.5);
	m_fence23.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence23);


	//11╫ц
	m_fence30.SetPlayerCharacter(L"MAP_FENCE", -190, 0, 520);//190 580
	m_fence30.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence30.SetRotationY(-PI);
	m_fence30.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence30);
	m_fence31.SetPlayerCharacter(L"MAP_FENCE", -190, 0, 425);
	m_fence31.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence31.SetRotationY(-PI);
	m_fence31.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence31);
	m_fence32.SetPlayerCharacter(L"MAP_FENCE", -550, 0, 200);
	m_fence32.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence32.SetRotationY(PI*-0.5);
	m_fence32.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence32);
	m_fence33.SetPlayerCharacter(L"MAP_FENCE", -455, 0, 200);
	m_fence33.SetScale(D3DXVECTOR3(2.0f, 1.5f, 2.0f));
	m_fence33.SetRotationY(PI*-0.5);
	m_fence33.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_fence33);
#pragma endregion

#pragma region MyCarpet

	m_carpet_blacksmith.SetPlayerCharacter(L"MAP_Carpet", 300, 0, 300);
	m_carpet_blacksmith.SetScale(D3DXVECTOR3(4, 4, 4));
	m_carpet_blacksmith.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_carpet_blacksmith);

	m_carpet_church.SetPlayerCharacter(L"MAP_Carpet", -300, 0, 300);
	m_carpet_church.SetScale(D3DXVECTOR3(4, 4, 4));
	m_carpet_church.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_carpet_church);

	m_carpet_windmill.SetPlayerCharacter(L"MAP_Carpet", 300, 0, -300);
	m_carpet_windmill.SetScale(D3DXVECTOR3(4, 4, 4));
	m_carpet_windmill.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_carpet_windmill);

	m_carpet_towerRound.SetPlayerCharacter(L"MAP_Carpet", -300, 0, -300);
	m_carpet_towerRound.SetScale(D3DXVECTOR3(4, 4, 4));
	m_carpet_towerRound.m_objType = EObjType::AObject;
	ObjectManager::Get().PushObject(&m_carpet_towerRound);
#pragma endregion

//#pragma region SetMatrix
//
//	m_blacksmith.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_fountain.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_church.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_towerRound.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_windmill.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//
//
//	m_wall00.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall01.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall02.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall03.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall04.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall05.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall10.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall11.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall12.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall13.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall14.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall15.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall20.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall21.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall22.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall23.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall24.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall25.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall30.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall31.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall32.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall33.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall34.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_wall35.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//
//
//	m_towerTR.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_towerTL.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_towerBR.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_towerBL.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//
//	m_tower0.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_tower1.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_tower2.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_tower3.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_tower4.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_tower5.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_tower6.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_tower7.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//
//	m_house00.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_house01.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_house02.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//
//	m_house10.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_house11.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_house12.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//
//	m_house20.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_house21.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_house22.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//
//	m_house30.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_house31.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//	m_house32.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
//
//
//#pragma endregion
//
//


	return true;
}

bool GameMap::Frame(const float& spf, const float& accTime)	noexcept
{
	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		if (iter == nullptr || iter->m_pParent == nullptr)
			continue;
		iter->m_mapHeight = m_pHeightMap->GetMapHeight(iter->m_pParent->GetPosition());
		//iter->m_mapHeight = m_pMap->GetHeight(iter->m_pParent->GetPosition().x, iter->m_pParent->GetPosition().z);
	}

	return true;
}

bool GameMap::Render(ID3D11DeviceContext* pDContext)			noexcept
{
	

	return true;

}

bool GameMap::Release()										noexcept
{

	return true;
}

Component* GameMap::clone() noexcept
{
	return 0;
}

GameMap::GameMap()
{

}


GameMap::~GameMap()
{

}
