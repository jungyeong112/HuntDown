#include "pch.h"
#include "CUI_StageScore.h"
#include "CObjManager.h"
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
	SolidBrush overlay(Color(225, 0, 0, 0));
	g.FillRectangle(&overlay, Rect(dstX, dstY, dstW, dstH));
}

void CUI_StageScore::Release()
{
}
