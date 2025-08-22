#pragma once

#include "CMainUI.h"
#include "CSubWeapon.h"
class CUI_SubSlot : public CMainUI
{
public:
	CUI_SubSlot();
	~CUI_SubSlot();

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
	void Num_UpdateRect();

private:
	CSubWeapon::THROWTYPE m_eType;
	int m_iQuantity{ 0 };
	INFO m_tNumInfo{};
	RECT m_tNumRect{};
};

