#include "pch.h"
#include "CUI_StageScore.h"
#include "CObjManager.h"
#include "CUIManager.h"
#include "CBmpMgr.h"


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


	//수집품  
	wstring strCollection = L"";
	int iCollection = CUIManager::Get_Instance()->Get_Collection();
	if (iCollection >= 3)
		strCollection = L"Success_Collection";
	else
		strCollection = L"Fail_Collection";


	//걸린 시간
	float ftime = CUIManager::Get_Instance()->Get_ClearTime();
	int iMin = ((int)ftime / 60);
	int iSecond = (int)ftime % 60;
	wstring strTime[4] = {};
	strTime[0] = L"Kill_" + to_wstring(iMin);
	strTime[1] = L"Colon";
	strTime[2] = L"Kill_" + to_wstring((iSecond / 100) % 10+1);
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
		
		HDC hTimeDC = CBmpMgr::Get_Instance()->Find_Image(strTime[i+1].c_str());
		GdiTransparentBlt(hDC,
			dstX + 255 + (i*iOffsetX), dstY + 258 + iOffsetY,
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
		strPlayerDie = L"Fail_Survivol";
	else
		strPlayerDie = L"Success_Survivol";


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

	HDC hStDC = CBmpMgr::Get_Instance()->Find_Image(L"Stamp");
	GdiTransparentBlt(hDC,
		dstX + 300, dstY + 140,
		300, 200,
		hStDC,
		0,
		0,
		100, 50,                          //복사할 가로 세로 사이즈
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
