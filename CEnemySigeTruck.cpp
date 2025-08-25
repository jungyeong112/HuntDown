#include"pch.h"
#include "CEnemySigeTruck.h"
#include "CAbstarctFactory.h"
#include "CUIManager.h"
#include "CUI_EnemyHp.h"
#include "TimeManager.h"
#include  "CBmpMgr.h"
#include "CObjManager.h"

CEnemySigeTruck::CEnemySigeTruck()
{
   
}

CEnemySigeTruck::~CEnemySigeTruck()
{
    
}

void CEnemySigeTruck::Initialize()
{
    m_tInfo = { 0,0, 100.f,200.f };
    m_iMaxHp = m_iCurHp = 50;
    m_ObjId = ENEMY;
    m_eCurEnemyState = IDLE;
    m_fShootingRange = 350.f;

    Set_Target(CObjManager::Get_Instance()->Get_Player());
    CObj::Update_Rect();

    CUIManager::Get_Instance()->Add_UI(HP_BAR, CAbstractFactory<CUI_EnemyHp>::Create_UI(this));
    Set_LegFrame(0, 3, 0, 200.f);
}

int CEnemySigeTruck::Update()
{
    float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
    CObj::UI_ActiveTimer(fDeltaTime);
    CObj::Update_Rect();
    if (m_bIsDead)
        return OBJ_DIE;
    else
        return OBJ_NO_EVENT;
}

void CEnemySigeTruck::LateUpdate()
{
    Move_LegFrame();
}

void CEnemySigeTruck::Render(HDC hDC)
{
    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EnemySiegeTruck");

    GdiTransparentBlt(hDC,
        m_tRect.left , m_tRect.top-200,
        300.f, 300.f,                           //12는 피격 박스와 스프라이트 크기 보정
        hMemDC,
        200 * m_tLegFrame.iStart,           //원본 - 복사 시작위치x
        200 * m_tLegFrame.iMotion,           //원본 - 복사 시작위치 y
        200, 200,                                      //복사할 가로 세로 사이즈
        RGB(255, 0, 255));                                    //마젠타

    //BOXTYPE * Motion으로 시트에서 출력할거임.
    if (DebugMode)
    {
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
        HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

        Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

        SelectObject(hDC, hOldPen);
        SelectObject(hDC, hOldBrush);
    }
}

void CEnemySigeTruck::Release()
{
}

void CEnemySigeTruck::OnCollision(FCollision _Collison)
{
    if (_Collison.m_OBJID == BULLET)
    {
        if (m_iCurHp > 0)
        {
            --m_iCurHp;
            m_bUISetActive = true;
        }
        else
            m_eCurEnemyState = DIE;
    }
}
