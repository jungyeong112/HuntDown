#include "pch.h"
#include "CUI_StageScore.h"
#include "CObjManager.h"
#include "CUIManager.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"

CUI_StageScore::CUI_StageScore()
{
}

CUI_StageScore::~CUI_StageScore()
{
}

void CUI_StageScore::Initialize()
{
	m_tInfo = { 0,0,WINCX,WINCY };
	Update_Rect();
	CSoundMgr::Get_Instance()->PlayBGM(L"AreaComplete.wav", 0.8f);
}

int CUI_StageScore::Update()
{
	return 0;
}

void CUI_StageScore::LateUpdate()
{
}

void CUI_StageScore::Render(HDC hDC)
{
	Graphics g(hDC);
	g.SetCompositingMode(CompositingModeSourceOver);
	g.SetCompositingQuality(CompositingQualityHighQuality);

	const int dstX = static_cast<int>(CObjManager::Get_Instance()->Get_PlayerPos().fx - 400.f);
	const int dstY = 0;
	const int dstW = WINCX;
	const int dstH = WINCY;

	// 알파 128 (50%) 검은색 브러시
	SolidBrush overlay(Color(180, 0, 0, 0));
	g.FillRectangle(&overlay, Rect(dstX, dstY, dstW, dstH));

	wstring strHunter = L"";
	int iEnemyKill = CUIManager::Get_Instance()->Get_EnemyKill();
	int iMaxKill = 30;
	if (iEnemyKill == 0)
		strHunter = L"Succes_Hunter";
	else
		strHunter = L"Fail_Hunter";

	HDC hStHDC = CBmpMgr::Get_Instance()->Find_Image(strHunter.c_str());
	GdiTransparentBlt(hDC,
		dstX + 290, dstY + 160,
		200, 50,
		hStHDC,
		0,
		0,
		85, 15,                          //복사할 가로 세로 사이즈
		RGB(255, 0, 255));


	wstring HuntEnemy[5] = {};
	HuntEnemy[0] = (iEnemyKill >= iMaxKill) ? to_wstring((iEnemyKill/10) % 10) : L"Kill_" + to_wstring((iEnemyKill/10) % 10);
	HuntEnemy[1] = (iEnemyKill >= iMaxKill) ? to_wstring(iEnemyKill  % 10) : L"Kill_" + to_wstring(iEnemyKill % 10);
	HuntEnemy[2] = (iEnemyKill >= iMaxKill) ? L"Success_Slash" : L"Fail_Slash";
	HuntEnemy[3] = (iEnemyKill >= iMaxKill) ? to_wstring((iMaxKill/10) % 10) : L"Kill_" + to_wstring((iMaxKill /10) % 10);
	HuntEnemy[4] = (iEnemyKill >= iMaxKill) ? to_wstring(iMaxKill % 10) : L"Kill_" + to_wstring(iMaxKill % 10);

	int iOffsetX2 = 15.f;
	int iOffsetY2 = 0;
	int iYSize2 = 25;
	for (int i = 0; i < 5; ++i)  //숫자 렌더
	{
		HDC hHuntDC = CBmpMgr::Get_Instance()->Find_Image(HuntEnemy[i].c_str());
		GdiTransparentBlt(hDC,
			dstX + 380 + (i * iOffsetX2), dstY + 180 + iOffsetY2,
			13, iYSize2,
			hHuntDC,
			0,
			0,
			6, 10,                          //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}







	//수집품  
	wstring strCollection = L"";
	int iCollection = CUIManager::Get_Instance()->Get_Collection();
	int MaxCollecion = 3;
	if (iCollection == MaxCollecion)
		strCollection = L"Succes_Collection";
	else
		strCollection = L"Fail_Collection";

	HDC hStDC = CBmpMgr::Get_Instance()->Find_Image(strCollection.c_str());
	GdiTransparentBlt(hDC,
		dstX + 310, dstY + 220,
		200, 50,
		hStDC,
		0,
		0,
		85, 15,                          //복사할 가로 세로 사이즈
		RGB(255, 0, 255));


	wstring Collcetion[3] = {};
	Collcetion[0] = (iCollection >= MaxCollecion) ? to_wstring(iCollection % 10) : L"Kill_" + to_wstring(iCollection % 10);
	Collcetion[1] = (iCollection >= MaxCollecion) ? L"Success_Slash" : L"Fail_Slash";
	Collcetion[2] = (iCollection >= MaxCollecion) ? to_wstring(3) : L"Kill_3";

	int iOffsetX1 = 15.f;
	int iOffsetY1 = 0;
	int iYSize1 = 25;
	for (int i = 0; i < 3; ++i)  //숫자 렌더
	{
		HDC hTimeDC = CBmpMgr::Get_Instance()->Find_Image(Collcetion[i].c_str());
		GdiTransparentBlt(hDC,
			dstX + 380 + (i * iOffsetX1), dstY + 240 + iOffsetY1,
			13, iYSize1,
			hTimeDC,
			0,
			0,
			6, 10,                          //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}



	//걸린 시간
	float ftime = CUIManager::Get_Instance()->Get_ClearTime();
	int iMin = ((int)ftime / 60);
	int iSecond = (int)ftime % 60;
	wstring strTime[4] = {};
	strTime[0] = L"Kill_" + to_wstring(iMin);
	strTime[1] = L"Colon";
	strTime[2] = L"Kill_" + to_wstring((iSecond / 10) % 10);
	strTime[3] = L"Kill_" + to_wstring(iSecond % 10);


	int iOffsetX = 15.f;
	int iOffsetY = 0;
	int iYSize = 25;
	for (int i = -1; i < 3; ++i)  //숫자 렌더
	{
		if (!i)
		{
			iYSize = 15;
			iOffsetY = 5;
		}
		else
		{
			iOffsetY = 0;
			iYSize = 25;
		}

		HDC hTimeDC = CBmpMgr::Get_Instance()->Find_Image(strTime[i + 1].c_str());
		GdiTransparentBlt(hDC,
			dstX + 255 + (i * iOffsetX), dstY + 258 + iOffsetY,
			13, iYSize,
			hTimeDC,
			0,
			0,
			6, 10,                          //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}



	//생존 여부
	wstring strPlayerDie = L"";
	int iDie = CUIManager::Get_Instance()->Get_Die();
	if (iDie)
		strPlayerDie = L"Fail_Survivor";
	else
		strPlayerDie = L"Succes_Survivor";

	HDC hStDDC = CBmpMgr::Get_Instance()->Find_Image(strPlayerDie.c_str());
	GdiTransparentBlt(hDC,
		dstX + 300, dstY + 280,
		200, 50,
		hStDDC,
		0,
		0,
		85, 15,                          //복사할 가로 세로 사이즈
		RGB(255, 0, 255));


	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BountyCollection"); //타이틀

	GdiTransparentBlt(hDC,
		dstX + 200, dstY + 80,
		400, 50,
		hMemDC,
		0,
		0,
		275, 25,                          //복사할 가로 세로 사이즈
		RGB(255, 0, 255));

	HDC hFaceDC = CBmpMgr::Get_Instance()->Find_Image(L"BossFace");
	GdiTransparentBlt(hDC,
		dstX + 200, dstY + 140,
		100, 100,
		hFaceDC,
		0,
		0,
		50, 49,                          //복사할 가로 세로 사이즈
		RGB(255, 0, 255));


	HDC hClockDC = CBmpMgr::Get_Instance()->Find_Image(L"Clock");
	GdiTransparentBlt(hDC,
		dstX + 200, dstY + 250,
		30, 35,
		hClockDC,
		0,
		0,
		33, 45,                          //복사할 가로 세로 사이즈
		RGB(255, 0, 255));
}

void CUI_StageScore::Release()
{
}
