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
#include "CUIManager.h"
#include "CMainUI.h"
#include "CUI_PlayerHP.h"
#include "CUI_SubweaponCoolTime.h"
#include "CUI_MainSlot.h"
#include "CUI_EnemyKill.h"
#include "CSubWeapon_Item.h"
#include "CUI_SubSlot.h"
#include "CMeleeEnemy.h"
#include "CScrollBackGround.h"
#include "CSceneManager.h"
#include "CEnemySigeTruck.h"

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
	CUIManager::Get_Instance()->Set_Player(static_cast<CPlayer*>(CObjManager::Get_Instance()->Get_Player()));
	m_pFrameKey = L"StageBg1";
	Set_InsertBmp();
	Set_CollsionMask();
	CreateMap();
	CreateUI();
	Set_BackGround();

}

void CStage::Update()
{
	CObjManager::Get_Instance()->Update();
	CUIManager::Get_Instance()->Update();

	if (CKeyMgr::Get_Instance()->Get_Instance()->Key_Down('D'))
	{
		DebugMode = !DebugMode;
	}
}

void CStage::LateUpdate()
{
	if (m_iStageIndex == 0 && CObjManager::Get_Instance()->Get_PlayerPos().fx >= 3200)
	{
		Set_Stage2();
	}
	else if (m_iStageIndex == 1 && CObjManager::Get_Instance()->Get_PlayerPos().fx >=3300)
	{
		Set_Stage3();
	}
	CObjManager::Get_Instance()->LateUpdate();
	CUIManager::Get_Instance()->LateUdate();
}

void CStage::Render(HDC hDC)
{
	RECT camR = CScreenManager::Instance().GetCamRect();
	SaveDC(hDC);
	IntersectClipRect(hDC, camR.left, camR.top, camR.right, camR.bottom);

	HDC fixBackDC = CBmpMgr::Get_Instance()->Find_Image(L"FixedBG");
	HDC MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	POINT pt = CScreenManager::Instance().GetCamerPos();

	BitBlt(hDC, 0, 0, 3950, WINCY, fixBackDC, 0, 140, SRCCOPY);    //고정 배경
	m_midBld.Render(hDC, pt.x);                                    //패럴렉스 레이어 (빌딩)
	TransparentBlt(hDC, 0, m_mapY, m_mapW, m_mapH + 20, m_mapCacheDC, 0, 0, m_mapW, m_mapH, RGB(255, 0, 255)); //맵

	CUIManager::Get_Instance()->Render(hDC);
	CObjManager::Get_Instance()->Render(hDC);

	RestoreDC(hDC, -1);
}

void CStage::CreateMap()
{
	CScreenManager::Instance().Set_StageSize(3800);
	list<CObj*>* m_ObjList = CObjManager::Get_Instance()->Get_List();


	auto ObjMgr = CObjManager::Get_Instance();

	auto pGround = CAbstractFactory<CGround1>::Create();

	pGround->Set_Pos(200.f, 500.f);
	pGround->Set_Size(430.f, 190.f);

	m_ObjList[GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(395.f, 510.f);
	pGround->Set_Size(37.f, 195.f);

	m_ObjList[WALL].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(1912.f, 530.f);
	pGround->Set_Size(2995.f, 95.f);

	m_ObjList[GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(1230.f, 405.f);
	pGround->Set_Size(360.f, 10.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);


	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3115.f, 398.f);
	pGround->Set_Size(30.f, 200.f);

	m_ObjList[WALL].push_back(pGround);


	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3040, 460.f);
	pGround->Set_Size(80.f, 70.f);

	m_ObjList[GROUND].push_back(pGround);


	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(2913.f, 370.f);
	pGround->Set_Size(132.f, 15.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(2808.f, 294.f);
	pGround->Set_Size(80.f, 15.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3204.f, 293.f);
	pGround->Set_Size(527.f, 15.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();//
	pGround->Set_Pos(3333.f, 510.f);
	pGround->Set_Size(450.f, 175.f);

	m_ObjList[GROUND].push_back(pGround);//

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(1190.f, 450.f);
	pGround->Set_Size(100.f, 70.f);

	m_ObjList[HIDE_AREA].push_back(pGround);
	ObjMgr->Add_Object(BOX, CAbstractFactory<CBox>::CreateBox(1190.f, 450.f, WOOD_BOX, 14, 7));

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(2490.f, 450.f);
	pGround->Set_Size(26.f, 53.f);

	m_ObjList[HIDE_AREA].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(2590.f, 450.f);
	pGround->Set_Size(35.f, 53.f);

	m_ObjList[HIDE_AREA].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(2920.f, 450.f);
	pGround->Set_Size(57.f, 55.f);

	m_ObjList[HIDE_AREA].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create(); //
	pGround->Set_Pos(3190, 390.f);
	pGround->Set_Size(55.f, 54.f);

	m_ObjList[HIDE_AREA].push_back(pGround); //

	ObjMgr->Add_Object(ITEM, CAbstractFactory<CPotion>::Create(500.f, 450.f, 1));
	//ObjMgr->Add_Object(BOX, CAbstractFactory<CBox>::CreateBox(600.f, 455.f, GAS_BARREL, 14, 7));
	ObjMgr->Add_Object(BOX, CAbstractFactory<CBox>::CreateBox(730.f, 450.f, WOOD_BOX, 14, 7));


	//ObjMgr->Add_Object(ITEM, CAbstractFactory<Weapon_Item>::CreateMainItem(1000.f, 420.f, ITEM_SHOTGUN, 20));
	//ObjMgr->Add_Object(ITEM, CAbstractFactory<Weapon_Item>::CreateMainItem(1200.f, 420.f, ITEM_UZI, 50));
	//ObjMgr->Add_Object(ITEM, CAbstractFactory<CSubWeapon_Item>::CreateSubItem(1500.f, 420.f, ITEM_GRENADE,5));
	ObjMgr->Add_Object(ENEMY, CAbstractFactory<CShootingEnemy>::Create(1000.f, 420.f, 1));
	ObjMgr->Add_Object(ENEMY, CAbstractFactory<CMeleeEnemy>::Create(1000.f, 420.f, 1));
}

void CStage::CreateUI()
{
	CUIManager::Get_Instance()->Add_UI(MAIN_UI, CAbstractFactory<CMainUI>::Create_UI());
	CUIManager::Get_Instance()->Add_UI(MAIN_UI, CAbstractFactory<CUI_PlayerHP>::Create_UI());
	CUIManager::Get_Instance()->Add_UI(MAIN_UI, CAbstractFactory<CUI_SubweaponCoolTime>::Create_UI());
	CUIManager::Get_Instance()->Add_UI(MAIN_UI, CAbstractFactory<CUI_MainSlot>::Create_UI());
	CUIManager::Get_Instance()->Add_UI(MAIN_UI, CAbstractFactory<CUI_EnemyKill>::Create_UI());
	CUIManager::Get_Instance()->Add_UI(MAIN_UI, CAbstractFactory<CUI_SubSlot>::Create_UI());
}

void CStage::Set_InsertBmp()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/FixBg.bmp", L"FixedBG");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet.bmp", L"Pistol_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet_L.bmp", L"Pistol_Bullet_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/AK_Bullet.bmp", L"AK_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/AK_Bullet_L.bmp", L"AK_Bullet_L");
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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shootgun_BulletEffect.bmp", L"Shotgun_BulletEffect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shootgun_BulletEffect_L.bmp", L"Shotgun_BulletEffect_L");//샷건 발사 효과
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Throw_Knife.bmp", L"Throw_Knife");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Explosion.bmp", L"Explosion");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Grenade.bmp", L"Grenade");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Pistol_Enemy_R.bmp", L"Pistol_Enemy_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Pistol_Enemy_L.bmp", L"Pistol_Enemy_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Main_UI.bmp", L"Main_UI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Main_PlayerHP.bmp", L"Main_PlayerHP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI_Knife_bg.bmp", L"UI_Knife_bg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI_Knife.bmp", L"UI_Knife");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI_PISTOL_B.bmp", L"UI_PISTOL_B");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI_UZI_B.bmp", L"UI_UZI_B");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI_SHOTGUN_B.bmp", L"UI_SHOTGUN_B");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI_AK47_B.bmp", L"UI_AK47_B");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/00.bmp", L"88");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/0.bmp", L"0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/1.bmp", L"1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/2.bmp", L"2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/3.bmp", L"3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/4.bmp", L"4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/5.bmp", L"5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/6.bmp", L"6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/7.bmp", L"7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/8.bmp", L"8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/9.bmp", L"9");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Dark_0.bmp", L"Dark_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_0.bmp", L"Kill_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_1.bmp", L"Kill_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_2.bmp", L"Kill_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_3.bmp", L"Kill_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_4.bmp", L"Kill_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_5.bmp", L"Kill_5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_6.bmp", L"Kill_6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_7.bmp", L"Kill_7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_8.bmp", L"Kill_8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/kill_9.bmp", L"Kill_9");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Infinity.bmp", L"Infinity");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Main_Skul.bmp", L"Main_Skul");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/PlayerHP_bg.bmp", L"PlayerHP_bg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player_HP.bmp", L"Player_HP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/EnemyHP_bg.bmp", L"EnemyHP_bg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy_HP.bmp", L"Enemy_HP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item_Subweapon.bmp", L"Item_SubWeapon");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI_GRENADE_Q.bmp", L"UI_GRENADE_Q");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Knife_Enemy.bmp", L"Knife_Enemy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Knife_Enemy_L.bmp", L"Knife_Enemy_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/ScrollBg.bmp", L"ScrollBg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/TestBg.bmp", L"StageBg1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg2.bmp", L"StageBg2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg3.bmp", L"StageBg3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/EnemySiegeTruck.bmp", L"EnemySiegeTruck");
}

void CStage::Set_CollsionMask()
{
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
	CCollisionManager::Instance().ActiveCollision(ENEMY, BOX);
	CCollisionManager::Instance().ActiveCollision(ENEMY, FLAT_GROUND);
	CCollisionManager::Instance().ActiveCollision(PLAYER, ENEMY_MELEE);
	CCollisionManager::Instance().ActiveCollision(KICK, ENEMY);
	CCollisionManager::Instance().SetObjList(CObjManager::Get_Instance()->Get_List());
}

void CStage::Set_BackGround()
{
	Destroy_BackGroundCache();
	// 패럴랙스 레이어 초기화 
	HDC tileDC = CBmpMgr::Get_Instance()->Find_Image(L"ScrollBg");
	HBITMAP hBmp = (HBITMAP)GetCurrentObject(tileDC, OBJ_BITMAP);
	BITMAP bm{}; GetObject(hBmp, sizeof(bm), &bm);

	const int   tileW = bm.bmWidth;
	const int   tileH = bm.bmHeight;
	const float p = 0.12f;
	const int   y = 100;

	m_midBld.Initialize(tileDC, tileW, tileH, p, y);

	HDC back = CScreenManager::Instance().GetHDC();
	m_mapCacheDC = CreateCompatibleDC(back);
	m_mapCacheBMP = CreateCompatibleBitmap(back, m_mapW, m_mapH);
	m_oldMapCache = (HBITMAP)SelectObject(m_mapCacheDC, m_mapCacheBMP);

	HBRUSH hMag = CreateSolidBrush(RGB(255, 0, 255));
	RECT rc{ 0,0,m_mapW,m_mapH };
	FillRect(m_mapCacheDC, &rc, hMag);
	DeleteObject(hMag);

	HDC src = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	BITMAP bm2{};
	GetObject((HBITMAP)GetCurrentObject(src, OBJ_BITMAP), sizeof(bm2), &bm2);
	SetStretchBltMode(m_mapCacheDC, COLORONCOLOR);
	TransparentBlt(m_mapCacheDC, 0, 0, m_mapW, m_mapH, src, 0, 0, bm2.bmWidth, bm2.bmHeight, RGB(255, 0, 255));
}

void CStage::ResetStage()
{
	auto ObjMgr = CObjManager::Get_Instance();
	ObjMgr->Delete_Object(GROUND);
	ObjMgr->Delete_Object(BOX);
	ObjMgr->Delete_Object(ENEMY);
	ObjMgr->Delete_Object(FLAT_GROUND);
	ObjMgr->Delete_Object(HIDE_AREA);
	ObjMgr->Delete_Object(WALL);
	ObjMgr->Delete_Object(EFFECT);
	ObjMgr->Delete_Object(ITEM);
	ObjMgr->Delete_Object(BULLET);
	ObjMgr->Delete_Object(ENEMYBULLET);
}

void CStage::Set_Stage2()
{
	CScreenManager::Instance().Set_StageSize(3900);
	++m_iStageIndex;
	ResetStage();
	list<CObj*>* m_ObjList = CObjManager::Get_Instance()->Get_List();
	m_pFrameKey = L"StageBg2";
	Set_BackGround();
	auto pGround = CAbstractFactory<CGround1>::Create();

	pGround->Set_Pos(225.f, 602.f);
	pGround->Set_Size(460.f, 170.f);

	m_ObjList[GROUND].push_back(pGround);


	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(1694, 550.f);
	pGround->Set_Size(2466.f, 256.f);

	m_ObjList[GROUND].push_back(pGround);


	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3075.f, 400.f);
	pGround->Set_Size(455.f, 140.f);

	m_ObjList[GROUND].push_back(pGround);

	pGround = CAbstractFactory<CEnemySigeTruck>::Create();
	pGround->Set_Pos(2480.f, 400.f);
	pGround->Set_Size(250.f, 140.f);

	m_ObjList[ENEMY].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3525.f, 380.f);
	pGround->Set_Size(585.f, 300.f);

	m_ObjList[GROUND].push_back(pGround);

	m_ObjList[PLAYER].front()->Set_Pos(0, 400.f);
}

void CStage::Set_Stage3()
{
	CScreenManager::Instance().Set_StageSize(3900);
	++m_iStageIndex;
	ResetStage();
	list<CObj*>* m_ObjList = CObjManager::Get_Instance()->Get_List();
	m_pFrameKey = L"StageBg3";
	Set_BackGround();

	auto pGround = CAbstractFactory<CGround1>::Create();

	pGround->Set_Pos(2000.f, 485.f);
	pGround->Set_Size(4000.f, 200.f);

	m_ObjList[GROUND].push_back(pGround);

	pGround = CAbstractFactory<CGround1>::Create();
	pGround->Set_Pos(3214.f, 260.f);
	pGround->Set_Size(950.f, 25.f);

	m_ObjList[FLAT_GROUND].push_back(pGround);


	m_ObjList[PLAYER].front()->Set_Pos(100, 400.f);
}

void CStage::Destroy_BackGroundCache()
{
	if (m_mapCacheDC) {
		SelectObject(m_mapCacheDC, m_oldMapCache);
		DeleteObject(m_mapCacheBMP);
		DeleteDC(m_mapCacheDC);
		m_mapCacheDC = nullptr; m_mapCacheBMP = nullptr; m_oldMapCache = nullptr;
	}
}

void CStage::Release()
{


}

