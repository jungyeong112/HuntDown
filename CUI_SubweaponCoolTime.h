#pragma once
#include "CMainUI.h"

class CUI_SubweaponCoolTime : public CMainUI
{
public:
	CUI_SubweaponCoolTime();
	~CUI_SubweaponCoolTime();

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;

private:
	float m_fCoolRatio{1.f};
};

