#pragma once
#include "Component.h"
#include "AHeroObj.h"
#include "HeightMap.h"

class GameMap : public Component
{
	HeightMap*		m_pHeightMap;

	AHeroObj		m_fountain;

	AHeroObj		m_blacksmith;
	AHeroObj		m_church;
	AHeroObj		m_towerRound;
	AHeroObj		m_windmill;

	AHeroObj		m_tower0;
	AHeroObj		m_tower1;
	AHeroObj		m_tower2;
	AHeroObj		m_tower3;

	AHeroObj		m_tower4;
	AHeroObj		m_tower5;
	AHeroObj		m_tower6;
	AHeroObj		m_tower7;


	AHeroObj		m_towerTR;
	AHeroObj		m_towerTL;
	AHeroObj		m_towerBR;
	AHeroObj		m_towerBL;


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
public:
//	virtual void Update()										noexcept override;
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone()									noexcept override;

public:
	GameMap();
	virtual ~GameMap();
};

