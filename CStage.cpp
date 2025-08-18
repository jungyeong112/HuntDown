#include "pch.h"
#include "CStage.h"
#include "CPlayer.h"
#include "CAbstarctFactory.h"
#include "CObjManager.h"
#include "CollisionManager.h"
#include "CGround1.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CPotion.h"
#include "CBox.h"
#include "Weapon_Item.h"
#include "CBaseEnemy.h"


bool DebugMode = false;
CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CObjManager::Get_Instance()->Add_Object(PLAYER, CAbstractFactory<CPlayer>::Create());


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Statge_1Bg.bmp", L"Bg1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet.bmp", L"Pistol_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/AK_Bullet.bmp", L"AK_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Box_Anim.bmp", L"Box_Anim");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Reg_R.bmp", L"1PlayerLeg"); //오른쪽 다리
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Reg_L.bmp", L"-1PlayerLeg"); //왼쪽 다리 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MainGun_Body_R.bmp", L"1PlayerBody0"); //오른쪽, 상체, 권총 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MainGun_Body_L.bmp", L"-1PlayerBody0");//왼쪽 , 상체, 권총
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Uzi_Body_R.bmp", L"1PlayerBody1"); //오른쪽 상체 Uzi
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Uzi_Body_L.bmp", L"-1PlayerBody1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shot_Body_R.bmp", L"1PlayerBody2"); //오른쪽 상체 Shotgun
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shot_Body_L.bmp", L"-1PlayerBody2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/AK_Body_R.bmp", L"1PlayerBody3");  //오른쪽 상체  AK-47
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/AK_Body_L.bmp", L"-1PlayerBody3"); //왼쪽 , 상체 AK
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item_Potion.bmp", L"Potion");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item_Weapon.bmp", L"Item_Weapon");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shootgun_BulletEffect.bmp", L"Shotgun_BulletEffect"); //샷건 발사 효과
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Throw_Knife.bmp", L"Throw_Knife");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Explosion.bmp", L"Explosion");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Grenade.bmp", L"Grenade");

	CCollisionManager::Instance().Clear();
	CCollisionManager::Instance().ActiveCollision(PLAYER, GROUND);
	CCollisionManager::Instance().ActiveCollision(PLAYER, FLAT_GROUND);
	CCollisionManager::Instance().ActiveCollision(PLAYER, HIDE_AREA);
	CCollisionManager::Instance().ActiveCollision(PLAYER, WALL);
	CCollisionManager::Instance().ActiveCollision(BULLET, WALL);
	CCollisionManager::Instance().ActiveCollision(PLAYER, ITEM);
	CCollisionManager::Instance().ActiveCollision(BULLET, BOX);
	CCollisionManager::Instance().ActiveCollision(BULLET, ENEMY);
	CCollisionManager::Instance().ActiveCollision(PLAYER, BOX);
	CCollisionManager::Instance().ActiveCollision(EFFECT, GROUND);
	CCollisionManager::Instance().ActiveCollision(EFFECT, WALL);
	CCollisionManager::Instance().ActiveCollision(EFFECT, BOX);
	CCollisionManager::Instance().ActiveCollision(ENEMY, HIDE_AREA);
	CCollisionManager::Instance().ActiveCollision(PLAYER, ENEMYBULLET);
	CCollisionManager::Instance().ActiveCollision(ENEMYBULLET, BOX);
	CCollisionManager::Instance().ActiveCollision(ENEMY, GROUND);
	CCollisionManager::Instance().ActiveCollision(ENEMY, FLAT_GROUND);


	CCollisionManager::Instance().SetObjList(CObjManager::Get_Instance()->Get_List());

	CreateMap();


}

void CStage::Update()
{
	CObjManager::Get_Instance()->Update();
	if (CKeyMgr::Get_Instance()->Get_Instance()->Key_Down('D'))
	{
		DebugMode = !DebugMode;
	}
}

void CStage::LateUpdate()
{
	CObjManager::Get_Instance()->LateUpdate();
}

void CStage::Render(HDC hDC)
{
	HDC fixBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Bg1");

	BitBlt(hDC, 0, 0, 3950, WINCY, fixBackDC, 0, 155, SRCCOPY);                   //여기서 배경 사진 폭만큼 그려야함.
	CObjManager::Get_Instance()->Render(hDC);
}

void CStage::CreateMap()
{

	list<CObj*>* m_ObjList = CObjManager::Get_Instance()->Get_List();

	auto ObjMgr = CObjManager::Get_Instance();

	auto pGround = CAbstractFactory<CGround1>::Create();

	pGround->Set_Pos(200.f, 455.f);
	pGround->Set_Size(430.f, 190.f);

	m_ObjList[GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(405.f, 460.f);
	pGround->Set_Size(37.f, 195.f);

	m_ObjList[WALL].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(1912.f, 495.f);
	pGround->Set_Size(2995.f, 95.f);

	m_ObjList[GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(1260.f, 365.f);
	pGround->Set_Size(400.f, 10.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);


	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3420.f, 350.f);
	pGround->Set_Size(50.f, 200.f);

	m_ObjList[WALL].push_back(pGround);


	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3320.f, 410.f);
	pGround->Set_Size(83.f, 80.f);

	m_ObjList[GROUND].push_back(pGround);


	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3180.f, 320.f);
	pGround->Set_Size(165.f, 15.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3060.f, 235.f);
	pGround->Set_Size(90.f, 15.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3500.f, 235.f);
	pGround->Set_Size(565.f, 15.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3670.f, 470.f);
	pGround->Set_Size(470.f, 175.f);

	m_ObjList[GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(1210.f, 410.f);
	pGround->Set_Size(130.f, 70.f);

	m_ObjList[HIDE_AREA].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(2680.f, 420.f);
	pGround->Set_Size(46.f, 53.f);

	m_ObjList[HIDE_AREA].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(2790.f, 420.f);
	pGround->Set_Size(55.f, 53.f);

	m_ObjList[HIDE_AREA].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3180.f, 415.f);
	pGround->Set_Size(57.f, 55.f);

	m_ObjList[HIDE_AREA].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3510.f, 350.f);
	pGround->Set_Size(54.f, 54.f);

	m_ObjList[HIDE_AREA].push_back(pGround);

	ObjMgr->Add_Object(ITEM, CAbstractFactory<CPotion>::Create(500.f, 420.f, 1));
	ObjMgr->Add_Object(BOX, CAbstractFactory<CBox>::CreateBox(2500.f, 420.f, GAS_BARREL, 14, 7));
	ObjMgr->Add_Object(BOX, CAbstractFactory<CBox>::CreateBox(2700.f, 420.f, WOOD_BOX, 14, 7));

	auto pItem = CAbstractFactory<Weapon_Item>::Create();
	static_cast<Weapon_Item*>(pItem)->Set_Tag(ITEM_AK47);
	pItem->Set_BodyFrame(0, 3, 2, 200.f);
	pItem->Set_Pos(600.f, 420.f);
	m_ObjList[ITEM].push_back(pItem);

	pItem = CAbstractFactory<Weapon_Item>::Create();
	static_cast<Weapon_Item*>(pItem)->Set_Tag(ITEM_UZI);
	pItem->Set_BodyFrame(0, 3, 0, 200.f);
	pItem->Set_Pos(800.f, 420.f);
	m_ObjList[ITEM].push_back(pItem);

	pItem = CAbstractFactory<Weapon_Item>::Create();
	static_cast<Weapon_Item*>(pItem)->Set_Tag(ITEM_SHOTGUN);
	pItem->Set_BodyFrame(0, 3, 1, 200.f);
	pItem->Set_Pos(1000.f, 420.f);
	m_ObjList[ITEM].push_back(pItem);


	ObjMgr->Add_Object(ENEMY, CAbstractFactory<CShootingEnemy>::Create(2000.f,420.f,1));

}

void CStage::Release()
{

}

