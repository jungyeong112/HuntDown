#pragma once
#include "CMainUI.h"
#include "CGun.h"



class CUI_MainSlot : public CMainUI
{
public:
	CUI_MainSlot();
	~CUI_MainSlot();

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
	void Num_UpdateRect();

private:
	CGun::GUNTYPE m_eGunType;
	int m_iMagazine{0};
	INFO m_tNumInfo{};
	RECT m_tNumRect{};
};

