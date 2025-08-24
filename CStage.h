#pragma once
#include "CScene.h"
#include "CScrollBackGround.h"

class CStage : public CScene
{
public:
	CStage();
	~CStage();

public:

	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void CreateMap();
	void CreateUI();
	void Set_InsertBmp();
	void Set_CollsionMask();
	void Set_BackGround();

private:
	CScrollBackGround m_midBld; // 중간 빌딩 패럴랙스 레이어
	HDC     m_mapCacheDC = nullptr;
	HBITMAP m_mapCacheBMP = nullptr;
	HBITMAP m_oldMapCache = nullptr;
	int     m_mapW = 3950;
	int     m_mapH = WINCY;
	int     m_mapY = -45;
};

