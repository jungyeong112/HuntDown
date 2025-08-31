#pragma once

#include "pch.h"
#include "Define.h"
#include <algorithm>
#include <cmath>

class CScreenManager
{
    // 화면/맵 기본
    int ScreenCX = WINCX;
    int ScreenCY = WINCY;
    int ScreenFPS = WINFPS;
    int MapSize = 16;

    // 흔들림
    float m_shakeTime = 0.f;
    float m_shakeDuration = 0.15f;
    float m_shakeAmp = 8.f;
    POINT m_shakeOffset = { 0,0 };

    // 카메라 상태
    POINT m_posCamera = { 0,0 };  // 실제 카메라(렌더 소스)
    POINT m_targetCamera = { 0,0 };  // 목표 카메라(플레이어가 던짐)
    RECT  m_camRect = { 0,0,WINCX,WINCY };

    // 스테이지/경계
    int   m_iStageSize = 0;        // 맵 전체 폭 (월드 좌표)
    LONG   m_scrollMinX = 0;        // 허용 최소 카메라 X(좌상단X)
    LONG   m_scrollMaxX = 0;        // 허용 최대 카메라 X(좌상단X)

    // 스무딩 파라미터
    float m_smoothTime = 0.20f;    // 작을수록 더 빠르게 목표로 수렴
    float m_velX = 0.f;      // 내부 감쇠 속도 상태(X)
    float m_velY = 0.f;      // 내부 감쇠 속도 상태(Y)

    // GDI 자원
    HDC     m_frontHDC = nullptr;
    HDC     m_backHDC = nullptr;
    HBITMAP m_hFrontBitmap = nullptr;
    HBITMAP m_hBackBitmap = nullptr;
    HBITMAP m_hOldFrontBitmap = nullptr;
    HBITMAP m_hOldBackBitmap = nullptr;

    // --- 1D SmoothDamp (Unity 유사 크리티컬 감쇠) ---
    static float SmoothDamp1D(float current, float target, float& currentVelocity, float smoothTime, float dt)
    {
        if (smoothTime < 0.0001f) smoothTime = 0.0001f;
        float omega = 2.0f / smoothTime;
        float x = current - target;
        float e = std::exp(-omega * dt);
        float temp = (currentVelocity + omega * x) * dt;
        float newVal = target + (x + temp) * e;
        currentVelocity = (currentVelocity - omega * temp) * e;
        return newVal;
    }
  
    CScreenManager() = default;
    ~CScreenManager() { Release(); }

    void Release()
    {
        if (m_frontHDC) {
            SelectObject(m_frontHDC, m_hOldFrontBitmap);
            DeleteObject(m_hFrontBitmap);
            ReleaseDC(g_hWnd, m_frontHDC);
            m_frontHDC = nullptr;
        }
        if (m_backHDC) {
            SelectObject(m_backHDC, m_hOldBackBitmap);
            DeleteObject(m_hBackBitmap);
            DeleteDC(m_backHDC);
            m_backHDC = nullptr;
        }
    }

public:
    CScreenManager(const CScreenManager&) = delete;
    CScreenManager& operator=(const CScreenManager&) = delete;

    static CScreenManager& Instance() {
        static CScreenManager inst;
        return inst;
    }

    // --- 초기화 ---
    void Initialize()
    {
        if (!m_frontHDC) {
            m_frontHDC = GetDC(g_hWnd);
        }
        if (!m_backHDC) {
            m_backHDC = CreateCompatibleDC(m_frontHDC);
            m_hBackBitmap = CreateCompatibleBitmap(m_frontHDC, MapSize * ScreenCX, MapSize * ScreenCY);
            m_hOldBackBitmap = (HBITMAP)SelectObject(m_backHDC, m_hBackBitmap);
        }

        // 초기 값
        m_posCamera = { 0,0 };
        m_targetCamera = { 0,0 };
        m_camRect = { 0,0, WINCX, WINCY };
        m_velX = m_velY = 0.f;

        // 기본 스크롤 경계는 전체 맵
        m_scrollMinX = 0;
        m_scrollMaxX = std::max(0, m_iStageSize - WINCX);
    }

    // --- 그리기용 클리어 ---
    void Clear()
    {
        PatBlt(m_backHDC, m_posCamera.x, m_posCamera.y, ScreenCX, ScreenCY, WHITENESS);
    }

    // --- 프레임 업데이트(흔들림 + 스무딩) ---
    void Update(double dt)
    {
        // 흔들림
        if (m_shakeTime > 0.f) {
            m_shakeTime -= (float)dt;
            if (m_shakeTime < 0.f) m_shakeTime = 0.f;
            float k = (m_shakeTime / m_shakeDuration);
            int ox = (rand() % 3 - 1) * (int)(m_shakeAmp * k);
            int oy = (rand() % 3 - 1) * (int)(m_shakeAmp * k * 0.6f);
            m_shakeOffset = { ox, oy };
        }
        else {
            m_shakeOffset = { 0,0 };
        }

        // 스무스 추적: m_posCamera -> m_targetCamera
        float cx = (float)m_posCamera.x;
        float cy = (float)m_posCamera.y;
        float tx = (float)m_targetCamera.x;
        float ty = (float)m_targetCamera.y;

        cx = SmoothDamp1D(cx, tx, m_velX, m_smoothTime, (float)dt);
        cy = SmoothDamp1D(cy, ty, m_velY, m_smoothTime, (float)dt);

        // 정수화 + 경계 클램프
        m_posCamera.x = std::clamp((LONG)lroundf(cx), m_scrollMinX, m_scrollMaxX);
        m_posCamera.y = std::clamp((int)lroundf(cy), 0, MapSize * WINCY);

        SetCameraRect();
    };
    POINT GetShakeOffset() const { return m_shakeOffset; }

    // --- 프론트 버퍼로 내보내기 ---
    void Present()
    {
        POINT src{ m_posCamera.x + m_shakeOffset.x, m_posCamera.y + m_shakeOffset.y };

        // 안전 클램프(백버퍼 경계)
        src.x = std::max(0L, std::min(src.x, (LONG)(MapSize * ScreenCX - ScreenCX)));
        src.y = std::max(0L, std::min(src.y, (LONG)(MapSize * ScreenCY - ScreenCY)));

        BitBlt(m_frontHDC, 0, 0, ScreenCX, ScreenCY, m_backHDC, src.x, src.y, SRCCOPY);
    }

    // --- HDC/사각/크기 ---
    HDC   GetHDC()     const { return m_backHDC; }
    RECT  GetCamRect() const { return m_camRect; }
    POINT GetCamerPos()const { return m_posCamera; }   // 흔들림 제외(논리 카메라)

    void  SetCameraRect()
    {
        m_camRect = { m_posCamera.x, m_posCamera.y, m_posCamera.x + ScreenCX, m_posCamera.y + ScreenCY };
    }

    // --- 스테이지 크기(맵 폭) 설정 + 기본 경계 재설정 ---
    void Set_StageSize(int stageWidth)
    {
        m_iStageSize = stageWidth;
        m_scrollMinX = 0;
        m_scrollMaxX = std::max(0, m_iStageSize - WINCX);

        // 현재/목표 보정
        m_targetCamera.x = std::clamp(m_targetCamera.x, m_scrollMinX, m_scrollMaxX);
        m_posCamera.x = std::clamp(m_posCamera.x, m_scrollMinX, m_scrollMaxX);
    }

    // --- 카메라 목표 위치 지정(부드럽게 이동) ---
    void SetCameraTarget(POINT p, bool clampMap = true)
    {
        if (clampMap) {
            p.x = std::clamp((LONG)p.x, m_scrollMinX, m_scrollMaxX);
            p.y = std::clamp((int)p.y, 0, MapSize * WINCY);
        }
        m_targetCamera = p; // 속도는 유지 → 자연스러운 이동
    }

    // --- 즉시 스냅 이동(컷신 등 필요 시만 사용) ---
    void SetCameraPos(POINT p, bool clampMap = true)
    {
        if (clampMap) {
            p.x = std::clamp((LONG)p.x, m_scrollMinX, m_scrollMaxX);
            p.y = std::clamp((int)p.y, 0, MapSize * WINCY);
        }
        m_posCamera = p;
        m_targetCamera = p;
        m_velX = m_velY = 0.f; // 정지
        SetCameraRect();
    }

    // --- 스무딩 강도 조절(선택) ---
    void SetSmoothTime(float sec) { m_smoothTime = std::max(0.01f, sec); }

    // --- 카메라 X 경계 설정(보스 아레나 구간 잠금) ---
    // leftX/rightX: 카메라 좌상단 X 기준(월드 좌표)
    void SetCameraBounds(int leftX, int rightX)
    {
        if (leftX > rightX) std::swap(leftX, rightX);
        m_scrollMinX = leftX;
        m_scrollMaxX = rightX;

        m_targetCamera.x = std::clamp(m_targetCamera.x, m_scrollMinX, m_scrollMaxX);
        m_posCamera.x = std::clamp(m_posCamera.x, m_scrollMinX, m_scrollMaxX);
    }

    // --- 경계 해제(맵 전체로 복구) ---
    void ClearCameraBounds()
    {
        m_scrollMinX = 0;
        m_scrollMaxX = std::max(0, m_iStageSize - WINCX);

        m_targetCamera.x = std::clamp(m_targetCamera.x, m_scrollMinX, m_scrollMaxX);
        m_posCamera.x = std::clamp(m_posCamera.x, m_scrollMinX, m_scrollMaxX);
    }

    // --- 카메라 흔들림 트리거 ---
    void CamShake(float amp = 8.f, float duration = 0.15f)
    {
        m_shakeAmp = amp;
        m_shakeDuration = duration;
        m_shakeTime = duration;
    }
};
