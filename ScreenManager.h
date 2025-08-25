#pragma once

#include "pch.h"
#include "Define.h"



class CScreenManager
{
    int ScreenCX = WINCX;
    int ScreenCY = WINCY;
    int ScreenFPS = WINFPS;
    int MapSize = 16;

public:
    CScreenManager(const CScreenManager&)               = delete;
    CScreenManager& operator=(const CScreenManager&)    = delete;

public:
    void Initialize() 
    {
        if (!m_frontHDC) {
            m_frontHDC          = GetDC(g_hWnd);
           // m_hFrontBitmap      = CreateCompatibleBitmap(m_frontHDC, MapSize *ScreenCX, MapSize * ScreenCY);
           // m_hOldFrontBitmap   = (HBITMAP)SelectObject(m_frontHDC, m_hFrontBitmap);
        }
        if (!m_backHDC) {
            m_backHDC           = CreateCompatibleDC(m_frontHDC);
            m_hBackBitmap       = CreateCompatibleBitmap(m_frontHDC, MapSize * ScreenCX, MapSize * ScreenCY);
            m_hOldBackBitmap    = (HBITMAP)SelectObject(m_backHDC, m_hBackBitmap);
        }
      
    };
    
    void Update(double _deltaTime) {};
    //    double _duration = (1.0 / ScreenFPS);

    //    ////m_dElapsedTime += _deltaTime;
    //    //if (m_dElapsedTime >= _duration) {
    //    //    m_dElapsedTime -= _duration;
    //    //    if (m_dElapsedTime >= _duration) m_iCatchUpCnt++;
    //    //}
    //    //else
    //    //    m_iCatchUpCnt = 0;

    //    //if (m_iCatchUpCnt > 3) {
    //    //    m_iCatchUpCnt = 0;
    //    //    m_dElapsedTime = 0.0;
    //    //}
    //};

    void Clear() {
        PatBlt(m_backHDC,
            m_posCamera.x,  
            m_posCamera.y,
            ScreenCX, ScreenCY,
            WHITENESS);
    }

  

    void Present() {
        BitBlt(m_frontHDC, 0, 0, ScreenCX, ScreenCY, m_backHDC, m_posCamera.x, m_posCamera.y, SRCCOPY);
    };

    static CScreenManager& Instance() {
        static CScreenManager instance;
        return instance;
    }

    HDC GetHDC() const { return m_backHDC; }

    void SetCameraPos(POINT _cameraPos, bool _bClampingMap = true) { 
        m_posCamera = _cameraPos;
        if (_bClampingMap) {
            m_posCamera.x = clamp((int)m_posCamera.x, 0, m_iStageSize-WINCX);
            m_posCamera.y = clamp((int)m_posCamera.y, 0, MapSize * WINCY );
        }
        SetCameraRect();
    }

    POINT GetCamerPos() const { return m_posCamera; }
    RECT  GetCamRect() const { return m_camRect; }
    void  Set_StageSize(int iDst) { m_iStageSize = iDst; }
    void  SetCameraRect() { m_camRect = { m_posCamera.x,m_posCamera.y , m_posCamera.x + ScreenCX,m_posCamera.y + ScreenCY }; }
private:
    CScreenManager() {
        m_frontHDC = m_backHDC = nullptr;
        m_hFrontBitmap = m_hBackBitmap = m_hOldFrontBitmap = m_hOldBackBitmap = nullptr;
        m_posCamera = { 0,0 };
    };
    ~CScreenManager() { Release(); };
    
    void Release() {
        SelectObject(m_frontHDC, m_hOldFrontBitmap);
        DeleteObject(m_hFrontBitmap);
        ReleaseDC(g_hWnd, m_frontHDC);

        SelectObject(m_backHDC, m_hOldBackBitmap);
        DeleteObject(m_hBackBitmap);
        DeleteDC(m_backHDC);
    };

    POINT   m_posCamera;
    RECT    m_camRect;

    HDC     m_frontHDC;
    HDC     m_backHDC;

    HBITMAP m_hFrontBitmap;
    HBITMAP m_hBackBitmap;

    HBITMAP m_hOldFrontBitmap;
    HBITMAP m_hOldBackBitmap;

    int m_iStageSize{ 0 };

    //double		m_dElapsedTime;
    //unsigned	m_iCatchUpCnt;

};

