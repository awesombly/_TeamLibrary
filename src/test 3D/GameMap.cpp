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

	m_church.SetPlayerCharacter(L"MAP_Church", -450, 0, 450);	//280
	m_church.SetScale(D3DXVECTOR3(4, 4, 4));
	m_church.SetRotationY(67);

	m_blacksmith.SetPlayerCharacter(L"MAP_Blacksmith", 400, 0, 400);	//280
	m_blacksmith.SetScale(D3DXVECTOR3(0.6, 0.6, 0.3));
	m_blacksmith.SetRotationY(70);


	m_windmill.SetPlayerCharacter(L"MAP_WINDMILL", 450, 0, -450);	//280
	m_windmill.SetScale(D3DXVECTOR3(1.8, 1.8, 1.8));
	m_windmill.SetRotationY(PI*0.7);

	m_towerRound.SetPlayerCharacter(L"MAP_TowerRound", -450, 0, -450);	//280
	m_towerRound.SetScale(D3DXVECTOR3(1.4, 1.6, 1.4));

#pragma region MyHouse


	m_house00.SetPlayerCharacter(L"MAP_HOUSE_C", -680, 0, 680);	//280
	m_house00.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house01.SetPlayerCharacter(L"MAP_HOUSE_B", -400, 0, 680);	//280
	m_house01.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house02.SetPlayerCharacter(L"MAP_HOUSE_D", -680, 0, 400);	//280
	m_house02.SetScale(D3DXVECTOR3(130, 130, 130));

	m_house10.SetPlayerCharacter(L"MAP_HOUSE_C", 680, 0, 680);	//280
	m_house10.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house11.SetPlayerCharacter(L"MAP_HOUSE_B", 400, 0, 680);	//280
	m_house11.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house12.SetPlayerCharacter(L"MAP_HOUSE_D", 680, 0, 400);	//280
	m_house12.SetScale(D3DXVECTOR3(130, 130, 130));

	m_house20.SetPlayerCharacter(L"MAP_HOUSE_C", 680, 0, -680);	//280
	m_house20.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house20.SetRotationY(-PI);
	m_house21.SetPlayerCharacter(L"MAP_HOUSE_B", 400, 0, -680);	//280
	m_house21.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house21.SetRotationY(-PI);
	m_house22.SetPlayerCharacter(L"MAP_HOUSE_D", 680, 0, -400);	//280
	m_house22.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house22.SetRotationY(-PI);


	m_house30.SetPlayerCharacter(L"MAP_HOUSE_C", -680, 0, -680);	//280
	m_house30.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house30.SetRotationY(-PI);
	m_house31.SetPlayerCharacter(L"MAP_HOUSE_B", -400, 0, -680);	//280
	m_house31.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house31.SetRotationY(-PI);
	m_house32.SetPlayerCharacter(L"MAP_HOUSE_D", -680, 0, -400);	//280
	m_house32.SetScale(D3DXVECTOR3(130, 130, 130));
	m_house32.SetRotationY(-PI);
#pragma endregion

#pragma region MyTower

	//╩С
	m_tower0.SetPlayerCharacter(L"MAP_Tower", 250, 0, 900);	//280;
	m_tower0.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_tower1.SetPlayerCharacter(L"MAP_Tower", -250, 0, 900);	//280
	m_tower1.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;


	//©Л
	m_tower2.SetPlayerCharacter(L"MAP_Tower", 900, 0, 250);	//280;
	m_tower2.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_tower2.SetRotationY(PI*0.5);
	m_tower3.SetPlayerCharacter(L"MAP_Tower", 900, 0, -250);	//280;
	m_tower3.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_tower3.SetRotationY(PI*0.5);

	//го
	m_tower4.SetPlayerCharacter(L"MAP_Tower", 250, 0, -900);	//280;
	m_tower4.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
	m_tower5.SetPlayerCharacter(L"MAP_Tower", -250, 0, -900);	//280;
	m_tower5.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));

	//аб
	m_tower6.SetPlayerCharacter(L"MAP_Tower", -900, 0, 250);	//280;
	m_tower6.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_tower6.SetRotationY(PI*0.5);
	m_tower7.SetPlayerCharacter(L"MAP_Tower", -900, 0, -250);	//280;
	m_tower7.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;
	m_tower7.SetRotationY(PI*0.5);


	m_towerTR.SetPlayerCharacter(L"MAP_Tower", 900, 0, 900);
	m_towerTR.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;

	m_towerTL.SetPlayerCharacter(L"MAP_Tower", -900, 0, 900);
	m_towerTL.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));

	m_towerBR.SetPlayerCharacter(L"MAP_Tower", 900, 0, -900);
	m_towerBR.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));;

	m_towerBL.SetPlayerCharacter(L"MAP_Tower", -900, 0, -900);
	m_towerBL.SetScale(D3DXVECTOR3(0.8, 0.8, 0.8));
#pragma endregion

#pragma region MyWall
	//╩С
	m_wall00.SetPlayerCharacter(L"MAP_WALL", 402, 0, 900);
	m_wall00.SetRotationY(PI*-0.5);
	m_wall00.SetScaleZ(2.2f);
	m_wall01.SetPlayerCharacter(L"MAP_WALL", 575, 0, 900);
	m_wall01.SetRotationY(PI*-0.5);
	m_wall01.SetScaleZ(2.2f);
	m_wall02.SetPlayerCharacter(L"MAP_WALL", 748, 0, 900);
	m_wall02.SetRotationY(PI*-0.5);
	m_wall02.SetScaleZ(2.2f);
	m_wall03.SetPlayerCharacter(L"MAP_WALL", -402, 0, 900);
	m_wall03.SetRotationY(PI*-0.5);
	m_wall03.SetScaleZ(2.2f);
	m_wall04.SetPlayerCharacter(L"MAP_WALL", -575, 0, 900);
	m_wall04.SetRotationY(PI*-0.5);
	m_wall04.SetScaleZ(2.2f);
	m_wall05.SetPlayerCharacter(L"MAP_WALL", -748, 0, 900);
	m_wall05.SetRotationY(PI*-0.5);
	m_wall05.SetScaleZ(2.2f);


	//©Л
	m_wall10.SetPlayerCharacter(L"MAP_WALL", 900, 0, 402);
	m_wall10.SetScaleZ(2.2f);
	m_wall11.SetPlayerCharacter(L"MAP_WALL", 900, 0, 575);
	m_wall11.SetScaleZ(2.2f);
	m_wall12.SetPlayerCharacter(L"MAP_WALL", 900, 0, 748);
	m_wall12.SetScaleZ(2.2f);
	m_wall13.SetPlayerCharacter(L"MAP_WALL", 900, 0, -402);
	m_wall13.SetScaleZ(2.2f);
	m_wall14.SetPlayerCharacter(L"MAP_WALL", 900, 0, -575);
	m_wall14.SetScaleZ(2.2f);
	m_wall15.SetPlayerCharacter(L"MAP_WALL", 900, 0, -748);
	m_wall15.SetScaleZ(2.2f);

	//го
	m_wall20.SetPlayerCharacter(L"MAP_WALL", 402, 0, -900);
	m_wall20.SetRotationY(PI*0.5);
	m_wall20.SetScaleZ(2.2f);
	m_wall21.SetPlayerCharacter(L"MAP_WALL", 575, 0, -900);
	m_wall21.SetRotationY(PI*0.5);
	m_wall21.SetScaleZ(2.2f);
	m_wall22.SetPlayerCharacter(L"MAP_WALL", 748, 0, -900);
	m_wall22.SetRotationY(PI*0.5);
	m_wall22.SetScaleZ(2.2f);
	m_wall23.SetPlayerCharacter(L"MAP_WALL", -402, 0, -900);
	m_wall23.SetRotationY(PI*0.5);
	m_wall23.SetScaleZ(2.2f);
	m_wall24.SetPlayerCharacter(L"MAP_WALL", -575, 0, -900);
	m_wall24.SetRotationY(PI*0.5);
	m_wall24.SetScaleZ(2.2f);
	m_wall25.SetPlayerCharacter(L"MAP_WALL", -748, 0, -900);
	m_wall25.SetRotationY(PI*0.5);
	m_wall25.SetScaleZ(2.2f);


	//аб
	m_wall30.SetPlayerCharacter(L"MAP_WALL", -900, 0, 402);
	m_wall30.SetRotationY(-PI);
	m_wall30.SetScaleZ(2.2f);
	m_wall31.SetPlayerCharacter(L"MAP_WALL", -900, 0, 575);
	m_wall31.SetRotationY(-PI);
	m_wall31.SetScaleZ(2.2f);
	m_wall32.SetPlayerCharacter(L"MAP_WALL", -900, 0, 748);
	m_wall32.SetRotationY(-PI);
	m_wall32.SetScaleZ(2.2f);
	m_wall33.SetPlayerCharacter(L"MAP_WALL", -900, 0, -402);
	m_wall33.SetRotationY(-PI);
	m_wall33.SetScaleZ(2.2f);
	m_wall34.SetPlayerCharacter(L"MAP_WALL", -900, 0, -575);
	m_wall34.SetRotationY(-PI);
	m_wall34.SetScaleZ(2.2f);
	m_wall35.SetPlayerCharacter(L"MAP_WALL", -900, 0, -748);
	m_wall35.SetRotationY(-PI);
	m_wall35.SetScaleZ(2.2f);


#pragma endregion

#pragma region SetMatrix

	m_blacksmith.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_fountain.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_church.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_towerRound.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_windmill.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);


	m_wall00.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall01.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall02.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall03.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall04.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall05.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall10.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall11.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall12.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall13.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall14.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall15.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall20.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall21.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall22.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall23.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall24.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall25.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall30.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall31.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall32.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall33.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall34.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_wall35.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);


	m_towerTR.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_towerTL.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_towerBR.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_towerBL.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);

	m_tower0.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_tower1.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_tower2.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_tower3.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_tower4.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_tower5.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_tower6.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_tower7.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);

	m_house00.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_house01.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_house02.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);

	m_house10.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_house11.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_house12.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);

	m_house20.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_house21.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_house22.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);

	m_house30.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_house31.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_house32.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);


#pragma endregion




	return true;
}

bool GameMap::Frame(const float& spf, const float& accTime)	noexcept
{

#pragma region Frame

	m_blacksmith.Frame(Timer::SPF, Timer::AccumulateTime);
	m_fountain.Frame(Timer::SPF, Timer::AccumulateTime);
	m_church.Frame(Timer::SPF, Timer::AccumulateTime);
	m_towerRound.Frame(Timer::SPF, Timer::AccumulateTime);
	m_windmill.Frame(Timer::SPF, Timer::AccumulateTime);






	m_wall00.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall01.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall02.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall03.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall04.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall05.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall10.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall11.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall12.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall13.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall14.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall15.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall20.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall21.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall22.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall23.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall24.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall25.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall30.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall31.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall32.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall33.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall34.Frame(Timer::SPF, Timer::AccumulateTime);
	m_wall35.Frame(Timer::SPF, Timer::AccumulateTime);

	m_towerTR.Frame(Timer::SPF, Timer::AccumulateTime);
	m_towerTL.Frame(Timer::SPF, Timer::AccumulateTime);
	m_towerBR.Frame(Timer::SPF, Timer::AccumulateTime);
	m_towerBL.Frame(Timer::SPF, Timer::AccumulateTime);

	m_tower0.Frame(Timer::SPF, Timer::AccumulateTime);
	m_tower1.Frame(Timer::SPF, Timer::AccumulateTime);
	m_tower2.Frame(Timer::SPF, Timer::AccumulateTime);
	m_tower3.Frame(Timer::SPF, Timer::AccumulateTime);
	m_tower4.Frame(Timer::SPF, Timer::AccumulateTime);
	m_tower5.Frame(Timer::SPF, Timer::AccumulateTime);
	m_tower6.Frame(Timer::SPF, Timer::AccumulateTime);
	m_tower7.Frame(Timer::SPF, Timer::AccumulateTime);

	m_house00.Frame(Timer::SPF, Timer::AccumulateTime);
	m_house01.Frame(Timer::SPF, Timer::AccumulateTime);
	m_house02.Frame(Timer::SPF, Timer::AccumulateTime);

	m_house10.Frame(Timer::SPF, Timer::AccumulateTime);
	m_house11.Frame(Timer::SPF, Timer::AccumulateTime);
	m_house12.Frame(Timer::SPF, Timer::AccumulateTime);

	m_house20.Frame(Timer::SPF, Timer::AccumulateTime);
	m_house21.Frame(Timer::SPF, Timer::AccumulateTime);
	m_house22.Frame(Timer::SPF, Timer::AccumulateTime);

	m_house30.Frame(Timer::SPF, Timer::AccumulateTime);
	m_house31.Frame(Timer::SPF, Timer::AccumulateTime);
	m_house32.Frame(Timer::SPF, Timer::AccumulateTime);
#pragma endregion

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

#pragma region Render


	m_blacksmith.Render(DxManager::GetDContext());
	m_fountain.Render(DxManager::GetDContext());
	m_church.Render(DxManager::GetDContext());
	m_towerRound.Render(DxManager::GetDContext());
	m_windmill.Render(DxManager::GetDContext());


	m_wall00.Render(DxManager::GetDContext());
	m_wall01.Render(DxManager::GetDContext());
	m_wall02.Render(DxManager::GetDContext());
	m_wall03.Render(DxManager::GetDContext());
	m_wall04.Render(DxManager::GetDContext());
	m_wall05.Render(DxManager::GetDContext());
	m_wall10.Render(DxManager::GetDContext());
	m_wall11.Render(DxManager::GetDContext());
	m_wall12.Render(DxManager::GetDContext());
	m_wall13.Render(DxManager::GetDContext());
	m_wall14.Render(DxManager::GetDContext());
	m_wall15.Render(DxManager::GetDContext());
	m_wall20.Render(DxManager::GetDContext());
	m_wall21.Render(DxManager::GetDContext());
	m_wall22.Render(DxManager::GetDContext());
	m_wall23.Render(DxManager::GetDContext());
	m_wall24.Render(DxManager::GetDContext());
	m_wall25.Render(DxManager::GetDContext());
	m_wall30.Render(DxManager::GetDContext());
	m_wall31.Render(DxManager::GetDContext());
	m_wall32.Render(DxManager::GetDContext());
	m_wall33.Render(DxManager::GetDContext());
	m_wall34.Render(DxManager::GetDContext());
	m_wall35.Render(DxManager::GetDContext());




	m_towerTR.Render(DxManager::GetDContext());
	m_towerTL.Render(DxManager::GetDContext());
	m_towerBR.Render(DxManager::GetDContext());
	m_towerBL.Render(DxManager::GetDContext());

	m_tower0.Render(DxManager::GetDContext());
	m_tower1.Render(DxManager::GetDContext());
	m_tower2.Render(DxManager::GetDContext());
	m_tower3.Render(DxManager::GetDContext());
	m_tower4.Render(DxManager::GetDContext());
	m_tower5.Render(DxManager::GetDContext());
	m_tower6.Render(DxManager::GetDContext());
	m_tower7.Render(DxManager::GetDContext());

	m_house00.Render(DxManager::GetDContext());
	m_house01.Render(DxManager::GetDContext());
	m_house02.Render(DxManager::GetDContext());

	m_house10.Render(DxManager::GetDContext());
	m_house11.Render(DxManager::GetDContext());
	m_house12.Render(DxManager::GetDContext());

	m_house20.Render(DxManager::GetDContext());
	m_house21.Render(DxManager::GetDContext());
	m_house22.Render(DxManager::GetDContext());

	m_house30.Render(DxManager::GetDContext());
	m_house31.Render(DxManager::GetDContext());
	m_house32.Render(DxManager::GetDContext());
#pragma endregion


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
