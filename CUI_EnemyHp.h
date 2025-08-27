#pragma once

#include "CUI.h"

class CUI_EnemyHp : public CUI
{
public:
	CUI_EnemyHp();
	~CUI_EnemyHp();

public:
	
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Update_BGRect();

protected:
	INFO m_tBgInfo{};
	RECT m_tBgRect{};
};

