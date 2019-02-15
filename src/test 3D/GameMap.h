#pragma once
#include "IDxBasis.h"
#include "ISingleton.h"
#include "AHeroObj.h"

class HeightMap;

class GameMap : public IDxBasis, public ISingleton<GameMap>
{
private:
	HeightMap*		m_pHeightMap;

	AHeroObj		m_fountain;

	AHeroObj		m_blacksmith;
	AHeroObj		m_church;
	AHeroObj		m_towerRound;
	AHeroObj		m_windmill;


	AHeroObj		m_towerTR;
	AHeroObj		m_towerTL;
	AHeroObj		m_towerBR;
	AHeroObj		m_towerBL;


#pragma region MyWall
	AHeroObj		m_wall00;
	AHeroObj		m_wall01;
	AHeroObj		m_wall02;
	AHeroObj		m_wall03;
	AHeroObj		m_wall04;
	AHeroObj		m_wall05;

	AHeroObj		m_wall10;
	AHeroObj		m_wall11;
	AHeroObj		m_wall12;
	AHeroObj		m_wall13;
	AHeroObj		m_wall14;
	AHeroObj		m_wall15;

	AHeroObj		m_wall20;
	AHeroObj		m_wall21;
	AHeroObj		m_wall22;
	AHeroObj		m_wall23;
	AHeroObj		m_wall24;
	AHeroObj		m_wall25;

	AHeroObj		m_wall30;
	AHeroObj		m_wall31;
	AHeroObj		m_wall32;
	AHeroObj		m_wall33;
	AHeroObj		m_wall34;
	AHeroObj		m_wall35;
#pragma endregion
#pragma region MyHouse
	AHeroObj		m_house00;
	AHeroObj		m_house01;
	AHeroObj		m_house02;

	AHeroObj		m_house10;
	AHeroObj		m_house11;
	AHeroObj		m_house12;

	AHeroObj		m_house20;
	AHeroObj		m_house21;
	AHeroObj		m_house22;

	AHeroObj		m_house30;
	AHeroObj		m_house31;
	AHeroObj		m_house32;
#pragma endregion
#pragma region MyTree

	AHeroObj		m_tree00;
	AHeroObj		m_tree01;

	AHeroObj		m_tree02;
	AHeroObj		m_tree03;

	AHeroObj		m_tree04;
	AHeroObj		m_tree05;

	AHeroObj		m_tree06;
	AHeroObj		m_tree07;
#pragma endregion
#pragma region MyFence

	AHeroObj		m_fence00;
	AHeroObj		m_fence01;
	AHeroObj		m_fence02;
	AHeroObj		m_fence03;


	AHeroObj		m_fence10;
	AHeroObj		m_fence11;
	AHeroObj		m_fence12;
	AHeroObj		m_fence13;


	AHeroObj		m_fence20;
	AHeroObj		m_fence21;
	AHeroObj		m_fence22;
	AHeroObj		m_fence23;

	AHeroObj		m_fence30;
	AHeroObj		m_fence31;
	AHeroObj		m_fence32;
	AHeroObj		m_fence33;
#pragma endregion
#pragma region MyBarrel
	AHeroObj		m_barrel00;
	AHeroObj		m_barrel01;
	AHeroObj		m_barrel02;
	AHeroObj		m_barrel03;

	AHeroObj		m_barrel10;
	AHeroObj		m_barrel11;
	AHeroObj		m_barrel12;
	AHeroObj		m_barrel13;

	AHeroObj		m_barrel20;
	AHeroObj		m_barrel21;
	AHeroObj		m_barrel22;
	AHeroObj		m_barrel23;

	AHeroObj		m_barrel30;
	AHeroObj		m_barrel31;
	AHeroObj		m_barrel32;
	AHeroObj		m_barrel33;
#pragma endregion
#pragma region MywagonSack
	AHeroObj		m_wagon1;
	AHeroObj		m_wagon2;
	AHeroObj		m_wagon3;

	AHeroObj		m_Sack00;
	AHeroObj		m_Sack01;
	AHeroObj		m_Sack10;
	AHeroObj		m_Sack11;
	AHeroObj		m_Sack20;
	AHeroObj		m_Sack21;
#pragma endregion
#pragma region MyOutTree

	AHeroObj		m_oTree00;
	AHeroObj		m_oTree01;
	AHeroObj		m_oTree02;
	AHeroObj		m_oTree03;
	AHeroObj		m_oTree04;
	AHeroObj		m_oTree05;
	AHeroObj		m_oTree06;
	AHeroObj		m_oTree07;
	AHeroObj		m_oTree08;
	AHeroObj		m_oTree09;

	AHeroObj		m_oTree10;
	AHeroObj		m_oTree11;
	AHeroObj		m_oTree12;
	AHeroObj		m_oTree13;
	AHeroObj		m_oTree14;
	AHeroObj		m_oTree15;
	AHeroObj		m_oTree16;
	AHeroObj		m_oTree17;
	AHeroObj		m_oTree18;
	AHeroObj		m_oTree19;

	AHeroObj		m_oTree20;
	AHeroObj		m_oTree21;
	AHeroObj		m_oTree22;
	AHeroObj		m_oTree23;
	AHeroObj		m_oTree24;
	AHeroObj		m_oTree25;
	AHeroObj		m_oTree26;
	AHeroObj		m_oTree27;
	AHeroObj		m_oTree28;
	AHeroObj		m_oTree29;

	AHeroObj		m_oTree30;
	AHeroObj		m_oTree31;
	AHeroObj		m_oTree32;
	AHeroObj		m_oTree33;
	AHeroObj		m_oTree34;
	AHeroObj		m_oTree35;
	AHeroObj		m_oTree36;
	AHeroObj		m_oTree37;
	AHeroObj		m_oTree38;
	AHeroObj		m_oTree39;
#pragma endregion

	GameObject*		m_pTree2D;
public:
	AHeroObj		m_tower[8];
	//AHeroObj		m_tower1;
	//AHeroObj		m_tower2;
	//AHeroObj		m_tower3;
	//AHeroObj		m_tower4;
	//AHeroObj		m_tower5;
	//AHeroObj		m_tower6;
	//AHeroObj		m_tower7;

	AHeroObj		m_carpet_blacksmith;
	AHeroObj		m_carpet_church;
	AHeroObj		m_carpet_windmill;
	AHeroObj		m_carpet_towerRound;
private:
	friend class ISingleton<GameMap>;
	GameMap() = default;
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
public:
	virtual ~GameMap() = default;
};

