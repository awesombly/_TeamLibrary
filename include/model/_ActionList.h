#pragma once

#define MatrixPATH L"../../data/Model/MatrixViewer.hlsl"
#define TablePATH  L"../../data/Model/CharacterTable.cit"

//���� 0 : ĳ���� �̸�
//*CHARACTERNAME		guard
//*CHARACTERNAME		zombie

//���� 1 : �׼Ǹ���Ʈ
//////////////////////////////////////////////////////////////////////////
//Guard
//ANIM_COUNT  9
#define  Guard				L"guard"
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
//Zombie
//ANIM_COUNT  9
#define  Zombie				L"zombie"
#define	 Zombie_DANCE1		L"zombie_dance1.Mat"
#define	 Zombie_DANCE2		L"zombie_dance2.Mat"
#define	 Zombie_RUN			L"zombie_run.Mat"
#define	 Zombie_FLY			L"zombie_fly.Mat"
#define	 Zombie_LEFT		L"zombie_left.Mat"
#define	 Zombie_RIGHT		L"zombie_right.Mat"
#define	 Zombie_BACKWARD	L"zombie_back.Mat"
#define	 Zombie_FORWARD		L"zombie_walk.Mat"
#define	 Zombie_IDLE		L"zombie_idle.Mat"







//NPC_NPC_NPC_NPC_NPC_NPC_NPC
//NPC_NPC_NPC_NPC_NPC_NPC_NPC
//NPC_NPC_NPC_NPC_NPC_NPC_NPC
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
//m_Hero.SetANIM(Guard_DANCE3);						//����ִϸ��̼� ����

//m_Hero.m_objType = EObjType::Object;


//////////////////////////////////Frame()
//m_Hero.Frame(Timer::SPF, Timer::AccumulateTime);



//////////////////////////////////Render()
//m_Hero.SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);	(����,��,����)
//m_Hero.Render(DxManager::GetDContext());																									(����)


//////////////////////////////////////////////////