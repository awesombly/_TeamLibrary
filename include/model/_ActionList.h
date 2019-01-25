#pragma once

#define MatrixPATH L"../../data/Model/MatrixViewer.hlsl"
#define TablePATH  L"../../data/Model/CharacterTable.cit"

//���� 0 : ĳ���� �̸�
//*CHARACTERNAME		guard
//*CHARACTERNAME		zombie

//���� 1 : �׼Ǹ���Ʈ
//////////////////////////////////////////////////////////////////////////
//Guard
//ANIM_COUNT  11
#define  Guard				L"guard"
#define	 Guard_PUNCH		L"HERO_PUNCH.Mat"
#define	 Guard_DASHJUMP		L"HERO_DASHJUMP.Mat"
#define	 Guard_THROW		L"HERO_THROW.Mat"
#define	 Guard_HAPPYWALK	L"HERO_HAPPYWALK.Mat"
#define	 Guard_LEFT			L"HERO_LEFT.Mat"
#define	 Guard_RIGHT		L"HERO_RIGHT.Mat"
#define	 Guard_BACKWARD		L"HERO_MOVE_BACK.Mat"
#define	 Guard_FORWARD		L"HERO_MOVE.Mat"
#define	 Guard_IDLE			L"HERO_IDLE.Mat"


#define	 Guard_DANCE1		L"HERO_DANCE.mat"
#define	 Guard_DANCE2		L"HERO_DANCE2.mat"
#define	 Guard_DANCE3		L"HERO_DANCE3.mat"

//////////////////////////////////////////////////////////////////////////
//ZombieKing
#define  Zombie_KING				L"zombie_king"
//ANIM_COUNT  4
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




//���� 2 : �߰� ��� �� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include AHero.h


//////////////////////////////////Init()

//I_CHARMGR.Init();
//if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), _T("../../data/Model/CharacterTable.cit")))		//��� �߿���
//{
//	return false;
//}

//ex) AHero = m_Hero
//m_Hero.SetPlayerCharacter(L"guard", 0, 0, 0);			//ĳ����  �ʱ⼳�� ���� ---->> // �̸� �߿��� -> �Ŵ����� ���ؼ� �ҷ���


//�ִ� ��ɾ�
//m_Hero.SetANIM_Loop(Guard_DANCE3);						//����ִϸ��̼� ����

//m_Hero.m_objType = EObjType::Object;


//////////////////////////////////Frame()
//m_Hero.Frame(Timer::SPF, Timer::AccumulateTime);



//////////////////////////////////Render()
//m_Hero.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);	(����,��,����)
//m_Hero.Render(DxManager::GetDContext());																									(����)


//////////////////////////////////////////////////