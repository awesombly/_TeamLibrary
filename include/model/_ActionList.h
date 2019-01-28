#pragma once

#define MatrixPATH L"../../data/Model/MatrixViewer.hlsl"
#define TablePATH  L"../../data/Model/CharacterTable.cit"

//////////////////////////////////////////////////////////////////////////
#define  Paladin			L"paladin"
//ANIM_COUNT  10
#define	 Paladin_BLOCK		L"Paladin_Block.Mat"
#define	 Paladin_ATTACK		L"Paladin_Atk.Mat"
#define	 Paladin_THROW		L"Paladin_Throw.Mat"
#define	 Paladin_LEFT		L"Paladin_Left.Mat"
#define	 Paladin_RIGHT		L"Paladin_Right.Mat"
#define	 Paladin_WALK		L"Paladin_Walk.Mat"
#define	 Paladin_BACK		L"Paladin_Back.Mat"
#define	 Paladin_RUN		L"Paladin_Run.Mat"
#define	 Paladin_POWERUP	L"Paladin_POWERUP.Mat"
#define	 Paladin_IDLE		L"Paladin_idle.Mat"


//////////////////////////////////////////////////////////////////////////
//ZombieKing
#define  Zombie_KING				L"zombie_king"
//ANIM_COUNT  5
#define	 Zombie_KING_ROAR			L"ZOMBIE_KING_ROAR.Mat"
#define	 Zombie_KING_JUMP_ATTACK	L"ZOMBIE_KING_JUMP_ATTACK.Mat"
#define	 Zombie_KING_ATTACK			L"ZOMBIE_KING_ATTACK.Mat"
#define	 Zombie_KING_WALK			L"ZOMBIE_KING_WALK.Mat"
#define	 Zombie_KING_IDLE			L"ZOMBIE_KING_IDLE.Mat"


//////////////////////////////////////////////////////////////////////////
//Zombie
#define  Zombie				L"zombie"
//ANIM_COUNT  5
#define	 Zombie_RUN			L"zombie_run.Mat"
#define	 Zombie_CRAWL		L"zombie_crawl.Mat"
#define	 Zombie_ATTACK		L"zombie_attack.Mat"
#define	 Zombie_WALK		L"zombie_walk.Mat"
#define	 Zombie_IDLE		L"zombie_idle.Mat"

//////////////////////////////////////////////////////////////////////////
//Zombie
#define  ZombieEX				L"zombieEX"
//ANIM_COUNT  5
#define	 ZombieEX_RUN			L"zombieEX_run.Mat"
#define	 ZombieEX_ATTACK		L"zombieEX_attack.Mat"
#define	 ZombieEX_IDLE			L"zombieEX_idle.Mat"



//////////////////////////////////////////////////////////////////////////
//NPC_BIRD
//ANIM_COUNT  1
#define	 NPC_Bird			L"NPC_bird"
//#define	 NPC_Bird_IDLE		L"NPC_bird.Mat"


//NPC_Chicken
//ANIM_COUNT  1
#define	 NPC_Chicken			L"NPC_Chicken"
//#define	 NPC_Chicken_IDLE		L"chicken.Mat"





//ITEM_Dagger
//ANIM_COUNT  1
#define	 ITEM_Dagger			L"ITEM_Dagger"

//#define	 NPC_Chicken_IDLE		L"dagger.Mat"




//참고 2 : 추가 방법 및 예제
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include AHero.h


//////////////////////////////////Init()

//I_CHARMGR.Init();
//if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), _T("../../data/Model/CharacterTable.cit")))		//경로 중요함
//{
//	return false;
//}

//ex) AHero = m_Hero
//m_Hero.SetPlayerCharacter(L"guard", 0, 0, 0);			//캐릭터  초기설정 설정 ---->> // 이름 중요함 -> 매니저를 통해서 불러움


//애님 명령어
//m_Hero.SetANIM_Loop(Guard_DANCE3);						//재생애니메이션 설정

//m_Hero.m_objType = EObjType::Object;


//////////////////////////////////Frame()
//m_Hero.Frame(Timer::SPF, Timer::AccumulateTime);



//////////////////////////////////Render()
//m_Hero.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);	(월드,뷰,투영)
//m_Hero.Render(DxManager::GetDContext());																									(랜더)


//////////////////////////////////////////////////