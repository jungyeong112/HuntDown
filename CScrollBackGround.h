#pragma once
#include "pch.h"
#include "Define.h"

// TransparentBlt 필요
#pragma comment(lib, "Msimg32.lib")

class CScrollBackGround
{
public:
    CScrollBackGround();
    ~CScrollBackGround();

    // tileDC  : 타일 BMP가 SelectObject 되어있는 메모리 DC 
    // tileW/H : BMP 실제 폭/높이
    // parallax: 패럴랙스 비율
    // yWorld  : 월드 Y(그릴 기준선)
    void Initialize(HDC tileDC, int tileW, int tileH, float parallax, int yWorld);

    // hDC  : 목적지(백버퍼) DC
    // camX : 카메라 X (ScreenManager::Instance().GetCamerPos().x)
    void Render(HDC hDC, int camX);

private:
    static int SafeMod(int a, int b) {
        if (b <= 0) return 0;
        int m = a % b;
        if (m < 0) m += b;
        return m;
    }

private:
    HDC   m_tileDC = nullptr; // 소스 DC (타일 BMP 선택 상태)
    int   m_tileW = 0;
    int   m_tileH = 0;
    float m_parallax = 0.12f;
    int   m_yWorld = 0;
};
